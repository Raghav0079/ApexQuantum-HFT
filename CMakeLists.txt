cmake_minimum_required(VERSION 3.22)
project(ApexQuantumHFT VERSION 0.1.0 LANGUAGES CXX)

option(APEXQUANTUM_BUILD_TESTS "Build C++ tests" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerFlags)
add_subdirectory(core-cpp)
