#ifndef _LIVE_INDEX_H_
#define _LIVE_INDEX_H_

#include "../def_global.h"
#include "../containers/buffer.h"



class LiveIndex
{
public:
    LiveIndex() {};
    virtual void insert(RecordId id, Timestamp start) {};
    virtual void insert_secAttr(RecordId id, Timestamp start, int secAttr) {};
    virtual Timestamp remove(RecordId id) {};
    virtual Timestamp remove_secAttr(RecordId id) {};
    virtual size_t getNumBuffers() {};
    virtual size_t getSize() {};
    virtual void mergeBuffers() {};
    virtual void reorganize(size_t capacity) {};
//    virtual void removeEmptyBuffers() {};
    virtual void print(char c) {};
    virtual ~LiveIndex() {};

    // Querying
    virtual size_t execute_pureTimeTravel(RangeQuery Q) { return 0; };
    virtual size_t execute_rangeTimeTravel(RangeQuery Q, int secondAttrLowerConstraint, int secondAttrUpperConstraint) { return 0; };
    virtual size_t executeTimeTravel_greaterthan(RangeQuery Q, int secondAttrLowerConstraint) { return 0; };
    virtual size_t executeTimeTravel_lowerthan(RangeQuery Q, int secondAttrUpperConstraint) { return 0; };
};



template <class T>
class LiveIndexCapacityConstrainted : public LiveIndex
{
private:
    size_t maxCapacity, minCapacity;
    vector<Timestamp> offsets_starts;       // On-top auxiliary struct which stores the lowest start per buffer
    vector<RecordId>  offsets_ids;          // On-top auxiliary struct which stores the first id per buffer: to deal with merged buffers
    vector<T> buffers;
    typename vector<T>::iterator lastBuffer;
    size_t lastBufferSize;
    bool merged;

public:
    LiveIndexCapacityConstrainted(size_t maxCapacity);
    void insert(RecordId id, Timestamp start);
    void insert_secAttr(RecordId id, Timestamp start, int secondAttr);
    Timestamp remove(RecordId id);
    Timestamp remove_secAttr(RecordId id);
    size_t getNumBuffers();
    void mergeBuffers(size_t from, size_t to);
    void mergeBuffers_secAttr(size_t from, size_t to);
    void mergeBuffers();
    void reorganize(size_t maxCapacity);
//    void removeEmptyBuffers();
    void print(char c);
    ~LiveIndexCapacityConstrainted();

    // Querying
    size_t execute_pureTimeTravel(RangeQuery Q);
    size_t execute_rangeTimeTravel(RangeQuery Q, int secondAttrLowerConstraint, int secondAttrUpperConstraint);
    size_t executeTimeTravel_greaterthan(RangeQuery Q, int secondAttrLowerConstraint);
    size_t executeTimeTravel_lowerthan(RangeQuery Q, int secondAttrUppclearerConstraint);

};

typedef LiveIndexCapacityConstrainted<Buffer_Map>    LiveIndexCapacityConstraintedMap;
typedef LiveIndexCapacityConstrainted<Buffer_Vector> LiveIndexCapacityConstraintedVector;
typedef LiveIndexCapacityConstrainted<Buffer_List>   LiveIndexCapacityConstraintedList;
typedef LiveIndexCapacityConstrainted<Buffer_ICDE16> LiveIndexCapacityConstraintedICDE16;



template <class T>
class LiveIndexDurationConstrainted : public LiveIndex
{
private:
    Timestamp duration;
    vector<Timestamp> offsets_starts;       // On-top auxiliary struct which stores the lowest start per buffer: to avoid storing empty buffers
    vector<RecordId>  offsets_ids;          // On-top auxiliary struct which stores the first id per buffer
    vector<T> buffers;
    typename vector<T>::iterator lastBuffer;
    Timestamp lastBufferEnd;

public:
    LiveIndexDurationConstrainted(Timestamp duration);
    void insert(RecordId id, Timestamp start);
    Timestamp remove(RecordId id);
    size_t getNumBuffers();
    void mergeBuffers();
    void print(char c);
    ~LiveIndexDurationConstrainted();

    // Querying
    size_t execute_gOverlaps(RangeQuery Q);
};

typedef LiveIndexDurationConstrainted<Buffer_Map>    LiveIndexDurationConstraintedMap;
typedef LiveIndexDurationConstrainted<Buffer_Vector> LiveIndexDurationConstraintedVector;
typedef LiveIndexDurationConstrainted<Buffer_List>   LiveIndexDurationConstraintedList;
typedef LiveIndexDurationConstrainted<Buffer_ICDE16> LiveIndexDurationConstraintedICDE16;
#endif // _LIVE_INDEX_H_
