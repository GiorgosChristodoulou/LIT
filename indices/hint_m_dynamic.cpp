#include "hint_m.h"



inline void HINT_M_Subs_CM::updateCounters(const Record &r)
{
    int level = 0;
    Timestamp a = r.start >> (this->maxBits-this->numBits);
    Timestamp b = r.end   >> (this->maxBits-this->numBits);
    Timestamp prevb;
    int firstfound = 0, lastfound = 0;
    
    
    while (level < this->height && a <= b)
    {
        if (a%2)
        { //last bit of a is 1
            if (firstfound)
            {
                if ((a == b) && (!lastfound))
                {
                    this->pRepsIn_sizes[level][a]++;
                    lastfound = 1;
                }
                else
                    this->pRepsAft_sizes[level][a]++;
            }
            else
            {
                if ((a == b) && (!lastfound))
                    this->pOrgsIn_sizes[level][a]++;
                else
                    this->pOrgsAft_sizes[level][a]++;
                firstfound = 1;
            }
            a++;
        }
        if (!(b%2))
        { //last bit of b is 0
            prevb = b;
            b--;
            if ((!firstfound) && b < a)
            {
                if (!lastfound)
                    this->pOrgsIn_sizes[level][prevb]++;
                else
                    this->pOrgsAft_sizes[level][prevb]++;
            }
            else
            {
                if (!lastfound)
                {
                    this->pRepsIn_sizes[level][prevb]++;
                    lastfound = 1;
                }
                else
                {
                    this->pRepsAft_sizes[level][prevb]++;
                }
            }
        }
        a >>= 1; // a = a div 2
        b >>= 1; // b = b div 2
        level++;
    }
}


// Assumes that partitions to be updated already exist
inline void HINT_M_Dynamic::updatePartitions(const Record &r)
{
    int level = 0;
    Timestamp a = r.start >> (this->maxBits-this->numBits);
    Timestamp b = r.end   >> (this->maxBits-this->numBits);
    Timestamp prevb;
    int firstfound = 0, lastfound = 0;
    
 
//    cout << "a = " << a << ", b = " << b << endl;
    while (level < this->height && a <= b)
    {
        if (a%2)
        { //last bit of a is 1
            if (firstfound)
            {
                if ((a == b) && (!lastfound))
                {
//                    this->pRepsInTmp[level][a][this->pRepsIn_sizes[level][a]] = Record(r.id, r.start, r.end);
                    this->pRepsInIds[level][a].emplace_back(r.id);
                    this->pRepsInTimestamps[level][a].emplace_back(r.start, r.end);
//                    this->pRepsIn_sizes[level][a]++;
                    lastfound = 1;
                }
                else
                {
//                    this->pRepsAftTmp[level][a][this->pRepsAft_sizes[level][a]] = Record(r.id, r.start, r.end);
                    this->pRepsAftIds[level][a].emplace_back(r.id);
                    this->pRepsAftTimestamps[level][a].emplace_back(r.start, r.end);
//                    this->pRepsAft_sizes[level][a]++;
                }
            }
            else
            {
                if ((a == b) && (!lastfound))
                {
//                    this->pOrgsInTmp[level][a][this->pOrgsIn_sizes[level][a]] = Record(r.id, r.start, r.end);
                    this->pOrgsInIds[level][a].emplace_back(r.id);
                    this->pOrgsInTimestamps[level][a].emplace_back(r.start, r.end);
//                    this->pOrgsIn_sizes[level][a]++;
                }
                else
                {
//                    this->pOrgsAftTmp[level][a][this->pOrgsAft_sizes[level][a]] = Record(r.id, r.start, r.end);
                    this->pOrgsAftIds[level][a].emplace_back(r.id);
                    this->pOrgsAftTimestamps[level][a].emplace_back(r.start, r.end);
//                    this->pOrgsAft_sizes[level][a]++;
                }
                firstfound = 1;
            }
            a++;
        }
        if (!(b%2))
        { //last bit of b is 0
            prevb = b;
            b--;
            if ((!firstfound) && b < a)
            {
                if (!lastfound)
                {
//                    this->pOrgsInTmp[level][prevb][this->pOrgsIn_sizes[level][prevb]] = Record(r.id, r.start, r.end);
                    this->pOrgsInIds[level][prevb].emplace_back(r.id);
                    this->pOrgsInTimestamps[level][prevb].emplace_back(r.start, r.end);
//                    this->pOrgsIn_sizes[level][prevb]++;
                }
                else
                {
//                    this->pOrgsAftTmp[level][prevb][this->pOrgsAft_sizes[level][prevb]] = Record(r.id, r.start, r.end);
                    this->pOrgsAftIds[level][prevb].emplace_back(r.id);
                    this->pOrgsAftTimestamps[level][prevb].emplace_back(r.start, r.end);
//                    this->pOrgsAft_sizes[level][prevb]++;
                }
            }
            else
            {
                if (!lastfound)
                {
//                    this->pRepsInTmp[level][prevb][this->pRepsIn_sizes[level][prevb]] = Record(r.id, r.start, r.end);
                    this->pRepsInIds[level][prevb].emplace_back(r.id);
                    this->pRepsInTimestamps[level][prevb].emplace_back(r.start, r.end);
//                    this->pRepsIn_sizes[level][prevb]++;
                    lastfound = 1;
                }
                else
                {
//                    this->pRepsAftTmp[level][prevb][this->pRepsAft_sizes[level][prevb]] = Record(r.id, r.start, r.end);
                    this->pRepsAftIds[level][prevb].emplace_back(r.id);
                    this->pRepsAftTimestamps[level][prevb].emplace_back(r.start, r.end);
//                    this->pRepsAft_sizes[level][prevb]++;
                }
            }
        }
        a >>= 1; // a = a div 2
        b >>= 1; // b = b div 2
        level++;
    }
}


