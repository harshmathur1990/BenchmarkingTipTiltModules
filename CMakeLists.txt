cmake_minimum_required(VERSION 3.25)
project(Benchmarking)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_BUILD_TYPE RELEASE)

find_package(benchmark REQUIRED)

set(CMAKE_C_FLAGS  "-Ofast -march=native")
set(CMAKE_CXX_FLAGS "-Ofast -march=native")

set(INCLUDE_DIR C:/PROGRA~2/NATION~1/Shared/EXTERN~1/C/include)
set(INCLUDE_lIB_DIR C:/PROGRA~2/NATION~1/Shared/EXTERN~1/C/lib64/msvc)
set(FFTW C:/Users/IIAP-IPC/Documents/fftw-3)
set(FFTWLINK ${FFTW}/libfftw3l-3.lib ${FFTW}/libfftw3f-3.lib ${FFTW}/libfftw3-3.lib)
set(INTEL_FFTWLINK C:/PROGRA~2/Intel/oneAPI/mkl/2023.1.0/lib/intel64)
set(INTEL_FFTW_INCLUDE C:/PROGRA~2/Intel/oneAPI/mkl/2023.1.0/include)
set(INTEL_FFTWLINKLIB ${INTEL_FFTWLINK}/mkl_intel_lp64.lib ${INTEL_FFTWLINK}/mkl_core.lib ${INTEL_FFTWLINK}/mkl_sequential.lib)

add_executable(FFTWCompare main.cpp)
target_include_directories(FFTWCompare PRIVATE ${INTEL_FFTW_INCLUDE} ${FFTW})
target_link_libraries(FFTWCompare PRIVATE ${INTEL_FFTWLINKLIB} ${FFTWLINK} benchmark::benchmark)


add_executable(BinningCompare binning.cpp)
#target_include_directories(BinningCompare PRIVATE ${BENCHMARK}/include)
target_link_libraries(BinningCompare PRIVATE benchmark::benchmark)

add_executable(NIDaQBenchmark NIDaQBenchmark.cpp)
target_include_directories(NIDaQBenchmark PRIVATE ${INCLUDE_DIR})
target_link_libraries(NIDaQBenchmark PRIVATE ${INCLUDE_lIB_DIR}/NIDAQmx.lib benchmark::benchmark)