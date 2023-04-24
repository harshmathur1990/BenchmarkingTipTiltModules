cmake_minimum_required(VERSION 3.25)
project(Benchmarking)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_BUILD_TYPE RELEASE)

set(FFTWINCLUDE /usr/local/Cellar/fftw/3.3.10_1/include)
set (FFTWLINK /usr/local/Cellar/fftw/3.3.10_1/lib)
set(INTEL_FFTW_INCLUDE /opt/intel/oneapi/mkl/2023.1.0/include)
set(INTEL_FFTW_LINK /opt/intel/oneapi/mkl/2023.1.0/lib)
set(MKLROOT /opt/intel/oneapi/mkl/2023.1.0)
set(GOOGLEBENCHMARK /usr/local/Cellar/google-benchmark/1.7.1)
set(CMAKE_C_FLAGS  "-Ofast -march=native -L${MKLROOT}/lib -Wl,-rpath,${MKLROOT}/lib -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl")
set(CMAKE_CXX_FLAGS "-Ofast -march=native -L${MKLROOT}/lib -Wl,-rpath,${MKLROOT}/lib -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl")


add_executable(FFTWCompare main.cpp)
target_include_directories(FFTWCompare PRIVATE ${INTEL_FFTW_INCLUDE} ${GOOGLEBENCHMARK}/include ${FFTWINCLUDE})
target_link_libraries(FFTWCompare PRIVATE ${INTEL_FFTW_LINK}/libmkl_intel_ilp64.dylib ${INTEL_FFTW_LINK}/libmkl_core.dylib ${INTEL_FFTW_LINK}/libmkl_intel_lp64.dylib ${FFTWLINK}/libfftw3.a ${GOOGLEBENCHMARK}/lib/libbenchmark.a)


add_executable(BinningCompare binning.cpp)
target_include_directories(BinningCompare PRIVATE ${GOOGLEBENCHMARK}/include)
target_link_libraries(BinningCompare PRIVATE ${GOOGLEBENCHMARK}/lib/libbenchmark.a)