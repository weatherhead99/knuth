#include "optbins.hh"
#include <gsl/gsl_specfunc.h>
#include <cmath>
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

const double lngammahalf = gsl_sf_lngamma(0.5);

double knuth::optbins_logp(const gsl::Histogram& hist)
{
    
    int M = hist.nbins();
    int N = hist.sum();
    
    
    auto gammasum = std::accumulate(hist.begin(), hist.end(), 0.0, 
                                    [] (double cursum, double item)
                                    {
                                        return cursum + gsl_sf_lngamma(item + 0.5);
                                        
                                    });
    
    double p1 = N * std::log(M) + gsl_sf_lngamma(M / 2.) - gsl_sf_lngamma(N + M / 2.);
    //TODO: check this line
    
    
    double p2 = -1 * M * lngammahalf + gammasum;
    
    
    return p1 + p2;
}
