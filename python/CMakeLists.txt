
add_library(knuth_python MODULE python_bindings.cpp)
target_link_libraries(knuth_python pybind11::module knuth)

file(GLOB pyfiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.py)

message(STATUS "python files: ${pyfiles}")

file(COPY ${pyfiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
