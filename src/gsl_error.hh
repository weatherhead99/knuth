#pragma once
#include <stdexcept>

namespace gsl {
    
    class Error : public std::exception 
    {
    public:
        const char * what() const noexcept override;
        Error(int status);
        
    private:
        int status_;
        
        
    };
    
    void install_error_handler(bool onoff);
    
    
}
