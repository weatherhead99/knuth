#include <hist_utils.hh>

gsl::Histogram knuth::collapse_histogram(const gsl::Histogram& in, int N)
{
    if(N < 1)
    {
        throw std::out_of_range("N must be >=1");
    }
    
    if(in.nbins() % (2 << (N-1)))
    {
        throw std::runtime_error("nbins must divide equally into 2^N");
    }
        
    
    std::size_t nbins_out = in.nbins() / (2 << (N-1));
    
    gsl::Histogram out(nbins_out, 0, 50.);
    
    //TODO: need histogram iterator
    
    
}
