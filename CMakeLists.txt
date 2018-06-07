cmake_minimum_required(VERSION 3.7)
project(oneLoopCpp)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES
        Headers/*.h
        Sources/*.cpp
        )

add_executable(oneLoopCpp ${SOURCES})
target_link_libraries(oneLoopCpp asound)