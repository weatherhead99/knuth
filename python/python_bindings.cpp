#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <nested_sampling.hh>

namespace py = pybind11;

using knuth::NestedSamplingOptbins;

PYBIND11_MODULE(libknuth_python, m) {
    py::class_<NestedSamplingOptbins>(m,"NestedSamplingOptbins")
        .def(py::init<const std::vector<double>&, int, int, std::size_t>());
    
    
}
