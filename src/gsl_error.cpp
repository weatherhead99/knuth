#include "gsl_error.hh"
#include <gsl/gsl_errno.h>
#include <iostream>

gsl::Error::Error(int status)
: status_(status)
{
}

const char * gsl::Error::what() const noexcept
{
    return gsl_strerror(status_);
}


void handler (const char* reason, const char* file,
              int line, int gsl_errno)
{
    std::cerr << file << " ( " << line << " ) " << " reason: " << reason << std::endl;
    
    throw gsl::Error(gsl_errno);
};


void gsl::install_error_handler(bool onoff)
{
    if(onoff)
    {
        gsl_set_error_handler(&handler);
        
    }
    else 
    {
        gsl_set_error_handler_off();
    }
    
}


