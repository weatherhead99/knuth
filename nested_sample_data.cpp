#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <nested_sampling.hh>

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
  
    if(argc < 2)
    {
        cout << "must supply a file name..."<<endl ;
        return -1;
    }
    
    cout <<"file name: " << argv[1] << endl;
    
    std::ifstream ifs(argv[1]);
    
    if(!ifs.is_open())
    {
        cout << "couldn't open file!" << endl;
        return -1;
    };
    
    std::vector<double> data;
    
    for(std::string line; std::getline(ifs,line);)
    {
        data.push_back(std::stod(line));   
    };
    
    knuth::NestedSamplingOptbins samp(data,10,100,1000);
    samp.init_samples_uniform();
    
    for(int i=0; i < 1000; i++)
    {
        samp.iterate(100);
    }
    
    auto pts_after = samp.get_current_points();
    
    for(auto& v : pts_after)
    {
        cout << v.first << " : " << v.second << endl;
    }
    
    
    
};
