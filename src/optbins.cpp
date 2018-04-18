#include "optbins.hh"
#include <gsl/gsl_specfunc.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>


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


std::tuple<gsl::Histogram, double> knuth::optbins_nested_sampling(int niterations, int npoints, const std::vector<double>& data)
{
    std::default_random_engine generator;
    
    
    double Z = 0.0;
    
    auto maxpoints = data.size();
    std::uniform_int_distribution<int> distribution(1,maxpoints);
    
    double maxval = *std::max_element(data.begin(),data.end());
    double minval = *std::min_element(data.begin(), data.end());
    
    //sample points from the prior
    std::vector<double> sample_points(npoints);
    std::vector<int> sample_N(npoints);
    auto nit = sample_N.begin();
    
    for(auto& v : sample_points)
    {
        *nit = distribution(generator);
        
        gsl::Histogram hist(*nit,minval,maxval);
        v = optbins_logp(hist);
        ++nit;
    };
    
    double X = 1.;
    
    for (int i=1; i <= niterations; ++i)
    {
      double L = *std::min_element(sample_points.begin(), sample_points.end());  
      double nextX = std::exp(- i / npoints);
      double w = X - nextX;
      X = nextX;
      
      
    };
    
    
    
    
    
}



