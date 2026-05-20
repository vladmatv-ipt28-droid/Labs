#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <opencv2/opencv.hpp>
#include "FrameProcessor.hpp"
#include "KeyProcessor.hpp"

class Display {
private:
    std::string windowName;
    double fps;
    int frameCount;
    cv::TickMeter tickMeter;

public:
    Display(const std::string& name = "Camera Feed");
    ~Display();
    
    void show(const cv::Mat& frame, const FrameProcessor& processor, 
              const KeyProcessor& keyProcessor, double currentFPS);
    void createTrackbars(KeyProcessor& keyProcessor);
    static void onTrackbarChange(int value, void* userdata);
};

#endif