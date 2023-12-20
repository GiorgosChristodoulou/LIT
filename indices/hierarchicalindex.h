#ifndef _HIERARCHICALINDEX_H_
#define _HIERARCHICALINDEX_H_

#include "../def_global.h"
#include "../containers/relation.h"



// Framework
class HierarchicalIndex
{
protected:
    size_t numIndexedRecords;
    unsigned int maxBits;
    unsigned int height;
    
    // Construction
    virtual inline void updateCounters(const Record &r) {};
    virtual inline void updatePartitions(const Record &r) {};

public:
    unsigned int numBits;

    // Statistics
    size_t numPartitions;
    size_t numEmptyPartitions;
    float avgPartitionSize;
    size_t numOriginals, numReplicas;
    size_t numOriginalsIn, numOriginalsAft, numReplicasIn, numReplicasAft;


    // Construction
    HierarchicalIndex();
    HierarchicalIndex(const Relation &R, const unsigned int numBits, const unsigned int maxBits);
    virtual void print(const char c) {};
    virtual void getStats() {};
    virtual ~HierarchicalIndex() {};
    

    // Querying
    // HINT
    // Basic predicates of Allen's algebra
    virtual size_t execute_Equals(RangeQuery Q) {return 0;};
    virtual size_t execute_Starts(RangeQuery Q) {return 0;};
    virtual size_t execute_Started(RangeQuery Q) {return 0;};
    virtual size_t execute_Finishes(RangeQuery Q) {return 0;};
    virtual size_t execute_Finished(RangeQuery Q) {return 0;};
    virtual size_t execute_Meets(RangeQuery Q) {return 0;};
    virtual size_t execute_Met(RangeQuery Q) {return 0;};
    virtual size_t execute_Overlaps(RangeQuery Q) {return 0;};
    virtual size_t execute_Overlapped(RangeQuery Q) {return 0;};
    virtual size_t execute_Contains(RangeQuery Q) {return 0;};
    virtual size_t execute_Contained(RangeQuery Q) {return 0;};
    virtual size_t execute_Precedes(RangeQuery Q) {return 0;};
    virtual size_t execute_Preceded(RangeQuery Q) {return 0;};
    
    // Generalized predicate, ACM SIGMOD'22 gOverlaps
    virtual size_t execute_gOverlaps(StabbingQuery Q) {return 0;};
    virtual size_t execute_gOverlaps(RangeQuery Q) {return 0;};

    
    // HINT^m
    // Basic predicates of Allen's algebra
    virtual size_t executeBottomUp_Equals(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Starts(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Started(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Finishes(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Finished(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Meets(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Met(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Overlaps(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Overlapped(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Contains(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Contained(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Precedes(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_Preceded(RangeQuery Q) {return 0;};

    // Generalized predicate, ACM SIGMOD'22 gOverlaps
    virtual size_t executeTopDown_gOverlaps(StabbingQuery Q) {return 0;};
    virtual size_t executeTopDown_gOverlaps(RangeQuery Q) {return 0;};
    virtual size_t executeBottomUp_gOverlaps(StabbingQuery Q) {return 0;};
    virtual size_t executeBottomUp_gOverlaps(RangeQuery Q) {return 0;};
    
    
    // Updating
    virtual void insert(const Record &r) {};
};
#endif // _HIERARCHICALINDEX_H_
