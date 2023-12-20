#include "live_index.h"



template <class T>
LiveIndexCapacityConstrainted<T>::LiveIndexCapacityConstrainted(size_t maxCapacity)
{
    this->maxCapacity = maxCapacity;
    this->minCapacity = ceil(this->maxCapacity*0.4);
    this->merged = false;

//    this->buffers.emplace_back(this->maxCapacity);
//    this->lastBuffer = this->buffers.begin();
//    this->lastBufferSize = 0;
}


template <class T>
void LiveIndexCapacityConstrainted<T>::insert(RecordId id, Timestamp start)
{
    // If current (last) buffer is full, we need a new buffer
    if ((this->buffers.size() == 0) || (this->lastBufferSize == this->maxCapacity))
    {
        this->offsets_starts.push_back(start);
        this->offsets_ids.push_back(id);
        this->buffers.emplace_back(this->maxCapacity);
        
//        this->lastBuffer++;   // why doesn't ++ work??
        this->lastBuffer = (this->buffers.end()-1);
        
        this->lastBufferSize = 0;
//        cout<<"\tNEW buffer created" << endl;
//        this->print('r');
    }
    this->lastBuffer->insert(id, start);
    this->lastBufferSize++;
//    this->latestStart = start;
//    cout << "\tINSERTED in b" << (this->buffers.size()-1) << ": r" << id << " starting at " << start << endl;
}


template <class T>
void LiveIndexCapacityConstrainted<T>::insert_secAttr(RecordId id, Timestamp start, int secAttr)
{
    // If current (last) buffer is full, we need a new buffer
    if ((this->buffers.size() == 0) || (this->lastBufferSize == this->maxCapacity))
    {
        this->offsets_starts.push_back(start);
        this->offsets_ids.push_back(id);
        this->buffers.emplace_back(this->maxCapacity);
        
//        this->lastBuffer++;   // why doesn't ++ work??
        this->lastBuffer = (this->buffers.end()-1);
        
        this->lastBufferSize = 0;
//        cout<<"\tNEW buffer created" << endl;
//        this->print('r');
    }
    // this->lastBuffer->insert(id, start);
    this->lastBuffer->insert_secAttr(id, start, secAttr);
    this->lastBufferSize++;
//    this->latestStart = start;
   // cout << "\tINSERTED in b" << (this->buffers.size()-1) << ": r" << id << " starting at " << start << endl;
}

template <class T>
Timestamp LiveIndexCapacityConstrainted<T>::remove(RecordId id)
{
//    auto bid = ceil((float)(id+1)/this->maxCapacity)-1;
    auto bid = 0;
    auto bsize = 0;
    Timestamp start;
    

    // First locate the buffer that contains the record.
    if (!merged)
        bid = id/this->maxCapacity;
    else
    {
        vector<RecordId>::iterator pivot = lower_bound(this->offsets_ids.begin(), this->offsets_ids.end(), id+1);
        bid = (pivot-1-this->offsets_ids.begin());
    }

    // Remove record from buffer bid.

    start = this->buffers[bid].remove(id);

//     // Merge bid with one of the adjacent buffers if its capacity dropped below the minCapacity threshold.
//     bsize = this->buffers[bid].getSize();
//     if ((this->buffers.size() > 1) && (bsize < this->minCapacity))
//     {
//         // bid-1
//         if ((bid > 1) && (bsize+this->buffers[bid-1].getSize() < this->maxCapacity))
//         {
// //            cout << "\t\t\tMerge Buffer" << bid << " (" << bsize << ") with Buffer" << (bid-1) << " (" << this->buffers[bid-1].getSize() << ")" << endl;
//             this->mergeBuffers(bid-1, bid);
//         }
//         // bid+1
//         else if ((bid+1 < this->buffers.size()) && (bsize+this->buffers[bid+1].getSize() < this->maxCapacity))
//         {
// //            cout << "\t\t\tMerge Buffer" << bid << " (" << bsize << ") with Buffer" << (bid+1) << " (" << this->buffers[bid+1].getSize() << ")" << endl;
//             this->mergeBuffers(bid, bid+1);
//         }
//         // Else, cannot merge buffer
//     }
    
   
    return start;
}

