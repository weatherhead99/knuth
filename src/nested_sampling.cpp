#include "nested_sampling.hh"

#include <algorithm>

knuth::optmap::iterator knuth::find_worst(optmap& map)
{
    auto it = std::min_element(map.begin(), map.end(),
                               [] (const optmap::value_type& a, const optmap::value_type& b)
                               {
                                   return a.second.logP < b.second.logP;
                               });
    return it;
    
}
