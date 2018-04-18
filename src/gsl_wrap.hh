#pragma once
#include <stdexcept>
#include <cstddef>
#include <gsl/gsl_histogram.h>
#include <algorithm>


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
        
    private:
        gsl_histogram* hist_;
        
    };
    
    
    
}
