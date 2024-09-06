#include <iostream>
#include <HalconCpp.h>
#include <HDevThread.h>
#include <Windows.h>
#include "ImageConvertion.h"

using namespace std;
using namespace cv;
using namespace HalconCpp;

int main()
{
    HTuple h_video_image;
    HTuple hv_WindowHandle;
    HObject  ho_Image, img;
    Mat cv_image;
    HTuple width{ 0 }, height{ 0 };
    HTuple file_name = "test.dxf";
    OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false",
        "default", "[0] YW500", 0, -1, &h_video_image);
    Search search;
    search.action_init(file_name);

    SetSystem("tsp_width", 3000);
    SetSystem("tsp_height", 3000);
    SetWindowAttr("background_color", "black");
    OpenWindow(0, 0, 512, 512, 0, "", "", &hv_WindowHandle);
    HDevWindowStack::Push(hv_WindowHandle);


    while (1)
    {
        GrabImageAsync(&ho_Image, h_video_image, -1);
        GetImageSize(ho_Image, &width, &height);
        search.action(ho_Image);
        search.return_img(img);

        ImageConvertion::HImageToCVImage(ho_Image, cv_image);

        imshow("TEST", cv_image);
        waitKey(100);
    }
 //   waitKey(1000);
    CloseFramegrabber(h_video_image);
    return 0;
}