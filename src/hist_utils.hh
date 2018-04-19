#pragma once
#include <gsl_histogram.hh>

namespace knuth {
 
    gsl::Histogram collapse_histogram(const gsl::Histogram& in, int N=1);
    
    
}
