cmake_minimum_required(VERSION 3.7)
project(RpiAnc)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES
        Headers/*.h
        Sources/*.cpp
        )

add_executable(RpiAnc ${SOURCES} Headers/common.h)
target_link_libraries(RpiAnc asound fir)