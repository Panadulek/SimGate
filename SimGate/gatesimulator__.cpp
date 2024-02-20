#include "gatesimulator.h"

GateSimulator::GateSimulator()
{
    m_libname = "sim.dll";
}

GateSimulator& GateSimulator::Instance()
{
    static GateSimulator sim;
    return sim;
}

