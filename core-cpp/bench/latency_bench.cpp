#include <benchmark/benchmark.h>

#include "order_book/order_book.hpp"

static void BM_OrderBookAdd(benchmark::State& state) {
    apexquantum::OrderBook book;
    for (auto _ : state) {
        apexquantum::Order order{};
        order.id = 1;
        order.price = 100000;
        order.quantity = 100;
        order.side = apexquantum::Side::Buy;
        benchmark::DoNotOptimize(book.add(order));
    }
}

BENCHMARK(BM_OrderBookAdd)
    ->Unit(benchmark::kNanosecond)
    ->MinTime(2.0)
    ->Repetitions(20);

BENCHMARK_MAIN();
