#include"Halcon.h"
#include"halconcpp\HalconCpp.h"
#include <QPalette>
#include <QWidget>
#include <QMessageBox>
#include <QPainter>


using namespace HalconCpp;

// Procedure declarations 
// External procedures 
// Chapter: Graphics / Text
// Short Description: This procedure displays 'Click 'Run' to continue' in the lower right corner of the screen.
void disp_continue_message(HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box);
// Chapter: Graphics / Text
// Short Description: This procedure writes a text message.
void disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
    HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);
// Chapter: Graphics / Text
// Short Description: Set font independent of OS
void set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
    HTuple hv_Slant);

// Procedures 
// External procedures 
// Chapter: Graphics / Text
// Short Description: This procedure displays 'Click 'Run' to continue' in the lower right corner of the screen.
void disp_continue_message(HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box)
{

    // Local control variables 
    HTuple  hv_ContinueMessage, hv_Row, hv_Column;
    HTuple  hv_Width, hv_Height, hv_Ascent, hv_Descent, hv_TextWidth;
    HTuple  hv_TextHeight;

    //This procedure displays 'Press Run (F5) to continue' in the
    //lower right corner of the screen.
    //It uses the procedure disp_message.
    //
    //Input parameters:
    //WindowHandle: The window, where the text shall be displayed
    //Color: defines the text color.
    //   If set to '' or 'auto', the currently set color is used.
    //Box: If set to 'true', the text is displayed in a box.
    //
    hv_ContinueMessage = "Press Run (F5) to continue";
    GetWindowExtents(hv_WindowHandle, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
    GetStringExtents(hv_WindowHandle, (" " + hv_ContinueMessage) + " ", &hv_Ascent, &hv_Descent,
        &hv_TextWidth, &hv_TextHeight);
    disp_message(hv_WindowHandle, hv_ContinueMessage, "window", (hv_Height - hv_TextHeight) - 12,
        (hv_Width - hv_TextWidth) - 12, hv_Color, hv_Box);
    return;
}

// Chapter: Graphics / Text
// Short Description: This procedure writes a text message.
void disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
    HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box)
{

    // Local control variables 
    HTuple  hv_Red, hv_Green, hv_Blue, hv_Row1Part;
    HTuple  hv_Column1Part, hv_Row2Part, hv_Column2Part, hv_RowWin;
    HTuple  hv_ColumnWin, hv_WidthWin, hv_HeightWin, hv_MaxAscent;
    HTuple  hv_MaxDescent, hv_MaxWidth, hv_MaxHeight, hv_R1;
    HTuple  hv_C1, hv_FactorRow, hv_FactorColumn, hv_Width;
    HTuple  hv_Index, hv_Ascent, hv_Descent, hv_W, hv_H, hv_FrameHeight;
    HTuple  hv_FrameWidth, hv_R2, hv_C2, hv_DrawMode, hv_Exception;
    HTuple  hv_CurrentColor;

    //This procedure displays text in a graphics window.
    //
    //Input parameters:
    //WindowHandle: The WindowHandle of the graphics window, where
    //   the message should be displayed
    //String: A tuple of strings containing the text message to be displayed
    //CoordSystem: If set to 'window', the text position is given
    //   with respect to the window coordinate system.
    //   If set to 'image', image coordinates are used.
    //   (This may be useful in zoomed images.)
    //Row: The row coordinate of the desired text position
    //   If set to -1, a default value of 12 is used.
    //Column: The column coordinate of the desired text position
    //   If set to -1, a default value of 12 is used.
    //Color: defines the color of the text as string.
    //   If set to [], '' or 'auto' the currently set color is used.
    //   If a tuple of strings is passed, the colors are used cyclically
    //   for each new textline.
    //Box: If set to 'true', the text is written within a white box.
    //
    //prepare window
    GetRgb(hv_WindowHandle, &hv_Red, &hv_Green, &hv_Blue);
    GetPart(hv_WindowHandle, &hv_Row1Part, &hv_Column1Part, &hv_Row2Part, &hv_Column2Part);
    GetWindowExtents(hv_WindowHandle, &hv_RowWin, &hv_ColumnWin, &hv_WidthWin, &hv_HeightWin);
    SetPart(hv_WindowHandle, 0, 0, hv_HeightWin - 1, hv_WidthWin - 1);
    //
    //default settings
    if (0 != (hv_Row == -1))
    {
        hv_Row = 12;
    }
    if (0 != (hv_Column == -1))
    {
        hv_Column = 12;
    }
    if (0 != (hv_Color == HTuple()))
    {
        hv_Color = "";
    }
    //
    hv_String = (("" + hv_String) + "").TupleSplit("\n");
    //
    //Estimate extentions of text depending on font size.
    GetFontExtents(hv_WindowHandle, &hv_MaxAscent, &hv_MaxDescent, &hv_MaxWidth, &hv_MaxHeight);
    if (0 != (hv_CoordSystem == HTuple("window")))
    {
        hv_R1 = hv_Row;
        hv_C1 = hv_Column;
    }
    else
    {
        //transform image to window coordinates
        hv_FactorRow = (1. * hv_HeightWin) / ((hv_Row2Part - hv_Row1Part) + 1);
        hv_FactorColumn = (1. * hv_WidthWin) / ((hv_Column2Part - hv_Column1Part) + 1);
        hv_R1 = ((hv_Row - hv_Row1Part) + 0.5) * hv_FactorRow;
        hv_C1 = ((hv_Column - hv_Column1Part) + 0.5) * hv_FactorColumn;
    }
    //
    //display text box depending on text size
    if (0 != (hv_Box == HTuple("true")))
    {
        //calculate box extents
        hv_String = (" " + hv_String) + " ";
        hv_Width = HTuple();
        HTuple end_val57 = (hv_String.TupleLength()) - 1;
        HTuple step_val57 = 1;
        for (hv_Index = 0; hv_Index.Continue(end_val57, step_val57); hv_Index += step_val57)
        {
            GetStringExtents(hv_WindowHandle, HTuple(hv_String[hv_Index]), &hv_Ascent,
                &hv_Descent, &hv_W, &hv_H);
            hv_Width = hv_Width.TupleConcat(hv_W);
        }
        hv_FrameHeight = hv_MaxHeight * (hv_String.TupleLength());
        hv_FrameWidth = (HTuple(0).TupleConcat(hv_Width)).TupleMax();
        hv_R2 = hv_R1 + hv_FrameHeight;
        hv_C2 = hv_C1 + hv_FrameWidth;
        //display rectangles
        GetDraw(hv_WindowHandle, &hv_DrawMode);
        SetDraw(hv_WindowHandle, "fill");
        SetColor(hv_WindowHandle, "light gray");
        DispRectangle1(hv_WindowHandle, hv_R1 + 3, hv_C1 + 3, hv_R2 + 3, hv_C2 + 3);
        SetColor(hv_WindowHandle, "white");
        DispRectangle1(hv_WindowHandle, hv_R1, hv_C1, hv_R2, hv_C2);
        SetDraw(hv_WindowHandle, hv_DrawMode);
    }
    else if (0 != (hv_Box != HTuple("false")))
    {
        hv_Exception = "Wrong value of control parameter Box";
        throw HalconCpp::HException(hv_Exception);
    }
    //Write text.
    HTuple end_val78 = (hv_String.TupleLength()) - 1;
    HTuple step_val78 = 1;
    for (hv_Index = 0; hv_Index.Continue(end_val78, step_val78); hv_Index += step_val78)
    {
        hv_CurrentColor = ((const HTuple&)hv_Color)[hv_Index % (hv_Color.TupleLength())];
        if (0 != (HTuple(hv_CurrentColor != HTuple("")).TupleAnd(hv_CurrentColor != HTuple("auto"))))
        {
            SetColor(hv_WindowHandle, hv_CurrentColor);
        }
        else
        {
            SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
        }
        hv_Row = hv_R1 + (hv_MaxHeight * hv_Index);
        SetTposition(hv_WindowHandle, hv_Row, hv_C1);
        WriteString(hv_WindowHandle, HTuple(hv_String[hv_Index]));
    }
    //reset changed window settings
    SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
    SetPart(hv_WindowHandle, hv_Row1Part, hv_Column1Part, hv_Row2Part, hv_Column2Part);
    return;
}

