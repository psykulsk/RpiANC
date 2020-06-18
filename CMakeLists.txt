cmake_minimum_required(VERSION 3.7)
project(RpiAnc)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

file(GLOB SOURCES
        Headers/*.h
        Sources/*.cpp
        )

# Dependency of matplotlibcpp
find_package(PythonLibs 2.7)

set(SOUND_INTERFACE_SOURCE_FILES Sources/capture.cpp Sources/playback.cpp)
set(SOURCE_FILES Sources/common.cpp Sources/processing.cpp Sources/delay_tests.cpp)
set(HEADER_FILES Headers/common.h Headers/constants.h Headers/FIRFilter.h Headers/LMSFilter.h Headers/BlockLMSFilter.h Headers/matplotlibcpp.h Headers/FxLMSFilter.h Headers/test_commons.h Headers/delay_tests.h Headers/subband_filtering_constants.h)

add_executable(ffANC Mains/feedforward_anc.cpp ${SOURCE_FILES} ${SOUND_INTERFACE_SOURCE_FILES} ${HEADER_FILES})
target_include_directories(ffANC PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(ffANC asound ${PYTHON_LIBRARIES})

add_executable(subFFANC Mains/feedforward_anc_subband.cpp ${SOURCE_FILES} ${SOUND_INTERFACE_SOURCE_FILES} ${HEADER_FILES})
target_include_directories(subFFANC PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(subFFANC asound ${PYTHON_LIBRARIES})

add_executable(secPath Mains/offline_sec_path_modelling.cpp ${SOURCE_FILES} ${SOUND_INTERFACE_SOURCE_FILES} ${HEADER_FILES})
target_include_directories(secPath PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(secPath asound ${PYTHON_LIBRARIES})

add_executable(recNoise Mains/record_noise.cpp ${SOURCE_FILES} ${SOUND_INTERFACE_SOURCE_FILES} ${HEADER_FILES})
target_include_directories(recNoise PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(recNoise asound ${PYTHON_LIBRARIES})

add_executable(delayCheck Mains/delay_check.cpp ${SOURCE_FILES} ${SOUND_INTERFACE_SOURCE_FILES} ${HEADER_FILES})
target_include_directories(delayCheck PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(delayCheck asound ${PYTHON_LIBRARIES})

add_executable(recTestData Mains/record_test_data.cpp ${SOURCE_FILES} ${SOUND_INTERFACE_SOURCE_FILES} ${HEADER_FILES})
target_include_directories(recTestData PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(recTestData asound ${PYTHON_LIBRARIES})

add_executable(lmstest Mains/lmstest.cpp ${SOURCE_FILES} ${SOUND_INTERFACE_SOURCE_FILES} ${HEADER_FILES})
target_include_directories(lmstest PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(lmstest asound ${PYTHON_LIBRARIES})

add_executable(fxlmstest Mains/fxlmstest.cpp ${SOURCE_FILES} ${SOUND_INTERFACE_SOURCE_FILES} ${HEADER_FILES})
target_include_directories(fxlmstest PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(fxlmstest asound ${PYTHON_LIBRARIES})
