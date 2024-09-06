#pragma once
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

class Search {
private:
    HalconCpp::HObject ho_Contours, ho_ModelContours, ho_ContoursProjTrans;
    HalconCpp::HTuple hv_DxfStatus, hv_ModelID, hv_GenParamValue,
        hv_HomMat2D, hv_Score, hv_J, hv_HomMatSelected;
public:
     void action_init(HalconCpp::HTuple file_name);
     void action(HalconCpp::HObject& ho_Image);
     void return_img(HalconCpp::HObject& ho_Image);
};