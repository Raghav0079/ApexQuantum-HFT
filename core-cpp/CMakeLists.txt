add_library(apexquantum_core
    src/execution_engine.cpp
    src/market_data/market_data_parser.cpp
    src/matching/matching_engine.cpp
    src/network/udp_socket.cpp
    src/order_book/order_book.cpp
)

target_include_directories(apexquantum_core PUBLIC include)
apply_hft_compiler_flags(apexquantum_core)

if(APEXQ_ENABLE_SANITIZERS)
    target_compile_options(apexquantum_core PRIVATE
        -fsanitize=address,undefined,leak
        -fno-omit-frame-pointer
    )
    target_link_options(apexquantum_core PRIVATE
        -fsanitize=address,undefined,leak
    )
endif()

add_executable(apexquantum_engine src/main.cpp)
target_link_libraries(apexquantum_engine PRIVATE apexquantum_core)
apply_hft_compiler_flags(apexquantum_engine)

if(APEXQ_ENABLE_SANITIZERS)
    target_compile_options(apexquantum_engine PRIVATE
        -fsanitize=address,undefined,leak
        -fno-omit-frame-pointer
    )
    target_link_options(apexquantum_engine PRIVATE
        -fsanitize=address,undefined,leak
    )
endif()

if(APEXQ_BUILD_TESTS)
    enable_testing()
    add_executable(apexquantum_core_tests tests/core_tests.cpp)
    target_link_libraries(apexquantum_core_tests PRIVATE apexquantum_core)
    apply_hft_compiler_flags(apexquantum_core_tests)
    add_test(NAME apexquantum_core_tests COMMAND apexquantum_core_tests)
endif()

if(APEXQ_ENABLE_BENCHMARKS)
    find_package(benchmark REQUIRED)
    add_executable(apexquantum_latency_bench bench/latency_bench.cpp)
    target_link_libraries(apexquantum_latency_bench PRIVATE benchmark::benchmark apexquantum_core)
    apply_hft_compiler_flags(apexquantum_latency_bench)
endif()