// Chapter: Graphics / Text
// Short Description: Set font independent of OS
void set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
    HTuple hv_Slant)
{

    // Local control variables 
    HTuple  hv_OS, hv_PreferenceValue, hv_Exception;
    HTuple  hv_BoldString, hv_SlantString, hv_AllowedFontSizes;
    HTuple  hv_Distances, hv_Indices, hv_Fonts, hv_FontSelRegexp;
    HTuple  hv_FontsCourier;

    //This procedure sets the text font of the current window with
    //the specified attributes.
    //It is assumed that following fonts are installed on the system:
    //Windows: Courier New, Arial Times New Roman
    //Mac OS X: CourierNewPS, Arial, TimesNewRomanPS
    //Linux: courier, helvetica, times
    //Because fonts are displayed smaller on Linux than on Windows,
    //a scaling factor of 1.25 is used the get comparable results.
    //For Linux, only a limited number of font sizes is supported,
    //to get comparable results, it is recommended to use one of the
    //following sizes: 9, 11, 14, 16, 20, 27
    //(which will be mapped internally on Linux systems to 11, 14, 17, 20, 25, 34)
    //
    //Input parameters:
    //WindowHandle: The graphics window for which the font will be set
    //Size: The font size. If Size=-1, the default of 16 is used.
    //Bold: If set to 'true', a bold font is used
    //Slant: If set to 'true', a slanted font is used
    //
    GetSystem("operating_system", &hv_OS);
    // dev_get_preferences(...); only in hdevelop
    // dev_set_preferences(...); only in hdevelop
    if (0 != (HTuple(hv_Size == HTuple()).TupleOr(hv_Size == -1)))
    {
        hv_Size = 16;
    }
    if (0 != ((hv_OS.TupleSubstr(0, 2)) == HTuple("Win")))
    {
        //Set font on Windows systems
        if (0 != (HTuple(HTuple(hv_Font == HTuple("mono")).TupleOr(hv_Font == HTuple("Courier"))).TupleOr(hv_Font == HTuple("courier"))))
        {
            hv_Font = "Courier New";
        }
        else if (0 != (hv_Font == HTuple("sans")))
        {
            hv_Font = "Arial";
        }
        else if (0 != (hv_Font == HTuple("serif")))
        {
            hv_Font = "Times New Roman";
        }
        if (0 != (hv_Bold == HTuple("true")))
        {
            hv_Bold = 1;
        }
        else if (0 != (hv_Bold == HTuple("false")))
        {
            hv_Bold = 0;
        }
        else
        {
            hv_Exception = "Wrong value of control parameter Bold";
            throw HalconCpp::HException(hv_Exception);
        }
        if (0 != (hv_Slant == HTuple("true")))
        {
            hv_Slant = 1;
        }
        else if (0 != (hv_Slant == HTuple("false")))
        {
            hv_Slant = 0;
        }
        else
        {
            hv_Exception = "Wrong value of control parameter Slant";
            throw HalconCpp::HException(hv_Exception);
        }
        try
        {
            SetFont(hv_WindowHandle, ((((((("-" + hv_Font) + "-") + hv_Size) + "-*-") + hv_Slant) + "-*-*-") + hv_Bold) + "-");
        }
        // catch (Exception) 
        catch (HalconCpp::HException& HDevExpDefaultException)
        {
            HDevExpDefaultException.ToHTuple(&hv_Exception);
            //throw (Exception)
        }
    }
    else if (0 != ((hv_OS.TupleSubstr(0, 2)) == HTuple("Dar")))
    {
        //Set font on Mac OS X systems
        if (0 != (hv_Bold == HTuple("true")))
        {
            hv_BoldString = "Bold";
        }
        else if (0 != (hv_Bold == HTuple("false")))
        {
            hv_BoldString = "";
        }
        else
        {
            hv_Exception = "Wrong value of control parameter Bold";
            throw HalconCpp::HException(hv_Exception);
        }
        if (0 != (hv_Slant == HTuple("true")))
        {
            hv_SlantString = "Italic";
        }
        else if (0 != (hv_Slant == HTuple("false")))
        {
            hv_SlantString = "";
        }
        else
        {
            hv_Exception = "Wrong value of control parameter Slant";
            throw HalconCpp::HException(hv_Exception);
        }
        if (0 != (HTuple(HTuple(hv_Font == HTuple("mono")).TupleOr(hv_Font == HTuple("Courier"))).TupleOr(hv_Font == HTuple("courier"))))
        {
            hv_Font = "CourierNewPS";
        }
        else if (0 != (hv_Font == HTuple("sans")))
        {
            hv_Font = "Arial";
        }
        else if (0 != (hv_Font == HTuple("serif")))
        {
            hv_Font = "TimesNewRomanPS";
        }
        if (0 != (HTuple(hv_Bold == HTuple("true")).TupleOr(hv_Slant == HTuple("true"))))
        {
            hv_Font = ((hv_Font + "-") + hv_BoldString) + hv_SlantString;
        }
        hv_Font += HTuple("MT");
        try
        {
            SetFont(hv_WindowHandle, (hv_Font + "-") + hv_Size);
        }
        // catch (Exception) 
        catch (HalconCpp::HException& HDevExpDefaultException)
        {
            HDevExpDefaultException.ToHTuple(&hv_Exception);
            //throw (Exception)
        }
    }
    else
    {
        //Set font for UNIX systems
        hv_Size = hv_Size * 1.25;
        hv_AllowedFontSizes.Clear();
        hv_AllowedFontSizes[0] = 11;
        hv_AllowedFontSizes[1] = 14;
        hv_AllowedFontSizes[2] = 17;
        hv_AllowedFontSizes[3] = 20;
        hv_AllowedFontSizes[4] = 25;
        hv_AllowedFontSizes[5] = 34;
        if (0 != ((hv_AllowedFontSizes.TupleFind(hv_Size)) == -1))
        {
            hv_Distances = (hv_AllowedFontSizes - hv_Size).TupleAbs();
            TupleSortIndex(hv_Distances, &hv_Indices);
            hv_Size = ((const HTuple&)hv_AllowedFontSizes)[HTuple(hv_Indices[0])];
        }
        if (0 != (HTuple(hv_Font == HTuple("mono")).TupleOr(hv_Font == HTuple("Courier"))))
        {
            hv_Font = "courier";
        }
        else if (0 != (hv_Font == HTuple("sans")))
        {
            hv_Font = "helvetica";
        }
        else if (0 != (hv_Font == HTuple("serif")))
        {
            hv_Font = "times";
        }
        if (0 != (hv_Bold == HTuple("true")))
        {
            hv_Bold = "bold";
        }
        else if (0 != (hv_Bold == HTuple("false")))
        {
            hv_Bold = "medium";
        }
        else
        {
            hv_Exception = "Wrong value of control parameter Bold";
            throw HalconCpp::HException(hv_Exception);
        }
        if (0 != (hv_Slant == HTuple("true")))
        {
            if (0 != (hv_Font == HTuple("times")))
            {
                hv_Slant = "i";
            }
            else
            {
                hv_Slant = "o";
            }
        }
        else if (0 != (hv_Slant == HTuple("false")))
        {
            hv_Slant = "r";
        }
        else
        {
            hv_Exception = "Wrong value of control parameter Slant";
            throw HalconCpp::HException(hv_Exception);
        }
        try
        {
            SetFont(hv_WindowHandle, ((((((("-adobe-" + hv_Font) + "-") + hv_Bold) + "-") + hv_Slant) + "-normal-*-") + hv_Size) + "-*-*-*-*-*-*-*");
        }
        // catch (Exception) 
        catch (HalconCpp::HException& HDevExpDefaultException)
        {
            HDevExpDefaultException.ToHTuple(&hv_Exception);
            if (0 != (HTuple((hv_OS.TupleSubstr(0, 4)) == HTuple("Linux")).TupleAnd(hv_Font == HTuple("courier"))))
            {
                QueryFont(hv_WindowHandle, &hv_Fonts);
                hv_FontSelRegexp = (("^-[^-]*-[^-]*[Cc]ourier[^-]*-" + hv_Bold) + "-") + hv_Slant;
                hv_FontsCourier = (hv_Fonts.TupleRegexpSelect(hv_FontSelRegexp)).TupleRegexpMatch(hv_FontSelRegexp);
                if (0 != ((hv_FontsCourier.TupleLength()) == 0))
                {
                    hv_Exception = "Wrong font name";
                    //throw (Exception)
                }
                else
                {
                    try
                    {
                        SetFont(hv_WindowHandle, ((HTuple(hv_FontsCourier[0]) + "-normal-*-") + hv_Size) + "-*-*-*-*-*-*-*");
                    }
                    // catch (Exception) 
                    catch (HalconCpp::HException& HDevExpDefaultException)
                    {
                        HDevExpDefaultException.ToHTuple(&hv_Exception);
                        //throw (Exception)
                    }
                }
            }
            //throw (Exception)
        }
    }
    // dev_set_preferences(...); only in hdevelop
    return;
}




