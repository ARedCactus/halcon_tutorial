#include "track.h"

void dev_open_window_fit_image(HObject ho_Image, HTuple hv_Row, HTuple hv_Column,
    HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple* hv_WindowHandle)
{
    HTuple  hv_MinWidth, hv_MaxWidth, hv_MinHeight;
    HTuple  hv_MaxHeight, hv_ResizeFactor, hv_ImageWidth, hv_ImageHeight;
    HTuple  hv_TempWidth, hv_TempHeight, hv_WindowWidth, hv_WindowHeight;


    if (0 != (HTuple(int((hv_WidthLimit.TupleLength()) == 0)).TupleOr(int(hv_WidthLimit < 0))))
    {
        hv_MinWidth = 500;
        hv_MaxWidth = 800;
    }
    else if (0 != (int((hv_WidthLimit.TupleLength()) == 1)))
    {
        hv_MinWidth = 0;
        hv_MaxWidth = hv_WidthLimit;
    }
    else
    {
        hv_MinWidth = ((const HTuple&)hv_WidthLimit)[0];
        hv_MaxWidth = ((const HTuple&)hv_WidthLimit)[1];
    }
    //Parse input tuple HeightLimit
    if (0 != (HTuple(int((hv_HeightLimit.TupleLength()) == 0)).TupleOr(int(hv_HeightLimit < 0))))
    {
        hv_MinHeight = 400;
        hv_MaxHeight = 600;
    }
    else if (0 != (int((hv_HeightLimit.TupleLength()) == 1)))
    {
        hv_MinHeight = 0;
        hv_MaxHeight = hv_HeightLimit;
    }
    else
    {
        hv_MinHeight = ((const HTuple&)hv_HeightLimit)[0];
        hv_MaxHeight = ((const HTuple&)hv_HeightLimit)[1];
    }
    //
    //Test, if window size has to be changed.
    hv_ResizeFactor = 1;
    GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
    //First, expand window to the minimum extents (if necessary).
    if (0 != (HTuple(int(hv_MinWidth > hv_ImageWidth)).TupleOr(int(hv_MinHeight > hv_ImageHeight))))
    {
        hv_ResizeFactor = (((hv_MinWidth.TupleReal()) / hv_ImageWidth).TupleConcat((hv_MinHeight.TupleReal()) / hv_ImageHeight)).TupleMax();
    }
    hv_TempWidth = hv_ImageWidth * hv_ResizeFactor;
    hv_TempHeight = hv_ImageHeight * hv_ResizeFactor;
    //Then, shrink window to maximum extents (if necessary).
    if (0 != (HTuple(int(hv_MaxWidth < hv_TempWidth)).TupleOr(int(hv_MaxHeight < hv_TempHeight))))
    {
        hv_ResizeFactor = hv_ResizeFactor * ((((hv_MaxWidth.TupleReal()) / hv_TempWidth).TupleConcat((hv_MaxHeight.TupleReal()) / hv_TempHeight)).TupleMin());
    }
    hv_WindowWidth = hv_ImageWidth * hv_ResizeFactor;
    hv_WindowHeight = hv_ImageHeight * hv_ResizeFactor;
    //Resize window
    SetWindowAttr("background_color", "black");
    OpenWindow(hv_Row, hv_Column, hv_WindowWidth, hv_WindowHeight, 0, "visible", "", &(*hv_WindowHandle));
    HDevWindowStack::Push((*hv_WindowHandle));
    if (HDevWindowStack::IsOpen())
        SetPart(HDevWindowStack::GetActive(), 0, 0, hv_ImageHeight - 1, hv_ImageWidth - 1);
    return;
}

void dev_update_off()
{
    return;
}

