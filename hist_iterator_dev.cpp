#include <iostream>

#include <gsl_histogram.hh>
#include <gsl_error.hh>
#include <optbins.hh>

using std::cout;
using std::endl;


int main(int, char**)
{
    gsl::install_error_handler(true);
    
    gsl::Histogram hist(-50, 0, 50);
    
    hist.accumulate(10.5);
    hist.accumulate(30);
    
    cout << "sum: " << hist.sum() << endl;
    cout << "nbins: " << hist.nbins() << endl;
    
    for(auto it = hist.begin(); it != hist.end(); it++)
    {
        
        std::cout << "it == end? " << (it == hist.end() ) << std::endl;
        
        auto binrange = it.bin();
        std::cout << std::get<0>(binrange) << " - " << std::get<1>(binrange) << " : " << *it << std::endl;
        
    };
    
    std::cout << "logp: " << knuth::optbins_logp(hist) << std::endl;
    
    
};
