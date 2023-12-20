#ifndef _RELATION_H_
#define _RELATION_H_

#include "../def_global.h"



class Record
{
public:
    RecordId id;
    Timestamp start;
    Timestamp end;
    int secondAttr;

    Record();
    Record(RecordId id, Timestamp start, Timestamp end);
    Record(RecordId id, Timestamp start, Timestamp end, int secondAttr);
    
    bool operator < (const Record &rhs) const;
    bool operator >= (const Record &rhs) const;
    void print(const char c) const;
    ~Record();
};

inline bool CompareRecordsByEnd(const Record &lhs, const Record &rhs)
{
    if (lhs.end == rhs.end)
        return (lhs.id < rhs.id);
    else
        return (lhs.end < rhs.end);
}




class RecordStart
{
public:
    RecordId id;
    Timestamp start;
    
    RecordStart();
    RecordStart(RecordId id, Timestamp start);
    
    bool operator < (const RecordStart &rhs) const;
    bool operator >= (const RecordStart &rhs) const;
    void print(const char c) const;
    ~RecordStart();
};



// copy of RecordStart
class RecordEnd
{
public:
    RecordId id;
    Timestamp end;
    
    RecordEnd();
    RecordEnd(RecordId id, Timestamp end);
    
    bool operator < (const RecordEnd &rhs) const;
    bool operator >= (const RecordEnd &rhs) const;
    void print(const char c) const;
    ~RecordEnd();
};



class TimestampPair
{
public:
    Timestamp start;
    Timestamp end;
    
    TimestampPair();
    TimestampPair(Timestamp start, Timestamp end);
    
    bool operator < (const TimestampPair &rhs) const;
    bool operator >= (const TimestampPair &rhs) const;
    void print(const char c) const;
    ~TimestampPair();
};



// Descending order
inline bool CompareByEnd(const Record &lhs, const Record &rhs)
{
    if (lhs.end == rhs.end)
        return (lhs.id < rhs.id);
    else
        return (lhs.end < rhs.end);
};



class Relation : public vector<Record>
{
public:
    Timestamp gstart;
    Timestamp gend;
    Timestamp longestRecord;
    float avgRecordExtent;
    Relation();
    Relation(Relation &R);
    void load(const char *filename);
    void sortByStart();
    void sortByEnd();
    void print(char c);
    ~Relation();
    
    // Querying
    // Basic predicates of Allen's algebra
    size_t execute_Equals(RangeQuery Q);
    size_t execute_Starts(RangeQuery Q);
    size_t execute_Started(RangeQuery Q);
    size_t execute_Finishes(RangeQuery Q);
    size_t execute_Finished(RangeQuery Q);
    size_t execute_Meets(RangeQuery Q);
    size_t execute_Met(RangeQuery Q);
    size_t execute_Overlaps(RangeQuery Q);
    size_t execute_Overlapped(RangeQuery Q);
    size_t execute_Contains(RangeQuery Q);
    size_t execute_Contained(RangeQuery Q);
    size_t execute_Precedes(RangeQuery Q);
    size_t execute_Preceded(RangeQuery Q);

    // Generalized predicate, ACM SIGMOD'22 gOverlaps
    size_t execute_gOverlaps(StabbingQuery Q);
    size_t execute_gOverlaps(RangeQuery Q);
    size_t executeTimeTravel_greaterthan(RangeQuery Q, int secondAttrConstraint);
    size_t executeTimeTravel_lowerthan(RangeQuery Q, int secondAttrConstraint);
    size_t executeTimeTravel(RangeQuery Q, int lowerConstraint, int upperConstraint);
};
typedef Relation::const_iterator RelationIterator;



class RelationStart : public vector<RecordStart>
{
public:
    RelationStart();
    void sortByStart();
    void print(char c);
    ~RelationStart();
};
typedef RelationStart::const_iterator RelationStartIterator;



// Copy of RelationStart
class RelationEnd : public vector<RecordEnd>
{
public:
    RelationEnd();
    void sortByEnd();
    void print(char c);
    ~RelationEnd();
};
typedef RelationEnd::const_iterator RelationEndIterator;



class RelationId : public vector<RecordId>
{
public:
    RelationId();
    void print(char c);
    ~RelationId();
};
typedef RelationId::iterator RelationIdIterator;
#endif //_RELATION_H_