void set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
    HTuple hv_Slant)
{
    HTuple  hv_OS, hv_Fonts, hv_Style, hv_Exception;
    HTuple  hv_AvailableFonts, hv_Fdx, hv_Indices;

    GetSystem("operating_system", &hv_OS);
    if (0 != (HTuple(int(hv_Size == HTuple())).TupleOr(int(hv_Size == -1))))
    {
        hv_Size = 16;
    }
    if (0 != (int((hv_OS.TupleSubstr(0, 2)) == HTuple("Win"))))
    {
        //Restore previous behaviour
        hv_Size = (1.13677 * hv_Size).TupleInt();
    }
    else
    {
        hv_Size = hv_Size.TupleInt();
    }
    if (0 != (int(hv_Font == HTuple("Courier"))))
    {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Courier";
        hv_Fonts[1] = "Courier 10 Pitch";
        hv_Fonts[2] = "Courier New";
        hv_Fonts[3] = "CourierNew";
        hv_Fonts[4] = "Liberation Mono";
    }
    else if (0 != (int(hv_Font == HTuple("mono"))))
    {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Consolas";
        hv_Fonts[1] = "Menlo";
        hv_Fonts[2] = "Courier";
        hv_Fonts[3] = "Courier 10 Pitch";
        hv_Fonts[4] = "FreeMono";
        hv_Fonts[5] = "Liberation Mono";
    }
    else if (0 != (int(hv_Font == HTuple("sans"))))
    {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Luxi Sans";
        hv_Fonts[1] = "DejaVu Sans";
        hv_Fonts[2] = "FreeSans";
        hv_Fonts[3] = "Arial";
        hv_Fonts[4] = "Liberation Sans";
    }
    else if (0 != (int(hv_Font == HTuple("serif"))))
    {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Times New Roman";
        hv_Fonts[1] = "Luxi Serif";
        hv_Fonts[2] = "DejaVu Serif";
        hv_Fonts[3] = "FreeSerif";
        hv_Fonts[4] = "Utopia";
        hv_Fonts[5] = "Liberation Serif";
    }
    else
    {
        hv_Fonts = hv_Font;
    }
    hv_Style = "";
    if (0 != (int(hv_Bold == HTuple("true"))))
    {
        hv_Style += HTuple("Bold");
    }
    else if (0 != (int(hv_Bold != HTuple("false"))))
    {
        hv_Exception = "Wrong value of control parameter Bold";
        throw HException(hv_Exception);
    }
    if (0 != (int(hv_Slant == HTuple("true"))))
    {
        hv_Style += HTuple("Italic");
    }
    else if (0 != (int(hv_Slant != HTuple("false"))))
    {
        hv_Exception = "Wrong value of control parameter Slant";
        throw HException(hv_Exception);
    }
    if (0 != (int(hv_Style == HTuple(""))))
    {
        hv_Style = "Normal";
    }
    QueryFont(hv_WindowHandle, &hv_AvailableFonts);
    hv_Font = "";
    {
        HTuple end_val48 = (hv_Fonts.TupleLength()) - 1;
        HTuple step_val48 = 1;
        for (hv_Fdx = 0; hv_Fdx.Continue(end_val48, step_val48); hv_Fdx += step_val48)
        {
            hv_Indices = hv_AvailableFonts.TupleFind(HTuple(hv_Fonts[hv_Fdx]));
            if (0 != (int((hv_Indices.TupleLength()) > 0)))
            {
                if (0 != (int(HTuple(hv_Indices[0]) >= 0)))
                {
                    hv_Font = HTuple(hv_Fonts[hv_Fdx]);
                    break;
                }
            }
        }
    }
    if (0 != (int(hv_Font == HTuple(""))))
    {
        throw HException("Wrong value of control parameter Font");
    }
    hv_Font = (((hv_Font + "-") + hv_Style) + "-") + hv_Size;
    SetFont(hv_WindowHandle, hv_Font);
    return;
}

void display_found_data_codes(HObject ho_SymbolXLDs, HTuple hv_WindowHandle, HTuple hv_DecodedDataStrings,
    HTuple hv_TitleMessage, HTuple hv_ResultMessage, HTuple hv_ColorDecodedStrings,
    HTuple hv_ColorResult)
{

    // Local iconic variables
    HObject  ho_Image;

    // Local control variables
    HTuple  hv_ImageFiles, hv_ImageNum, hv_DataCodeHandle;
    HTuple  hv_Index, hv_ResultHandles, hv_Message;

    dev_update_off();
    if (HDevWindowStack::IsOpen())
        CloseWindow(HDevWindowStack::Pop());
    hv_ImageFiles = "datacode/qrcode/qr_workpiece_";
    hv_ImageNum = 9;
    ReadImage(&ho_Image, hv_ImageFiles + "01");
    dev_open_window_fit_image(ho_Image, 0, 0, -1, -1, &hv_WindowHandle);
    set_display_font(hv_WindowHandle, 16, "mono", "true", "false");
    if (HDevWindowStack::IsOpen())
        SetLineWidth(HDevWindowStack::GetActive(), 3);
    if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(), "green");

    CreateDataCode2dModel("QR Code", HTuple(), HTuple(), &hv_DataCodeHandle);

    {
        HTuple end_val26 = hv_ImageNum;
        HTuple step_val26 = 1;
        for (hv_Index = 1; hv_Index.Continue(end_val26, step_val26); hv_Index += step_val26)
        {
            ReadImage(&ho_Image, hv_ImageFiles + (hv_Index.TupleString(".2d")));
            FindDataCode2d(ho_Image, &ho_SymbolXLDs, hv_DataCodeHandle, HTuple(), HTuple(),
                &hv_ResultHandles, &hv_DecodedDataStrings);
            //
            //Display the results.
            if (HDevWindowStack::IsOpen())
                DispObj(ho_Image, HDevWindowStack::GetActive());
            if (HDevWindowStack::IsOpen())
                DispObj(ho_SymbolXLDs, HDevWindowStack::GetActive());
            if (HDevWindowStack::IsOpen())
                DispText(HDevWindowStack::GetActive(), (("Image " + hv_Index) + " of ") + hv_ImageNum,
                    "window", 12, 12, "black", HTuple(), HTuple());
            if (HDevWindowStack::IsOpen())
                DispText(HDevWindowStack::GetActive(), hv_DecodedDataStrings, "window", 40,
                    12, "black", HTuple(), HTuple());
            hv_Message = "No data code found.";
            hv_Message[1] = "The symbol could not be found with the standard";
            hv_Message[2] = "default setting. Please adjust the model parameters";
            hv_Message[3] = "to read this symbol.";
            //
            //If no data code could be found.
            if (0 != (int((hv_DecodedDataStrings.TupleLength()) == 0)))
            {
                if (HDevWindowStack::IsOpen())
                    DispText(HDevWindowStack::GetActive(), hv_Message, "window", 40, 12, "red",
                        HTuple(), HTuple());
            }
            //
            //Deactivate the following lines to run the program without breaks.
            if (0 != (int(hv_Index < hv_ImageNum)))
            {
                if (HDevWindowStack::IsOpen())
                    DispText(HDevWindowStack::GetActive(), "Press Run (F5) to continue", "window",
                        "bottom", "right", "black", HTuple(), HTuple());
                // stop(...); only in hdevelop
            }
        }
    }
}

