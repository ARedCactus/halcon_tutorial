#include "ImageConvertion.h"

ImageConvertion::ImageConvertion() {

}

ImageConvertion::~ImageConvertion() {

}


cv::Mat ImageConvertion::HImageToCVImage(HalconCpp::HObject& h_image) {
    cv::Mat cv_image;
    HalconCpp::HTuple h_channels;
    HalconCpp::HTuple h_row{ 0 }, h_col{ 0 };
    HalconCpp::ConvertImageType(h_image, &h_image, "byte");
    HalconCpp::CountChannels(h_image, &h_channels);
    HalconCpp::HTuple h_gray_value;
    if (h_channels.I() == 1) {
        HalconCpp::GetImageSize(h_image, &h_col, &h_row);
        cv_image = cv::Mat::zeros(h_row.I(), h_col.I(), CV_8UC1);
        for (int i = 0; i < h_row.I(); ++i) {
            for (int j = 0; j < h_col.I(); ++j) {
                HalconCpp::GetGrayval(h_image, i, j, &h_gray_value);
                cv_image.at<uchar>(i, j) = static_cast<uchar>(h_gray_value.I());
            }
        }
    }
    else if (h_channels.I() == 3) {
        HalconCpp::GetImageSize(h_image, &h_col, &h_row);
        cv_image = cv::Mat::zeros(h_row.I(), h_col.I(), CV_8UC3);
        for (int i = 0; i < h_row.I(); ++i) {
            for (int j = 0; j < h_col.I(); ++j) {
                HalconCpp::GetGrayval(h_image, i, j, &h_gray_value);
                // Warning: OpenCV's color image channel order is BGR, not RGB!!!
                cv_image.at<uchar>(i, j * 3) = static_cast<uchar>(h_gray_value[2].I()); //blue color
                cv_image.at<uchar>(i, j * 3 + 1) = static_cast<uchar>(h_gray_value[1].I()); //green color
                cv_image.at<uchar>(i, j * 3 + 2) = static_cast<uchar>(h_gray_value[0].I()); //red color
            }
        }
    }
    return cv_image;
}

void ImageConvertion::HImageToCVImage(HalconCpp::HObject& h_image, cv::Mat& cv_image) {
    HalconCpp::HTuple h_channels;
    HalconCpp::HTuple h_row{ 0 }, h_col{ 0 };
    HalconCpp::ConvertImageType(h_image, &h_image, "byte");
    HalconCpp::CountChannels(h_image, &h_channels);
    HalconCpp::HTuple h_gray_value;
    if (h_channels.I() == 1) {
        HalconCpp::GetImageSize(h_image, &h_col, &h_row);
        cv_image = cv::Mat::zeros(h_row.I(), h_col.I(), CV_8UC1);
        for (int i = 0; i < h_row.I(); ++i) {
            for (int j = 0; j < h_col.I(); ++j) {
                HalconCpp::GetGrayval(h_image, i, j, &h_gray_value);
                cv_image.at<uchar>(i, j) = static_cast<uchar>(h_gray_value.I());
            }
        }
    }
    else if (h_channels.I() == 3) {
        HalconCpp::GetImageSize(h_image, &h_col, &h_row);
        cv_image = cv::Mat::zeros(h_row.I(), h_col.I(), CV_8UC3);
        for (int i = 0; i < h_row.I(); ++i) {
            for (int j = 0; j < h_col.I(); ++j) {
                HalconCpp::GetGrayval(h_image, i, j, &h_gray_value);
                // Warning: OpenCV's color image channel order is BGR, not RGB!!!
                cv_image.at<uchar>(i, j * 3) = static_cast<uchar>(h_gray_value[2].I()); //blue color
                cv_image.at<uchar>(i, j * 3 + 1) = static_cast<uchar>(h_gray_value[1].I()); //green color
                cv_image.at<uchar>(i, j * 3 + 2) = static_cast<uchar>(h_gray_value[0].I()); //red color
            }
        }
    }
}

HalconCpp::HObject ImageConvertion::CVImageToHImage(cv::Mat& cv_image) {
    HalconCpp::HObject h_image;
    if (cv_image.channels() == 3) {
        size_t length = static_cast<size_t>(cv_image.rows * cv_image.cols);
        uchar* BlueData = new uchar[length];
        uchar* GreenData = new uchar[length];
        uchar* RedData = new uchar[length];
        for (int i = 0; i < cv_image.rows; ++i) {
            uchar* ptr = cv_image.ptr<uchar>(i);
            for (int j = 0; j < cv_image.cols; ++j) {
                BlueData[i * cv_image.cols + j] = ptr[3 * j];
                GreenData[i * cv_image.cols + j] = ptr[3 * j + 1];
                RedData[i * cv_image.cols + j] = ptr[3 * j + 2];
            }
        }
        HalconCpp::GenImage3(&h_image,
            "byte",
            cv_image.cols,
            cv_image.rows,
            reinterpret_cast<Hlong>(RedData),
            reinterpret_cast<Hlong>(GreenData),
            reinterpret_cast<Hlong>(BlueData));
        delete[] RedData;
        delete[] GreenData;
        delete[] BlueData;
    }
    else if (cv_image.channels() == 1) {
        uchar* GrayData = new uchar[static_cast<size_t>(cv_image.rows * cv_image.cols)];
        memcpy(GrayData, cv_image.data, static_cast<size_t>(cv_image.rows * cv_image.cols));
        HalconCpp::GenImage1(&h_image, "byte", cv_image.cols, cv_image.rows, reinterpret_cast<Hlong>(GrayData));
        delete[] GrayData;
    }
    return h_image;
}

void ImageConvertion::CVImageToHImage(cv::Mat& cv_image, HalconCpp::HObject& h_image) {
    if (cv_image.channels() == 3) {
        std::vector<cv::Mat> BgrVector(3);
        cv::split(cv_image, BgrVector);
        size_t size = static_cast<size_t>(cv_image.rows * cv_image.cols);
        uchar* BlueData = new uchar[size];
        uchar* GreenData = new uchar[size];
        uchar* RedData = new uchar[size];
        memcpy(BlueData, BgrVector[0].data, size);
        memcpy(GreenData, BgrVector[1].data, size);
        memcpy(RedData, BgrVector[2].data, size);
        HalconCpp::GenImage3(&h_image,
            "byte",
            cv_image.cols,
            cv_image.rows,
            reinterpret_cast<Hlong>(RedData),
            reinterpret_cast<Hlong>(GreenData),
            reinterpret_cast<Hlong>(BlueData));
        delete[] RedData;
        delete[] GreenData;
        delete[] BlueData;
    }
    else if (cv_image.channels() == 1) {
        uchar* GrayData = new uchar[static_cast<size_t>(cv_image.rows * cv_image.cols)];
        memcpy(GrayData, cv_image.data, static_cast<size_t>(cv_image.rows * cv_image.cols));
        HalconCpp::GenImage1(&h_image, "byte", cv_image.cols, cv_image.rows, reinterpret_cast<Hlong>(GrayData));
        delete[] GrayData;
    }
}
