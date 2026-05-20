#ifndef CAMERA_PROVIDER_HPP
#define CAMERA_PROVIDER_HPP

#include <opencv2/opencv.hpp>

class CameraProvider {
private:
    cv::VideoCapture cap;
    int cameraID;

public:
    CameraProvider(int id = 0);
    ~CameraProvider();
    
    bool isOpened() const;
    cv::Mat getFrame();
    double getFPS();
};

#endif