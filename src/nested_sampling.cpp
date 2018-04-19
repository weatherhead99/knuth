#include "nested_sampling.hh"

#include <algorithm>
#include <optbins.hh>


knuth::NestedSamplingOptbins::NestedSamplingOptbins(const std::vector<double>& data, int step_size, int npoints, std::size_t history_limit)
:  data_(data), step_size_(step_size), history_limit_(history_limit)
{
    binomial_dist_ = std::binomial_distribution<int>(step_size,0.5);
    data_min_ = *std::min_element(data.begin(), data.end());
    data_max_ = *std::max_element(data.begin(), data.end());
    
    selected_points.resize(npoints);
    
}

knuth::optmap::iterator knuth::NestedSamplingOptbins::MCMCMove(optvec::iterator& point)
{
    
    int testm = (*point)->first + binomial_dist_(generator_);
    
    //keep guessing if it's negative
    while(testm < 0)
    {
        testm = (*point)->first + binomial_dist_(generator_);
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

void knuth::NestedSamplingOptbins::trim_stored_calcs()
{
    while(stored_calcs.size() > history_limit_)
    {
        stored_calcs.erase(find_worst(stored_calcs));
    };
    
}

void knuth::NestedSamplingOptbins::init_samples_uniform()
{
    //TODO: data size should be configurable?
    std::uniform_int_distribution<int> dist(1,data_.size());
    
    for(std::size_t i = 0; i < selected_points.size(); i++)
    {
        auto bins = dist(generator_);
        while( stored_calcs.find(bins) == stored_calcs.end())
        {
            bins = dist(generator_);
        }
        
        optpoint pt;
        gsl::Histogram h(bins, data_min_, data_max_);
        
        pt.logP = knuth::optbins_logp(h);
        
        auto newpt = stored_calcs.emplace(bins,pt);
        selected_points[i] = newpt.first;
        
        logP_constraint = find_worst(stored_calcs)->second.logP;
        
    };
    
    
}


void knuth::NestedSamplingOptbins::iterate(int n_MCMC_trials)
{
    auto worst = find_worst(selected_points);
    
    //choose another random point that isn't the worst one...
    auto pt = select_random(selected_points,generator_, worst);
    
    //kill the worst in favour of another entry
    *worst = *pt;
    
    //MCMC 
    for(int i= 0; i < n_MCMC_trials; ++i)
    {
        auto newpt = MCMCMove(pt);
        if(newpt != stored_calcs.end())
        {
            *pt = newpt;
        }
        
        MCMCRefineStepSize();
    };
    
    trim_stored_calcs();
    
}