template <class T>
Timestamp LiveIndexCapacityConstrainted<T>::remove_secAttr(RecordId id)
{
//    auto bid = ceil((float)(id+1)/this->maxCapacity)-1;
    auto bid = 0;
    auto bsize = 0;
    Timestamp start;
    

    // First locate the buffer that contains the record.
    // if (!merged)
    //     bid = id/this->maxCapacity;
    // else
    // {
        vector<RecordId>::iterator pivot = lower_bound(this->offsets_ids.begin(), this->offsets_ids.end(), id+1);
        bid = (pivot-1-this->offsets_ids.begin());
    // }
    // Remove record from buffer bid.
    // cout << bid << " " << id << endl;
    start = this->buffers[bid].remove_secAttr(id);
    // cout << bid << " " << id << endl;

    // Merge bid with one of the adjacent buffers if its capacity dropped below the minCapacity threshold.
    bsize = this->buffers[bid].getSize();
    if ((this->buffers.size() > 1) && (bsize < this->minCapacity))
    {
        // bid-1
        if ((bid > 1) && (bsize+this->buffers[bid-1].getSize() < this->maxCapacity))
        {
//            cout << "\t\t\tMerge Buffer" << bid << " (" << bsize << ") with Buffer" << (bid-1) << " (" << this->buffers[bid-1].getSize() << ")" << endl;
            this->mergeBuffers_secAttr(bid-1, bid);
        }
        // bid+1
        else if ((bid+1 < this->buffers.size()) && (bsize+this->buffers[bid+1].getSize() < this->maxCapacity))
        {
//            cout << "\t\t\tMerge Buffer" << bid << " (" << bsize << ") with Buffer" << (bid+1) << " (" << this->buffers[bid+1].getSize() << ")" << endl;
            this->mergeBuffers_secAttr(bid, bid+1);
        }
        // Else, cannot merge buffer
    }
    
    
    return start;
}

template <class T>
size_t LiveIndexCapacityConstrainted<T>::getNumBuffers()
{
    return this->buffers.size();
}



template <class T>
void LiveIndexCapacityConstrainted<T>::mergeBuffers(size_t from, size_t to)
{
    typename vector<T>::iterator iterBBegin  = this->buffers.begin()+to;
    vector<Timestamp>::iterator  iterOSBegin = this->offsets_starts.begin()+to;
    vector<RecordId>::iterator   iterOIBegin = this->offsets_ids.begin()+to;

    this->buffers[from].insert(*iterBBegin);

    this->buffers.erase(iterBBegin, iterBBegin+1);
    this->offsets_starts.erase(iterOSBegin, iterOSBegin+1);
    this->offsets_ids.erase(iterOIBegin, iterOIBegin+1);
    
    this->lastBuffer = (this->buffers.end()-1);
    this->lastBufferSize = this->lastBuffer->getSize();
    this->merged = true;
}

template <class T>
void LiveIndexCapacityConstrainted<T>::mergeBuffers_secAttr(size_t from, size_t to)
{
    typename vector<T>::iterator iterBBegin  = this->buffers.begin()+to;
    vector<Timestamp>::iterator  iterOSBegin = this->offsets_starts.begin()+to;
    vector<RecordId>::iterator   iterOIBegin = this->offsets_ids.begin()+to;

    this->buffers[from].insert_secAttr(*iterBBegin);

    this->buffers.erase(iterBBegin, iterBBegin+1);
    this->offsets_starts.erase(iterOSBegin, iterOSBegin+1);
    this->offsets_ids.erase(iterOIBegin, iterOIBegin+1);
    
    this->lastBuffer = (this->buffers.end()-1);
    this->lastBufferSize = this->lastBuffer->getSize();
    this->merged = true;
}

