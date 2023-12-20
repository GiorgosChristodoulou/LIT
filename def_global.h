#pragma once
#ifndef _GLOBAL_DEF_H_
#define _GLOBAL_DEF_H_

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <tuple>
#include <limits.h>

using namespace std;


// Comment out the following for XOR workload
// #define WORKLOAD_COUNT

// Basic predicates of Allen's algebra
#define PREDICATE_EQUALS     1
#define PREDICATE_STARTS     2
#define PREDICATE_STARTED    3
#define PREDICATE_FINISHES   4
#define PREDICATE_FINISHED   5
#define PREDICATE_MEETS      6
#define PREDICATE_MET        7
#define PREDICATE_OVERLAPS   8
#define PREDICATE_OVERLAPPED 9
#define PREDICATE_CONTAINS   10
#define PREDICATE_CONTAINED  11
#define PREDICATE_PRECEDES   12
#define PREDICATE_PRECEDED   13

// Generalized predicates, ACM SIGMOD'22 gOverlaps
#define PREDICATE_GOVERLAPS  14

#define SECOND_ATTR_GREATERTHAN 0
#define SECOND_ATTR_LOWERTHAN 1

#define HINT_OPTIMIZATIONS_NO          0
#define HINT_OPTIMIZATIONS_SS          1

#define HINT_M_OPTIMIZATIONS_NO                   0
#define HINT_M_OPTIMIZATIONS_SUBS                 1
#define HINT_M_OPTIMIZATIONS_SUBS_SORT            2
#define HINT_M_OPTIMIZATIONS_SUBS_SOPT            3
#define HINT_M_OPTIMIZATIONS_SUBS_SORT_SOPT       4
#define HINT_M_OPTIMIZATIONS_SUBS_SORT_SOPT_SS    5
#define HINT_M_OPTIMIZATIONS_SUBS_SORT_SOPT_CM    6
#define HINT_M_OPTIMIZATIONS_SUBS_CM              7
#define HINT_M_OPTIMIZATIONS_SUBS_SORT_SS_CM      8
#define HINT_M_OPTIMIZATIONS_ALL                  9
#define HINT_M_DYNAMIC                            10


#define MAX_ICDE16_CAPACITY   500000

#define MAX_ATTRIBUTE_VALUE 31406400

typedef int PartitionId;
typedef int RecordId;
typedef int Timestamp;


struct RunSettings
{
	string       method;
	const char   *dataFile;
	const char   *queryFile;
	bool         verbose;
    unsigned int typeQuery;
    unsigned int typePredicate;
    unsigned int typeSecondAttributeConstraint;
	unsigned int numPartitions;
	unsigned int numBits;
	unsigned int maxBits;
	bool         topDown;
	bool         isAutoTuned;
	unsigned int numRuns;
    unsigned int typeOptimizations;
	
	void init()
	{
        numBits           = 0;
		verbose	          = false;
		topDown           = false;
		isAutoTuned       = false;
		numRuns           = 1;
        typeOptimizations = 0;
	};
};


struct StabbingQuery
{
	size_t id;
	Timestamp point;
    
    StabbingQuery()
    {
        
    };
    StabbingQuery(size_t i, Timestamp p)
    {
        id = i;
        point = p;
    };
};

struct RangeQuery
{
	size_t id;
	Timestamp start, end;

    RangeQuery()
    {
        
    };
    RangeQuery(size_t i, Timestamp s, Timestamp e)
    {
        id = i;
        start = s;
        end = e;
    };
};


class Timer
{
private:
	using Clock = std::chrono::high_resolution_clock;
	Clock::time_point start_time, stop_time;
	
public:
	Timer()
	{
		start();
	}
	
	void start()
	{
		start_time = Clock::now();
	}
	
	
	double getElapsedTimeInSeconds()
	{
		return std::chrono::duration<double>(stop_time - start_time).count();
	}
	
	
	double stop()
	{
		stop_time = Clock::now();
		return getElapsedTimeInSeconds();
	}
};


// Imports from utils
string toUpperCase(char *buf);
bool checkPredicate(string strPredicate, RunSettings &settings);
bool checkAttributeConstraint(string typeSecondAttributeConstraint, RunSettings &settings);
bool checkOptimizations(string strOptimizations, RunSettings &settings);
void process_mem_usage(double& vm_usage, double& resident_set);
#endif // _GLOBAL_DEF_H_
