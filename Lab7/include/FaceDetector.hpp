#ifndef FACE_DETECTOR_HPP
#define FACE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

struct Face {
    cv::Rect rect;
    float confidence;
};

class FaceDetector {
private:
    cv::dnn::Net net;
    std::thread workerThread;
    std::mutex frameMutex;
    std::mutex resultMutex;
    std::atomic<bool> running;
    std::atomic<bool> hasNewFrame;
    
    cv::Mat currentFrame;
    std::vector<Face> currentFaces;
    
    void workerFunction();

public:
    FaceDetector();
    ~FaceDetector();
    
    void updateFrame(const cv::Mat& frame);
    std::vector<Face> getFaces();
    bool isRunning() const;
};

#endif