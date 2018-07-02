
add_library(knuth_python MODULE python_bindings.cpp)
target_link_libraries(knuth_python pybind11::module knuth)
