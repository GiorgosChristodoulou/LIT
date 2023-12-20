#include "offsets.h"



OffsetEntry_SS_CM::OffsetEntry_SS_CM()
{
}
    

OffsetEntry_SS_CM::OffsetEntry_SS_CM(Timestamp tstamp, RelationIdIterator iterI, vector<pair<Timestamp, Timestamp> >::iterator iterT, PartitionId pid)
{
    this->tstamp = tstamp;
    this->iterI  = iterI;
    this->iterT  = iterT;
    this->pid    = pid;
}


bool OffsetEntry_SS_CM::operator < (const OffsetEntry_SS_CM &rhs) const
{
    return this->tstamp < rhs.tstamp;
}


bool OffsetEntry_SS_CM::operator >= (const OffsetEntry_SS_CM &rhs) const
{
    return this->tstamp >= rhs.tstamp;
}


OffsetEntry_SS_CM::~OffsetEntry_SS_CM()
{
}



OffsetEntry_ALL::OffsetEntry_ALL()
{
    }


OffsetEntry_ALL::OffsetEntry_ALL(Timestamp tstamp, RelationIdIterator iterI, PartitionId pid)
{
    this->tstamp = tstamp;
    this->iterI  = iterI;
    this->pid    = pid;
}


bool OffsetEntry_ALL::operator < (const OffsetEntry_ALL &rhs) const
{
    return this->tstamp < rhs.tstamp;
}


bool OffsetEntry_ALL::operator >= (const OffsetEntry_ALL &rhs) const
{
    return this->tstamp >= rhs.tstamp;
}


OffsetEntry_ALL::~OffsetEntry_ALL()
{
}



OffsetEntry_ALL_Timestamp::OffsetEntry_ALL_Timestamp()
{
}


OffsetEntry_ALL_Timestamp::OffsetEntry_ALL_Timestamp(Timestamp tstamp, RelationIdIterator iterI, vector<Timestamp>::iterator iterT, PartitionId pid) : OffsetEntry_ALL(tstamp, iterI, pid)
{
    this->iterT = iterT;
}



bool OffsetEntry_ALL_Timestamp::operator < (const OffsetEntry_ALL_Timestamp &rhs) const
{
    return this->tstamp < rhs.tstamp;
}


bool OffsetEntry_ALL_Timestamp::operator >= (const OffsetEntry_ALL_Timestamp &rhs) const
{
    return this->tstamp >= rhs.tstamp;
}


OffsetEntry_ALL_Timestamp::~OffsetEntry_ALL_Timestamp()
{
}


    
OffsetEntry_ALL_Timestamps::OffsetEntry_ALL_Timestamps()
{
}


OffsetEntry_ALL_Timestamps::OffsetEntry_ALL_Timestamps(Timestamp tstamp, RelationIdIterator iterI, vector<pair<Timestamp, Timestamp> >::iterator iterT, PartitionId pid) : OffsetEntry_ALL(tstamp, iterI, pid)
{
    this->iterT = iterT;
}


bool OffsetEntry_ALL_Timestamps::operator < (const OffsetEntry_ALL_Timestamps &rhs) const
{
    return this->tstamp < rhs.tstamp;
}


bool OffsetEntry_ALL_Timestamps::operator >= (const OffsetEntry_ALL_Timestamps &rhs) const
{
    return this->tstamp >= rhs.tstamp;
}


OffsetEntry_ALL_Timestamps::~OffsetEntry_ALL_Timestamps()
{
}