// Creates a HINT^m containing one empty partition, no hierarchy
HINT_M_Dynamic::HINT_M_Dynamic(Timestamp leafPartitionExtent)
{
    this->leafPartitionExtent = leafPartitionExtent;
    this->gstart = 0;
    this->gend   = this->leafPartitionExtent;
    this->maxBits = int(log2(this->gend-this->gstart)+1);
    
    // Size for RecordId, vector<pair<Timestamp, Timestamp > > unset.
    this->pOrgsInIds.resize(this->height);
    this->pOrgsInIds[0].resize(1);
    this->pOrgsInTimestamps.resize(this->height);
    this->pOrgsInTimestamps[0].resize(1);

    this->pOrgsAftIds.resize(this->height);
    this->pOrgsAftIds[0].resize(1);
    this->pOrgsAftTimestamps.resize(this->height);
    this->pOrgsAftTimestamps[0].resize(1);

    this->pRepsInIds.resize(this->height);
    this->pRepsInIds[0].resize(1);
    this->pRepsInTimestamps.resize(this->height);
    this->pRepsInTimestamps[0].resize(1);

    this->pRepsAftIds.resize(this->height);
    this->pRepsAftIds[0].resize(1);
    this->pRepsAftTimestamps.resize(this->height);
    this->pRepsAftTimestamps[0].resize(1);
}