template <class T>
void LiveIndexCapacityConstrainted<T>::mergeBuffers()
{
    auto collectiveSize = 0;
    auto b = 0, from = 0, to = 0;


//    cout << endl << "\tMerging buffers" << endl;
    while (b < this->buffers.size())
    {
        auto bsize = this->buffers[b].getSize();
        
        // If merging also with current buffer will exceed the maxCapacity constraint then merge only the previous buffers, and continue
//        cout << "\t\tBuffer" << b << ": " << this->buffers[b].getSize() << ", |B| = " << this->buffers.size() << ", collectiveSize = " << collectiveSize << endl;
        if (collectiveSize+bsize > this->maxCapacity)
        {
            // Merging occurs only for at least two concecutive buffers.
            if (b-1 > from)
            {
//                cout << "\t\t\tMerge from Buffer" << from << " to Buffer" << (b-1) << " with " << collectiveSize << " collective size" << endl;
                typename vector<T>::iterator iterBBegin = this->buffers.begin()+(from+1);
                typename vector<T>::iterator iterBEnd   = this->buffers.begin()+b;
                vector<Timestamp>::iterator  iterOSBegin = this->offsets_starts.begin()+(from+1);
                vector<Timestamp>::iterator  iterOSEnd   = this->offsets_starts.begin()+b;
                vector<RecordId>::iterator   iterOIBegin = this->offsets_ids.begin()+(from+1);
                vector<RecordId>::iterator   iterOIEnd   = this->offsets_ids.begin()+b;

                auto i = from+1;
                for (auto iter = iterBBegin; iter != iterBEnd; iter++)
//                auto U = b;
//                for (auto i = from+1; i < U; i++)
                {
//                    cout << "\t\t\terase b" << i << ": " << iter->getSize() << endl;
                    i++;
                    this->buffers[from].insert((*iter));
                    b--;
                }
                this->buffers.erase(iterBBegin, iterBEnd);
                this->offsets_starts.erase(iterOSBegin, iterOSEnd);
                this->offsets_ids.erase(iterOIBegin, iterOIEnd);
            }
            collectiveSize = bsize;
            from = b;
        }
        else
        {
            collectiveSize += bsize;
        }

        b++;
    }
    
    // Merging with the last buffer.
//    cout << "from = " << from << "\tb = " << b << "\tcollectiveSize = " << collectiveSize << "\tthis->buffers.size() = " << this->buffers.size() << endl;
    if (collectiveSize < this->maxCapacity)
    {
        // Merging occurs only for at least two concecutive buffers.
        if (b-1 > from)
        {
//            cout << "\t\tBuffer" << b-1 << ": " << this->buffers[b-1].getSize() << ", |B| = " << this->buffers.size() << ", collectiveSize = " << collectiveSize << endl;
//            cout << "\t\t\tMerge from Buffer" << from << " to Buffer" << (b-1) << " with " << collectiveSize << " collective size" << endl;
            typename vector<T>::iterator iterBBegin = this->buffers.begin()+(from+1);
            typename vector<T>::iterator iterBEnd   = this->buffers.begin()+b;
            vector<Timestamp>::iterator  iterOSBegin = this->offsets_starts.begin()+(from+1);
            vector<Timestamp>::iterator  iterOSEnd   = this->offsets_starts.begin()+b;
            vector<RecordId>::iterator   iterOIBegin = this->offsets_ids.begin()+(from+1);
            vector<RecordId>::iterator   iterOIEnd   = this->offsets_ids.begin()+b;

            auto i = from+1;
            for (auto iter = iterBBegin; iter != iterBEnd; iter++)
//                auto U = b;
//                for (auto i = from+1; i < U; i++)
            {
//                cout << "\t\t\terase b" << i << ": " << iter->getSize() << endl;
                i++;
                this->buffers[from].insert((*iter));
            }
            this->buffers.erase(iterBBegin, iterBEnd);
            this->offsets_starts.erase(iterOSBegin, iterOSEnd);
            this->offsets_ids.erase(iterOIBegin, iterOIEnd);
        }
    }

    this->lastBuffer = (this->buffers.end()-1);
    this->lastBufferSize = this->lastBuffer->getSize();
    this->merged = true;
}


