#pragma once
#include <gsl_histogram.hh>
#include <tuple>


namespace knuth {
    
    double optbins_logp(const gsl::Histogram& hist);
    
    template<typename Iterator>
    double optbins_logp(const Iterator& start, const Iterator& end, std::size_t nbins)
    {
        auto max = *std::max_element(start,end);
        auto min = *std::min_element(start,end);
        
        gsl::Histogram h(nbins, min, max);
        for(auto it = start; it != end; ++it)
        {
            h.accumulate(*it);
        };
        
        return optbins_logp(h);
        
    }
    
    
    std::tuple<gsl::Histogram, double> optbins_nested_sampling(int niterations, int npoints, const std::vector<double>& data);
    
    
    
}
