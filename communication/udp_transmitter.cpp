#include "udp_transmitter.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

void send_udp_packet(const unsigned char* data, size_t size,
                     const char* ip, int port) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "UDP socket failed\n";
        return;
    }

    sockaddr_in dest{};
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_pton(AF_INET, ip, &dest.sin_addr);

    sendto(sock, data, size, 0, (sockaddr*)&dest, sizeof(dest));
    close(sock);
}
