cmake_minimum_required(VERSION 3.22)
project(ApexQuantumHFT VERSION 0.1.0 LANGUAGES CXX)

option(APEXQUANTUM_BUILD_TESTS "Build C++ tests" ON)
option(APEXQ_BUILD_TESTS "Build C++ tests" ON)
option(APEXQ_ENABLE_SANITIZERS "Enable ASan/UBSan/LSan" OFF)
option(APEXQ_ENABLE_BENCHMARKS "Enable Google Benchmark" OFF)
option(ENABLE_ASAN "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

if(ENABLE_ASAN AND ENABLE_TSAN)
    message(FATAL_ERROR "ENABLE_ASAN and ENABLE_TSAN are mutually exclusive. Use only one sanitizer configuration per build tree.")
endif()

if((ENABLE_ASAN OR ENABLE_TSAN) AND NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Sanitize"))
    message(FATAL_ERROR "Sanitizer builds require CMAKE_BUILD_TYPE=Debug or CMAKE_BUILD_TYPE=Sanitize.")
endif()

if(APEXQUANTUM_BUILD_TESTS OR APEXQ_BUILD_TESTS)
    enable_testing()
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerFlags)
add_subdirectory(core-cpp)
