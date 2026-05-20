#include "../include/FrameProcessor.hpp"
#include <iostream>

FrameProcessor::FrameProcessor() : overlayLoaded(false) {
    overlayImage = cv::imread("assets/overlay.png", cv::IMREAD_UNCHANGED);
    overlayLoaded = !overlayImage.empty();
}

cv::Mat FrameProcessor::process(const cv::Mat& frame, const KeyProcessor& keyProcessor) {
    if (frame.empty()) return frame;
    
    cv::Mat result = frame.clone();
    
    result = applyBrightnessContrast(result, keyProcessor.getBrightness(), keyProcessor.getContrast());
    result = applyZoom(result, keyProcessor.getZoomLevel());
    
    switch (keyProcessor.getCurrentMode()) {
        case ProcessMode::NORMAL:
            break;
        case ProcessMode::INVERT:
            cv::bitwise_not(result, result);
            break;
        case ProcessMode::GRAYSCALE:
            cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
            cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
            break;
        case ProcessMode::BLUR:
            cv::GaussianBlur(result, result, cv::Size(15, 15), 0);
            break;
        case ProcessMode::CANNY:
            cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
            cv::Canny(result, result, 50, 150);
            cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
            break;
        case ProcessMode::SOBEL: {
            cv::Mat grad_x, grad_y, abs_grad;
            cv::Sobel(result, grad_x, CV_16S, 1, 0);
            cv::Sobel(result, grad_y, CV_16S, 0, 1);
            cv::convertScaleAbs(grad_x, grad_x);
            cv::convertScaleAbs(grad_y, grad_y);
            cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, result);
            break;
        }
        case ProcessMode::BINARY:
            cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
            cv::threshold(result, result, 127, 255, cv::THRESH_BINARY);
            cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
            break;
        case ProcessMode::GLITCH: {
            std::vector<cv::Mat> channels;
            cv::split(result, channels);
            if (channels.size() >= 3) {
                cv::Mat shift(channels[0].size(), CV_8UC1, cv::Scalar(0));
                int offset = 10;
                channels[0].copyTo(shift(cv::Rect(offset, 0, channels[0].cols - offset, channels[0].rows)));
                shift(cv::Rect(0, 0, offset, channels[0].rows)).copyTo(channels[0](cv::Rect(channels[0].cols - offset, 0, offset, channels[0].rows)));
                cv::merge(channels, result);
            }
            break;
        }
        case ProcessMode::SEPIA: {
            cv::Mat sepiaKernel = (cv::Mat_<float>(3,3) << 
                0.272f, 0.534f, 0.131f,
                0.349f, 0.686f, 0.168f,
                0.393f, 0.769f, 0.189f);
            cv::transform(result, result, sepiaKernel);
            break;
        }
    }
    
    drawOnFrame(result, keyProcessor);
    if (keyProcessor.shouldShowOverlay() && overlayLoaded) {
        addOverlay(result);
    }
    
    return result;
}

cv::Mat FrameProcessor::applyZoom(const cv::Mat& frame, int zoomLevel) {
    if (zoomLevel == 100) return frame;
    
    float scale = zoomLevel / 100.0f;
    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(), scale, scale);
    
    if (scale > 1.0f) {
        int x = (resized.cols - frame.cols) / 2;
        int y = (resized.rows - frame.rows) / 2;
        return resized(cv::Rect(x, y, frame.cols, frame.rows));
    } else {
        cv::Mat result;
        cv::resize(resized, result, frame.size());
        return result;
    }
}

cv::Mat FrameProcessor::applyBrightnessContrast(const cv::Mat& frame, int brightness, float contrast) {
    cv::Mat result;
    frame.convertTo(result, -1, contrast, brightness);
    return result;
}

void FrameProcessor::drawOnFrame(cv::Mat& frame, const KeyProcessor& keyProcessor) {
    if (!keyProcessor.isDrawingMode()) return;
    
    cv::Point start = keyProcessor.getStartPoint();
    cv::Point end = keyProcessor.getEndPoint();
    
    if (keyProcessor.getMouseMode() == MouseMode::DRAW_LINE) {
        cv::line(frame, start, end, cv::Scalar(0, 255, 0), 2);
    } else if (keyProcessor.getMouseMode() == MouseMode::DRAW_RECT) {
        cv::rectangle(frame, start, end, cv::Scalar(0, 0, 255), 2);
    }
}

void FrameProcessor::addOverlay(cv::Mat& frame) {
    if (overlayImage.empty()) return;
    
    cv::Mat resizedOverlay;
    int overlayWidth = frame.cols / 4;
    int overlayHeight = overlayImage.rows * overlayWidth / overlayImage.cols;
    cv::resize(overlayImage, resizedOverlay, cv::Size(overlayWidth, overlayHeight));
    
    cv::Mat roi = frame(cv::Rect(10, 10, overlayWidth, overlayHeight));
    if (resizedOverlay.channels() == 4) {
        std::vector<cv::Mat> channels;
        cv::split(resizedOverlay, channels);
        cv::Mat overlayBGR, alpha;
        cv::merge(std::vector<cv::Mat>{channels[0], channels[1], channels[2]}, overlayBGR);
        alpha = channels[3] / 255.0;
        
        for (int y = 0; y < overlayHeight; y++) {
            for (int x = 0; x < overlayWidth; x++) {
                if (alpha.at<float>(y, x) > 0) {
                    roi.at<cv::Vec3b>(y, x) = overlayBGR.at<cv::Vec3b>(y, x);
                }
            }
        }
    } else {
        resizedOverlay.copyTo(roi);
    }
}

double FrameProcessor::calculateAverageIntensity(const cv::Mat& frame) const {
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    return cv::mean(gray)[0];
}

cv::Scalar FrameProcessor::calculateChannelIntensities(const cv::Mat& frame) const {
    return cv::mean(frame);
}