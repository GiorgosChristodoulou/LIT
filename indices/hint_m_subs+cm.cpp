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


inline void HINT_M_Subs_CM::updatePartitions(const Record &r)
//{
//    int level = 0;
//    Timestamp a = r.start >> (this->maxBits-this->numBits);
//    Timestamp b = r.end   >> (this->maxBits-this->numBits);
//    Timestamp prevb;
//    int firstfound = 0, lastfound = 0;
//
//
//    while (level < this->height && a <= b)
//    {
//        if (a%2)
//        { //last bit of a is 1
//            if (firstfound)
//            {
//                if ((a == b) && (!lastfound))
//                {
//                    this->pRepsInTmp[level][a][this->pRepsIn_sizes[level][a]] = Record(r.id, r.start, r.end);
//                    this->pRepsIn_sizes[level][a]++;
//                    lastfound = 1;
//                }
//                else
//                {
//                    this->pRepsAftTmp[level][a][this->pRepsAft_sizes[level][a]] = Record(r.id, r.start, r.end);
//                    this->pRepsAft_sizes[level][a]++;
//                }
//            }
//            else
//            {
//                if ((a == b) && (!lastfound))
//                {
//                    this->pOrgsInTmp[level][a][this->pOrgsIn_sizes[level][a]] = Record(r.id, r.start, r.end);
//                    this->pOrgsIn_sizes[level][a]++;
//                }
//                else
//                {
//                    this->pOrgsAftTmp[level][a][this->pOrgsAft_sizes[level][a]] = Record(r.id, r.start, r.end);
//                    this->pOrgsAft_sizes[level][a]++;
//                }
//                firstfound = 1;
//            }
//            a++;
//        }
//        if (!(b%2))
//        { //last bit of b is 0
//            prevb = b;
//            b--;
//            if ((!firstfound) && b < a)
//            {
//                if (!lastfound)
//                {
//                    this->pOrgsInTmp[level][prevb][this->pOrgsIn_sizes[level][prevb]] = Record(r.id, r.start, r.end);
//                    this->pOrgsIn_sizes[level][prevb]++;
//                }
//                else
//                {
//                    this->pOrgsAftTmp[level][prevb][this->pOrgsAft_sizes[level][prevb]] = Record(r.id, r.start, r.end);
//                    this->pOrgsAft_sizes[level][prevb]++;
//                }
//            }
//            else
//            {
//                if (!lastfound)
//                {
//                    this->pRepsInTmp[level][prevb][this->pRepsIn_sizes[level][prevb]] = Record(r.id, r.start, r.end);
//                    this->pRepsIn_sizes[level][prevb]++;
//                    lastfound = 1;
//                }
//                else
//                {
//                    this->pRepsAftTmp[level][prevb][this->pRepsAft_sizes[level][prevb]] = Record(r.id, r.start, r.end);
//                    this->pRepsAft_sizes[level][prevb]++;
//                }
//            }
//        }
//        a >>= 1; // a = a div 2
//        b >>= 1; // b = b div 2
//        level++;
//    }
//}
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
                    this->pRepsInIds[level][a][this->pRepsIn_sizes[level][a]] = r.id;
                    this->pRepsInTimestamps[level][a][this->pRepsIn_sizes[level][a]].first = r.start;
                    this->pRepsInTimestamps[level][a][this->pRepsIn_sizes[level][a]].second = r.end;
                    this->pRepsIn_sizes[level][a]++;
                    lastfound = 1;
                }
                else
                {
                    this->pRepsAftIds[level][a][this->pRepsAft_sizes[level][a]] = r.id;
                    this->pRepsAftTimestamps[level][a][this->pRepsAft_sizes[level][a]].first = r.start;
                    this->pRepsAftTimestamps[level][a][this->pRepsAft_sizes[level][a]].second = r.start;
                    this->pRepsAft_sizes[level][a]++;
                }
            }
            else
            {
                if ((a == b) && (!lastfound))
                {
                    this->pOrgsInIds[level][a][this->pOrgsIn_sizes[level][a]] = r.id;
                    this->pOrgsInTimestamps[level][a][this->pOrgsIn_sizes[level][a]].first = r.start;
                    this->pOrgsInTimestamps[level][a][this->pOrgsIn_sizes[level][a]].second = r.end;
                    this->pOrgsIn_sizes[level][a]++;
                }
                else
                {
                    this->pOrgsAftIds[level][a][this->pOrgsAft_sizes[level][a]] = r.id;
                    this->pOrgsAftTimestamps[level][a][this->pOrgsAft_sizes[level][a]].first = r.start;
                    this->pOrgsAftTimestamps[level][a][this->pOrgsAft_sizes[level][a]].second = r.end;
                    this->pOrgsAft_sizes[level][a]++;
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
                    this->pOrgsInIds[level][prevb][this->pOrgsIn_sizes[level][prevb]] = r.id;
                    this->pOrgsInTimestamps[level][prevb][this->pOrgsIn_sizes[level][prevb]].first = r.start;
                    this->pOrgsInTimestamps[level][prevb][this->pOrgsIn_sizes[level][prevb]].second = r.end;
                    this->pOrgsIn_sizes[level][prevb]++;
                }
                else
                {
                    this->pOrgsAftIds[level][prevb][this->pOrgsAft_sizes[level][prevb]] = r.id;
                    this->pOrgsAftTimestamps[level][prevb][this->pOrgsAft_sizes[level][prevb]].first = r.start;
                    this->pOrgsAftTimestamps[level][prevb][this->pOrgsAft_sizes[level][prevb]].second = r.end;
                    this->pOrgsAft_sizes[level][prevb]++;
                }
            }
            else
            {
                if (!lastfound)
                {
                    this->pRepsInIds[level][prevb][this->pRepsIn_sizes[level][prevb]] = r.id;
                    this->pRepsInTimestamps[level][prevb][this->pRepsIn_sizes[level][prevb]].first = r.start;
                    this->pRepsInTimestamps[level][prevb][this->pRepsIn_sizes[level][prevb]].second = r.end;
                    this->pRepsIn_sizes[level][prevb]++;
                    lastfound = 1;
                }
                else
                {
                    this->pRepsAftIds[level][prevb][this->pRepsAft_sizes[level][prevb]] = r.id;
                    this->pRepsAftTimestamps[level][prevb][this->pRepsAft_sizes[level][prevb]].first = r.start;
                    this->pRepsAftTimestamps[level][prevb][this->pRepsAft_sizes[level][prevb]].second = r.end;
                    this->pRepsAft_sizes[level][prevb]++;
                }
            }
        }
        a >>= 1; // a = a div 2
        b >>= 1; // b = b div 2
        level++;
    }
}


