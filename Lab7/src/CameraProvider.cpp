#include "../include/CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider(int id) : cameraID(id) {
    cap.open(cameraID);
    if (!cap.isOpened()) {
        std::cerr << "Помилка: не вдалося відкрити камеру з ID " << cameraID << std::endl;
    }
}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) {
        cap.release();
    }
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    if (cap.isOpened()) {
        cap >> frame;
    }
    return frame;
}

double CameraProvider::getFPS() {
    return cap.get(cv::CAP_PROP_FPS);
}