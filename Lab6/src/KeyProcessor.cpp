#include "../include/KeyProcessor.hpp"

KeyProcessor::KeyProcessor() 
    : currentMode(ProcessMode::NORMAL), 
      mouseMode(MouseMode::NONE),
      isDrawing(false),
      zoomLevel(100),
      brightness(0),
      contrast(1.0f),
      showFPS(true),
      showOverlay(false) {}

void KeyProcessor::handleKey(int key) {
    switch (key) {
        // Режими обробки
        case '1': currentMode = ProcessMode::NORMAL; break;
        case '2': currentMode = ProcessMode::INVERT; break;
        case '3': currentMode = ProcessMode::GRAYSCALE; break;
        case '4': currentMode = ProcessMode::BLUR; break;
        case '5': currentMode = ProcessMode::CANNY; break;
        case '6': currentMode = ProcessMode::SOBEL; break;
        case '7': currentMode = ProcessMode::BINARY; break;
        case '8': currentMode = ProcessMode::GLITCH; break;
        case '9': currentMode = ProcessMode::SEPIA; break;
        
        // Малювання
        case 'l': mouseMode = MouseMode::DRAW_LINE; break;
        case 'r': mouseMode = MouseMode::DRAW_RECT; break;
        case 'c': mouseMode = MouseMode::NONE; resetDrawing(); break;
        
        // Інші функції
        case 'f': showFPS = !showFPS; break;
        case 'o': showOverlay = !showOverlay; break;
        
        // Зум
        case '+': case '=': zoomLevel = std::min(zoomLevel + 10, 200); break;
        case '-': case '_': zoomLevel = std::max(zoomLevel - 10, 50); break;
        
        // Яскравість/контраст
        case 'b': brightness = std::min(brightness + 10, 100); break;
        case 'B': brightness = std::max(brightness - 10, -100); break;
        case 'k': contrast = std::min(contrast + 0.1f, 3.0f); break;
        case 'K': contrast = std::max(contrast - 0.1f, 0.3f); break;
        
        // Скидання
        case '0': 
            currentMode = ProcessMode::NORMAL;
            zoomLevel = 100;
            brightness = 0;
            contrast = 1.0f;
            break;
    }
}

void KeyProcessor::handleMouse(int event, int x, int y, int flags) {
    if (mouseMode == MouseMode::NONE) return;
    
    if (event == cv::EVENT_LBUTTONDOWN) {
        isDrawing = true;
        startPoint = cv::Point(x, y);
        endPoint = startPoint;
    }
    else if (event == cv::EVENT_MOUSEMOVE && isDrawing) {
        endPoint = cv::Point(x, y);
    }
    else if (event == cv::EVENT_LBUTTONUP) {
        isDrawing = false;
    }
}

ProcessMode KeyProcessor::getCurrentMode() const { return currentMode; }
MouseMode KeyProcessor::getMouseMode() const { return mouseMode; }
bool KeyProcessor::isDrawingMode() const { return isDrawing; }
cv::Point KeyProcessor::getStartPoint() const { return startPoint; }
cv::Point KeyProcessor::getEndPoint() const { return endPoint; }
int KeyProcessor::getZoomLevel() const { return zoomLevel; }
int KeyProcessor::getBrightness() const { return brightness; }
float KeyProcessor::getContrast() const { return contrast; }
bool KeyProcessor::shouldShowFPS() const { return showFPS; }
bool KeyProcessor::shouldShowOverlay() const { return showOverlay; }

void KeyProcessor::resetDrawing() {
    isDrawing = false;
    startPoint = cv::Point(0, 0);
    endPoint = cv::Point(0, 0);
}