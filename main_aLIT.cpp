#include "getopt.h"
#include "def_global.h"
#include "./containers/relation.h"
#include "./indices/hint_m.h"
#include "./indices/live_index.cpp"


void usage()
{
    cerr << endl;
    cerr << "PROJECT" << endl;
    cerr << "       LIT: Lightning-fast In-memory Temporal Indexing" << endl << endl;
    cerr << "USAGE" << endl;
    cerr << "       ./query_aLIT.exec [OPTIONS] [STREAMFILE]" << endl << endl;
    cerr << "DESCRIPTION" << endl;
    cerr << "       -? or -h" << endl;
    cerr << "              display this help message and exit" << endl;
    cerr << "       -e" << endl;
    cerr << "              set the leaf partition extent; it is set in seconds" << endl;
    cerr << "       -b" << endl;
    cerr << "              set the type of data structure for the LIVE INDEX" << endl;
    cerr << "       -c" << endl;
    cerr << "              set the capacity constraint number for the LIVE INDEX" << endl; 
    cerr << "       -n" << endl;
    cerr << "              set the number of LIT indices for non-temporal attribute indexing" << endl;    
    cerr << "       -r runs" << endl;
    cerr << "              set the number of runs per query; by default 1" << endl << endl;
    cerr << "EXAMPLE" << endl;
    cerr << "       ./query_aLIT.exec -e 86400 -b ENHANCEDHASHMAP -c 10000 -n 8 streams/BOOKS.mix" << endl << endl;
}


