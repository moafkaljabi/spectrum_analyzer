#include "sdr_capture.h"
#include <rtl-sdr.h>
#include <iostream>
#include <cstring>

static rtlsdr_dev_t* dev = nullptr;

void init_rtl_sdr() {
    int r = rtlsdr_open(&dev, 0);
    if (r < 0 || !dev) {
        std::cerr << "Failed to open RTL-SDR\n";
        exit(1);
    }
    rtlsdr_set_sample_rate(dev, 2400000);
    rtlsdr_set_center_freq(dev, 100000000);  // 100 MHz
    rtlsdr_set_tuner_gain_mode(dev, 1);
    rtlsdr_set_tuner_gain(dev, 400);         // ~40 dB
    rtlsdr_reset_buffer(dev);
}

std::vector<uint8_t> read_rtl_samples(int count) {
    if (!dev) init_rtl_sdr();

    std::vector<uint8_t> buffer(count * 2);  // I/Q = 2 bytes per sample
    int n_read;
    rtlsdr_read_sync(dev, buffer.data(), buffer.size(), &n_read);

    if (n_read != buffer.size()) {
        std::cerr << "Short read: " << n_read << "\n";
        buffer.resize(n_read);
    }
    return buffer;
}
