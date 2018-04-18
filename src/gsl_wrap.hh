#pragma once
#include <stdexcept>
#include <cstddef>
#include <gsl/gsl_histogram.h>
#include <algorithm>
#include <tuple>


namespace gsl {
    
    
    class Histogram{
    public:
        Histogram(std::size_t n);
        ~Histogram();
        
        //copy constructor
        Histogram(const Histogram& other);
        Histogram& operator=(const Histogram& other);
        
        //move constructor
        Histogram(Histogram&& other);
        Histogram& operator=(Histogram&& other);
        
        //number of bins
        std::size_t nbins() const;
        
        //number of samples
        unsigned long nsamp() const;
        
        //access element
        double operator[](std::size_t i) const;
        
        //add values
        void accumulate(double x);
        void accumulate(double x, int n);

        //find bin for value
        std::size_t find(double x) const;
        
        //stats
        double max() const;
        std::size_t max_bin() const;
        double min() const;
        std::size_t min_bin() const;
        double mean() const;
        double sigma() const;
        double sum() const;
        
        
        //histogram operations with other histograms
        Histogram& operator+=(const Histogram& other);
        const Histogram operator+(const Histogram& other);
        
        Histogram& operator-=(const Histogram& other);
        const Histogram operator-(const Histogram& other);
        
        Histogram& operator*= (const Histogram& other);
        const Histogram operator* (const Histogram& other);
        
        Histogram& operator/= (const Histogram& other);
        const Histogram operator/ (const Histogram& other);
        
        //histogram operations with scalars
        Histogram& operator*= (double x);
        const Histogram operator* (double x);
        
        Histogram& operator/= (double x);
        const Histogram operator/ (double x);
        
        Histogram& operator+= (double x);
        const Histogram operator+ (double x);
        
        Histogram& operator-= (double x);
        const Histogram operator- (double x);
        
        
        //iterators
        class iterator{
        public:
            
        private:
            iterator(const Histogram& hist);
            int i_;
        };
        
        Histogram::iterator& begin();
        Histogram::iterator& end();
        
        const Histogram::iterator& cbegin();
        const Histogram::iterator& cend();
        
        
    private:
        unsigned long nsamp_ = 0;
        gsl_histogram* hist_;
        
    };
    
    //TODO: SFINAE on container data type, iterable
    template<typename Container>
    const Histogram histogram_from_raw_data(const Container& data)
    {
        Histogram out(data.size());
        
        for(const auto& d : data)
        {
            out.accumulate(d);
        }
        
        return out;
    };
    
    template<typename DataContainer, typename WeightContainer>
    const Histogram histogram_from_raw_data(const DataContainer& data, const WeightContainer& weights)
    {
        if(data.size() != weights.size())
        {
            //TODO: throw exception
        }
        
        Histogram out(data.size());
        
        auto wit = weights.begin();
        for(auto dit = data.begin(); dit != data.end(); dit++)
        {
            out.accumulate(*dit, *(wit++));
        }
        
        return out;
    };
    
    
    template<typename Iterator>
    const Histogram histogram_from_raw_data(const Iterator& begin, const Iterator& end)
    {
        Histogram out(std::distance(begin, end));
        
        for(auto it = begin; it != end; it++)
        {
            out.accumulate(*it);
        }
        
        return out;
    }
    
    
    std::tuple<double,double> lngamma(double x);
    
    
    
}
