#include <benchmark/benchmark.h>
#include "page_replacement.h"
#include <vector>
#include <random>

// Genera richieste casuali
static std::vector<int> generate_requests(size_t n, int max_page = 50) {
    std::vector<int> v(n);
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dist(0, max_page);

    for (size_t i = 0; i < n; i++) {
        v[i] = dist(gen);
    }
    return v;
}

// Benchmark FIFO
static void BM_FIFO(benchmark::State& state) {
    size_t capacity = state.range(0);
    auto requests = generate_requests(10000);

    for (auto _ : state) {
        fifo_page_faults(capacity, requests.data(), requests.size());
    }
}

// Benchmark LRU
static void BM_LRU(benchmark::State& state) {
    size_t capacity = state.range(0);
    auto requests = generate_requests(10000);

    for (auto _ : state) {
        lru_page_faults(capacity, requests.data(), requests.size());
    }
}

// Test con diverse dimensioni memoria
BENCHMARK(BM_FIFO)->Arg(4)->Arg(8)->Arg(16)->Arg(32);
BENCHMARK(BM_LRU)->Arg(4)->Arg(8)->Arg(16)->Arg(32);

BENCHMARK_MAIN();