#include "getopt.h"
#include "def_global.h"
#include "./containers/relation.h"
#include "./indices/hint_m.h"
#include "./indices/live_index.cpp"

//#define ACTIVATE_PROGRESS_BAR



void printProgressBar(LiveIndex *lidx, size_t count)
{
#ifdef ACTIVATE_PROGRESS_BAR
    if (count % 500000 == 0)
    {
        cout << count << endl;//"\t" << lidxR->getSize() << endl;
        lidx->print('r');
    }
#endif
}


void usage()
{
    cerr << endl;
    cerr << "PROJECT" << endl;
    cerr << "       LIT: Lightning-fast In-memory Temporal Indexing" << endl << endl;
    cerr << "USAGE" << endl;
    cerr << "       ./query_pureLIT.exec [OPTIONS] [STREAMFILE]" << endl << endl;
    cerr << "DESCRIPTION" << endl;
    cerr << "       -? or -h" << endl;
    cerr << "              display this help message and exit" << endl;
    cerr << "       -e" << endl;
    cerr << "              set the leaf partition extent; it is set in seconds" << endl;
    cerr << "       -b" << endl;
    cerr << "              set the type of data structure for the LIVE INDEX" << endl;
    cerr << "       -c" << endl;
    cerr << "              set the capacity constraint number for the LIVE INDEX" << endl; 
    cerr << "       -d" << endl;
    cerr << "              set the duration constraint number for the LIVE INDEX" << endl;      
    cerr << "       -r runs" << endl;
    cerr << "              set the number of runs per query; by default 1" << endl << endl;
    cerr << "EXAMPLE" << endl;
    cerr << "       ./query_pureLIT.exec -e 86400 -b ENHANCEDHASHMAP -c 10000 streams/BOOKS.mix" << endl << endl;
}


int main(int argc, char **argv)
{
    Timer tim;
    Record r;
    HINT_M_Dynamic *idxR;
    LiveIndex *lidxR;
    size_t totalResult = 0, queryresult = 0, numQueries = 0, numUpdates = 0;
    double b_starttime = 0, b_endtime = 0, i_endtime = 0, b_querytime = 0, i_querytime = 0, avgQueryTime = 0;
    double totalIndexTime = 0, totalBufferStartTime = 0, totalBufferEndTime = 0, totalIndexEndTime = 0, totalQueryTime_b = 0, totalQueryTime_i = 0, totalBufferMergingTime = 0;
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

    
    settings.init();
    settings.method = "pureLIT";
    while ((c = getopt(argc, argv, "?hq:e:c:d:b:r:")) != -1)
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
                
            case 'b':
                typeBuffer = toUpperCase((char*)optarg);
                break;

            case 'c':
                maxCapacity = atoi(optarg);
                break;

            case 'd':
                maxDuration = atoi(optarg);
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
    idxR = new HINT_M_Dynamic(leafPartitionExtent);
    totalIndexTime = tim.stop();
    

    
    if (maxCapacity != -1)
    {
        if (typeBuffer == "MAP")
            lidxR = new LiveIndexCapacityConstraintedMap(maxCapacity);
        else if (typeBuffer == "VECTOR")
            lidxR = new LiveIndexCapacityConstraintedVector(maxCapacity);
        else if (typeBuffer == "ENHANCEDHASHMAP")
            lidxR = new LiveIndexCapacityConstraintedICDE16(maxCapacity);
        else
        {
                usage();
                return 1;
        }
    }
    else if (maxDuration != -1)
    {
        if (typeBuffer == "MAP")
            lidxR = new LiveIndexDurationConstraintedMap(maxDuration);
        else if (typeBuffer == "VECTOR")
            lidxR = new LiveIndexDurationConstraintedVector(maxDuration);
        else if (typeBuffer == "ENHANCEDHASHMAP")
            lidxR = new LiveIndexDurationConstraintedICDE16(maxDuration);
        else
        {
            usage();
            return 1;
        }
    }
    else
    {
        usage();
        return 1;
    }

    
    

    // Load stream
    settings.queryFile = argv[optind];
    ifstream fQ(settings.queryFile);
    if (!fQ)
    {
        usage();
        return 1;
    }


    // Read stream
    size_t sumQ = 0;
    size_t count = 0;


    while (fQ >> operation >> first >> second >> third >> fourth)
    {
        switch (operation)
        {
            case 'S':
                tim.start();
                lidxR->insert(first, second);
                b_starttime = tim.stop();
                totalBufferStartTime += b_starttime;
                
                numUpdates++;
                
                process_mem_usage(vm, rss);
                vmMax = max(vm, vmMax);
                rssMax = max(rss, rssMax);
                break;

            case 'E':
                tim.start();
                startEndpoint = lidxR->remove(first);
                b_endtime = tim.stop();
                totalBufferEndTime += b_endtime;
                
                tim.start();
                idxR->insert(Record(first, startEndpoint, second));
                i_endtime = tim.stop();
                totalIndexEndTime += i_endtime;

                numUpdates++;
                
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
                    queryresult = lidxR->execute_pureTimeTravel(RangeQuery(numQueries, first, second));
                    b_querytime = tim.stop();

                    tim.start();
                    if (first <= idxR->gend){
#ifdef WORKLOAD_COUNT
                        queryresult += idxR->execute_pureTimeTravel(RangeQuery(numQueries, first, second));
#else
                        queryresult ^= idxR->execute_pureTimeTravel(RangeQuery(numQueries, first, second));
#endif
                    }
                    i_querytime = tim.stop();

                    totalQueryTime_b += b_querytime;
                    totalQueryTime_i += i_querytime;

                }
                totalResult += queryresult;
                avgQueryTime += sumT/settings.numRuns;
                
                process_mem_usage(vm, rss);
                vmMax = max(vm, vmMax);
                rssMax = max(rss, rssMax);
                break;
        }     
        maxNumBuffers = max(maxNumBuffers, lidxR->getNumBuffers());

    }
    fQ.close();
    
    
    // Report
    idxR->getStats();
    cout << endl;
    cout << "pureLIT" << endl;
    cout << "====================" << endl;
    cout << endl;
    cout << "Buffer info" << endl;
    cout << "Type                               : " << typeBuffer << endl;
    if (maxCapacity != -1)
        cout << "Buffer capacity                    : " << maxCapacity << endl << endl;
    else
        cout << "Buffer duration                    : " << maxDuration << endl << endl;
    cout << "Index info" << endl;
    cout << "Updates report" << endl;
    cout << "Num of updates                     : " << numUpdates << endl;
    cout << "Num of buffers  (max)              : " << maxNumBuffers << endl;
    printf( "Total updating time (buffer) [secs]: %f\n", (totalBufferStartTime+totalBufferEndTime));
    printf( "Total updating time (index)  [secs]: %f\n\n", totalIndexEndTime);
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
    printf( "Total querying time (buffer) [secs]: %f\n", totalQueryTime_b/settings.numRuns);
    printf( "Total querying time (index)  [secs]: %f\n\n", totalQueryTime_i/settings.numRuns);


    delete lidxR;
    delete idxR;
    
    
    return 0;
}
