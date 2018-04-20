#include "nested_sampling.hh"

#include <algorithm>
#include <optbins.hh>

#include <iostream>

knuth::NestedSamplingOptbins::NestedSamplingOptbins(const std::vector<double>& data, int step_size, int npoints, std::size_t history_limit)
:  data_(data), step_size_(step_size), history_limit_(history_limit)
{
    
    if(npoints > data.size())
    {
        throw std::out_of_range("can't have more sample points than data size!");
    };
    
    if(history_limit > data.size())
    {
        throw std::out_of_range("can't have higher history limit than data size!");
    }
    
    binomial_dist_ = std::binomial_distribution<int>(step_size,0.5);
    plus_minus_dist_ = std::uniform_int_distribution<int>(0,2);
    data_min_ = *std::min_element(data.begin(), data.end());
    data_max_ = *std::max_element(data.begin(), data.end());
    
    selected_points.resize(npoints);
    
}

knuth::optmap::iterator knuth::NestedSamplingOptbins::MCMCMove(optvec::iterator& point)
{
    
    int testm = (*point)->first + (plus_minus_dist_(generator_) -1) * binomial_dist_(generator_);
    //NOTE: data.size() this should be maxbins (might be the same or different)
    testm %= data_.size();
    
    std::cout << "testm: " << testm << std::endl;
    //keep guessing if it's negative
    while(testm <= 0)
    {
        testm = (*point)->first + (plus_minus_dist_(generator_) -1) * binomial_dist_(generator_);
    }
    
    
    
    //check if we have this result already
    //TODO: limit previously stored results
    
    auto loc = stored_calcs.find(testm);
    if(loc == stored_calcs.end())
    {
        std::cout << "new calculation..." << "testm: " << testm <<  std::endl;
        
        //not in the collection, calculate and add
        gsl::Histogram h(testm,data_min_, data_max_);
        h.accumulate(data_.begin(), data_.end());
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
    
    std::cout << "accept: " << MCMC_accepted_ << "reject: " << MCMC_rejected_ << std::endl;
    
    if(MCMC_accepted_ > MCMC_rejected_)
    {
        step_size_ *= std::ceil(  std::exp(1. / MCMC_accepted_));
        changed = true;
    }
    
    if(MCMC_accepted_ < MCMC_rejected_)
    {
        step_size_ /= std::floor( std::exp(1./MCMC_rejected_));
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
        while( stored_calcs.find(bins) != stored_calcs.end())
        {
            bins = dist(generator_);
        }
        
        optpoint pt;
        gsl::Histogram h(bins, data_min_, data_max_);
        h.accumulate(data_.begin(), data_.end());
        pt.logP = knuth::optbins_logp(h);
        
        
        auto newpt = stored_calcs.emplace(bins,pt);
        selected_points[i] = newpt.first;
        
        logP_constraint = find_worst(stored_calcs)->second.logP;
        
    };
    
    
}


void knuth::NestedSamplingOptbins::iterate(int n_MCMC_trials)
{
    auto worst = find_worst(selected_points);
    
    std::cout << "worst n: " << (*worst)->first << " logP: " << (*worst)->second.logP << std::endl;
    
    //choose another random point that isn't the worst one...
    auto pt = select_random(selected_points,generator_, worst);
    
    std::cout << "random n: " << (*pt)->first << " logP: " << (*pt)->second.logP << std::endl;
    
    //kill the worst in favour of another entry
    *worst = *pt;
    
    logP_constraint = (*pt)->second.logP;
    
    std::cout << "re-assigned worst n: " << (*worst)->first << " logP: " << (*worst)->second.logP << std::endl;
    
    
    //MCMC 
    for(int i= 0; i < n_MCMC_trials; ++i)
    {
        optmap::iterator newpt = MCMCMove(pt);
        if(newpt != stored_calcs.end())
        {
            std::cout << "new point found!" << " n: " << newpt->first << " logP: " << newpt->second.logP << std::endl;
            
            *worst = newpt;
            
            std::cout << "re-assigned worst n: " << (*worst)->first << " logP: " << (*worst)->second.logP << std::endl;
        }
        else
        {
            std::cout << "no new point found!" << std::endl;
        };
        
        MCMCRefineStepSize();
        std::cout << "step size: " << step_size_ << std::endl;

        
    };
    
    
    
    
//     trim_stored_calcs();
    
}


std::vector<std::pair<int, double> > knuth::NestedSamplingOptbins::get_current_points() const
{
    std::vector<std::pair<int,double>> out(selected_points.size());
    
    auto oit = out.begin();
    for(auto it : selected_points)
    {
        *(oit++) = std::make_pair(it->first, it->second.logP);
    }
    
    return out;
}



