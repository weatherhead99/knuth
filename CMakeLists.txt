project(knuth)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)

find_package(GSL REQUIRED)

add_library(knuth src/gsl_histogram.cpp src/gsl_error.cpp src/optbins.cpp src/hist_utils.cpp src/nested_sampling.cpp)
target_include_directories(knuth PUBLIC src)
target_link_libraries(knuth PUBLIC ${GSL_LIBRARIES})

add_executable(hist_iterator_dev hist_iterator_dev.cpp)
target_link_libraries(hist_iterator_dev PRIVATE knuth)

add_executable(optbins_examples optbins_examples.cpp)
target_link_libraries(optbins_examples PRIVATE knuth)

add_executable(nested_sampling_play nested_sampling_play.cpp)
target_link_libraries(nested_sampling_play PRIVATE knuth)
