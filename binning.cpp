#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <memory>
#include <benchmark/benchmark.h>

using namespace std;

int binning(uint16_t* image, uint16_t* binnedImage);
void bin_separately(uint16_t* const image, uint16_t* const binnedImage);

constexpr unsigned WIDTH = 768;
constexpr unsigned HEIGHT = 768;

constexpr unsigned NX = 256;
constexpr unsigned NY = 256;

static_assert(WIDTH % NX == 0);
static_assert(HEIGHT % NY == 0);

constexpr auto BINNINGFACTORWIDTH = WIDTH / NX;
constexpr auto BINNINGFACTORHEIGHT = HEIGHT / NY;

constexpr auto DIVISIONFACTOR = BINNINGFACTORWIDTH * BINNINGFACTORHEIGHT;

static void BM_BINWITHOUTOVERFLOW(benchmark::State& state) {

    uint16_t *image = (uint16_t*) malloc(sizeof(uint16_t) * WIDTH * HEIGHT);

    for (unsigned i=0; i < WIDTH * HEIGHT; i++) {
        image[i] = 4095;
    }

    uint16_t *binnedIimage = (uint16_t*) calloc(sizeof(uint16_t), NX * NY);
    uint16_t *binnedSeparatelyIimage = (uint16_t*) calloc(sizeof(uint16_t), NX * NY);

    for (auto _ : state) {
        binning(image, binnedIimage);
        benchmark::DoNotOptimize(binnedIimage);
        benchmark::ClobberMemory();
    }
}

static void BM_BINSEPARATELY(benchmark::State& state) {

    uint16_t *image = (uint16_t*) malloc(sizeof(uint16_t) * WIDTH * HEIGHT);

    for (unsigned i=0; i < WIDTH * HEIGHT; i++) {
        image[i] = 4095;
    }

    uint16_t *binnedIimage = (uint16_t*) calloc(sizeof(uint16_t), NX * NY);
    uint16_t *binnedSeparatelyIimage = (uint16_t*) calloc(sizeof(uint16_t), NX * NY);

    for (auto _ : state) {
        bin_separately(image, binnedIimage);
        benchmark::DoNotOptimize(binnedIimage);
        benchmark::ClobberMemory();
    }
}

// Register the function as a benchmark
BENCHMARK(BM_BINWITHOUTOVERFLOW);
BENCHMARK(BM_BINSEPARATELY);
// Run the benchmark
BENCHMARK_MAIN();


int binning(uint16_t* image, uint16_t* binnedImage){
    int i, j, k, l;

    for (j=0; j< NY; j++) {
        for (i=0;i < NX; i++) {
            binnedImage[j * NX + i] = 0;
            for (k=i * BINNINGFACTORWIDTH; k<i * BINNINGFACTORWIDTH + BINNINGFACTORWIDTH; k++) {
                for (l=j * BINNINGFACTORHEIGHT; l<j * BINNINGFACTORHEIGHT + BINNINGFACTORHEIGHT; l++) {
                    binnedImage[j * NX + i] += (image[l * HEIGHT + k]/ DIVISIONFACTOR);
                }
            }
        }
    }

    return 0;
}

void bin_separately(uint16_t* const image, uint16_t* const binnedImage)
{
    // just some stuff for static_assert
    using PixelValueType = std::remove_cvref_t<decltype(*image)>;

    constexpr PixelValueType AllOnes = ~static_cast<PixelValueType>(0);
    constexpr unsigned BitCount = 12;
    constexpr uint64_t PixelInValueMax = static_cast<PixelValueType>(~(AllOnes << BitCount)); // use 64 bit to prevent overflow issues
    constexpr uint64_t PixelTypeMax = (std::numeric_limits<PixelValueType>::max)();
    // end static_assert stuff


    {
        // compress horizontally
        static_assert(PixelInValueMax * BINNINGFACTORWIDTH <= PixelTypeMax,
                      "cannot compress horizontally without risking overflow");

        auto out = image;
        for (auto inPos = image, end = image + WIDTH * HEIGHT; inPos != end;)
        {
            uint_fast16_t sum = 0;
            for (unsigned i = 0; i != BINNINGFACTORWIDTH; ++i)
            {
                sum += *(inPos++);
            }
            *(out++) = sum;
        }
    }

    {
        // compress vertically, divide and write to out

        //read pointers
        uint16_t* inPoss[BINNINGFACTORHEIGHT];
        for (unsigned i = 0; i != BINNINGFACTORHEIGHT; ++i)
        {
            inPoss[i] = image + (NX * i);
        }

        for (auto out = binnedImage, end = binnedImage + NX * NY; out != end;) // for all output rows
        {
            for (auto const rowEnd = out + NX; out != rowEnd;)
            {
                uint_fast32_t sum = 0;

                static_assert(PixelInValueMax * BINNINGFACTORWIDTH * BINNINGFACTORHEIGHT <= (std::numeric_limits<decltype(sum)>::max)(),
                              "type of sum needs replacement, since it cannot hold the result of adding up all source pixels for one target pixel");

                for (unsigned i = 0; i != BINNINGFACTORHEIGHT; ++i)
                {
                    sum += *(inPoss[i]++);
                }
                *(out++) = sum / DIVISIONFACTOR;
            }

            // we advanced each pointer by one row -> advance by (BINNINGFACTORHEIGHT - 1) more
            for (unsigned i = 0; i != BINNINGFACTORHEIGHT; ++i)
            {
                inPoss[i] += NX * (BINNINGFACTORHEIGHT - 1);
            }
        }
    }

}