#pragma once
#include <vector>
#include <cstdint>
#include <rtl-sdr.h>

// ADD THIS LINE
extern rtlsdr_dev_t* dev;   // ← makes dev visible to main.cpp

std::vector<uint8_t> read_rtl_samples(int count);
