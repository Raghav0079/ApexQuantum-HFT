add_library(apexquantum_core
    src/execution_engine.cpp
    src/market_data/market_data_parser.cpp
    src/matching/matching_engine.cpp
    src/network/udp_socket.cpp
    src/order_book/order_book.cpp
)

target_include_directories(apexquantum_core PUBLIC include)
apply_hft_compiler_flags(apexquantum_core)

add_executable(apexquantum_engine src/main.cpp)
target_link_libraries(apexquantum_engine PRIVATE apexquantum_core)
apply_hft_compiler_flags(apexquantum_engine)

if(APEXQUANTUM_BUILD_TESTS)
    enable_testing()
    add_executable(apexquantum_core_tests tests/core_tests.cpp)
    target_link_libraries(apexquantum_core_tests PRIVATE apexquantum_core)
    apply_hft_compiler_flags(apexquantum_core_tests)
    add_test(NAME apexquantum_core_tests COMMAND apexquantum_core_tests)
endif()
