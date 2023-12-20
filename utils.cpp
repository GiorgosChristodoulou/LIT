#include "def_global.h"



string toUpperCase(char *buf)
{
    auto i = 0;
    while (buf[i])
    {
        buf[i] = toupper(buf[i]);
        i++;
    }
    
    return string(buf);
}


bool checkPredicate(string strPredicate, RunSettings &settings)
{
    if (strPredicate == "EQUALS")
    {
        settings.typePredicate = PREDICATE_EQUALS;
        return true;
    }
    else if (strPredicate == "STARTS")
    {
        settings.typePredicate = PREDICATE_STARTS;
        return true;
    }
    else if (strPredicate == "STARTED")
    {
        settings.typePredicate = PREDICATE_STARTED;
        return true;
    }
    else if (strPredicate == "FINISHES")
    {
        settings.typePredicate = PREDICATE_FINISHES;
        return true;
    }
    else if (strPredicate == "FINISHED")
    {
        settings.typePredicate = PREDICATE_FINISHED;
        return true;
    }
    else if (strPredicate == "MEETS")
    {
        settings.typePredicate = PREDICATE_MEETS;
        return true;
    }
    else if (strPredicate == "MET")
    {
        settings.typePredicate = PREDICATE_MET;
        return true;
    }
    else if (strPredicate == "OVERLAPS")
    {
        settings.typePredicate = PREDICATE_OVERLAPS;
        return true;
    }
    else if (strPredicate == "OVERLAPPED")
    {
        settings.typePredicate = PREDICATE_OVERLAPPED;
        return true;
    }
    else if (strPredicate == "CONTAINS")
    {
        settings.typePredicate = PREDICATE_CONTAINS;
        return true;
    }
    else if (strPredicate == "CONTAINED")
    {
        settings.typePredicate = PREDICATE_CONTAINED;
        return true;
    }
    else if (strPredicate == "BEFORE")
    {
        settings.typePredicate = PREDICATE_PRECEDES;
        return true;
    }
    else if (strPredicate == "AFTER")
    {
        settings.typePredicate = PREDICATE_PRECEDED;
        return true;
    }
    if (strPredicate == "GOVERLAPS")
    {
        settings.typePredicate = PREDICATE_GOVERLAPS;
        return true;
    }

    return false;
}

bool checkAttributeConstraint(string strSecondAttributeConstraint, RunSettings &settings)
{
    if (strSecondAttributeConstraint == "GREATER")
    {
        settings.typeSecondAttributeConstraint = SECOND_ATTR_GREATERTHAN;
        return true;
    }
    else if (strSecondAttributeConstraint == "LOWER")
    {
        settings.typeSecondAttributeConstraint = SECOND_ATTR_LOWERTHAN;
        return true;
    }

    return false;
}

bool checkOptimizations(string strOptimizations, RunSettings &settings)
{
    if (strOptimizations == "")
    {
        settings.typeOptimizations = HINT_M_OPTIMIZATIONS_NO;
        return true;
    }
    else if (strOptimizations == "SUBS+SORT")
    {
        settings.typeOptimizations = HINT_M_OPTIMIZATIONS_SUBS_SORT;
        return true;
    }
    else if (strOptimizations == "SUBS+SOPT")
    {
        settings.typeOptimizations = HINT_M_OPTIMIZATIONS_SUBS_SOPT;
        return true;
    }
    else if (strOptimizations == "SUBS+SORT+SOPT")
    {
        settings.typeOptimizations = HINT_M_OPTIMIZATIONS_SUBS_SORT_SOPT;
        return true;
    }
    else if (strOptimizations == "SUBS+SORT+SOPT+SS")
    {
        settings.typeOptimizations = HINT_M_OPTIMIZATIONS_SUBS_SORT_SOPT_SS;
        return true;
    }
    else if (strOptimizations == "SUBS+SORT+SOPT+CM")
    {
        settings.typeOptimizations = HINT_M_OPTIMIZATIONS_SUBS_SORT_SOPT_CM;
        return true;
    }
    else if (strOptimizations == "SUBS+CM")
    {
        settings.typeOptimizations = HINT_M_OPTIMIZATIONS_SUBS_CM;
        return true;
    }
    else if (strOptimizations == "SUBS+SORT+SS+CM")
    {
        settings.typeOptimizations = HINT_M_OPTIMIZATIONS_SUBS_SORT_SS_CM;
        return true;
    }
    else if (strOptimizations == "ALL")
    {
        settings.typeOptimizations = HINT_M_OPTIMIZATIONS_ALL;
        return true;
    }
    else if (strOptimizations == "SS")
    {
        settings.typeOptimizations = HINT_OPTIMIZATIONS_SS;
        return true;
    }


    return false;
}


void process_mem_usage(double& vm_usage, double& resident_set)
{
    vm_usage     = 0.0;
    resident_set = 0.0;
    
    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
        >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
        >> ignore >> ignore >> vsize >> rss;
    }
    
    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}
