// main.cpp – Pi (C++17)
#include "sdr/sdr_capture.h"
#include "communication/udp_transmitter.h"
#include <thread>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    std::cout << "SDR → UDP streamer + command listener\n";

    // UDP listener for commands (port 5001)
    int cmd_sock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in cmd_addr{};
    cmd_addr.sin_family = AF_INET;
    cmd_addr.sin_port = htons(5001);
    cmd_addr.sin_addr.s_addr = INADDR_ANY;
    bind(cmd_sock, (sockaddr*)&cmd_addr, sizeof(cmd_addr));

    while (true) {
        // 1. Read command (non-blocking)
        char cmd_buf[64]{};
        int n = recvfrom(cmd_sock, cmd_buf, sizeof(cmd_buf)-1, MSG_DONTWAIT, nullptr, nullptr);
        if (n > 0) {
            std::string cmd(cmd_buf, n);
            if (cmd.find("FREQ:") == 0) {
                uint64_t freq = std::stoull(cmd.substr(5));
                rtlsdr_set_center_freq(dev, freq);  // dev from sdr_capture.cpp
                std::cout << "→ Freq set: " << freq << " Hz\n";
            }
            if (cmd.find("GAIN:") == 0) {
                int gain = std::stoi(cmd.substr(5));
                rtlsdr_set_tuner_gain(dev, gain);
                std::cout << "→ Gain set: " << gain << "\n";
            }
        }

        // 2. Stream I/Q
        auto samples = read_rtl_samples(4096);
        send_udp_packet(samples.data(), samples.size(), "100.106.128.46", 5000); 
        std::cout << "Sent " << samples.size() << " bytes\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    close(cmd_sock);
    return 0;
}
