#ifndef _OFFSETS_H_
#define _OFFSETS_H_

#include "relation.h"



class OffsetEntry_SS_CM
{
public:
    Timestamp tstamp;
    RelationIdIterator iterI;
    vector<pair<Timestamp, Timestamp> >::iterator iterT;
    PartitionId pid;
    
    OffsetEntry_SS_CM();
    OffsetEntry_SS_CM(Timestamp tstamp, RelationIdIterator iterI, vector<pair<Timestamp, Timestamp> >::iterator iterT, PartitionId pid);
    bool operator < (const OffsetEntry_SS_CM &rhs) const;
    bool operator >= (const OffsetEntry_SS_CM &rhs) const;
    ~OffsetEntry_SS_CM();
};

typedef vector<OffsetEntry_SS_CM> Offsets_SS_CM;
typedef Offsets_SS_CM::const_iterator Offsets_SS_CM_Iterator;



class OffsetEntry_ALL
{
public:
    Timestamp tstamp;
    RelationIdIterator iterI;
    PartitionId pid;
    
    OffsetEntry_ALL();
    OffsetEntry_ALL(Timestamp tstamp, RelationIdIterator iterI, PartitionId pid);
    bool operator < (const OffsetEntry_ALL &rhs) const;
    bool operator >= (const OffsetEntry_ALL &rhs) const;
    ~OffsetEntry_ALL();
};


class OffsetEntry_ALL_Timestamp : public OffsetEntry_ALL
{
public:
    vector<Timestamp>::iterator iterT;
    
    OffsetEntry_ALL_Timestamp();
    OffsetEntry_ALL_Timestamp(Timestamp tstamp, RelationIdIterator iterI, vector<Timestamp>::iterator iterT, PartitionId pid);
    bool operator < (const OffsetEntry_ALL_Timestamp &rhs) const;
    bool operator >= (const OffsetEntry_ALL_Timestamp &rhs) const;
    ~OffsetEntry_ALL_Timestamp();
};


class OffsetEntry_ALL_Timestamps : public OffsetEntry_ALL
{
public:
    vector<pair<Timestamp, Timestamp> >::iterator iterT;
    
    OffsetEntry_ALL_Timestamps();
    OffsetEntry_ALL_Timestamps(Timestamp tstamp, RelationIdIterator iterI, vector<pair<Timestamp, Timestamp> >::iterator iterT, PartitionId pid);
    bool operator < (const OffsetEntry_ALL_Timestamps &rhs) const;
    bool operator >= (const OffsetEntry_ALL_Timestamps &rhs) const;
    ~OffsetEntry_ALL_Timestamps();
};

typedef OffsetEntry_ALL_Timestamps OffsetEntry_ALL_OrgsIn;
typedef OffsetEntry_ALL_Timestamp  OffsetEntry_ALL_OrgsAft;
typedef OffsetEntry_ALL_Timestamp  OffsetEntry_ALL_RepsIn;
typedef OffsetEntry_ALL            OffsetEntry_ALL_RepsAft;

typedef vector<OffsetEntry_ALL_OrgsIn>  Offsets_ALL_OrgsIn;
typedef vector<OffsetEntry_ALL_OrgsAft> Offsets_ALL_OrgsAft;
typedef vector<OffsetEntry_ALL_RepsIn>  Offsets_ALL_RepsIn;
typedef vector<OffsetEntry_ALL_RepsAft> Offsets_ALL_RepsAft;

typedef Offsets_ALL_OrgsIn::const_iterator  Offsets_ALL_OrgsIn_Iterator;
typedef Offsets_ALL_OrgsAft::const_iterator Offsets_ALL_OrgsAft_Iterator;
typedef Offsets_ALL_RepsIn::const_iterator  Offsets_ALL_RepsIn_Iterator;
typedef Offsets_ALL_RepsAft::const_iterator Offsets_ALL_RepsAft_Iterator;
#endif //_OFFSETS_H_
