#include <iostream>
#include <random>

#include <nested_sampling.hh>

using std::cout;
using std::endl;

const int nsamp = 75;

int main(int, char**)
{
    std::default_random_engine generator;
    
    std::normal_distribution<double> dist(0.0,3.0);
    
    std::vector<double> samples(nsamp);
    
    for(auto& i : samples)
    {
        i = dist(generator);
    }

    
    knuth::NestedSamplingOptbins opt(samples, 10000, 50, 75);

    cout << "init samples..." << endl;
    opt.init_samples_uniform();
    
    cout << "get points... " << endl;
    auto pts_before = opt.get_current_points();
    
    
    for(int i= 0; i < 100; i++)
    {
        opt.iterate(100);
    }
    
    
    auto pts_after = opt.get_current_points();
    
    int n_diff = 0;
    auto pbit = pts_before.begin();
    for(auto& v : pts_after)
    {
        if(v != *(pbit++))
        {
            n_diff++;
        }
    }
    
    pbit = pts_before.begin();
    for(auto& v : pts_after)
    {
     std::cout << v.first << " : " << v.second  << " --- " << pbit->first << " : " << pbit->second <<  std::endl;   
     ++pbit;
    }
    std::cout << "---------------------------" << std::endl;
    
    std::cout << "ndiff: " << n_diff << std::endl;

};
