#include "hierarchicalindex.h"



HierarchicalIndex::HierarchicalIndex()
{
    this->numPartitions      = 1;
    this->numIndexedRecords  = 0;
    this->numBits            = 0;
    this->maxBits            = 0;
    this->numEmptyPartitions = 1;
    this->avgPartitionSize   = 0;
    this->height             = 1;
    this->numOriginals       = 0;
    this->numReplicas        = 0;
    this->numOriginalsIn     = 0;
    this->numOriginalsAft    = 0;
    this->numReplicasIn      = 0;
    this->numReplicasAft     = 0;
}


HierarchicalIndex::HierarchicalIndex(const Relation &R, const unsigned int numBits, const unsigned int maxBits)
{
    this->numPartitions      = 0;
    this->numIndexedRecords  = R.size();
    this->numBits            = numBits;
    this->maxBits            = maxBits;
    this->numEmptyPartitions = 0;
    this->avgPartitionSize   = 0;
    this->height             = this->numBits+1;
    this->numOriginals       = 0;
    this->numReplicas        = 0;
    this->numOriginalsIn     = 0;
    this->numOriginalsAft    = 0;
    this->numReplicasIn      = 0;
    this->numReplicasAft     = 0;
}
