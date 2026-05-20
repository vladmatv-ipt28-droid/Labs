#include "../include/CameraProvider.hpp"
#include "../include/KeyProcessor.hpp"
#include "../include/FrameProcessor.hpp"
#include "../include/Display.hpp"

#include <iostream>

cv::Point mousePoint;

void onMouse(int event, int x, int y, int flags, void* userdata) {
    KeyProcessor* kp = static_cast<KeyProcessor*>(userdata);
    kp->handleMouse(event, x, y, flags);
}

int main() {
    std::cout << "=== Лабораторна робота №6 ===" << std::endl;
    std::cout << "Запуск програми обробки відео з камери" << std::endl;
    
    CameraProvider camera(0);
    if (!camera.isOpened()) {
        std::cerr << "Помилка: не вдалося відкрити камеру!" << std::endl;
        return -1;
    }
    
    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    Display display("Lab 6 - Camera Processing");
    
    cv::setMouseCallback("Lab 6 - Camera Processing", onMouse, &keyProcessor);
    
    cv::Mat frame;
    double fps = 0;
    cv::TickMeter tickMeter;
    
    std::cout << "Програма запущена. Натисніть 'q' для виходу." << std::endl;
    std::cout << "Доступні клавіші:" << std::endl;
    std::cout << "  1-9 - різні режими обробки" << std::endl;
    std::cout << "  l - малювати лінії, r - малювати прямокутники, c - очистити" << std::endl;
    std::cout << "  f - показати FPS" << std::endl;
    std::cout << "  o - показати оверлей" << std::endl;
    std::cout << "  +/- - зум" << std::endl;
    std::cout << "  b/B - яскравіше/темніше" << std::endl;
    std::cout << "  k/K - більше/менше контрасту" << std::endl;
    std::cout << "  0 - скинути всі налаштування" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    while (true) {
        tickMeter.start();
        
        frame = camera.getFrame();
        if (frame.empty()) {
            std::cerr << "Помилка: порожній кадр!" << std::endl;
            break;
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
        
        // Оновлюємо позиції слайдерів при зміні значень через клавіші
        if (key == '+' || key == '-' || key == '=' || key == 'b' || key == 'B') {
            cv::setTrackbarPos("Zoom", "Lab 6 - Camera Processing", keyProcessor.getZoomLevel());
            cv::setTrackbarPos("Brightness", "Lab 6 - Camera Processing", keyProcessor.getBrightness() + 50);
        }
        
        keyProcessor.handleKey(key);
    }
    
    return 0;
}