int main(int argc, char **argv)
{
    Timer tim;
    vector<HINT_M_Dynamic_Second_Attr*> idxR;
    vector<LiveIndexCapacityConstraintedICDE16*> lidxR;
    size_t totalResult = 0, queryresult = 0, numQueries = 0, numInserts = 0, numUpdates = 0;
    double b_starttime = 0, b_endtime = 0, i_endtime = 0, b_querytime = 0, i_querytime = 0, avgQueryTime = 0;
    double totalIndexTime = 0, totalBufferStartTime = 0, totalBufferEndTime = 0, totalIndexEndTime = 0, totalQueryTime_b = 0, totalQueryTime_i = 0, totalBufferMergingTime = 0;
    Timestamp first, second, startEndpoint;
    RunSettings settings;
    int numberOfIndices;
    double third, fourth;
    char c, operation;
    double vm = 0, rss = 0, vmMax = 0, rssMax = 0;
    string strQuery = "", strPredicate = "", strOptimizations = "";
    Timestamp leafPartitionExtent = 0;
    string typeBuffer;
    size_t maxCapacity = -1;
    Timestamp maxDuration = -1;
    unsigned int mergeParameter = 0;
    size_t maxNumBuffers = 0;
    size_t segment = ((MAX_ATTRIBUTE_VALUE + numberOfIndices)/ numberOfIndices);
    size_t sumQ = 0;
    int indexStart = 0, indexEnd = 0;
    
    settings.init();
    settings.method = "hint_m_dynamic";
    while ((c = getopt(argc, argv, "?hq:e:c:d:b:m:n:r:")) != -1)
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

           case 'n':
               numberOfIndices = atoi(optarg);
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

    
    idxR.reserve(numberOfIndices);
    lidxR.reserve(numberOfIndices);    
    tim.start();
    for(int i = 0; i < numberOfIndices; i++)
        idxR[i] = new HINT_M_Dynamic_Second_Attr(leafPartitionExtent);
    totalIndexTime = tim.stop();
    

    if(maxCapacity != -1)
        for(int i = 0; i < numberOfIndices; i++)
            lidxR[i] = new LiveIndexCapacityConstraintedICDE16(maxCapacity);
    else
    {
        usage();
        return 1;
    }

    
    if (maxCapacity != -1)
    {
        if (typeBuffer != "ENHANCEDHASHMAP"){
            usage();
            return 1;
        }
    }
    else
    {
            usage();
            return 1;
    } 


    settings.queryFile = argv[optind];
    ifstream fQ(settings.queryFile);
    if (!fQ)
    {
        usage();
        return 1;
    }


    while (fQ >> operation >> first >> second  >> third >> fourth)
    {
        switch (operation)
        {
            case 'S':
                indexStart = third/segment;
                tim.start();
                lidxR[indexStart]->insert_secAttr(first, second, third);
                b_starttime = tim.stop();
                totalBufferStartTime += b_starttime;
                
                numInserts++;
                process_mem_usage(vm, rss);
                vmMax = max(vm, vmMax);
                rssMax = max(rss, rssMax);
                break;

            case 'E':
                indexStart = third/segment;

                tim.start();
                startEndpoint = lidxR[indexStart]->remove_secAttr(first);
                b_endtime = tim.stop();
                totalBufferEndTime += b_endtime;
                tim.start();
                idxR[indexStart]->insert(Record(first, startEndpoint, second, third));
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
                indexStart = third/segment;
                indexEnd = fourth/segment;

                double sumT = 0;
                for (auto r = 0; r < settings.numRuns; r++)
                {
                    if(indexStart != indexEnd){
                        tim.start();
                        queryresult = lidxR[indexStart]->executeTimeTravel_greaterthan(RangeQuery(numQueries, first, second), third);
                        b_querytime = tim.stop();

                        tim.start();
                        if (first <= idxR[indexStart]->gend){
#ifdef WORKLOAD_COUNT
                            queryresult += idxR[indexStart]->executeTimeTravel_greaterthan(RangeQuery(numQueries, first, second), third);
#else
                            queryresult ^= idxR[indexStart]->executeTimeTravel_greaterthan(RangeQuery(numQueries, first, second), third);
#endif
                        }
                        i_querytime = tim.stop();

                        for(int i = indexStart+1; i < indexEnd; i++){
                            tim.start();
#ifdef WORKLOAD_COUNT
                            queryresult += lidxR[i]->execute_pureTimeTravel(RangeQuery(numQueries, first, second));
#else
                            queryresult ^= lidxR[i]->execute_pureTimeTravel(RangeQuery(numQueries, first, second));
#endif
                            b_querytime += tim.stop();

                            tim.start();
                            if (first <= idxR[i]->gend){
#ifdef WORKLOAD_COUNT
                                queryresult += idxR[i]->execute_pureTimeTravel(RangeQuery(numQueries, first, second));
#else
                                queryresult ^= idxR[i]->execute_pureTimeTravel(RangeQuery(numQueries, first, second));
#endif
                            }
                                i_querytime += tim.stop();
                        }


                        tim.start();
#ifdef WORKLOAD_COUNT
                        queryresult += lidxR[indexEnd]->executeTimeTravel_lowerthan(RangeQuery(numQueries, first, second), fourth);
#else
                        queryresult ^= lidxR[indexEnd]->executeTimeTravel_lowerthan(RangeQuery(numQueries, first, second), fourth);
#endif
                        b_querytime += tim.stop();

                        tim.start();
                        if (first <= idxR[indexEnd]->gend){
#ifdef WORKLOAD_COUNT
                            queryresult += idxR[indexEnd]->executeTimeTravel_lowerthan(RangeQuery(numQueries, first, second), fourth);
#else
                            queryresult ^= idxR[indexEnd]->executeTimeTravel_lowerthan(RangeQuery(numQueries, first, second), fourth);
#endif
                        }
                        i_querytime += tim.stop();
                    }
                    else{
                        tim.start();
#ifdef WORKLOAD_COUNT
                        queryresult = lidxR[indexStart]->execute_rangeTimeTravel(RangeQuery(numQueries, first, second), third, fourth);
#else
                        queryresult = lidxR[indexStart]->execute_rangeTimeTravel(RangeQuery(numQueries, first, second), third, fourth);
#endif
                        b_querytime = tim.stop();

                        tim.start();
                        if (first <= idxR[indexStart]->gend){
#ifdef WORKLOAD_COUNT
                            queryresult += idxR[indexStart]->execute_rangeTimeTravel(RangeQuery(numQueries, first, second), third, fourth);
#else
                            queryresult ^= idxR[indexStart]->execute_rangeTimeTravel(RangeQuery(numQueries, first, second), third, fourth);
#endif
                        }
                        i_querytime = tim.stop();
                    }

                }
                totalQueryTime_b += b_querytime;
                totalQueryTime_i += i_querytime;
                totalResult += queryresult;
                break;
        }
        maxNumBuffers = max(maxNumBuffers, lidxR[0]->getNumBuffers());
                

                
        process_mem_usage(vm, rss);
        vmMax = max(vm, vmMax);
        rssMax = max(rss, rssMax);

    }
    fQ.close();
    

    cout << "aLIT" << endl;
    cout << "====================" << endl;
    cout << endl;
    cout << "Buffer info" << endl;
    cout << "Type                               : " << typeBuffer << endl;
    if (maxCapacity != -1)
        cout << "Buffer capacity                    : " << maxCapacity << endl << endl;
    else
        cout << "Buffer duration                    : " << maxDuration << endl << endl;
    cout << "Updates report" << endl;
    cout << "Num of inserts                     : " << numInserts << endl;
    cout << "Num of updates                     : " << numUpdates << endl;
    cout << "Num of buffers (max)               : " << maxNumBuffers << endl;
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

    for(int i = 0; i < numberOfIndices; i++){
        delete lidxR[i];
        delete idxR[i];
    }
    
    
    return 0;
}