#include <gsl_histogram.hh>
#include <gsl/gsl_specfunc.h>

gsl::Histogram::Histogram(std::size_t n, double min, double max)
{
    hist_ = gsl_histogram_alloc(n);
    
    int err = gsl_histogram_set_ranges_uniform(hist_,min,max);
    //TODO: error handling
    
}

gsl::Histogram::Histogram(std::size_t n, const std::vector<double>& bins)
{
    hist_ = gsl_histogram_alloc(n);
    int err = gsl_histogram_set_ranges(hist_,bins.data(), bins.size());
}


gsl::Histogram::~Histogram()
{
    gsl_histogram_free(hist_);
}


gsl::Histogram::Histogram(const gsl::Histogram& other)
{
    hist_ = gsl_histogram_clone(other.hist_);
}

gsl::Histogram & gsl::Histogram::operator=(const gsl::Histogram& other)
{
    //TODO: need to free existing histogram if it exists
    hist_ = gsl_histogram_clone(other.hist_);
    return *this;
}


gsl::Histogram::Histogram(gsl::Histogram && other)
{
    hist_ = other.hist_;
}

gsl::Histogram & gsl::Histogram::operator=(gsl::Histogram && other)
{
    hist_ = other.hist_;
    return *this;
}

std::size_t gsl::Histogram::nbins() const
{
    return gsl_histogram_bins(hist_);
}


double gsl::Histogram::operator[](std::size_t i) const
{
    double ret = gsl_histogram_get(hist_, i);
    
    if(ret == 0.0)
    {
        //TODO: GSL error handling to exceptions
    };
    
    return ret;
    
}

std::tuple<double, double> gsl::Histogram::binrange(std::size_t i) const
{
    std::tuple<double,double> out{0.,0.};
    
    int err = gsl_histogram_get_range(hist_,i,&std::get<0>(out), &std::get<1>(out));
    
    //TODO: error handling!
    
    return out;
    
}




void gsl::Histogram::accumulate(double x)
{
    int err = gsl_histogram_increment(hist_, x);
    //TODO error handling
}

void gsl::Histogram::accumulate(double x, int n)
{
    int err = gsl_histogram_accumulate(hist_, x, n);
    //TODO error handling
}

std::size_t gsl::Histogram::find(double x) const
{
    std::size_t res;
    int err = gsl_histogram_find(hist_, x, &res);
    
    
    //TODO error handling
    
    return res;
}



double gsl::Histogram::max() const //noexcept??
{
    return gsl_histogram_max_val(hist_);
}

std::size_t gsl::Histogram::max_bin() const
{
    return gsl_histogram_max_bin(hist_);
}

double gsl::Histogram::min() const
{
    return gsl_histogram_min(hist_);
}

std::size_t gsl::Histogram::min_bin() const
{
    return gsl_histogram_min_bin(hist_);
}

double gsl::Histogram::mean() const
{
    return gsl_histogram_mean(hist_);
}

double gsl::Histogram::sigma() const
{
    return gsl_histogram_sigma(hist_);
}

double gsl::Histogram::sum() const
{
    return gsl_histogram_sum(hist_);
}


gsl::Histogram & gsl::Histogram::operator+=(const gsl::Histogram& other)
{
    int err = gsl_histogram_add(hist_, other.hist_);
    //TODO error handling
    return *this;
    
}


const gsl::Histogram gsl::Histogram::operator+(const gsl::Histogram& other)
{
    Histogram out(*this);
    out += other;
    return out;
}

gsl::Histogram & gsl::Histogram::operator-=(const gsl::Histogram& other)
{
    int err = gsl_histogram_sub(hist_, other.hist_);
    //TODO error handling
    return *this;
}

const gsl::Histogram gsl::Histogram::operator-(const gsl::Histogram& other)
{
    Histogram out(*this);
    out -= other;
    return out;
}

gsl::Histogram & gsl::Histogram::operator*=(const gsl::Histogram& other)
{
    int err = gsl_histogram_mul(hist_, other.hist_);
    //TODO error handling
    return *this;
}

const gsl::Histogram gsl::Histogram::operator*(const gsl::Histogram& other)
{
    Histogram out(*this);
    out *= other;
    return out;
    
}

gsl::Histogram & gsl::Histogram::operator/=(const gsl::Histogram& other)
{
    int err = gsl_histogram_div(hist_, other.hist_);
    //TODO error handling
    return *this;
}

const gsl::Histogram gsl::Histogram::operator/(const gsl::Histogram& other)
{
    Histogram out(*this);
    out /= other;
    return out;
    
}


gsl::Histogram & gsl::Histogram::operator*=(double x)
{
    int err = gsl_histogram_scale(hist_, x);
    //TODO error handling
    
    return *this;
}

const gsl::Histogram gsl::Histogram::operator*(double x)
{
    Histogram out(*this);
    out *= x;
    return out;
}

gsl::Histogram & gsl::Histogram::operator/=(double x)
{
    return (*this)*=(1./x);
}

const gsl::Histogram gsl::Histogram::operator/(double x)
{
    return (*this) * (1./x);
}


gsl::Histogram & gsl::Histogram::operator+=(double x)
{
    int err = gsl_histogram_shift(hist_, x);
    //TODO error handling
    
    return (*this);
}

const gsl::Histogram gsl::Histogram::operator+(double x)
{
    Histogram out(*this);
    out += x;
    return out;
}

gsl::Histogram & gsl::Histogram::operator-=(double x)
{
    int err = gsl_histogram_shift(hist_, -1 * x);
    //TODO error handling
    
    return (*this);
}

const gsl::Histogram gsl::Histogram::operator-(double x)
{
    Histogram out(*this);
    out -= x;
    return out;
}


gsl::Histogram::iterator::iterator(const gsl::Histogram& hist, int begin)
: hist_(&hist), i_(begin)
{
}


gsl::Histogram::iterator & gsl::Histogram::iterator::operator=(const gsl::Histogram::iterator& other)
{
    hist_ = other.hist_;
    i_ = other.i_;
    
    return *this;
}

gsl::Histogram::iterator::iterator(const gsl::Histogram::iterator& other)
{
    hist_ = other.hist_;
    i_ = other.i_;
}

double gsl::Histogram::iterator::operator*()
{    
    return hist_->operator[](i_);
}

std::tuple<double, double> gsl::Histogram::iterator::bin()
{
    return hist_->binrange(i_);
    
}


gsl::Histogram::iterator& gsl::Histogram::iterator::operator++()
{
    ++i_;
    if(i_ == (hist_->nbins() ))
        i_ = -1;
    return *this;
}

gsl::Histogram::iterator gsl::Histogram::iterator::operator++(int)
{
    auto tmp = *this;
    ++i_;
    if(i_ == (hist_->nbins() ) )
        i_ = -1;
    
    return tmp;
}

bool gsl::Histogram::iterator::operator==(const gsl::Histogram::iterator& other)
{
    return i_ == other.i_;
}

bool gsl::Histogram::iterator::operator!=(const gsl::Histogram::iterator& other)
{
    return !(*this == other);
}


gsl::Histogram::iterator gsl::Histogram::begin() const
{
    return iterator(*this);
}

gsl::Histogram::iterator gsl::Histogram::end() const
{
    return iterator(*this,-1);
}





std::tuple<double, double> gsl::lngamma(double x)
{
    gsl_sf_result res;
    
    int err = gsl_sf_gamma_e(x, &res);
    
    //TODO: error handling!
    
    
    return std::make_tuple(res.val, res.err);
    
}



