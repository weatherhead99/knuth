project(knuth)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)

option(BUILD_PYTHON_BINDINGS "build pybind11 python bindings (requires pybind11)" OFF)
if(EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
    message(WARNING "found conan_paths.cmake, doing Conan build")
    include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()

find_package(GSL REQUIRED)

if(BUILD_PYTHON_BINDINGS)
find_package(pybind11 REQUIRED)
message(STATUS "found pybind11 v${pybind11_VERSION} : ${pybind11_INCLUDE_DIRS}")
add_subdirectory(python)
endif()
    

add_library(knuth src/gsl_histogram.cpp src/gsl_error.cpp src/optbins.cpp src/hist_utils.cpp src/nested_sampling.cpp)
target_include_directories(knuth PUBLIC src)
target_link_libraries(knuth PUBLIC ${GSL_LIBRARIES})

if(BUILD_PYTHON_BINDINGS)
    set_target_properties(knuth PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

add_executable(hist_iterator_dev hist_iterator_dev.cpp)
target_link_libraries(hist_iterator_dev PRIVATE knuth)

add_executable(optbins_examples optbins_examples.cpp)
target_link_libraries(optbins_examples PRIVATE knuth)

add_executable(nested_sampling_play nested_sampling_play.cpp)
target_link_libraries(nested_sampling_play PRIVATE knuth)

add_executable(nested_sample_data nested_sample_data.cpp)
target_link_libraries(nested_sample_data PRIVATE knuth)