HINT_M_Subs_CM::HINT_M_Subs_CM(const Relation &R, const unsigned int numBits, const unsigned int maxBits) : HierarchicalIndex(R, numBits, maxBits)
//{
//    // Step 1: one pass to count the contents inside each partition.
//    this->pOrgsIn_sizes  = (RecordId **)malloc(this->height*sizeof(RecordId *));
//    this->pOrgsAft_sizes = (RecordId **)malloc(this->height*sizeof(RecordId *));
//    this->pRepsIn_sizes  = (size_t **)malloc(this->height*sizeof(size_t *));
//    this->pRepsAft_sizes = (size_t **)malloc(this->height*sizeof(size_t *));
//
//    for (auto l = 0; l < this->height; l++)
//    {
//        auto cnt = (int)(pow(2, this->numBits-l));
//
//        //calloc allocates memory and sets each counter to 0
//        this->pOrgsIn_sizes[l]  = (RecordId *)calloc(cnt, sizeof(RecordId));
//        this->pOrgsAft_sizes[l] = (RecordId *)calloc(cnt, sizeof(RecordId));
//        this->pRepsIn_sizes[l]  = (size_t *)calloc(cnt, sizeof(size_t));
//        this->pRepsAft_sizes[l] = (size_t *)calloc(cnt, sizeof(size_t));
//    }
//
//    for (const Record &r : R)
//        this->updateCounters(r);
//
//    // Step 2: allocate necessary memory.
//    this->pOrgsInTmp  = new Relation*[this->height];
//    this->pOrgsAftTmp = new Relation*[this->height];
//    this->pRepsInTmp  = new Relation*[this->height];
//    this->pRepsAftTmp = new Relation*[this->height];
//    for (auto l = 0; l < this->height; l++)
//    {
//        auto cnt = (int)(pow(2, this->numBits-l));
//
//        this->pOrgsInTmp[l]  = new Relation[cnt];
//        this->pOrgsAftTmp[l] = new Relation[cnt];
//        this->pRepsInTmp[l]  = new Relation[cnt];
//        this->pRepsAftTmp[l] = new Relation[cnt];
//
//        for (auto pId = 0; pId < cnt; pId++)
//        {
//            this->pOrgsInTmp[l][pId].resize(this->pOrgsIn_sizes[l][pId]);
//            this->pOrgsAftTmp[l][pId].resize(this->pOrgsAft_sizes[l][pId]);
//            this->pRepsInTmp[l][pId].resize(this->pRepsIn_sizes[l][pId]);
//            this->pRepsAftTmp[l][pId].resize(this->pRepsAft_sizes[l][pId]);
//        }
//    }
//    for (auto l = 0; l < this->height; l++)
//    {
//        auto cnt = (int)(pow(2, this->numBits-l));
//
//        memset(this->pOrgsIn_sizes[l], 0, cnt*sizeof(RecordId));
//        memset(this->pOrgsAft_sizes[l], 0, cnt*sizeof(RecordId));
//        memset(this->pRepsIn_sizes[l], 0, cnt*sizeof(size_t));
//        memset(this->pRepsAft_sizes[l], 0, cnt*sizeof(size_t));
//    }
//
//    // Step 3: fill partitions.
//    for (const Record &r : R)
//        this->updatePartitions(r);
//
//    // TODO: we don't free if gaps are used
//    // Free auxiliary memory.
//    for (auto l = 0; l < this->height; l++)
//    {
//        free(this->pOrgsIn_sizes[l]);
//        free(this->pOrgsAft_sizes[l]);
//        free(this->pRepsIn_sizes[l]);
//        free(this->pRepsAft_sizes[l]);
//    }
//    free(this->pOrgsIn_sizes);
//    free(this->pOrgsAft_sizes);
//    free(this->pRepsIn_sizes);
//    free(this->pRepsAft_sizes);
//
//    // Copy and free auxiliary memory.
//    this->pOrgsInIds  = new RelationId*[this->height];
//    this->pOrgsAftIds = new RelationId*[this->height];
//    this->pRepsInIds  = new RelationId*[this->height];
//    this->pRepsAftIds = new RelationId*[this->height];
//    this->pOrgsInTimestamps  = new vector<pair<Timestamp, Timestamp> >*[this->height];
//    this->pOrgsAftTimestamps = new vector<pair<Timestamp, Timestamp> >*[this->height];
//    this->pRepsInTimestamps  = new vector<pair<Timestamp, Timestamp> >*[this->height];
//    this->pRepsAftTimestamps = new vector<pair<Timestamp, Timestamp> >*[this->height];
//    for (auto l = 0; l < this->height; l++)
//    {
//        auto cnt = (int)(pow(2, this->numBits-l));
//
//        this->pOrgsInIds[l]  = new RelationId[cnt];
//        this->pOrgsAftIds[l] = new RelationId[cnt];
//        this->pRepsInIds[l]  = new RelationId[cnt];
//        this->pRepsAftIds[l] = new RelationId[cnt];
//        this->pOrgsInTimestamps[l]  = new vector<pair<Timestamp, Timestamp> >[cnt];
//        this->pOrgsAftTimestamps[l] = new vector<pair<Timestamp, Timestamp> >[cnt];
//        this->pRepsInTimestamps[l]  = new vector<pair<Timestamp, Timestamp> >[cnt];
//        this->pRepsAftTimestamps[l] = new vector<pair<Timestamp, Timestamp> >[cnt];
//
//        for (auto pId = 0; pId < cnt; pId++)
//        {
//            auto cnt = this->pOrgsInTmp[l][pId].size();
//            this->pOrgsInIds[l][pId].resize(cnt);
//            this->pOrgsInTimestamps[l][pId].resize(cnt);
//            for (auto j = 0; j < cnt; j++)
//            {
//                this->pOrgsInIds[l][pId][j] = this->pOrgsInTmp[l][pId][j].id;
//                this->pOrgsInTimestamps[l][pId][j].first  = this->pOrgsInTmp[l][pId][j].start;
//                this->pOrgsInTimestamps[l][pId][j].second = this->pOrgsInTmp[l][pId][j].end;
//            }
//
//            cnt = this->pOrgsAftTmp[l][pId].size();
//            this->pOrgsAftIds[l][pId].resize(cnt);
//            this->pOrgsAftTimestamps[l][pId].resize(cnt);
//            for (auto j = 0; j < cnt; j++)
//            {
//                this->pOrgsAftIds[l][pId][j] = this->pOrgsAftTmp[l][pId][j].id;
//                this->pOrgsAftTimestamps[l][pId][j].first  = this->pOrgsAftTmp[l][pId][j].start;
//                this->pOrgsAftTimestamps[l][pId][j].second = this->pOrgsAftTmp[l][pId][j].end;
//            }
//
//            cnt = this->pRepsInTmp[l][pId].size();
//            this->pRepsInIds[l][pId].resize(cnt);
//            this->pRepsInTimestamps[l][pId].resize(cnt);
//            for (auto j = 0; j < cnt; j++)
//            {
//                this->pRepsInIds[l][pId][j] = this->pRepsInTmp[l][pId][j].id;
//                this->pRepsInTimestamps[l][pId][j].first  = this->pRepsInTmp[l][pId][j].start;
//                this->pRepsInTimestamps[l][pId][j].second = this->pRepsInTmp[l][pId][j].end;
//            }
//
//            cnt = this->pRepsAftTmp[l][pId].size();
//            this->pRepsAftIds[l][pId].resize(cnt);
//            this->pRepsAftTimestamps[l][pId].resize(cnt);
//            for (auto j = 0; j < cnt; j++)
//            {
//                this->pRepsAftIds[l][pId][j] = this->pRepsAftTmp[l][pId][j].id;
//                this->pRepsAftTimestamps[l][pId][j].first  = this->pRepsAftTmp[l][pId][j].start;
//                this->pRepsAftTimestamps[l][pId][j].second = this->pRepsAftTmp[l][pId][j].end;
//            }
//        }
//
//        delete[] this->pOrgsInTmp[l];
//        delete[] this->pOrgsAftTmp[l];
//        delete[] this->pRepsInTmp[l];
//        delete[] this->pRepsAftTmp[l];
//    }
//    delete[] this->pOrgsInTmp;
//    delete[] this->pOrgsAftTmp;
//    delete[] this->pRepsInTmp;
//    delete[] this->pRepsAftTmp;
//}
{
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
    this->pOrgsInIds  = new RelationId*[this->height];
    this->pOrgsAftIds = new RelationId*[this->height];
    this->pRepsInIds  = new RelationId*[this->height];
    this->pRepsAftIds = new RelationId*[this->height];
    this->pOrgsInTimestamps  = new vector<pair<Timestamp, Timestamp> >*[this->height];
    this->pOrgsAftTimestamps = new vector<pair<Timestamp, Timestamp> >*[this->height];
    this->pRepsInTimestamps  = new vector<pair<Timestamp, Timestamp> >*[this->height];
    this->pRepsAftTimestamps = new vector<pair<Timestamp, Timestamp> >*[this->height];
    for (auto l = 0; l < this->height; l++)
    {
        auto cnt = (int)(pow(2, this->numBits-l));
        
        this->pOrgsInIds[l]  = new RelationId[cnt];
        this->pOrgsAftIds[l] = new RelationId[cnt];
        this->pRepsInIds[l]  = new RelationId[cnt];
        this->pRepsAftIds[l] = new RelationId[cnt];
        this->pOrgsInTimestamps[l]  = new vector<pair<Timestamp, Timestamp> >[cnt];
        this->pOrgsAftTimestamps[l] = new vector<pair<Timestamp, Timestamp> >[cnt];
        this->pRepsInTimestamps[l]  = new vector<pair<Timestamp, Timestamp> >[cnt];
        this->pRepsAftTimestamps[l] = new vector<pair<Timestamp, Timestamp> >[cnt];

        for (auto pId = 0; pId < cnt; pId++)
        {
            this->pOrgsInIds[l][pId].resize(this->pOrgsIn_sizes[l][pId]);
            this->pOrgsAftIds[l][pId].resize(this->pOrgsAft_sizes[l][pId]);
            this->pRepsInIds[l][pId].resize(this->pRepsIn_sizes[l][pId]);
            this->pRepsAftIds[l][pId].resize(this->pRepsAft_sizes[l][pId]);
            this->pOrgsInTimestamps[l][pId].resize(this->pOrgsIn_sizes[l][pId]);
            this->pOrgsAftTimestamps[l][pId].resize(this->pOrgsAft_sizes[l][pId]);
            this->pRepsInTimestamps[l][pId].resize(this->pRepsIn_sizes[l][pId]);
            this->pRepsAftTimestamps[l][pId].resize(this->pRepsAft_sizes[l][pId]);
        }
    }
    for (auto l = 0; l < this->height; l++)
    {
        auto cnt = (int)(pow(2, this->numBits-l));
        
        memset(this->pOrgsIn_sizes[l], 0, cnt*sizeof(RecordId));
        memset(this->pOrgsAft_sizes[l], 0, cnt*sizeof(RecordId));
        memset(this->pRepsIn_sizes[l], 0, cnt*sizeof(size_t));
        memset(this->pRepsAft_sizes[l], 0, cnt*sizeof(size_t));
    }
    
    // Step 3: fill partitions.
    for (const Record &r : R)
        this->updatePartitions(r);
        
    // free auxiliary memory.
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


void HINT_M_Subs_CM::getStats()
{
    size_t sum = 0;
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


HINT_M_Subs_CM::~HINT_M_Subs_CM()
{
    for (auto l = 0; l < this->height; l++)
    {
        delete[] this->pOrgsInIds[l];
        delete[] this->pOrgsInTimestamps[l];
        delete[] this->pOrgsAftIds[l];
        delete[] this->pOrgsAftTimestamps[l];
        delete[] this->pRepsInIds[l];
        delete[] this->pRepsInTimestamps[l];
        delete[] this->pRepsAftIds[l];
    }
    
    delete[] this->pOrgsInIds;
    delete[] this->pOrgsInTimestamps;
    delete[] this->pOrgsAftIds;
    delete[] this->pOrgsAftTimestamps;
    delete[] this->pRepsInIds;
    delete[] this->pRepsInTimestamps;
    delete[] this->pRepsAftIds;
}


// Querying
size_t HINT_M_Subs_CM::executeBottomUp_gOverlaps(RangeQuery Q)
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
