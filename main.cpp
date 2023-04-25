#include <iostream>
#include <fstream>
#include <mkl.h>
#include <complex>
#include <chrono>
#include <fftw3.h>
#include <benchmark/benchmark.h>
#define NN 256
#define NPIXFFT NN * (1 + (NN / 2))
using namespace std;

typedef struct {
    double re;
    double im;
} mkl_double_complex;

fftw_plan PlanForward;
fftw_plan PlanInverse;

int getIntelFFTWPlans(DFTI_DESCRIPTOR_HANDLE *descHandle);

static void BM_FFTW(benchmark::State& state) {
    double *forIN;
    forIN = new double[NN * NN];
    fftw_complex *forOUT;
    forOUT = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * NPIXFFT);
    PlanForward = fftw_plan_dft_r2c_2d(NN, NN, forIN, forOUT, FFTW_MEASURE);

    // Inverse plan
    fftw_complex *invIN;
    invIN = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * NPIXFFT);
    double *invOUT;
    invOUT = new double[NPIXFFT];
    PlanInverse = fftw_plan_dft_c2r_2d(NN, NN, invIN, invOUT, FFTW_MEASURE);

    double *image = (double*)malloc(sizeof(double) * NN * NN);
    double *recoveredImage = (double*)malloc(sizeof(double) * NN * NN);
    fftw_complex *imageFT = (fftw_complex*) mkl_malloc(
            NPIXFFT * sizeof(fftw_complex),64);

    for (unsigned int i=0; i<NN * NN; i++){
        image[i] = i * i + i * 2 + 1;
    }

    for (auto _ : state) {
        // This code gets timed
        fftw_execute_dft_r2c( PlanForward, image, imageFT);
        fftw_execute_dft_c2r( PlanInverse, imageFT, recoveredImage);
        benchmark::DoNotOptimize(recoveredImage);
        benchmark::ClobberMemory();
    }
}

static void BM_IntelFFT(benchmark::State& state) {

    DFTI_DESCRIPTOR_HANDLE descHandle;
    getIntelFFTWPlans(&descHandle);

    double *image = (double*)malloc(sizeof(double) * NN * NN);
    double *recoveredImage = (double*)malloc(sizeof(double) * NN * NN);
    fftw_complex *imageFT = (fftw_complex*) mkl_malloc(
            NPIXFFT * sizeof(fftw_complex),64);

    for (unsigned int i=0; i<NN * NN; i++){
        image[i] = i * i + i * 2 + 1;
    }

    for (auto _ : state) {
        // This code gets timed
        DftiComputeForward(descHandle, image, imageFT);
        DftiComputeBackward(descHandle, imageFT, recoveredImage);
        benchmark::DoNotOptimize(recoveredImage);
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_IntelFFT);
BENCHMARK(BM_FFTW);
// Run the benchmark
BENCHMARK_MAIN();

int getIntelFFTWPlans(DFTI_DESCRIPTOR_HANDLE *descHandle){

    MKL_LONG lengths[2];
    lengths[0] = NN;
    lengths[1] = NN;
    MKL_LONG status = DftiCreateDescriptor(descHandle, DFTI_DOUBLE, DFTI_REAL, 2, lengths);

    if (status != 0) {
        cout << "DftiCreateDescriptor failed : " << status << endl;
        return -1;
    }
    status = DftiSetValue(*descHandle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
    if (status != 0) {
        cout << "DftiSetValue DFTI_PLACEMENT failed : " << status << endl;
        return -2;
    }

    status = DftiSetValue(*descHandle, DFTI_THREAD_LIMIT, 1);
    if (status != 0) {
        cout << "DftiSetValue DFTI_THREAD_LIMIT failed : " << status << endl;
        return -3;
    }

    status = DftiSetValue(*descHandle, DFTI_CONJUGATE_EVEN_STORAGE, DFTI_COMPLEX_COMPLEX);
    if (status != 0) {
        cout << "DftiSetValue DFTI_CONJUGATE_EVEN_STORAGE failed : " << status << endl;
        return -4;
    }

    status = DftiSetValue(*descHandle, DFTI_PACKED_FORMAT, DFTI_CCE_FORMAT);
    if (status != 0) {
        cout << "DftiSetValue DFTI_PACKED_FORMAT failed : " << status << endl;
        return -5;
    }

    MKL_LONG strides[3];
    strides[0] = 0;
    strides[1] = 1;
    strides[2] = NN;

    status = DftiSetValue(*descHandle, DFTI_INPUT_STRIDES, strides);
    if (status != 0) {
        cout << "DftiSetValue DFTI_INPUT_STRIDES failed : " << status << endl;
        return -6;
    }

    status = DftiSetValue(*descHandle, DFTI_OUTPUT_STRIDES, strides);
    if (status != 0) {
        cout << "DftiSetValue DFTI_OUTPUT_STRIDES failed : " << status << endl;
        return -7;
    }

    MKL_LONG format;
    status = DftiGetValue(*descHandle, DFTI_PACKED_FORMAT, &format);
    if (status != 0) {
        cout << "DftiGetValue DFTI_PACKED_FORMAT failed : " << status << endl;
        return -8;
    }
//    cout << "DftiGetValue DFTI_PACKED_FORMAT : " << format << endl;

    status = DftiCommitDescriptor(*descHandle);
    if (status != 0) {
        cout << "DftiCommitDescriptor failed : " << status << endl;
        return -9;
    }

    return status;
}
