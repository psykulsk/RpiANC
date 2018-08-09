cmake_minimum_required(VERSION 3.7)
project(RpiAnc)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES
        Headers/*.h
        Sources/*.cpp
        )

find_package(PythonLibs 2.7)
add_executable(RpiAnc Mains/main.cpp Headers/common.h Sources/FIRFilter.cpp Headers/FIRFilter.h Sources/LMSFilter.cpp Headers/LMSFilter.h Mains/lmstest.cpp Headers/matplotlibcpp.h)
target_link_libraries(RpiAnc asound fir)

add_executable(lmstest Mains/lmstest.cpp Headers/common.h Sources/FIRFilter.cpp Headers/FIRFilter.h Sources/LMSFilter.cpp Headers/LMSFilter.h Mains/lmstest.cpp Headers/matplotlibcpp.h)
target_include_directories(lmstest PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(lmstest ${PYTHON_LIBRARIES})
