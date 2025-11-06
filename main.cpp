#include "sdr/sdr_capture.h"
#include "communication/udp_transmitter.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "Starting RTL-SDR → UDP streamer...\n";

    while (true) {
        auto samples = read_rtl_samples(4096);  // 4096 I/Q pairs = 8192 bytes
        send_udp_packet(samples.data(), samples.size(), "192.168.1.100", 5000);
        std::cout << "Sent " << samples.size() << " bytes\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
