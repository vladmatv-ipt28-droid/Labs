#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"
#include "FaceDetector.hpp"

class FrameProcessor {
private:
    cv::Mat overlayImage;
    bool overlayLoaded;

public:
    FrameProcessor();
    
    cv::Mat process(const cv::Mat& frame, const KeyProcessor& keyProcessor);
    cv::Mat applyZoom(const cv::Mat& frame, int zoomLevel);
    cv::Mat applyBrightnessContrast(const cv::Mat& frame, int brightness, float contrast);
    void drawOnFrame(cv::Mat& frame, const KeyProcessor& keyProcessor);
    void addOverlay(cv::Mat& frame);
    void drawFaces(cv::Mat& frame, const std::vector<Face>& faces);
    double calculateAverageIntensity(const cv::Mat& frame) const;
    cv::Scalar calculateChannelIntensities(const cv::Mat& frame) const;
};

#endif