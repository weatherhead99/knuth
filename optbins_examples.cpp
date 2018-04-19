#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

#include <gsl_histogram.hh>
#include <optbins.hh>


using std::cout;
using std::endl;

const int nsamp = 500;


int main(int , char** )
{
    std::default_random_engine generator;
    
    std::normal_distribution<double> dist(0.0,3.0);
    
    std::vector<double> samples(nsamp);
    std::vector<double> logp(nsamp);
    
    for(auto& i : samples)
    {
        i = dist(generator);
    }

    for(auto s : samples)
    {
        cout << s << " ";
    }
    
    cout << endl;
    
    //optbins brute force
    auto logpit = logp.begin();
    
    
    for(int i=1; i < nsamp; i++)
    {
        
        *logpit = knuth::optbins_logp(samples.begin(),samples.end(), i);
        
        std::cout << i << "," << *logpit << endl;
        
        ++logpit;
        
    };

};
