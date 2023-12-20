#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "../def_global.h"
#include "../containers/relation.h"
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <list>
#include "../containers/enhanced_HashMap.h"


class Buffer
{
public:
    size_t capacity;

    Buffer();
    Buffer(size_t capacity);
    virtual void insert(RecordId id, Timestamp start) {};
    virtual Timestamp remove(RecordId id) {};
    virtual void insert(Buffer &) {};
    virtual void insert_secAttr(RecordId id, Timestamp start, int secAttr){};
    virtual Timestamp remove_secAttr(RecordId id){};
    virtual Timestamp getMaxStart() {};
    virtual RecordId getMaxRecordId() {};
    virtual size_t getSize() {};
    virtual void print(char c) {};
    virtual void destroy() {};

    // Querying
    virtual size_t execute_gOverlaps(RangeQuery Q) { return 0; };
    virtual size_t execute_gOverlaps() { return 0; };

    virtual size_t executeTimeTravel(RangeQuery Q, int secondAttrLowerConstraint, int secondAttrUpperConstraint){ return 0; };
    virtual size_t executeTimeTravel(int secondAttrLowerConstraint, int secondAttrUpperConstraint){ return 0; };
    
    virtual size_t executeTimeTravel_greaterthan(RangeQuery Q, int secondAttrLowerConstraint){ return 0; };
    virtual size_t executeTimeTravel_greaterthan(int secondAttrLowerConstraint){ return 0; };
    
    virtual size_t executeTimeTravel_lowerthan(RangeQuery Q, int secondAttrUpperConstraint){ return 0; };
    virtual size_t executeTimeTravel_lowerthan(int secondAttrUpperConstraint){ return 0; };
};



class Buffer_Map : public Buffer
{
private:
    map<RecordId, Timestamp> entries;

public:
    Buffer_Map();
    Buffer_Map(size_t capacity);
    void insert(RecordId id, Timestamp start);
    void insert(Buffer_Map &);
    void insert_secAttr(RecordId id, Timestamp start, int secAttr){};
    void insert_secAttr(Buffer_Map &){};
    Timestamp remove_secAttr(RecordId id){};
    Timestamp remove(RecordId id);
    size_t getSize();
    void print(char c);

    // Querying
    size_t execute_gOverlaps(RangeQuery Q);
    size_t execute_gOverlaps();
};



class Buffer_Vector : public Buffer
{
private:
    vector<Timestamp> entries;
    RecordId minRecordId;

public:
    Buffer_Vector();
    Buffer_Vector(size_t capacity);
    void insert(RecordId id, Timestamp start);
    void insert(Buffer_Vector &);
    void insert_secAttr(RecordId id, Timestamp start, int secAttr){};
    void insert_secAttr(Buffer_Vector &){};
    Timestamp remove_secAttr(RecordId id){};
    Timestamp remove(RecordId id);
    size_t getSize();
    void print(char c);

    // Querying
    size_t execute_gOverlaps(RangeQuery Q);
    size_t execute_gOverlaps();
};



class Buffer_List : public Buffer
{
private:
    list<pair<RecordId, Timestamp> > entries;

public:
    Buffer_List() {};
    Buffer_List(size_t capacity) {};
    void insert(RecordId id, Timestamp start);
//    bool operator< (pair<RecordId, Timestamp> lhs, pair<RecordId, Timestamp> rhs);
    void insert_secAttr(RecordId id, Timestamp start, int secAttr){};
    Timestamp remove_secAttr(RecordId id){};
    Timestamp remove(RecordId id);
    size_t getSize();
    void print(char c);

    // Querying
    size_t execute_gOverlaps(RangeQuery Q);
    size_t execute_gOverlaps() { return 0; };
};



class Buffer_ICDE16 : public Buffer
{
private:
    UnorderedHashMap<RecordId, pair<RecordId, Timestamp> > *entries;
    UnorderedHashMap<RecordId, pair<RecordId, int> > *secAttrs;
//    UnorderedHashMap<RecordId, pair<RecordId, Timestamp> > entries;

public:
    Buffer_ICDE16();
    Buffer_ICDE16(size_t capacity);
    void insert(RecordId id, Timestamp start);
    void insert_secAttr(RecordId id, Timestamp start, int secAttr);
    void insert(Buffer_ICDE16 &);
    void insert_secAttr(Buffer_ICDE16 &);
    Timestamp remove(RecordId id);
    Timestamp remove_secAttr(RecordId id);
    size_t getSize();
    void print(char c);
    void destroy();
    ~Buffer_ICDE16();

    // Querying
    size_t execute_gOverlaps(RangeQuery Q);
    size_t execute_gOverlaps();

    size_t executeTimeTravel(RangeQuery Q, int secondAttrLowerConstraint, int secondAttrUpperConstraint);
    size_t executeTimeTravel(int secondAttrLowerConstraint, int secondAttrUpperConstraint);

    size_t executeTimeTravel_greaterthan(RangeQuery Q, int secondAttrLowerConstraint);
    size_t executeTimeTravel_greaterthan(int secondAttrLowerConstraint);
    
    size_t executeTimeTravel_lowerthan(RangeQuery Q, int secondAttrUpperConstraint);
    size_t executeTimeTravel_lowerthan(int secondAttrUpperConstraint);

};
#endif // _BUFFER_H_