// Creates a HINT^m for input R.
HINT_M_Dynamic::HINT_M_Dynamic(const Relation &R, const unsigned int numBits, const unsigned int maxBits) : HierarchicalIndex(R, numBits, maxBits)
{
    this->gstart = R.gstart;
    this->gend = R.gend;
    this->leafPartitionExtent = ceil((R.gend-R.gstart)/(float)pow(2, this->numBits));
    

    // Step 1: one pass to count the contents inside each partition.
    this->pOrgsIn_sizes  = (RecordId **)malloc(this->height*sizeof(RecordId *));
    this->pOrgsAft_sizes = (RecordId **)malloc(this->height*sizeof(RecordId *));
    this->pRepsIn_sizes  = (size_t **)malloc(this->height*sizeof(size_t *));
    this->pRepsAft_sizes = (size_t **)malloc(this->height*sizeof(size_t *));
    
    for (auto l = 0; l < this->height; l++)
    {
        auto cnt = (int)(pow(2, this->numBits-l));
        
        //calloc allocates memory and sets each counter to 0
        this->pOrgsIn_sizes[l]  = (RecordId *)calloc(cnt, sizeof(RecordId));
        this->pOrgsAft_sizes[l] = (RecordId *)calloc(cnt, sizeof(RecordId));
        this->pRepsIn_sizes[l]  = (size_t *)calloc(cnt, sizeof(size_t));
        this->pRepsAft_sizes[l] = (size_t *)calloc(cnt, sizeof(size_t));
    }
    
    for (const Record &r : R)
        this->updateCounters(r);
    
    // Step 2: allocate necessary memory.
    this->pOrgsInIds.resize(this->height);
    this->pOrgsAftIds.resize(this->height);
    this->pRepsInIds.resize(this->height);
    this->pRepsAftIds.resize(this->height);
    this->pOrgsInTimestamps.resize(this->height);
    this->pOrgsAftTimestamps.resize(this->height);
    this->pRepsInTimestamps.resize(this->height);
    this->pRepsAftTimestamps.resize(this->height);
    for (auto l = 0; l < this->height; l++)
    {
        auto cnt = (int)(pow(2, this->numBits-l));
        
        this->pOrgsInIds[l].resize(cnt);
        this->pOrgsAftIds[l].resize(cnt);
        this->pRepsInIds[l].resize(cnt);
        this->pRepsAftIds[l].resize(cnt);
        this->pOrgsInTimestamps[l].resize(cnt);
        this->pOrgsAftTimestamps[l].resize(cnt);
        this->pRepsInTimestamps[l].resize(cnt);
        this->pRepsAftTimestamps[l].resize(cnt);

        for (auto pId = 0; pId < cnt; pId++)
        {
            this->pOrgsInIds[l][pId].reserve(this->pOrgsIn_sizes[l][pId]);
            this->pOrgsAftIds[l][pId].reserve(this->pOrgsAft_sizes[l][pId]);
            this->pRepsInIds[l][pId].reserve(this->pRepsIn_sizes[l][pId]);
            this->pRepsAftIds[l][pId].reserve(this->pRepsAft_sizes[l][pId]);
            this->pOrgsInTimestamps[l][pId].reserve(this->pOrgsIn_sizes[l][pId]);
            this->pOrgsAftTimestamps[l][pId].reserve(this->pOrgsAft_sizes[l][pId]);
            this->pRepsInTimestamps[l][pId].reserve(this->pRepsIn_sizes[l][pId]);
            this->pRepsAftTimestamps[l][pId].reserve(this->pRepsAft_sizes[l][pId]);
        }
    }
    
    // Step 3: fill partitions.
    for (const Record &r : R)
        this->updatePartitions(r);
    
    // Free auxiliary memory.
    for (auto l = 0; l < this->height; l++)
    {
        free(this->pOrgsIn_sizes[l]);
        free(this->pOrgsAft_sizes[l]);
        free(this->pRepsIn_sizes[l]);
        free(this->pRepsAft_sizes[l]);
    }
    free(this->pOrgsIn_sizes);
    free(this->pOrgsAft_sizes);
    free(this->pRepsIn_sizes);
    free(this->pRepsAft_sizes);
}


void HINT_M_Dynamic::getStats()
{
    size_t sum = 0;


    this->numPartitions = this->numEmptyPartitions = 0;
    for (auto l = 0; l < this->height; l++)
    {
        auto cnt = pow(2, this->numBits-l);
        
        this->numPartitions += cnt;
        for (int pid = 0; pid < cnt; pid++)
        {
            this->numOriginalsIn  += this->pOrgsInIds[l][pid].size();
            this->numOriginalsAft += this->pOrgsAftIds[l][pid].size();
            this->numReplicasIn   += this->pRepsInIds[l][pid].size();
            this->numReplicasAft  += this->pRepsAftIds[l][pid].size();
            if ((this->pOrgsInIds[l][pid].empty()) && (this->pOrgsAftIds[l][pid].empty()) && (this->pRepsInIds[l][pid].empty()) && (this->pRepsAftIds[l][pid].empty()))
                this->numEmptyPartitions++;
        }
    }
    
    this->avgPartitionSize = (float)(this->numIndexedRecords+this->numReplicasIn+this->numReplicasAft)/(this->numPartitions-numEmptyPartitions);
}


