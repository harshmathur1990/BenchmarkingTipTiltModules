#include <opencv2/opencv.hpp>
#include <benchmark/benchmark.h>

constexpr unsigned NX = 256;
constexpr unsigned NY = 256;

static void BM_OpenCVTest(benchmark::State& state) {
    double* image = (double *) malloc(sizeof(double ) * NX * NY);

    for (unsigned i=0; i < NX * NY; i++) {
        image[i] = i * i + 2 * i + 1;
    }

    for (auto _ : state) {
        cv::Mat img;
        img = cv::Mat(cv::Size(NX, NY), CV_64FC1, image);
        benchmark::DoNotOptimize(img);
        benchmark::ClobberMemory();
    }
}

static void BM_MemCopyTest(benchmark::State& state) {
    double* image = (double *) malloc(sizeof(double ) * NX * NY);
    double* recoveredMemcpy = (double *) malloc(sizeof(double ) * NX * NY);

    for (unsigned i=0; i < NX * NY; i++) {
        image[i] = i * i + 2 * i + 1;
    }

    for (auto _ : state) {
        cv::Mat img;
        img = cv::Mat(cv::Size(NX, NY), CV_64FC1, image);
        benchmark::DoNotOptimize(img);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_OpenCVTest);
// Run the benchmark
BENCHMARK_MAIN();