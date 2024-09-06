#pragma once
#  include "HalconCpp.h"
#  include "HDevThread.h"
using namespace HalconCpp;

void action()
{

    // Local iconic variables
    HObject  ho_Image_n0, ho_Image_n, ho_ClassRegions;
    HObject  ho_ObjectsSelected, ho_ConnectedRegions, ho_SelectedRegions;

    // Local control variables
    HTuple  hv_AcqHandle, hv_Width, hv_Height, hv_WindowHandle;
    HTuple  hv_Regions, hv_Highlight, hv_MLPHandle, hv_Figure;
    
//    OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false",
//        "default", "[0] YW500", 0, -1, &hv_AcqHandle);
    OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false",
        "default", "[0] Integrated Camera", 0, -1, &hv_AcqHandle);
    GrabImageStart(hv_AcqHandle, -1);
    GrabImageAsync(&ho_Image_n0, hv_AcqHandle, -1);
    GetImageSize(ho_Image_n0, &hv_Width, &hv_Height);
    SetWindowAttr("background_color", "white");
    OpenWindow(0, 0, hv_Width * 2, hv_Height * 2, 0, "visible", "", &hv_WindowHandle);
    HDevWindowStack::Push(hv_WindowHandle);
    if (HDevWindowStack::IsOpen())
        DispObj(ho_Image_n0, HDevWindowStack::GetActive());
    hv_Regions.Clear();
    hv_Regions[0] = "red";
    hv_Regions[1] = "green";
    hv_Regions[2] = "blue";
    hv_Regions[3] = "black";
    hv_Regions[4] = "background";
    hv_Highlight.Clear();
    hv_Highlight[0] = "red";
    hv_Highlight[1] = "green";
    hv_Highlight[2] = "blue";
    hv_Highlight[3] = "black";

    ReadClassMlp("train_mlp_3color", &hv_MLPHandle);
    while (true)
    {
        GrabImageAsync(&ho_Image_n, hv_AcqHandle, -1);
        DispObj(ho_Image_n, HDevWindowStack::GetActive());
        ClassifyImageClassMlp(ho_Image_n, &ho_ClassRegions, hv_MLPHandle, 0.5);

        for (hv_Figure = 1; hv_Figure <= 4; hv_Figure += 1)
        {
            CopyObj(ho_ClassRegions, &ho_ObjectsSelected, hv_Figure, 1);
            Connection(ho_ObjectsSelected, &ho_ConnectedRegions);
            SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 400, 99999);
            if (HDevWindowStack::IsOpen())
                SetColor(HDevWindowStack::GetActive(), HTuple(hv_Highlight[hv_Figure - 1]));
            if (HDevWindowStack::IsOpen())
                DispObj(ho_SelectedRegions, HDevWindowStack::GetActive());

        }
        WaitSeconds(0.01);
    }
    CloseFramegrabber(hv_AcqHandle);
}

void action_plus()
{

    // Local iconic variables
    HObject  ho_Image_n0, ho_Image_n, ho_ClassRegions;
    HObject  ho_ObjectsSelected, ho_ConnectedRegions, ho_RegionFillUp;
    HObject  ho_SelectedRegions, ho_RegionTrans;

    // Local control variables
    HTuple  hv_AcqHandle, hv_Width, hv_Height, hv_WindowHandle;
    HTuple  hv_Regions, hv_Highlight, hv_MLPHandle, hv_Figure;
    HTuple  hv_Area, hv_Row3, hv_Column3, hv_Area_max, hv_Row_x;
    HTuple  hv_Column_y;

    OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false",
        "default", "[0] YW500", 0, -1, &hv_AcqHandle);
    GrabImageStart(hv_AcqHandle, -1);
    GrabImageAsync(&ho_Image_n0, hv_AcqHandle, -1);
    GetImageSize(ho_Image_n0, &hv_Width, &hv_Height);
    SetWindowAttr("background_color", "white");
    OpenWindow(0, 0, hv_Width * 2, hv_Height * 2, 0, "visible", "", &hv_WindowHandle);
    HDevWindowStack::Push(hv_WindowHandle);
    if (HDevWindowStack::IsOpen())
        DispObj(ho_Image_n0, HDevWindowStack::GetActive());
    hv_Regions.Clear();
    hv_Regions[0] = "red";
    hv_Regions[1] = "green";
    hv_Regions[2] = "blue";
    hv_Regions[3] = "black";
    hv_Regions[4] = "background";
    hv_Highlight.Clear();
    hv_Highlight[0] = "red";
    hv_Highlight[1] = "green";
    hv_Highlight[2] = "blue";
    hv_Highlight[3] = "black";

    ReadClassMlp("train_mlp_3color", &hv_MLPHandle);
    while (true)
    {
        GrabImageAsync(&ho_Image_n, hv_AcqHandle, -1);
        DispObj(ho_Image_n, HDevWindowStack::GetActive());
        ClassifyImageClassMlp(ho_Image_n, &ho_ClassRegions, hv_MLPHandle, 0.5);

        for (hv_Figure = 1; hv_Figure <= 4; hv_Figure += 1)
        {
            CopyObj(ho_ClassRegions, &ho_ObjectsSelected, hv_Figure, 1);
            Connection(ho_ObjectsSelected, &ho_ConnectedRegions);
            FillUp(ho_ConnectedRegions, &ho_RegionFillUp);
            AreaCenter(ho_RegionFillUp, &hv_Area, &hv_Row3, &hv_Column3);
            SelectShape(ho_RegionFillUp, &ho_SelectedRegions, "area", "and", hv_Area.TupleMax(),
                99999);
            ShapeTrans(ho_SelectedRegions, &ho_RegionTrans, "inner_rectangle1");
            AreaCenter(ho_RegionTrans, &hv_Area_max, &hv_Row_x, &hv_Column_y);
    

            if (hv_Figure == 1) std::cout << "red_X : " << hv_Row_x.D() << "  red_Y : " << hv_Column_y.D() << std::endl;
 //         else  if (hv_Figure == 2) std::cout << "green_X : " << hv_Row_x.D() << "  red_Y : " << hv_Column_y.D() << std::endl;
 //         else  if (hv_Figure == 3) std::cout << "blue_X : " << hv_Row_x.D() << "  red_Y : " << hv_Column_y.D() << std::endl;
 //         else  if (hv_Figure == 4) std::cout << "black_X : " << hv_Row_x.D() << "  red_Y : " << hv_Column_y.D() << std::endl;

            if (HDevWindowStack::IsOpen())
                SetColor(HDevWindowStack::GetActive(), HTuple(hv_Highlight[hv_Figure - 1]));
            if (HDevWindowStack::IsOpen())
                DispObj(ho_RegionTrans, HDevWindowStack::GetActive());

        }
        WaitSeconds(0.1);
    }
    CloseFramegrabber(hv_AcqHandle);
}