JDBOX1::JDBOX1(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Local iconic variables 
    HObject  ho_Image, ho_GrayImage, ho_ImageMean;
    HObject  ho_ImageGauss, ho_Regions, ho_SelectedRegions2;
    HObject  ho_ConvexRegions, ho_SelectedRegions1, ho_SelectedRegions;
    HTuple  hv_Width, hv_Height, hv_WindowID, hv_Phi;

    ReadImage(&ho_Image, "D:/JDBOX5.jpg");
    GetImageSize(ho_Image, &hv_Width, &hv_Height);
    if (HDevWindowStack::IsOpen())
        CloseWindow(HDevWindowStack::Pop());
    SetWindowAttr("background_color", "black");



    Hlong winID = (Hlong)this->winId();//添加位置qt混合添加段
    OpenWindow(0, 0, hv_Width / 7, hv_Height / 7, winID, "", "", &hv_WindowID);//修改这句 添加winID



    HDevWindowStack::Push(hv_WindowID);
    if (HDevWindowStack::IsOpen())
        DispObj(ho_Image, HDevWindowStack::GetActive());
    set_display_font(hv_WindowID, 14, "mono", "true", "false");
    disp_continue_message(hv_WindowID, "black", "true");
    Rgb1ToGray(ho_Image, &ho_GrayImage);
    Threshold(ho_GrayImage, &ho_Regions, 130, 238);
    Connection(ho_Regions, &ho_Regions);
    SelectShape(ho_Regions, &ho_SelectedRegions2, "area", "and", 506286, 751910);
    if (HDevWindowStack::IsOpen())
        SetDraw(HDevWindowStack::GetActive(), "fill");
    if (HDevWindowStack::IsOpen())
        SetColored(HDevWindowStack::GetActive(), 12);
    if (HDevWindowStack::IsOpen())
        DispObj(ho_SelectedRegions2, HDevWindowStack::GetActive());
    disp_continue_message(hv_WindowID, "black", "true");
    if (HDevWindowStack::IsOpen())
        DispObj(ho_Image, HDevWindowStack::GetActive());
    if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(), "green");
    if (HDevWindowStack::IsOpen())
        DispObj(ho_SelectedRegions2, HDevWindowStack::GetActive());
    OrientationRegion(ho_SelectedRegions2, &hv_Phi);
    AreaCenter(ho_SelectedRegions2, &hv_Area, &hv_Row, &hv_Column);
    if (HDevWindowStack::IsOpen())
        SetLineWidth(HDevWindowStack::GetActive(), 3);
    if (HDevWindowStack::IsOpen())
        SetDraw(HDevWindowStack::GetActive(), "margin");
    hv_Length = 200;
    HTuple end_val38 = (hv_Phi.TupleLength()) - 1;
    HTuple step_val38 = 1;
    for (hv_i = 0; hv_i.Continue(end_val38, step_val38); hv_i += step_val38)
    {
        if (HDevWindowStack::IsOpen())
            SetColor(HDevWindowStack::GetActive(), "white");
        //disp_arrow (WindowID, Row[i], Column[i], Row[i] - Length * sin(Phi[i]), Column[i] + Length * cos(Phi[i]), 4)
        DispCross(hv_WindowID, HTuple(hv_Row[hv_i]), HTuple(hv_Column[hv_i]), 200, HTuple(hv_Phi[hv_i]));
        disp_message(hv_WindowID, ((HTuple(hv_Phi[hv_i]).TupleDeg()).TupleString("3.1f")) + " deg",
            "image", HTuple(hv_Row[hv_i]), HTuple(hv_Column[hv_i]) - 100, "black", "false");
    }

    setFixedSize(hv_Height / 7 + 200, hv_Height / 7); // 禁止改变窗口大小。

}



