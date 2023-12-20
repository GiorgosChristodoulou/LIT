#include "offsets_templates.h"



template <class T>
OffsetEntry_SS<T>::OffsetEntry_SS()
{
}


template <class T>
OffsetEntry_SS<T>::OffsetEntry_SS(Timestamp tstamp, typename T::iterator iter, PartitionId pid)
{
    this->tstamp = tstamp;
    this->iter   = iter;
    this->pid    = pid;
}


template <class T>
bool OffsetEntry_SS<T>::operator < (const OffsetEntry_SS &rhs) const
{
    return this->tstamp < rhs.tstamp;
}


template <class T>
bool OffsetEntry_SS<T>::operator >= (const OffsetEntry_SS &rhs) const
{
    return this->tstamp >= rhs.tstamp;
}


template <class T>
OffsetEntry_SS<T>::~OffsetEntry_SS()
{
}



template <class T>
Offsets_SS<T>::Offsets_SS()
{
}


template <class T>
Offsets_SS<T>::~Offsets_SS()
{
}
