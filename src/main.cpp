#include "../include/CameraProvider.hpp"
#include "../include/KeyProcessor.hpp"
#include "../include/FrameProcessor.hpp"
#include "../include/Display.hpp"
#include "../include/FaceDetector.hpp"

#include <iostream>

cv::Point mousePoint;

void onMouse(int event, int x, int y, int flags, void* userdata) {
    KeyProcessor* kp = static_cast<KeyProcessor*>(userdata);
    kp->handleMouse(event, x, y, flags);
}

int main() {
    std::cout << "=== Лабораторна робота №7 ===" << std::endl;
    std::cout << "Computer Vision та багатопотоковість у C++" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    CameraProvider camera(0);
    if (!camera.isOpened()) {
        std::cerr << "Помилка: не вдалося відкрити камеру!" << std::endl;
        return -1;
    }
    
    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    Display display("Lab 7 - Camera Processing with Face Detection");
    FaceDetector faceDetector;
    
    cv::setMouseCallback("Lab 7 - Camera Processing with Face Detection", onMouse, &keyProcessor);
    
    cv::Mat frame;
    double fps = 0;
    cv::TickMeter tickMeter;
    
    std::cout << "Програма запущена. Натисніть 'q' для виходу." << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Доступні режими обробки:" << std::endl;
    std::cout << "  1-9 - різні режими обробки зображень" << std::endl;
    std::cout << "  F   - детекція облич (багатопотоковий режим)" << std::endl;
    std::cout << "  l   - малювати лінії мишкою" << std::endl;
    std::cout << "  r   - малювати прямокутники мишкою" << std::endl;
    std::cout << "  c   - очистити малюнки" << std::endl;
    std::cout << "  d   - показати FPS та інформацію" << std::endl;
    std::cout << "  o   - показати оверлей" << std::endl;
    std::cout << "  +/- - зум" << std::endl;
    std::cout << "  b/B - яскравіше/темніше" << std::endl;
    std::cout << "  k/K - більше/менше контрасту" << std::endl;
    std::cout << "  0   - скинути всі налаштування" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    while (true) {
        tickMeter.start();
        
        frame = camera.getFrame();
        if (frame.empty()) {
            std::cerr << "Помилка: порожній кадр!" << std::endl;
            break;
        }
        
        if (keyProcessor.getCurrentMode() == ProcessMode::FACE_DETECTION) {
            faceDetector.updateFrame(frame);
            std::vector<Face> faces = faceDetector.getFaces();
            frameProcessor.drawFaces(frame, faces);
        }
        
        cv::Mat processedFrame = frameProcessor.process(frame, keyProcessor);
        display.show(processedFrame, frameProcessor, keyProcessor, fps);
        
        tickMeter.stop();
        fps = 1.0 / tickMeter.getTimeSec();
        tickMeter.reset();
        
        int key = cv::waitKey(1);
        if (key == 'q' || key == 'Q') {
            std::cout << "Завершення програми..." << std::endl;
            break;
        }
        
        keyProcessor.handleKey(key);
    }
    
    return 0;
}