void JDBOX1::paintEvent(QPaintEvent*)
{



    QPainter painter(this);
    // painter.drawLine(0,0,200,300);
   //   painter.drawEllipse(100,100,50,50);
   //  painter.setBrush(Qt::red);
  //    painter.drawRect(120,120,30,30);


     // QPoint p( 500, 500);
// 设置字体：微软雅黑、点大小50、斜体
//painter.setPen(QColor(0, 160, 230));
    painter.setPen(QColor(0, 0, 0));
    QFont font;
    font.setFamily("Microsoft YaHei");
    //font.setFamily("simsun");
    font.setPointSize(10);
    // font.setItalic(true);
    painter.setFont(font);



    painter.drawText(450, 20, QString::fromLocal8Bit("图上箱子的坐标及面积:"));


    for (int i = 0; i <= 7; i = i++)
    {

        QString Xword = QString::fromLocal8Bit("x轴%1: %2").arg(i).arg(QString::number(hv_Row[i].D()));
        painter.drawText(450, 50 + 30 * i, Xword);

    }


    for (int i = 0; i <= 7; i = i++)
    {

        QString Xword = QString::fromLocal8Bit("y轴%1: %2").arg(i).arg(QString::number(hv_Column[i].D()));
        painter.drawText(550, 50 + 30 * i, Xword);

    }


    for (int i = 0; i <= 7; i = i++)
    {

        QString Xword = QString::fromLocal8Bit("面积%1: %2").arg(i).arg(QString::number(hv_Area[i].D()));
        painter.drawText(650, 50 + 30 * i, Xword);

    }
}



JDBOX1::~JDBOX1()
{

}