#pragma once

#include <algorithm>
#include <map>
#include <utility>
#include <vector>
#include <random>

#include <util.hh>



namespace knuth{

    
    struct optpoint {
            double logP = 0.0;
            double logWt = 0.0;
    };
    
    using optmap = std::map<int, optpoint>;
    
    using optvec = std::vector<optmap::iterator>;


    template<typename T>
    typename T::iterator select_random(T& container, std::default_random_engine& generator, typename T::iterator const& worst);

    template<typename T>
    typename T::iterator find_worst(T& container);
    
    
    
    
    class NestedSamplingOptbins
    {
    public:
        NestedSamplingOptbins(const std::vector<double> & data, int step_size, int npoints, std::size_t history_limit);
        
        optmap::iterator MCMCMove(optvec::iterator& point);
        void MCMCRefineStepSize();
        void trim_stored_calcs();
        
        void init_samples_uniform();
        
        void iterate(int n_MCMC_trials);
        
        void iterator_test();
        
        std::vector<std::pair<int,double>> get_current_points() const;
        const optvec& view_current_points() const;
        
    private:
        //TODO: allow this to be supplied
        std::default_random_engine generator_;
        std::binomial_distribution<int> binomial_dist_;
        std::uniform_int_distribution<int> plus_minus_dist_;
        const std::vector<double>& data_;
        optmap stored_calcs;
        optvec selected_points;
        double data_min_;
        double data_max_;
        double logP_constraint;
        
        int MCMC_accepted_ = 0;
        int MCMC_rejected_ = 0;
        int step_size_;
        std::size_t history_limit_;
        
        
    };

        template<typename T>
    typename T::iterator select_random(T& container, std::default_random_engine& generator, typename T::iterator const& worst)
    {
        std::uniform_int_distribution<int> dist(0, std::distance(container.begin(), container.end()) -1);
    
        typename T::iterator out;
        
        do {
            out = container.begin();
            std::advance(out,dist(generator));
        } while (out == worst);
    
        
        return out;

    };

    
        template<typename T>
    typename T::iterator find_worst(T& container)
    {
        using E = typename T::value_type;
        
        auto it = std::min_element(container.begin(), container.end(),
                                   [] (const E& a, const E& b)
                                   {
                                      return detail::iterator_deref<E>::deref(a).second.logP < detail::iterator_deref<E>::deref(b).second.logP;
                                   });

        return it;
        
    };

    
    
}
