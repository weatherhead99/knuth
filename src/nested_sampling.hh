#pragma once

#include <algorithm>
#include <map>
#include <utility>

namespace knuth{

    
    struct optpoint {
            double m = 0.0;
            double logP = 0.0;
            double logWt = 0.0;
        
    };
    
    using optmap = std::map<std::size_t, optpoint>;
    
    optmap::iterator find_worst(optmap& map);
    
    

}
