#pragma once
#ifndef IMAGECONVERTION_H
#define IMAGECONVERTION_H

#include <HalconCpp.h>
#include <opencv2/opencv.hpp>

class ImageConvertion {
public:
    ImageConvertion();
    ~ImageConvertion();

public:
    static cv::Mat HImageToCVImage(HalconCpp::HObject& h_image);
    static void HImageToCVImage(HalconCpp::HObject& h_image, cv::Mat& cv_image);
    static HalconCpp::HObject CVImageToHImage(cv::Mat& cv_image);
    static void CVImageToHImage(cv::Mat& cv_image, HalconCpp::HObject& h_image);
};

#endif // IMAGECONVERTION_H
