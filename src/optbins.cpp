#include "optbins.hh"
#include <gsl/gsl_specfunc.h>
#include <cmath>

double knuth::optbins_logp(const gsl::Histogram& hist)
{
    
    auto M = hist.nbins();
    auto N = hist.nsamp();
    
    
    double p1 = N * std::log(M) + gsl_sf_lngamma(M / 2.) - gsl_sf_lngamma(N + M / 2.);
    //TODO: check this line
    
    
}
