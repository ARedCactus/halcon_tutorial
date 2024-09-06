#include <iostream>
#include <HalconCpp.h>
#include <HDevThread.h>
#include "ImageConvertion.h"

using namespace std;
using namespace HalconCpp;

int main() {
    cv::Mat cv_image = cv::imread("/home/liuqiang/image.png", cv::IMREAD_COLOR);
    HObject  ho_Image, ho_GrayImage, ho_Region;
    HTuple h_Image;
    //    ho_Image = ImageConvertion::CVImageToHImage(cv_image);

    OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false",
        "default", "[0] YW500", 0, -1, &h_Image);
    GrabImageAsync(&ho_Image, h_Image, -1);

//    ImageConvertion::CVImageToHImage(cv_image, ho_Image);
    //    ReadImage(&ho_Image, "/home/liuqiang/image.png");
    Rgb1ToGray(ho_Image, &ho_GrayImage);
    Threshold(ho_GrayImage, &ho_Region, 128, 255);
    HTuple width{ 0 }, height{ 0 };
    GetImageSize(ho_Image, &width, &height);
    std::cout << "ho_Image: " << width.I() << " * " << height.I() << "\n";
    GetImageSize(ho_GrayImage, &width, &height);
    std::cout << "ho_GrayImage: " << width.I() << " * " << height.I() << "\n";
    cv::namedWindow("ho_image", cv::WINDOW_NORMAL);
    cv::namedWindow("ho_GrayImage", cv::WINDOW_NORMAL);
    cv::Mat post_image;
    ImageConvertion::HImageToCVImage(ho_Image, post_image);
    cv::imshow("ho_image", post_image);
    //    cv::imshow("ho_image", ImageConvertion::HImageToCVImage(ho_Image));
    ImageConvertion::HImageToCVImage(ho_GrayImage, post_image);
    cv::imshow("ho_GrayImage", post_image);
    //    cv::imshow("ho_GrayImage", ImageConvertion::HImageToCVImage(ho_GrayImage));
    cv::waitKey(0);
    return 0;
}
