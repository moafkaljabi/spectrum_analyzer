#pragma once
#include <vector>
#include <cstdint>
#include <rtl-sdr.h>

extern rtlsdr_dev_t* dev;   

std::vector<uint8_t> read_rtl_samples(int count);
