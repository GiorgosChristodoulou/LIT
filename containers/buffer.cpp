#include "buffer.h"



Buffer::Buffer()
{
}


Buffer::Buffer(size_t capacity)
{
    this->capacity = capacity;
}



Buffer_Map::Buffer_Map()
{
}


Buffer_Map::Buffer_Map(size_t capacity) : Buffer(capacity)
{
}


void Buffer_Map::insert(RecordId id, Timestamp start)
{
    this->entries[id] = start;
}


void Buffer_Map::insert(Buffer_Map &B)
{
    for (auto iter = B.entries.begin(); iter != B.entries.end(); iter++)
        this->entries[iter->first] = iter->second;
}


Timestamp Buffer_Map::remove(RecordId id)
{
    map<RecordId, Timestamp>::iterator iter = this->entries.find(id);
    Timestamp start = iter->second;
    
    this->entries.erase(iter);
    
    return start;
}

size_t Buffer_Map::getSize()
{
    return this->entries.size();
}


void Buffer_Map::print(char c)
{
    cout << "{";
    for (auto iter = this->entries.begin(); iter != this->entries.end(); iter++)
    {
        cout << "<" << c << iter->first << "," << iter->second << ">";
    }
    cout << "}" << endl;
}


// Querying
size_t Buffer_Map::execute_gOverlaps(RangeQuery Q)
{
    size_t result = 0;
    map<RecordId, Timestamp>::iterator iter, iterEnd = this->entries.end();

    for (iter = this->entries.begin(); iter != iterEnd; iter++)
    {
        if (iter->second > Q.end)
            break;

        result ^= iter->first;
    }

    return result;
}


size_t Buffer_Map::execute_gOverlaps()
{
    size_t result = 0;
    map<RecordId, Timestamp>::iterator iter, iterEnd = this->entries.end();
    
    for (iter = this->entries.begin(); iter != iterEnd; iter++)
        result ^= iter->first;
    
    return result;
}




Buffer_Vector::Buffer_Vector()
{
    this->minRecordId = std::numeric_limits<Timestamp>::max();
}


Buffer_Vector::Buffer_Vector(size_t capacity) : Buffer(capacity)
{
    this->entries.reserve(this->capacity);
    this->minRecordId = std::numeric_limits<Timestamp>::max();
}


void Buffer_Vector::insert(RecordId id, Timestamp start)
{
    this->entries.push_back(start);
    this->minRecordId = min(this->minRecordId, id);
}


void Buffer_Vector::insert(Buffer_Vector &B)
{
    auto csize = this->entries.size();
    auto bsize = B.entries.size();

    this->entries.resize(csize+bsize);
    for (auto i = 0; i < bsize; i++)
        this->entries[i+csize] = B.entries[i];
}


Timestamp Buffer_Vector::remove(RecordId id)
{
//    cout << "id = " << id << "\tminRecordId = " << this->minRecordId << endl;
    RecordId fixed_id = id - this->minRecordId;
    Timestamp start = this->entries[fixed_id];
    this->entries[fixed_id] = -1;
    
    return start;
}


size_t Buffer_Vector::getSize()
{
    size_t count = 0;
    
    for (auto i = 0; i < this->entries.size(); i++)
    {
        if (this->entries[i] != -1)
            count++;
    }
    
    return count;
}


void Buffer_Vector::print(char c)
{
    cout << "{";
    for (auto i = 0; i < this->entries.size(); i++)
    {
        cout << "<" << c << i << "," << this->entries[i] << ">";
    }
    cout << "}" << endl;
}


size_t Buffer_Vector::execute_gOverlaps(RangeQuery Q)
{
    size_t result = 0, size = this->entries.size();

    for (auto i = 0; i < size; i++)
    {
        auto &start = this->entries[i];
        if (start > Q.end)
            break;
        
        if (start != -1)
            result ^= i+this->minRecordId;
    }
    
    return result;
}


size_t Buffer_Vector::execute_gOverlaps()
{
    size_t result = 0, size = this->entries.size();

    for (auto i = 0; i < size; i++)
    {
        if (this->entries[i] != -1)
            result ^= i+this->minRecordId;
    }
    
    return result;
}




inline bool comp(pair<RecordId, Timestamp> lhs, pair<RecordId, Timestamp> rhs) {
//    cout<<"yo"<<endl;
    return (lhs.first < rhs.first);
}


void Buffer_List::insert(RecordId id, Timestamp start)
{
    // this->map.insert(id);
    this->entries.push_back(make_pair(id,start));
//    cout << "S " << id << " " << start << endl;
}

