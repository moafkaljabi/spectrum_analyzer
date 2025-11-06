#pragma once 
#include <vector>
#include <cstdint>

// Reads N I/Q samples from the SDR
std::vector<uint8_t> read_rtl_samples(int count);

