#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <nested_sampling.hh>

namespace py = pybind11;
using namespace pybind11::literals;

using knuth::NestedSamplingOptbins;

PYBIND11_MODULE(libknuth_python, m) {
    py::class_<NestedSamplingOptbins>(m,"NestedSamplingOptbins")
        .def(py::init<const std::vector<double>&, int, int, std::size_t>(), 
            "data"_a, "step_size"_a, "npoints"_a, "history_limit"_a)
        .def("init_samples_uniform",&NestedSamplingOptbins::init_samples_uniform)
        .def("get_current_points", &NestedSamplingOptbins::get_current_points);
    
}
