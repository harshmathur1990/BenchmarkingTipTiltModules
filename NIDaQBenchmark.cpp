#include <iostream>
#include <benchmark/benchmark.h>
#include "NIDAQmx.h"


int initDAQ(TaskHandle &XDAQHandle){
    DAQmxCreateTask("", &XDAQHandle);

    DAQmxCreateAOVoltageChan(XDAQHandle, "Dev1/ao0:1", "", 0, 10, DAQmx_Val_Volts, "");

    DAQmxStartTask(XDAQHandle);

    return 0;
}

inline int setVoltagesXY(TaskHandle &XDAQHandle, float XShift, float YShift){
    int32 sampsPerChanWritten=0;
    double XData[1];
    XData[0] = (XShift+20.0)/15.0;
    XData[1] = (YShift+20.0)/15.0;

    DAQmxWriteAnalogF64(
            XDAQHandle, 1, 1, 0.0, DAQmx_Val_GroupByChannel, XData, NULL, NULL);

    return 0;
}

int closeDAQ(TaskHandle &XDAQHandle){
    DAQmxStopTask(XDAQHandle);
    DAQmxClearTask(XDAQHandle);
    return 0;
}

static void BM_NIDaQ(benchmark::State& state) {

    TaskHandle XDAQHandle;

    initDAQ(XDAQHandle);
    for (auto _ : state) {
        // This code gets timed
        setVoltagesXY(XDAQHandle, 60, 60);
    }
    closeDAQ(XDAQHandle);
}

BENCHMARK(BM_NIDaQ);
// Run the benchmark
BENCHMARK_MAIN();