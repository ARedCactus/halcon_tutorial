#pragma once
#  include "HalconCpp.h"
#  include "HDevThread.h"
using namespace HalconCpp;

void dev_open_window_fit_image(HObject ho_Image, HTuple hv_Row, HTuple hv_Column,
    HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple* hv_WindowHandle);

void dev_update_off();

void set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
    HTuple hv_Slant);

void display_found_data_codes(HObject ho_SymbolXLDs, HTuple hv_WindowHandle, HTuple hv_DecodedDataStrings,
    HTuple hv_TitleMessage, HTuple hv_ResultMessage, HTuple hv_ColorDecodedStrings,
    HTuple hv_ColorResult);

void action();