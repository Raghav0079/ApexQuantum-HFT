#include <benchmark/benchmark.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#include "common/spsc_queue.hpp"
#include "market_data/market_data_parser.hpp"
#include "order_book/order_book.hpp"

namespace {

template <typename T>
T percentile(const std::vector<T>& values, double p) {
    if (values.empty()) {
        return T{};
    }

    std::vector<T> sorted = values;
    std::sort(sorted.begin(), sorted.end());

    const auto index = static_cast<std::size_t>(std::ceil(p / 100.0 * static_cast<double>(sorted.size())) - 1.0);
    return sorted.at(std::min(index, sorted.size() - 1));
}

void report_latencies(const std::string& label, const std::vector<std::uint64_t>& latencies_ns) {
    if (latencies_ns.empty()) {
        return;
    }

    std::cout << label
              << " p50=" << percentile(latencies_ns, 50.0)
              << "ns p90=" << percentile(latencies_ns, 90.0)
              << "ns p99=" << percentile(latencies_ns, 99.0)
              << "ns" << std::endl;
}

}  // namespace

static void BM_SPSCQueue_EnqueueDequeue(benchmark::State& state) {
    apexquantum::SpscQueue<int, 1024> queue;
    std::vector<std::uint64_t> latencies_ns;
    latencies_ns.reserve(10'000);

    for (auto _ : state) {
        state.PauseTiming();
        int value = 42;
        int out = 0;
        state.ResumeTiming();

        const auto start = std::chrono::steady_clock::now();
        benchmark::DoNotOptimize(queue.push(value));
        benchmark::DoNotOptimize(queue.pop(out));
        const auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - start).count();

        latencies_ns.push_back(static_cast<std::uint64_t>(elapsed));
        state.SetIterationTime(static_cast<double>(elapsed) / 1'000'000'000.0);
    }

    report_latencies("SPSCQueue", latencies_ns);
}

static void BM_OrderBook_InsertCancelExecute(benchmark::State& state) {
    apexquantum::OrderBook book;
    std::vector<std::uint64_t> latencies_ns;
    latencies_ns.reserve(10'000);

    for (auto _ : state) {
        state.PauseTiming();
        apexquantum::Order order{};
        order.id = 1;
        order.price = 100000;
        order.quantity = 100;
        order.side = apexquantum::Side::Buy;
        state.ResumeTiming();

        const auto start = std::chrono::steady_clock::now();
        book.add(order);
        benchmark::DoNotOptimize(book.best_bid());
        const auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - start).count();

        latencies_ns.push_back(static_cast<std::uint64_t>(elapsed));
        state.SetIterationTime(static_cast<double>(elapsed) / 1'000'000'000.0);
    }

    report_latencies("OrderBook", latencies_ns);
}

static void BM_MarketDataParser_Parse(benchmark::State& state) {
    std::array<std::byte, 64> frame{};
    apexquantum::MarketDataHeader header{};
    std::vector<std::uint64_t> latencies_ns;
    latencies_ns.reserve(10'000);

    for (auto _ : state) {
        state.PauseTiming();
        header.msg_type = 1;
        header.channel_id = 2;
        header.seq_no = 42;
        header.nano_ts = 1000;
        header.payload_len = 8;
        std::memcpy(frame.data(), &header, sizeof(header));
        state.ResumeTiming();

        const auto start = std::chrono::steady_clock::now();
        apexquantum::MarketDataParser::parse_header(std::span<const std::byte>(frame), header);
        auto payload = apexquantum::MarketDataParser::payload(std::span<const std::byte>(frame));
        benchmark::DoNotOptimize(payload.size());
        const auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - start).count();

        latencies_ns.push_back(static_cast<std::uint64_t>(elapsed));
        state.SetIterationTime(static_cast<double>(elapsed) / 1'000'000'000.0);
    }

    report_latencies("MarketDataParser", latencies_ns);
}

BENCHMARK(BM_SPSCQueue_EnqueueDequeue)
    ->Unit(benchmark::kNanosecond)
    ->MinTime(2.0)
    ->Repetitions(10)
    ->UseRealTime();

BENCHMARK(BM_OrderBook_InsertCancelExecute)
    ->Unit(benchmark::kNanosecond)
    ->MinTime(2.0)
    ->Repetitions(10)
    ->UseRealTime();

BENCHMARK(BM_MarketDataParser_Parse)
    ->Unit(benchmark::kNanosecond)
    ->MinTime(2.0)
    ->Repetitions(10)
    ->UseRealTime();

BENCHMARK_MAIN();