template <class T>
void LiveIndexCapacityConstrainted<T>::reorganize(size_t maxCapacity)
{
    auto collectiveSize = 0;
    auto b = 0, from = 0, to = 0;


//    cout << endl << "\tMerging buffers" << endl;
    this->maxCapacity = maxCapacity;
    while (b < this->buffers.size())
    {
        auto bsize = this->buffers[b].getSize();
        
        // If merging also with current buffer will exceed the maxCapacity constraint then merge only the previous buffers, and continue
        cout << "\t\tBuffer" << b << ": " << this->buffers[b].getSize() << ", |B| = " << this->buffers.size() << ", collectiveSize = " << collectiveSize << endl;
        if (collectiveSize+bsize > this->maxCapacity)
        {
            // Merging occurs only for at least two concecutive buffers.
            if (b-1 > from)
            {
//                cout << "\t\t\tMerge from Buffer" << from << " to Buffer" << (b-1) << " with " << collectiveSize << " collective size" << endl;
                T newB;
                typename vector<T>::iterator iterBBegin = this->buffers.begin()+(from);
                typename vector<T>::iterator iterBEnd   = this->buffers.begin()+b;
                vector<Timestamp>::iterator  iterOSBegin = this->offsets_starts.begin()+(from);
                vector<Timestamp>::iterator  iterOSEnd   = this->offsets_starts.begin()+b;
                vector<RecordId>::iterator   iterOIBegin = this->offsets_ids.begin()+(from);
                vector<RecordId>::iterator   iterOIEnd   = this->offsets_ids.begin()+b;

                auto i = from;
                for (auto iter = iterBBegin; iter != iterBEnd; iter++)
//                auto U = b;
//                for (auto i = from+1; i < U; i++)
                {
//                    cout << "\t\t\terase b" << i << ": " << iter->getSize() << endl;
                    i++;
                    newB.insert((*iter));
                    b--;
                }
                this->buffers.erase(iterBBegin, iterBEnd);
                this->offsets_starts.erase(iterOSBegin, iterOSEnd);
                this->offsets_ids.erase(iterOIBegin, iterOIEnd);
            }
            collectiveSize = bsize;
            from = b;
        }
        else
        {
            collectiveSize += bsize;
        }

        b++;
    }
    
    // Merging with the last buffer.
//    cout << "from = " << from << "\tb = " << b << "\tcollectiveSize = " << collectiveSize << "\tthis->buffers.size() = " << this->buffers.size() << endl;
    if (collectiveSize < this->maxCapacity)
    {
        // Merging occurs only for at least two concecutive buffers.
        if (b-1 > from)
        {
            cout << "\t\tBuffer" << b-1 << ": " << this->buffers[b-1].getSize() << ", |B| = " << this->buffers.size() << ", collectiveSize = " << collectiveSize << endl;
            cout << "\t\t\tMerge from Buffer" << from << " to Buffer" << (b-1) << " with " << collectiveSize << " collective size" << endl;

            T newB;
            typename vector<T>::iterator iterBBegin = this->buffers.begin()+(from);
            typename vector<T>::iterator iterBEnd   = this->buffers.begin()+b;
            vector<Timestamp>::iterator  iterOSBegin = this->offsets_starts.begin()+(from);
            vector<Timestamp>::iterator  iterOSEnd   = this->offsets_starts.begin()+b;
            vector<RecordId>::iterator   iterOIBegin = this->offsets_ids.begin()+(from);
            vector<RecordId>::iterator   iterOIEnd   = this->offsets_ids.begin()+b;
            Timestamp minStart = (*iterOSBegin);
            RecordId minId = (*iterOIBegin);

            auto i = from+1;
            for (auto iter = iterBBegin; iter != iterBEnd; iter++)
//                auto U = b;
//                for (auto i = from+1; i < U; i++)
            {
//                cout << "\t\t\terase b" << i << ": " << iter->getSize() << endl;
                i++;
                newB.insert((*iter));
            }
            this->buffers.erase(iterBBegin, iterBEnd);
            this->offsets_starts.erase(iterOSBegin, iterOSEnd);
            this->offsets_ids.erase(iterOIBegin, iterOIEnd);
            
            this->buffers.push_back(newB);
            this->offsets_starts.push_back(minStart);
            this->offsets_ids.push_back(minId);
            cout << this->buffers.size() << endl;
            cout << newB.getSize() << endl;
        }
    }

    this->lastBuffer = (this->buffers.end()-1);
    this->lastBufferSize = this->lastBuffer->getSize();
    this->merged = true;
}


//template <class T>
//void LiveIndexCapacityConstrainted<T>::removeEmptyBuffers()
//{
//    vector<Timestamp>::iterator iterO = this->offsets.begin();
//    typename vector<T>::iterator iterB = this->buffers.begin();
//    auto count = 0;
//
//    while (iterB != this->buffers.end())
//    {
//        if (iterB->getSize() == 0)
//        {
//            // Remove buffer
//            iterO = this->offsets.erase(iterO);
////            iterB = this->buffers.erase(iterB);
//
//            count++;
//        }
//        else
//        {
//            iterO++;
//            iterB++;
//        }
//    }
//    if (count > 0)
//    {
//        if (this->buffers.size() == 1)
//            this->lastBuffer = this->buffers.begin();
//        else
//            this->lastBuffer = (this->buffers.end()-1);
//        
//        if (this->lastBuffer == this->buffers.end())
//        {
//            cerr << "Oops" << endl;
//            exit(1);
//        }
//        this->lastBufferSize = this->lastBuffer->getSize();
//    }
//
//    cout << "# empty buffers = " << count << endl;
//}