void HINT_M_Dynamic::print(char c)
{
    for (auto l = 0; l < this->height; l++)
    {
        auto cnt = pow(2, this->numBits-l);
        
        cout << "Level " << l << " contaning " << cnt << " partition(s)" << endl;
        for (auto p = 0; p < cnt; p++)
        {
            cout << "\tPartition " << p << endl;
            cout << "\t\tOrgsIn[" << p << "] (" << this->pOrgsInIds[l][p].size() << "):";
            for (auto i = 0; i < this->pOrgsInIds[l][p].size(); i++)
            {
                cout << " " << c << this->pOrgsInIds[l][p][i] << "[" << this->pOrgsInTimestamps[l][p][i].first << ".." << this->pOrgsInTimestamps[l][p][i].second << "]";
            }
            cout << endl;
            cout << "\t\tOrgsAft[" << p << "] (" << this->pOrgsAftIds[l][p].size() << "):";
            for (auto i = 0; i < this->pOrgsAftIds[l][p].size(); i++)
            {
                cout << " " << c << this->pOrgsAftIds[l][p][i] << "[" << this->pOrgsAftTimestamps[l][p][i].first << ".." << this->pOrgsAftTimestamps[l][p][i].second << "]";
            }
            cout << endl;
            cout << "\t\tRepsIn[" << p << "] (" << this->pRepsInIds[l][p].size() << "):";
            for (auto i = 0; i < this->pRepsInIds[l][p].size(); i++)
            {
                cout << " " << c << this->pRepsInIds[l][p][i] << "[" << this->pRepsInTimestamps[l][p][i].first << ".." << this->pRepsInTimestamps[l][p][i].second << "]";
            }
            cout << endl;
            cout << "\t\tRepsAft[" << p << "] (" << this->pRepsAftIds[l][p].size() << "):";
            for (auto i = 0; i < this->pRepsAftIds[l][p].size(); i++)
            {
                cout << " " << c << this->pRepsAftIds[l][p][i] << "[" << this->pRepsAftTimestamps[l][p][i].first << ".." << this->pRepsAftTimestamps[l][p][i].second << "]";
            }
            cout << endl;
        }
    }
}


