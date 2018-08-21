cmake_minimum_required(VERSION 3.7)
project(RpiAnc)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES
        Headers/*.h
        Sources/*.cpp
        )

set(SOURCE_FILES Sources/FIRFilter.cpp Sources/LMSFilter.cpp Sources/FxLMSFilter.cpp)
set(HEADER_FILES Headers/common.h Headers/FIRFilter.h Headers/LMSFilter.h Headers/matplotlibcpp.h Headers/FxLMSFilter.h)
add_executable(RpiAnc Mains/main.cpp ${SOURCE_FILES} ${HEADER_FILES})

find_package(PythonLibs 2.7)
target_link_libraries(RpiAnc asound fir)

add_executable(lmstest Mains/lmstest.cpp ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(lmstest PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(lmstest ${PYTHON_LIBRARIES})

add_executable(fxlmstest Mains/fxlmstest.cpp ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(fxlmstest PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(fxlmstest ${PYTHON_LIBRARIES})