Timestamp Buffer_List::remove(RecordId id)
{
//    cout << "E " << id << endl;
    pair<RecordId, Timestamp> temp = make_pair(id, 0);
    list<pair<RecordId, Timestamp>>::iterator index_to_remove = lower_bound(this->entries.begin(), this->entries.end(), temp, comp);
    this->entries.erase(index_to_remove);
    return index_to_remove->second;
    
//    binary_search(this->begin(), this->end(), temp, comp);
//    this->erase(this->begin());
//    return 10000000;
}


size_t Buffer_List::getSize()
{
    return this->entries.size();
}


void Buffer_List::print(char c)
{
    cout << "{";
    for (auto iter = this->entries.begin(); iter != this->entries.end(); iter++)
    {
        cout << "<" << c << iter->first << "," << iter->second << ">";
    }
    cout << "}" << endl;
}


size_t Buffer_List::execute_gOverlaps(RangeQuery Q){
    // cout << "mphka3" << endl;
    size_t result = 0;
    list<pair<RecordId, Timestamp> >::iterator iter, iterStart, iterEnd;
    iterStart = this->entries.begin();
    iterEnd = this->entries.end();
    for (iter = iterStart; iter != iterEnd; iter++)
    {
        if (iter->second > Q.end)
            break;
        
        result ^= iter->first;
    }
    
    return result;
}




Buffer_ICDE16::Buffer_ICDE16()
{
    this->entries = new UnorderedHashMap<RecordId, pair<RecordId, Timestamp> >(MAX_ICDE16_CAPACITY);
    this->secAttrs = new UnorderedHashMap<RecordId, pair<RecordId, Timestamp> >(MAX_ICDE16_CAPACITY);
}


Buffer_ICDE16::Buffer_ICDE16(size_t capacity) : Buffer(capacity)
{
    this->entries = new UnorderedHashMap<RecordId, pair<RecordId, Timestamp> >(this->capacity);
    this->secAttrs = new UnorderedHashMap<RecordId, pair<RecordId, Timestamp> >(this->capacity);
}


void Buffer_ICDE16::insert(RecordId id, Timestamp start)
{
    this->entries->insert(id, make_pair(id, start));
}

void Buffer_ICDE16::insert_secAttr(RecordId id, Timestamp start, int secAttr)
{
    this->entries->insert(id, make_pair(id, start));
    this->secAttrs->insert(id, make_pair(id, secAttr));
}


void Buffer_ICDE16::insert(Buffer_ICDE16 &B)
{
    for (auto iter = B.entries->begin(); iter != B.entries->end(); iter++)
    {
        this->entries->insert(iter->first, make_pair(iter->first, iter->second));
    }

}

void Buffer_ICDE16::insert_secAttr(Buffer_ICDE16 &B)
{
    for (auto iter = B.entries->begin(); iter != B.entries->end(); iter++)
    {
        this->entries->insert(iter->first, make_pair(iter->first, iter->second));
    }

    for (auto iter = B.secAttrs->begin(); iter != B.secAttrs->end(); iter++)
    {
        this->secAttrs->insert(iter->first, make_pair(iter->first, iter->second));
    }
}

Timestamp Buffer_ICDE16::remove(RecordId id)
{
    Timestamp start = (*this->entries)[id].second;
    
    this->entries->erase(id);
//    Timestamp start = this->entries[id].second;
//    this->entries.erase(id);

    return start;
}

Timestamp Buffer_ICDE16::remove_secAttr(RecordId id)
{
    Timestamp start = (*this->entries)[id].second;
    
    // for( auto x : *this->entries)
    //     cout << x.first << " " << x.second << endl;
    this->entries->erase(id);
    this->secAttrs->erase(id);
//    Timestamp start = this->entries[id].second;
//    this->entries.erase(id);

    return start;
}

size_t Buffer_ICDE16::getSize()
{
    return this->entries->size();
//    return this->entries.size();
}


void Buffer_ICDE16::print(char c)
{
    cout << "{";
    for (auto iter = this->entries->begin(); iter != this->entries->end(); iter++)
//    for (auto iter = this->entries.begin(); iter != this->entries.end(); iter++)
    {
        cout << "<" << c << iter->first << "," << iter->second << ">";
    }
    cout << "}" << endl;
}


void Buffer_ICDE16::destroy()
{
    delete this->entries;
}


Buffer_ICDE16::~Buffer_ICDE16()
{
    // TODO: deletion has to happen but not inside the destructor because emplace_back in LiveIndex calls it when creating a new buffer.
//    cout << "delete? " << this->getSize() << endl;
//    delete this->entries;
}


