function(apply_sanitizer_flags TARGET_NAME)
    if(ENABLE_ASAN)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND NOT WIN32)
            target_compile_options(${TARGET_NAME} PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
            )
            target_link_options(${TARGET_NAME} PRIVATE
                -fsanitize=address,undefined
            )
        else()
            message(WARNING "ENABLE_ASAN requested but the current toolchain does not provide ASan/UBSan runtime support; sanitizer flags are skipped.")
        endif()
    elseif(ENABLE_TSAN)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND NOT WIN32)
            target_compile_options(${TARGET_NAME} PRIVATE
                -fsanitize=thread
                -fno-omit-frame-pointer
            )
            target_link_options(${TARGET_NAME} PRIVATE
                -fsanitize=thread
            )
        else()
            message(WARNING "ENABLE_TSAN requested but the current toolchain does not provide TSan runtime support; sanitizer flags are skipped.")
        endif()
    endif()
endfunction()

add_library(apexquantum_core
    src/execution_engine.cpp
    src/market_data/market_data_parser.cpp
    src/matching/matching_engine.cpp
    src/network/udp_socket.cpp
    src/order_book/order_book.cpp
)

target_include_directories(apexquantum_core PUBLIC include)
apply_hft_compiler_flags(apexquantum_core)
apply_sanitizer_flags(apexquantum_core)

add_executable(apexquantum_engine src/main.cpp)
target_link_libraries(apexquantum_engine PRIVATE apexquantum_core)
apply_hft_compiler_flags(apexquantum_engine)
apply_sanitizer_flags(apexquantum_engine)

if(APEXQ_BUILD_TESTS)
    enable_testing()
    add_executable(apexquantum_core_tests tests/core_tests.cpp)
    target_link_libraries(apexquantum_core_tests PRIVATE apexquantum_core)
    apply_hft_compiler_flags(apexquantum_core_tests)
    apply_sanitizer_flags(apexquantum_core_tests)
    add_test(NAME apexquantum_core_tests COMMAND apexquantum_core_tests)
endif()

if(APEXQ_ENABLE_BENCHMARKS)
    find_package(benchmark QUIET)
    if(benchmark_FOUND)
        add_executable(apexquantum_latency_bench bench/latency_bench.cpp)
        target_link_libraries(apexquantum_latency_bench PRIVATE benchmark::benchmark apexquantum_core)
        apply_hft_compiler_flags(apexquantum_latency_bench)
        apply_sanitizer_flags(apexquantum_latency_bench)
    else()
        message(WARNING "Google Benchmark not found. Set benchmark_DIR or install benchmark to enable apexquantum_latency_bench.")
    endif()
endif()
