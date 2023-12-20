#include "getopt.h"
#include "def_global.h"
#include "./containers/relation.h"
#include "./indices/hint_m.h"

void usage()
{
    cerr << endl;
    cerr << "PROJECT" << endl;
    cerr << "       LIT: Lightning-fast In-memory Temporal Indexing" << endl << endl;
    cerr << "USAGE" << endl;
    cerr << "       ./query_teHINT.exec [OPTIONS] [STREAMFILE]" << endl << endl;
    cerr << "DESCRIPTION" << endl;
    cerr << "       -? or -h" << endl;
    cerr << "              display this help message and exit" << endl;
    cerr << "       -e" << endl;
    cerr << "              set the leaf partition extent; it is set in seconds" << endl;     
    cerr << "       -r runs" << endl;
    cerr << "              set the number of runs per query; by default 1" << endl << endl;
    cerr << "EXAMPLE" << endl;
    cerr << "       ./query_teHINT.exec -e 86400 streams/BOOKS.mix" << endl << endl;
}


int main(int argc, char **argv)
{
    Timer tim;
    Record r;
    HINT_M_Dynamic_Naive *idxR;
    size_t totalResult = 0, queryresult = 0, numQueries = 0, numUpdates = 0, numExpansions = 0;
    double b_starttime = 0, b_endtime = 0, i_endtime = 0, b_querytime = 0, i_querytime = 0, avgQueryTime = 0, expand_time = 0;
    double totalIndexTime = 0, totalInsertTmpTime = 0, totalRemoveTmpTime = 0, totalIndexEndTime = 0, totalQueryTime_b = 0, totalQueryTime_i = 0, totalBufferMergingTime = 0, totalExpandTime = 0;
    Timestamp first, second, startEndpoint;
    RunSettings settings;
    char c, operation;
    double third, fourth;
    double vm = 0, rss = 0, vmMax = 0, rssMax = 0;
    string strQuery = "", strPredicate = "", strOptimizations = "";
    Timestamp leafPartitionExtent = 0;
    string typeBuffer;
    size_t maxCapacity = -1;
    Timestamp maxDuration = -1;
    unsigned int mergeParameter = 0;
    size_t maxNumBuffers = 0;

    
    // Parse command line input
    settings.init();
    settings.method = "teHINT";
    while ((c = getopt(argc, argv, "?hq:e:c:d:b:m:r:")) != -1)
    {
        switch (c)
        {
            case '?':
            case 'h':
                usage();
                return 0;

            case 'e':
                leafPartitionExtent = atoi(optarg);
                break;                
            case 'r':
                settings.numRuns = atoi(optarg);
                break;

            default:
                cerr << endl << "Error - unknown option '" << c << "'" << endl << endl;
                usage();
                return 1;
        }
    }
    
    
    // Sanity check
    if (argc-optind != 1)
    {
        usage();
        return 1;
    }

    if (leafPartitionExtent <= 0)
    {
        usage();
        return 1;
    }


    
    
    tim.start();
    idxR = new HINT_M_Dynamic_Naive(leafPartitionExtent);
    totalIndexTime = tim.stop();
    
    

    settings.queryFile = argv[optind];
    ifstream fQ(settings.queryFile);
    if (!fQ)
    {
        usage();
        return 1;
    }

    size_t sumQ = 0;
    size_t count = 0;
    if (settings.verbose)
        cout << "Operation\tInput1\tInput2\tBuffer_time\tIndex_time\tPredicate\tResult" << endl;

    while (fQ >> operation >> first >> second >> third >> fourth)
    {
        switch (operation)
        {

            case 'S':
                tim.start();
                idxR->insertTmp(first, second);
                b_starttime = tim.stop();
                totalInsertTmpTime += b_starttime;
                
                numUpdates++;
                if (settings.verbose)
                {
                    cout << "S\t" << first << "\t" << second;
                    printf("\t%f\t0", b_starttime);
                    cout << "\t-\t-" << endl;
                }
                
                process_mem_usage(vm, rss);
                vmMax = max(vm, vmMax);
                rssMax = max(rss, rssMax);
                break;

            case 'E':
                tim.start();
                startEndpoint = idxR->removeTmp(first);
                b_endtime = tim.stop();
                totalRemoveTmpTime += b_endtime;
                tim.start();
                idxR->insert(Record(first, startEndpoint, second));
                i_endtime = tim.stop();
                totalIndexEndTime += i_endtime;

                numUpdates++;
                if (settings.verbose)
                {
                    cout << "E\t" << first << "\t" << second;
                    printf("\t%f\t%f", b_endtime, i_endtime);
                    cout << "\t-\t-" << endl;
                }
                
                process_mem_usage(vm, rss);
                vmMax = max(vm, vmMax);
                rssMax = max(rss, rssMax);
                break;

            case 'Q':
                numQueries++;
                sumQ += second-first;

                double sumT = 0;
                for (auto r = 0; r < settings.numRuns; r++)
                {

                    tim.start();
                    queryresult = idxR->execute_pureTimeTravel(RangeQuery(numQueries, first, second));
                    i_querytime = tim.stop();

                    totalQueryTime_b += b_querytime;
                    totalQueryTime_i += i_querytime;

                    if (settings.verbose)
                    {
                        cout << "Q\t" << first << "\t" << second;
                        printf("\t%f\t%f", b_querytime, i_querytime);
                        cout << "\t" << strPredicate << "\t" << queryresult << endl;
                    }
                }
                totalResult += queryresult;
                avgQueryTime += sumT/settings.numRuns;
                
                process_mem_usage(vm, rss);
                vmMax = max(vm, vmMax);
                rssMax = max(rss, rssMax);
                break;
        }
    }
    fQ.close();
    

    
    // Report
    cout << endl;
    cout << "teHINT" << endl;
    cout << "====================" << endl;
    cout << endl;
    cout << "Index info" << endl;
    cout << "Updates report" << endl;
    cout << "Num of updates                     : " << numUpdates << endl;
    printf( "Total updating time          [secs]: %f\n", (totalInsertTmpTime+totalRemoveTmpTime));
    cout << "Queries report" << endl;
    cout << "Num of queries                     : " << numQueries << endl;
    cout << "Num of runs per query              : " << settings.numRuns << endl;
    cout << "Total result [";
#ifdef WORKLOAD_COUNT
    cout << "COUNT]               : ";
#else
    cout << "XOR]                 : ";
#endif
    cout << totalResult << endl;
    printf( "Total querying time (index)  [secs]: %f\n\n", totalQueryTime_i/settings.numRuns);



    // delete lidxR;
    delete idxR;
    
    
    return 0;
}