size_t Buffer_ICDE16::execute_gOverlaps(RangeQuery Q)
{
    size_t result = 0;
    auto iter = this->entries->begin(), iterEnd = this->entries->end();
//    auto iter = this->entries.begin(), iterEnd = this->entries.end();

    while (iter != iterEnd)
    {
        if (iter->second <= Q.end){
#ifdef WORKLOAD_COUNT
            result++;
#else
            result ^= iter->first;
#endif
            }
        iter++;
    }
    
    return result;
}

size_t Buffer_ICDE16::execute_gOverlaps()
{
    size_t result = 0;
    auto iter = this->entries->begin(), iterEnd = this->entries->end();
//    auto iter = this->entries.begin(), iterEnd = this->entries.end();

    while (iter != iterEnd)
    {
#ifdef WORKLOAD_COUNT
            result++;
#else
            result ^= iter->first;
#endif
        iter++;
    }
    
    return result;
}

size_t Buffer_ICDE16::executeTimeTravel(RangeQuery Q, int secondAttrLowerConstraint, int secondAttrUpperConstraint)
{
    size_t result = 0;
    auto iter = this->entries->begin(), iterEnd = this->entries->end();
//    auto iter = this->entries.begin(), iterEnd = this->entries.end();
    auto iterA = this->secAttrs->begin();
    while (iter != iterEnd)
    {
        if (iter->second <= Q.end)
            if((iterA->second) > secondAttrLowerConstraint && (iterA->second) < secondAttrUpperConstraint){
#ifdef WORKLOAD_COUNT
                result++;
#else
                result ^= iter->first;
#endif
            }
        iter++;
        iterA++;
    }
    
    return result;
}

size_t Buffer_ICDE16::executeTimeTravel(int secondAttrLowerConstraint, int secondAttrUpperConstraint)
{
    size_t result = 0;
    auto iter = this->entries->begin(), iterEnd = this->entries->end();
//    auto iter = this->entries.begin(), iterEnd = this->entries.end();
    auto iterA = this->secAttrs->begin();
    while (iter != iterEnd)
    {
        if((iterA->second) > secondAttrLowerConstraint && (iterA->second) < secondAttrUpperConstraint){
#ifdef WORKLOAD_COUNT
            result++;
#else
            result ^= iter->first;
#endif
        }
        iter++;
        iterA++;
    }
    
    return result;
}

size_t Buffer_ICDE16::executeTimeTravel_greaterthan(RangeQuery Q, int secondAttrLowerConstraint)
{
    size_t result = 0;
    auto iter = this->entries->begin(), iterEnd = this->entries->end();
//    auto iter = this->entries.begin(), iterEnd = this->entries.end();
    auto iterA = this->secAttrs->begin();
    while (iter != iterEnd)
    {
        if (iter->second <= Q.end)
            if((iterA->second) > secondAttrLowerConstraint){
#ifdef WORKLOAD_COUNT
            result++;
#else
            result ^= iter->first;
#endif
            }
        iter++;
        iterA++;
    }
    
    return result;
}

size_t Buffer_ICDE16::executeTimeTravel_greaterthan(int secondAttrLowerConstraint)
{
    size_t result = 0;
    auto iter = this->entries->begin(), iterEnd = this->entries->end();
//    auto iter = this->entries.begin(), iterEnd = this->entries.end();
    auto iterA = this->secAttrs->begin();
    while (iter != iterEnd)
    {
        if((iterA->second) > secondAttrLowerConstraint){
#ifdef WORKLOAD_COUNT
            result++;
#else
            result ^= iter->first;
#endif
        }
        iter++;
        iterA++;
    }
    
    return result;
}

size_t Buffer_ICDE16::executeTimeTravel_lowerthan(RangeQuery Q, int secondAttrUpperConstraint)
{
    size_t result = 0;
    auto iter = this->entries->begin(), iterEnd = this->entries->end();
//    auto iter = this->entries.begin(), iterEnd = this->entries.end();
    auto iterA = this->secAttrs->begin();
    while (iter != iterEnd)
    {
        if (iter->second <= Q.end)
            if((iterA->second) < secondAttrUpperConstraint){
#ifdef WORKLOAD_COUNT
                result++;
#else
                result ^= iter->first;
#endif            
        }
        iter++;
        iterA++;
    }
    
    return result;
}

size_t Buffer_ICDE16::executeTimeTravel_lowerthan(int secondAttrUpperConstraint)
{
    size_t result = 0;
    auto iter = this->entries->begin(), iterEnd = this->entries->end();
//    auto iter = this->entries.begin(), iterEnd = this->entries.end();
    auto iterA = this->secAttrs->begin();
    while (iter != iterEnd)
    {
        if((iterA->second) < secondAttrUpperConstraint){
#ifdef WORKLOAD_COUNT
            result++;
#else
            result ^= iter->first;
#endif
        }
        iter++;
        iterA++;
    }
    
    return result;
}

