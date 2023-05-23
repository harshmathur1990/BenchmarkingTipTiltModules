cmake_minimum_required(VERSION 3.25)
project(Benchmarking)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_BUILD_TYPE RELEASE)
#set(CMAKE_PREFIX_PATH /usr/local/Cellar)
find_package(benchmark REQUIRED)
#find_package(OpenCV REQUIRED )
#find_package(FFTW3 REQUIRED )
#find_package(MKL CONFIG REQUIRED)
#message(STATUS "${MKL_IMPORTED_TARGETS}")

set(CMAKE_C_FLAGS  "-Ofast -march=native")
set(CMAKE_CXX_FLAGS "-Ofast -march=native")

#set(MKL_ARCH intel64)
#set(MKL_LINK static)
#set(MKL_THREADING intel_thread)
#set(MKL_INTERFACE ilp64)
#set(MKL_MPI intelmpi)
#set(ENABLE_OMP_OFFLOAD ON)

#set(INCLUDE_DIR C:/PROGRA~2/NATION~1/Shared/EXTERN~1/C/include)
#set(INCLUDE_lIB_DIR C:/PROGRA~2/NATION~1/Shared/EXTERN~1/C/lib64/msvc)
#set(FFTW C:/Users/IIAP-IPC/Documents/fftw-3)
#set(FFTWLINK ${FFTW}/libfftw3l-3.lib ${FFTW}/libfftw3f-3.lib ${FFTW}/libfftw3-3.lib)
#set(INTEL_FFTWLINK C:/PROGRA~2/Intel/oneAPI/mkl/2023.1.0/lib/intel64)
#set(INTEL_FFTW_INCLUDE C:/PROGRA~2/Intel/oneAPI/mkl/2023.1.0/include)
#set(INTEL_FFTWLINKLIB ${INTEL_FFTWLINK}/mkl_intel_lp64.lib ${INTEL_FFTWLINK}/mkl_core.lib ${INTEL_FFTWLINK}/mkl_sequential.lib)
#
#add_executable(FFTWCompare main.cpp)
#target_include_directories(FFTWCompare PRIVATE $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES> )
#target_link_libraries(FFTWCompare PRIVATE $<LINK_ONLY:MKL::MKL> benchmark::benchmark)
#target_link_directories(FFTWCompare PRIVATE ${FFTW3_LIBRARY_DIRS})
#
#
#add_executable(BinningCompare binning.cpp)
##target_include_directories(BinningCompare PRIVATE ${BENCHMARK}/include)
#target_link_libraries(BinningCompare PRIVATE benchmark::benchmark)
#
#add_executable(NIDaQBenchmark NIDaQBenchmark.cpp)
#target_include_directories(NIDaQBenchmark PRIVATE ${INCLUDE_DIR})
#target_link_libraries(NIDaQBenchmark PRIVATE ${INCLUDE_lIB_DIR}/NIDAQmx.lib benchmark::benchmark)

#add_executable(ThreadTest threadtest.cpp)
#target_link_libraries(ThreadTest benchmark::benchmark)

#add_executable(OpenCVTest opencvtest.cpp)
#target_include_directories(OpenCVTest PRIVATE ${OpenCV_INCLUDE_DIRS})
#target_link_libraries(OpenCVTest PRIVATE ${OpenCV_LIBS} benchmark::benchmark)

add_executable(LPFTest butterworthtest.cpp Butterworth.h Butterworth.cpp Biquad.h Biquad.cpp)
target_include_directories(LPFTest PRIVATE ${BENCHMARK}/include)
target_link_libraries(LPFTest PRIVATE benchmark::benchmark)