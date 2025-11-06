cmake_minimum_required(VERSION 3.16)
project(sdr_streamer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(RTLSDR REQUIRED librtlsdr)

add_executable(sdr_streamer
    main.cpp
    sdr/sdr_capture.cpp
    communication/udp_transmitter.cpp
)

target_link_libraries(sdr_streamer ${RTLSDR_LIBRARIES})
target_include_directories(sdr_streamer PRIVATE ${RTLSDR_INCLUDE_DIRS})
