cmake_minimum_required(VERSION 3.10)
project(Lab7)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    include_directories(${OpenCV_INCLUDE_DIR})
    include_directories(${CMAKE_SOURCE_DIR}/include)
    
    add_executable(lab7
        src/main.cpp
        src/CameraProvider.cpp
        src/KeyProcessor.cpp
        src/FrameProcessor.cpp
        src/Display.cpp
        src/FaceDetector.cpp
    )
    
    target_link_libraries(lab7 ${OpenCV_LIB})
else()
    message(FATAL_ERROR "OpenCV not found!")
endif()