// Querying
size_t HINT_M_Dynamic::execute_pureTimeTravel(RangeQuery Q)
{
    size_t result = 0;
    vector<pair<Timestamp, Timestamp> >::iterator iter, iterStart, iterEnd;
    RelationIdIterator iterI, iterIStart, iterIEnd;
    Timestamp a = Q.start >> (this->maxBits-this->numBits); // prefix
    Timestamp b = Q.end   >> (this->maxBits-this->numBits); // prefix
    bool foundzero = false;
    bool foundone = false;
    

    for (auto l = 0; l < this->numBits; l++)
    {
        if (foundone && foundzero)
        {
            // Partition totally covers lowest-level partition range that includes query range
            // all contents are guaranteed to be results
            
            // Handle the partition that contains a: consider both originals and replicas
            iterIStart = this->pRepsInIds[l][a].begin();
            iterIEnd = this->pRepsInIds[l][a].end();
            for (iterI = iterIStart; iterI != iterIEnd; iterI++)
            {
#ifdef WORKLOAD_COUNT
                result++;
#else
                result ^= (*iterI);
#endif
            }
            iterIStart =this->pRepsAftIds[l][a].begin();
            iterIEnd = this->pRepsAftIds[l][a].end();
            for (iterI = iterIStart; iterI != iterIEnd; iterI++)
            {
#ifdef WORKLOAD_COUNT
                result++;
#else
                result ^= (*iterI);
#endif
            }
            
            // Handle rest: consider only originals
            for (auto j = a; j <= b; j++)
            {
                iterIStart = this->pOrgsInIds[l][j].begin();
                iterIEnd = this->pOrgsInIds[l][j].end();
                for (iterI = iterIStart; iterI != iterIEnd; iterI++)
                {
#ifdef WORKLOAD_COUNT
                    result++;
#else
                    result ^= (*iterI);
#endif
                }
                iterIStart = this->pOrgsAftIds[l][j].begin();
                iterIEnd = this->pOrgsAftIds[l][j].end();
                for (iterI = iterIStart; iterI != iterIEnd; iterI++)
                {
#ifdef WORKLOAD_COUNT
                    result++;
#else
                    result ^= (*iterI);
#endif
                }
            }
        }
        else
        {
            // Comparisons needed
            
            // Handle the partition that contains a: consider both originals and replicas, comparisons needed
            if (a == b)
            {
                // Special case when query overlaps only one partition, Lemma 3
                if (!foundzero && !foundone)
                {
                    iterI = this->pOrgsInIds[l][a].begin();
                    iterStart = this->pOrgsInTimestamps[l][a].begin();
                    iterEnd = this->pOrgsInTimestamps[l][a].end();
                    for (iter = iterStart; iter != iterEnd; iter++)
                    {
                        if ((iter->first <= Q.end) && (Q.start <= iter->second))
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                        iterI++;
                    }
                    iterI = this->pOrgsAftIds[l][a].begin();
                    iterStart = this->pOrgsAftTimestamps[l][a].begin();
                    iterEnd = this->pOrgsAftTimestamps[l][a].end();
                    for (iter = iterStart; iter != iterEnd; iter++)
                    {
                        if (iter->first <= Q.end)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                        iterI++;
                    }
                }
                else if (foundzero)
                {
                    iterI = this->pOrgsInIds[l][a].begin();
                    iterStart = this->pOrgsInTimestamps[l][a].begin();
                    iterEnd = this->pOrgsInTimestamps[l][a].end();
                    for (iter = iterStart; iter != iterEnd; iter++)
                    {
                        if (iter->first <= Q.end)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                        iterI++;
                    }
                    iterI = this->pOrgsAftIds[l][a].begin();
                    iterStart = this->pOrgsAftTimestamps[l][a].begin();
                    iterEnd = this->pOrgsAftTimestamps[l][a].end();
                    for (iter = iterStart; iter != iterEnd; iter++)
                    {
                        if (iter->first <= Q.end)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                        iterI++;
                    }
                }
                else if (foundone)
                {
                    iterI = this->pOrgsInIds[l][a].begin();
                    iterStart = this->pOrgsInTimestamps[l][a].begin();
                    iterEnd = this->pOrgsInTimestamps[l][a].end();
                    for (iter = iterStart; iter != iterEnd; iter++)
                    {
                        if (Q.start <= iter->second)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                        iterI++;
                    }
                    iterIStart = this->pOrgsAftIds[l][a].begin();
                    iterIEnd = this->pOrgsAftIds[l][a].end();
                    for (iterI = iterIStart; iterI != iterIEnd; iterI++)
                    {
#ifdef WORKLOAD_COUNT
                        result++;
#else
                        result ^= (*iterI);
#endif
                    }
                }
            }
            else
            {
                // Lemma 1
                if (!foundzero)
                {
                    iterI = this->pOrgsInIds[l][a].begin();
                    iterStart = this->pOrgsInTimestamps[l][a].begin();
                    iterEnd = this->pOrgsInTimestamps[l][a].end();
                    for (iter = iterStart; iter != iterEnd; iter++)
                    {
                        if (Q.start <= iter->second)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                        iterI++;
                    }
                }
                else
                {
                    iterIStart = this->pOrgsInIds[l][a].begin();
                    iterIEnd = this->pOrgsInIds[l][a].end();
                    for (iterI = iterIStart; iterI != iterIEnd; iterI++)
                    {
#ifdef WORKLOAD_COUNT
                        result++;
#else
                        result ^= (*iterI);
#endif
                    }
                }
                iterI = this->pOrgsAftIds[l][a].begin();
                iterStart = this->pOrgsAftTimestamps[l][a].begin();
                iterEnd = this->pOrgsAftTimestamps[l][a].end();
                for (iter = iterStart; iter != iterEnd; iter++)
                {
#ifdef WORKLOAD_COUNT
                    result++;
#else
                    result ^= (*iterI);
#endif
                    iterI++;
                }
            }

            // Lemma 1, 3
            if (!foundzero)
            {
                iterI = this->pRepsInIds[l][a].begin();
                iterStart = this->pRepsInTimestamps[l][a].begin();
                iterEnd = this->pRepsInTimestamps[l][a].end();
                for (iter = iterStart; iter != iterEnd; iter++)
                {
                    if (Q.start <= iter->second)
                    {
#ifdef WORKLOAD_COUNT
                        result++;
#else
                        result ^= (*iterI);
#endif
                    }
                    iterI++;
                }
            }
            else
            {
                iterIStart = this->pRepsInIds[l][a].begin();
                iterIEnd = this->pRepsInIds[l][a].end();
                for (iterI = iterIStart; iterI != iterIEnd; iterI++)
                {
#ifdef WORKLOAD_COUNT
                    result++;
#else
                    result ^= (*iterI);
#endif
                }
            }

            iterIStart = this->pRepsAftIds[l][a].begin();
            iterIEnd = this->pRepsAftIds[l][a].end();
            for (iterI = iterIStart; iterI != iterIEnd; iterI++)
            {
#ifdef WORKLOAD_COUNT
                result++;
#else
                result ^= (*iterI);
#endif
            }

            if (a < b)
            {
                if (!foundone)
                {
                    // Handle the rest before the partition that contains b: consider only originals, no comparisons needed
                    for (auto j = a+1; j < b; j++)
                    {
                        iterIStart = this->pOrgsInIds[l][j].begin();
                        iterIEnd = this->pOrgsInIds[l][j].end();
                        for (iterI = iterIStart; iterI != iterIEnd; iterI++)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                        iterIStart = this->pOrgsAftIds[l][j].begin();
                        iterIEnd = this->pOrgsAftIds[l][j].end();
                        for (iterI = iterIStart; iterI != iterIEnd; iterI++)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                    }

                    // Handle the partition that contains b: consider only originals, comparisons needed
                    iterI = this->pOrgsInIds[l][b].begin();
                    iterStart = this->pOrgsInTimestamps[l][b].begin();
                    iterEnd = this->pOrgsInTimestamps[l][b].end();
                    for (iter = iterStart; iter != iterEnd; iter++)
                    {
                        if (iter->first <= Q.end)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                        iterI++;
                    }
                    iterI = this->pOrgsAftIds[l][b].begin();
                    iterStart = this->pOrgsAftTimestamps[l][b].begin();
                    iterEnd = this->pOrgsAftTimestamps[l][b].end();
                    for (iter = iterStart; iter != iterEnd; iter++)
                    {
                        if (iter->first <= Q.end)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                        iterI++;
                    }
                }
                else
                {
                    for (auto j = a+1; j <= b; j++)
                    {
                        iterIStart = this->pOrgsInIds[l][j].begin();
                        iterIEnd = this->pOrgsInIds[l][j].end();
                        for (iterI = iterIStart; iterI != iterIEnd; iterI++)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                        iterIStart = this->pOrgsAftIds[l][j].begin();
                        iterIEnd = this->pOrgsAftIds[l][j].end();
                        for (iterI = iterIStart; iterI != iterIEnd; iterI++)
                        {
#ifdef WORKLOAD_COUNT
                            result++;
#else
                            result ^= (*iterI);
#endif
                        }
                    }
                }
            }
            
            if ((!foundone) && (b%2)) //last bit of b is 1
                foundone = 1;
            if ((!foundzero) && (!(a%2))) //last bit of a is 0
                foundzero = 1;
        }
        a >>= 1; // a = a div 2
        b >>= 1; // b = b div 2
    }
    
    // Handle root.
    if (foundone && foundzero)
    {
        // All contents are guaranteed to be results
        iterIStart = this->pOrgsInIds[this->numBits][0].begin();
        iterIEnd = this->pOrgsInIds[this->numBits][0].end();
        for (iterI = iterIStart; iterI != iterIEnd; iterI++)
        {
#ifdef WORKLOAD_COUNT
            result++;
#else
            result ^= (*iterI);
#endif
        }
    }
    else
    {
        // Comparisons needed
        iterI = this->pOrgsInIds[this->numBits][0].begin();
        iterStart = this->pOrgsInTimestamps[this->numBits][0].begin();
        iterEnd = this->pOrgsInTimestamps[this->numBits][0].end();
        for (iter = iterStart; iter != iterEnd; iter++)
        {
            if ((iter->first <= Q.end) && (Q.start <= iter->second))
            {
#ifdef WORKLOAD_COUNT
                result++;
#else
                result ^= (*iterI);
#endif
            }
            iterI++;
        }
    }
    
    return result;
}


