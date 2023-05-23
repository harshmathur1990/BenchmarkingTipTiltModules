#include <iostream>
#include <mutex>
#include <deque>
#include <benchmark/benchmark.h>

using namespace std;

constexpr unsigned NX = 256;
constexpr unsigned NY = 256;
mutex g_mutex;
deque<double> volQueue;

static void BM_ThreadTest(benchmark::State& state) {

    double* image = (double *) malloc(sizeof(double ) * NX * NY);
    double* recoveredImage = (double *) malloc(sizeof(double ) * NX * NY);

    for (unsigned i=0; i < NX * NY; i++) {
        image[i] = i * i + 2 * i + 1;
    }

    for (auto _ : state) {
        // This code gets timed
        unique_lock<mutex> ul(g_mutex);
        for (unsigned i=0; i < NX * NY; i++) {
            volQueue.push_back(image[i]);
        }
        ul.unlock();
        unique_lock<mutex> ul2(g_mutex);
        for (unsigned i=0; i < NX * NY; i++) {
            recoveredImage[i] = volQueue.front();
            volQueue.pop_front();
        }
        ul2.unlock();
        benchmark::DoNotOptimize(volQueue);
        benchmark::ClobberMemory();
    }
}


BENCHMARK(BM_ThreadTest);
// Run the benchmark
BENCHMARK_MAIN();