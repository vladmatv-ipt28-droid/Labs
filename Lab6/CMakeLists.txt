cmake_minimum_required(VERSION 3.10)
project(Lab6)

set(CMAKE_CXX_STANDARD 11)

find_path(OpenCV_INCLUDE_DIR
    NAMES opencv2/opencv.hpp
    PATHS "C:/opencv/build/include"
)

find_library(OpenCV_LIB
    NAMES opencv_world4100
    PATHS "C:/opencv/build/x64/vc16/lib"
)

if(OpenCV_INCLUDE_DIR AND OpenCV_LIB)
    message(STATUS "OpenCV found: ${OpenCV_INCLUDE_DIR}")
    add_executable(lab6
        src/main.cpp
        src/CameraProvider.cpp
        src/KeyProcessor.cpp
        src/FrameProcessor.cpp
        src/Display.cpp
    )
    target_include_directories(lab6 PRIVATE ${OpenCV_INCLUDE_DIR} include)
    target_link_libraries(lab6 ${OpenCV_LIB})
else()
    message(FATAL_ERROR "OpenCV not found!")
endif()