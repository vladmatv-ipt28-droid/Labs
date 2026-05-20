#include "../include/Display.hpp"
#include <iostream>

Display::Display(const std::string& name) : windowName(name), fps(0), frameCount(0) {
    cv::namedWindow(windowName, cv::WINDOW_NORMAL);
    cv::resizeWindow(windowName, 1280, 720);
}

Display::~Display() {
    cv::destroyWindow(windowName);
}

void Display::show(const cv::Mat& frame, const FrameProcessor& processor, 
                   const KeyProcessor& keyProcessor, double currentFPS) {
    if (frame.empty()) return;
    
    cv::Mat displayFrame = frame.clone();
    
    if (keyProcessor.shouldShowFPS()) {
        double avgIntensity = processor.calculateAverageIntensity(frame);
        
        std::string fpsText = "FPS: " + std::to_string(static_cast<int>(currentFPS));
        std::string intensityText = "Avg Intensity: " + std::to_string(static_cast<int>(avgIntensity));
        
        cv::putText(displayFrame, fpsText, cv::Point(10, 30), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
        cv::putText(displayFrame, intensityText, cv::Point(10, 60), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    }
    
    std::string helpText = "1:Normal 2:Invert 3:Gray 4:Blur 5:Canny 6:Sobel 7:Binary 8:Glitch 9:Sepia | l:Line r:Rect c:Clear | f:FPS o:Overlay | +/-:Zoom | b/B:Brighter/Darker | k/K:Contrast+/- | 0:Reset";
    cv::putText(displayFrame, helpText, cv::Point(10, displayFrame.rows - 10), 
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    
    cv::imshow(windowName, displayFrame);
}

void Display::createTrackbars(KeyProcessor& keyProcessor) {
    cv::createTrackbar("Brightness", windowName, nullptr, 100, 
        [](int value, void* userdata) {
            KeyProcessor* kp = static_cast<KeyProcessor*>(userdata);
            kp->handleKey('b');
        }, &keyProcessor);
    
    cv::createTrackbar("Zoom", windowName, nullptr, 200, 
        [](int value, void* userdata) {
            KeyProcessor* kp = static_cast<KeyProcessor*>(userdata);
            kp->handleKey('+');
        }, &keyProcessor);
}