template <class T>
void LiveIndexCapacityConstrainted<T>::print(char c)
{
    cout << "\tLIVE INDEX:" << endl;
    for (auto i = 0; i < this->buffers.size(); i++)
    {
        cout << "\t\tBuffer" << i << " (" << this->offsets_starts[i] << "): ";
//        this->buffers[i].print('r');
        cout << this->buffers[i].getSize();
        cout << endl;
    }
//    cout << "\t\tBuffer" << (this->buffers.size()-1) << " (" ): ";
//    this->buffers[this->buffers.size()-1].print('r');
//    cout << this->buffers[this->buffers.size()-1].getSize();
//    cout << endl;
}


template <class T>
LiveIndexCapacityConstrainted<T>::~LiveIndexCapacityConstrainted()
{
    for (auto i = 0; i < this->buffers.size(); i++)
        this->buffers[i].destroy();
}


template <class T>
size_t LiveIndexCapacityConstrainted<T>::execute_pureTimeTravel(RangeQuery Q)
{
    size_t result = 0;
    vector<Timestamp>::iterator iterO    = this->offsets_starts.begin();
    vector<Timestamp>::iterator iterOEnd = lower_bound(iterO, this->offsets_starts.end(), Q.end+1);
    typename vector<T>::iterator iterB   = this->buffers.begin();
    typename vector<T>::iterator pivot   = iterB+(iterOEnd-iterO-1);
//    size_t cnt = 0;


    // Comparisons are needed only at the last buffer - contents sorted on start by construction.
    while (iterB != pivot)
    {
        result ^= iterB->execute_gOverlaps();
        iterB++;
//        cnt++;
    }

    result ^= iterB->execute_gOverlaps(Q);


    return result;
}


template <class T>
size_t LiveIndexCapacityConstrainted<T>::execute_rangeTimeTravel(RangeQuery Q, int secondAttrLowerConstraint, int secondAttrUpperConstraint)
{
    size_t result = 0;
    vector<Timestamp>::iterator iter    = this->offsets_starts.begin();
    vector<Timestamp>::iterator iterEnd = lower_bound(iter, this->offsets_starts.end(), Q.end+1);
    typename vector<T>::iterator iterB  = this->buffers.begin();
    typename vector<T>::iterator pivot = iterB+(iterEnd-iter-1);


    // Comparisons are needed only at the last buffer - contents sorted on start by construction.
    while (iterB != pivot)
    {
#ifdef WORKLOAD_COUNT
        result += iterB->executeTimeTravel(secondAttrLowerConstraint, secondAttrUpperConstraint);
#else
        result ^= iterB->executeTimeTravel(secondAttrLowerConstraint, secondAttrUpperConstraint);
#endif
        iterB++;
    }
#ifdef WORKLOAD_COUNT
    result += iterB->executeTimeTravel(Q, secondAttrLowerConstraint, secondAttrUpperConstraint);
#else
    result ^= iterB->executeTimeTravel(Q, secondAttrLowerConstraint, secondAttrUpperConstraint);
#endif
    return result;
}

template <class T>
size_t LiveIndexCapacityConstrainted<T>::executeTimeTravel_greaterthan(RangeQuery Q, int secondAttrLowerConstraint)
{
    size_t result = 0;
    vector<Timestamp>::iterator iter    = this->offsets_starts.begin();
    vector<Timestamp>::iterator iterEnd = lower_bound(iter, this->offsets_starts.end(), Q.end+1);
    typename vector<T>::iterator iterB  = this->buffers.begin();
    typename vector<T>::iterator pivot = iterB+(iterEnd-iter-1);


    // Comparisons are needed only at the last buffer - contents sorted on start by construction.
    while (iterB != pivot)
    {
#ifdef WORKLOAD_COUNT
        result += iterB->executeTimeTravel_greaterthan(secondAttrLowerConstraint);
#else
        result ^= iterB->executeTimeTravel_greaterthan(secondAttrLowerConstraint);
#endif
        iterB++;
    }
#ifdef WORKLOAD_COUNT
    result += iterB->executeTimeTravel_greaterthan(Q, secondAttrLowerConstraint);
#else
    result ^= iterB->executeTimeTravel_greaterthan(Q, secondAttrLowerConstraint);
#endif
    return result;
}

