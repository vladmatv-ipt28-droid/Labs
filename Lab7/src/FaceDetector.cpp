#include "../include/FaceDetector.hpp"
#include <iostream>
#include <fstream>
#include <chrono>

FaceDetector::FaceDetector() : running(true), hasNewFrame(false) {
    std::string protoPath = "C:/Users/pmatv/OneDrive/Desktop/4 семестр/ЗПАД/Lab7/models/deploy.prototxt";
    std::string modelPath = "C:/Users/pmatv/OneDrive/Desktop/4 семестр/ЗПАД/Lab7/models/res10_300x300_ssd_iter_140000.caffemodel";
    
    std::ifstream protoFile(protoPath);
    std::ifstream modelFile(modelPath);
    
    if (!protoFile.good()) {
        std::cerr << "ERROR: File not found: " << protoPath << std::endl;
    } else {
        std::cout << "Proto file found: " << protoPath << std::endl;
    }
    if (!modelFile.good()) {
        std::cerr << "ERROR: File not found: " << modelPath << std::endl;
    } else {
        std::cout << "Model file found: " << modelPath << std::endl;
    }
    
    try {
        net = cv::dnn::readNetFromCaffe(protoPath, modelPath);
        std::cout << "Face detection model loaded successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error loading model: " << e.what() << std::endl;
    }
    
    workerThread = std::thread(&FaceDetector::workerFunction, this);
}

FaceDetector::~FaceDetector() {
    running = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void FaceDetector::updateFrame(const cv::Mat& frame) {
    if (frame.empty()) return;
    
    std::lock_guard<std::mutex> lock(frameMutex);
    currentFrame = frame.clone();
    hasNewFrame = true;
}

std::vector<Face> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(resultMutex);
    return currentFaces;
}

void FaceDetector::workerFunction() {
    cv::Mat frame;
    
    while (running) {
        {
            std::lock_guard<std::mutex> lock(frameMutex);
            if (!hasNewFrame) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            frame = currentFrame.clone();
            hasNewFrame = false;
        }
        
        if (frame.empty()) continue;
        
        std::vector<Face> detectedFaces;
        
        try {
            cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), 
                                                   cv::Scalar(104.0, 177.0, 123.0));
            net.setInput(blob);
            cv::Mat detections = net.forward();
            
            float* data = (float*)detections.data;
            int numDetections = detections.size[2];
            
            for (int i = 0; i < numDetections; i++) {
                float confidence = data[i * 7 + 2];
                
                if (confidence > 0.5) {
                    int x1 = (int)(data[i * 7 + 3] * frame.cols);
                    int y1 = (int)(data[i * 7 + 4] * frame.rows);
                    int x2 = (int)(data[i * 7 + 5] * frame.cols);
                    int y2 = (int)(data[i * 7 + 6] * frame.rows);
                    
                    x1 = std::max(0, x1);
                    y1 = std::max(0, y1);
                    x2 = std::min(frame.cols - 1, x2);
                    y2 = std::min(frame.rows - 1, y2);
                    
                    Face face;
                    face.rect = cv::Rect(x1, y1, x2 - x1, y2 - y1);
                    face.confidence = confidence;
                    detectedFaces.push_back(face);
                    
                    std::cout << "Face detected! Confidence: " << confidence << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Detection error: " << e.what() << std::endl;
        }
        
        {
            std::lock_guard<std::mutex> lock(resultMutex);
            currentFaces = detectedFaces;
        }
    }
}

bool FaceDetector::isRunning() const {
    return running;
}