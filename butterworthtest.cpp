#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include "Butterworth.h"
#include <benchmark/benchmark.h>

constexpr unsigned FPS = 655;
constexpr unsigned NDERIVATIVES = FPS * 5 - 1;

static void BM_LPFTest(benchmark::State& state) {

    std::deque<double> inputDerivative;
    auto lowPassFilteredDervative = new double[NDERIVATIVES];

    // read input derivatives
    std::string fname = "/Volumes/SeagateHarsh9599771751/Polcalobs/20230522_082101/derivative_shifts_x.dat";
    ifstream derivativeFile(fname,  ios::binary);
    double TEMP;
    for (unsigned int j=0; j<NDERIVATIVES; j++){
        derivativeFile.read((char*) &TEMP, sizeof(double));
        inputDerivative.push_back(TEMP);
    }
    derivativeFile.close();

    int filterOrder = 8;
    double overallGain = 1;

    vector <Biquad> coeffs;  // second-order sections (sos)
    Butterworth butterworth;

     butterworth.loPass(655,  // fs
                                                110,    // freq1
                                                0,      // freq2. N/A for lowpass
                                                filterOrder,
                                                coeffs,
                                                overallGain);

    BiquadChain chain(coeffs.size());

    for (auto _ : state) {

        chain.processBiquad(inputDerivative, lowPassFilteredDervative, 1, NDERIVATIVES, coeffs.data());

        benchmark::DoNotOptimize(lowPassFilteredDervative);
        benchmark::ClobberMemory();
    }

    // write low pass filtered derivatives
    FILE *fp;
    fname = "lowPassFilteredDerivative.dat";
    fp = std::fopen(fname.c_str(), "wb");
    fwrite(lowPassFilteredDervative, sizeof(*lowPassFilteredDervative), NDERIVATIVES, fp);
    fclose(fp);
}

BENCHMARK(BM_LPFTest);
// Run the benchmark
BENCHMARK_MAIN();