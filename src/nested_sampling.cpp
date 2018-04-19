#include "nested_sampling.hh"

#include <algorithm>
#include <optbins.hh>

knuth::optmap::iterator knuth::find_worst(optmap& map)
{
    auto it = std::min_element(map.begin(), map.end(),
                               [] (const optmap::value_type& a, const optmap::value_type& b)
                               {
                                   return a.second.logP < b.second.logP;
                               });
    return it;
    
}


knuth::NestedSamplingOptbins::NestedSamplingOptbins(const std::vector<double>& data, int step_size)
: data_(data), step_size_(step_size)
{
    binomial_dist_ = std::binomial_distribution<int>(step_size,0.5);
    data_min_ = *std::min_element(data.begin(), data.end());
    data_max_ = *std::max_element(data.begin(), data.end());
}

knuth::optmap::iterator knuth::NestedSamplingOptbins::MCMCMove(optmap::iterator& point)
{
    
    int testm = point->first + binomial_dist_(generator_);
    
    //keep guessing if it's negative
    while(testm < 0)
    {
        testm = point->first + binomial_dist_(generator_);
    }
    
    //NOTE: data.size() this should be maxbins (might be the same or different)
    testm %= data_.size();
    
    
    //check if we have this result already
    //TODO: limit previously stored results
    
    auto loc = stored_calcs.find(testm);
    if(loc == stored_calcs.end())
    {
        //not in the collection, calculate and add
        gsl::Histogram h(testm,data_min_, data_max_);
        optpoint pt;
        pt.logP = knuth::optbins_logp(h);
        
        //TODO: calculate logW if calculating mean
        
        auto res = stored_calcs.emplace(testm,pt);
        //TODO: check value of returned bool
        
        loc = res.first;
        
    }
    
    
    if(loc->second.logP > logP_constraint)
    {
        ++MCMC_accepted_;
    }
    else
    {
        ++MCMC_rejected_;
        loc = stored_calcs.end();
    }
    
    MCMCRefineStepSize();
    return loc;
    
}

void knuth::NestedSamplingOptbins::MCMCRefineStepSize()
{
    bool changed = false;
    
    
    if(MCMC_accepted_ > MCMC_rejected_)
    {
        step_size_ *= std::ceil(std::exp(1. / MCMC_accepted_));
        changed = true;
    }
    
    if(MCMC_rejected_ > MCMC_accepted_)
    {
        step_size_ /= std::ceil(std::exp(1./MCMC_rejected_));
        changed = true;
    }
    
    if(changed)
    {
        binomial_dist_ = std::binomial_distribution<int>(step_size_,0.5);
    }
}


