#pragma once

#include <algorithm>
#include <map>
#include <utility>
#include <vector>
#include <random>

namespace knuth{

    
    struct optpoint {
            double logP = 0.0;
            double logWt = 0.0;
    };
    
    using optmap = std::map<int, optpoint>;
    
    optmap::iterator find_worst(optmap& map);
    
    class NestedSamplingOptbins
    {
    public:
        NestedSamplingOptbins(const std::vector<double> & data, int step_size, std::size_t history_limit);
        
        optmap::iterator MCMCMove(optmap::iterator& point);
        void MCMCRefineStepSize();
        void trim_stored_calcs();
        
    private:
        //TODO: allow this to be supplied
        std::default_random_engine generator_;
        std::binomial_distribution<int> binomial_dist_;
        const std::vector<double>& data_;
        optmap stored_calcs;
        double data_min_;
        double data_max_;
        double logP_constraint;
        
        int MCMC_accepted_ = 0;
        int MCMC_rejected_ = 0;
        int step_size_;
        std::size_t history_limit_;
        
        
    };

}
