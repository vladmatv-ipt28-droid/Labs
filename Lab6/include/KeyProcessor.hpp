#ifndef KEY_PROCESSOR_HPP
#define KEY_PROCESSOR_HPP

#include <opencv2/opencv.hpp>

enum class ProcessMode {
    NORMAL,
    INVERT,
    GRAYSCALE,
    BLUR,
    CANNY,
    SOBEL,
    BINARY,
    GLITCH,
    SEPIA
};

enum class MouseMode {
    NONE,
    DRAW_LINE,
    DRAW_RECT
};

class KeyProcessor {
private:
    ProcessMode currentMode;
    MouseMode mouseMode;
    bool isDrawing;
    cv::Point startPoint, endPoint;
    int zoomLevel;
    int brightness;
    float contrast;
    bool showFPS;
    bool showOverlay;

public:
    KeyProcessor();
    
    void handleKey(int key);
    void handleMouse(int event, int x, int y, int flags);
    
    ProcessMode getCurrentMode() const;
    MouseMode getMouseMode() const;
    bool isDrawingMode() const;
    cv::Point getStartPoint() const;
    cv::Point getEndPoint() const;
    int getZoomLevel() const;
    int getBrightness() const;
    float getContrast() const;
    bool shouldShowFPS() const;
    bool shouldShowOverlay() const;
    void resetDrawing();
};

#endif