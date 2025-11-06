#pragma once 
#include <cstddef>

// Transmit raw bytes using UDP
void send_udp_packet(const unsigned char* data, size_t size, const char* ip = "127.0.0.1", int port = 5000);