// Updating
void HINT_M_Dynamic::insert(const Record &r)
{
//    cout << "Insert "; r.print('r');
//    cout << "======================" << endl;
//    cout << "partitionExtent: " << this->leafPartitionExtent << endl;

    
    // Check if the partitions where r will ne inserted already exist.
    if (this->gend >= r.end)
    {
//        cout << "\tgStart         : " << this->gstart << endl;
//        cout << "\tgEnd           : " << this->gend << endl;
//        cout << "\tnumBits        : " << this->numBits << endl;
//        cout << "\tmaxBits        : " << this->maxBits << endl;
        this->updatePartitions(r);
    }
    else    // Need to grow the index
    {
        // Update index parameters, calculate how many extra partitions are needed.
//        cout<< ceil(log2(ceil((r.end-this->gstart)/(float)partitionExtent))) << endl;
//
//        auto extraBitsNeeded = int(log2(r.end-this->gstart)+1)-this->maxBits;
//
//        cout << "\tint(log2(r.end-this->gstart)+1) = " << int(log2(r.end-this->gstart)+1) << endl;
//        cout << "\textraBitsNeeded = " << extraBitsNeeded << endl;
//        this->numBits += extraBitsNeeded;
//        this->gend = partitionExtent*pow(2, this->numBits);
//        this->maxBits = int(log2(this->gend-this->gstart)+1);
//        this->height = this->numBits+1;
        this->numBits = ceil(log2(ceil((r.end-this->gstart)/(float)this->leafPartitionExtent)));
        this->gend = this->leafPartitionExtent*pow(2, this->numBits);
        this->maxBits = int(log2(this->gend-this->gstart)+1);
        this->height = this->numBits+1;
        
//        cout << "\tgStart     : " << this->gstart << endl;
//        cout << "\tNew gEnd   : " << this->gend << endl;
//        cout << "\tNew numBits: " << this->numBits << endl;
//        cout << "\tNew maxBits: " << this->maxBits << endl;
//        cout << "\tNew height : " << this->height << endl;
        
        // Grow - increase the height of the index.
        this->pOrgsInIds.resize(this->height);
        this->pOrgsInTimestamps.resize(this->height);
        this->pOrgsAftIds.resize(this->height);
        this->pOrgsAftTimestamps.resize(this->height);
        this->pRepsInIds.resize(this->height);
        this->pRepsInTimestamps.resize(this->height);
        this->pRepsAftIds.resize(this->height);
        this->pRepsAftTimestamps.resize(this->height);
        for (auto l = 0; l < this->height; l++)
        {
            auto cnt = (int)(pow(2, this->numBits-l));

            this->pOrgsInIds[l].resize(cnt);
            this->pOrgsInTimestamps[l].resize(cnt);
            this->pOrgsAftIds[l].resize(cnt);
            this->pOrgsAftTimestamps[l].resize(cnt);
            this->pRepsInIds[l].resize(cnt);
            this->pRepsInTimestamps[l].resize(cnt);
            this->pRepsAftIds[l].resize(cnt);
            this->pRepsAftTimestamps[l].resize(cnt);
        }
        this->updatePartitions(r);
    }
    
    this->numIndexedRecords++;
//    cout << endl;
}
