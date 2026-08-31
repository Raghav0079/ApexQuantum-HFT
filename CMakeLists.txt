cmake_minimum_required(VERSION 3.22)
project(ApexQuantumHFT VERSION 0.1.0 LANGUAGES CXX)

option(APEXQUANTUM_BUILD_TESTS "Build C++ tests" ON)
option(APEXQ_BUILD_TESTS "Build C++ tests" ON)
option(APEXQ_ENABLE_SANITIZERS "Enable ASan/UBSan/LSan" OFF)
option(APEXQ_ENABLE_BENCHMARKS "Enable Google Benchmark" OFF)

if(APEXQUANTUM_BUILD_TESTS OR APEXQ_BUILD_TESTS)
    enable_testing()
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerFlags)
add_subdirectory(core-cpp)