template <class T>
size_t LiveIndexCapacityConstrainted<T>::executeTimeTravel_lowerthan(RangeQuery Q, int secondAttrUpperConstraint)
{
    size_t result = 0;
    vector<Timestamp>::iterator iter    = this->offsets_starts.begin();
    vector<Timestamp>::iterator iterEnd = lower_bound(iter, this->offsets_starts.end(), Q.end+1);
    typename vector<T>::iterator iterB  = this->buffers.begin();
    typename vector<T>::iterator pivot = iterB+(iterEnd-iter-1);


    // Comparisons are needed only at the last buffer - contents sorted on start by construction.
    while (iterB != pivot)
    {
#ifdef WORKLOAD_COUNT
        result += iterB->executeTimeTravel_lowerthan(secondAttrUpperConstraint);
#else
        result ^= iterB->executeTimeTravel_lowerthan(secondAttrUpperConstraint);
#endif
        iterB++;
    }
#ifdef WORKLOAD_COUNT
    result += iterB->executeTimeTravel_lowerthan(Q, secondAttrUpperConstraint);
#else
    result ^= iterB->executeTimeTravel_lowerthan(Q, secondAttrUpperConstraint);
#endif
    return result;
}

template <class T>
LiveIndexDurationConstrainted<T>::LiveIndexDurationConstrainted(Timestamp duration)
{
    this->duration = duration;
}


template <class T>
void LiveIndexDurationConstrainted<T>::insert(RecordId id, Timestamp start)
{
    // If current (last) buffer is full, we need a new buffer
    if ((this->buffers.size() == 0) || (start > this->lastBufferEnd))
    {
        this->offsets_starts.push_back(start);
        this->offsets_ids.push_back(id);
        this->buffers.emplace_back();
        
        auto bid = ceil((float)start/this->duration);  // Id of the buffer to store the id
        this->lastBuffer = (this->buffers.end()-1);
        this->lastBufferEnd = (bid+1)*this->duration - 1;
//        cout<<"\tNEW buffer created with end = " << lastBufferEnd << endl;
//        cout << "\tINSERTED in b" << (this->buffers.size()-1) << ": r" << id << " starting at " << start << endl;
    }
    this->lastBuffer->insert(id, start);
//    cout << "\tINSERTED in b" << (this->buffers.size()-1) << ": r" << id << " starting at " << start << endl;
}


template <class T>
Timestamp LiveIndexDurationConstrainted<T>::remove(RecordId id)
{
    vector<RecordId>::iterator pivot = lower_bound(this->offsets_ids.begin(), this->offsets_ids.end(), id+1);
    auto bid = (pivot-1-this->offsets_ids.begin());
    Timestamp start = this->buffers[bid].remove(id);
    

    return start;
}


template <class T>
size_t LiveIndexDurationConstrainted<T>::getNumBuffers()
{
    return this->buffers.size();
}


template <class T>
void LiveIndexDurationConstrainted<T>::mergeBuffers()
{
//    auto collectiveSize = 0;
//    auto b = 0, from = 0, to = 0;
//
////    cout << endl << "\tMerging buffers" << endl;
//    while (b < this->buffers.size())
//    {
//        Timestamp start = buffers[b].begin();
//        b++;
//    }
}


template <class T>
void LiveIndexDurationConstrainted<T>::print(char c)
{
    cout << "\tLIVE INDEX:" << endl;
    for (auto i = 0; i < this->buffers.size(); i++)
    {
        cout << "\t\tBuffer" << i << " (" << this->offsets_ids[i] << ") (" << this->offsets_starts[i] << "): ";
//        this->buffers[i].print('r');
        cout << this->buffers[i].getSize();
        cout << endl;
    }
//    cout << "\t\tBuffer" << (this->buffers.size()-1) << " (" ): ";
//    this->buffers[this->buffers.size()-1].print('r');
//    cout << this->buffers[this->buffers.size()-1].getSize();
//    cout << endl;
}


template <class T>
LiveIndexDurationConstrainted<T>::~LiveIndexDurationConstrainted()
{
    for (auto i = 0; i < this->buffers.size(); i++)
        this->buffers[i].destroy();
}


template <class T>
size_t LiveIndexDurationConstrainted<T>::execute_gOverlaps(RangeQuery Q)
{
    size_t result = 0;
    vector<Timestamp>::iterator iter    = this->offsets_starts.begin();
    vector<Timestamp>::iterator iterEnd = lower_bound(iter, this->offsets_starts.end(), Q.end+1);
    typename vector<T>::iterator iterB  = this->buffers.begin();
    typename vector<T>::iterator pivot = iterB+(iterEnd-iter-1);


    // Comparisons are needed only at the last buffer - contents sorted on start by construction.
    while (iterB != pivot)
    {
        result ^= iterB->execute_gOverlaps();
        iterB++;
    }
    result ^= iterB->execute_gOverlaps(Q);
    
    
    return result;
}



