#pragma once
#ifndef JDBOX1_H
#define JDBOX1_H
#include <QtWidgets/QMainWindow>
#include "ui_jdbox1.h"
#include"Halcon.h"
#include"halconcpp\HalconCpp.h"

using namespace HalconCpp;

class JDBOX1 : public QMainWindow
{
	Q_OBJECT
public:
	JDBOX1(QWidget* parent = 0);
	~JDBOX1();
	void paintEvent(QPaintEvent*);
	HalconCpp::HTuple hv_Area, hv_Row, hv_Column, hv_Length, hv_i;//halcon�еĲ������Ҹ���Ϊ�˹������������㴫��ֵ	
private:
	Ui::JDBOX1Class ui;

};

#endif // JDBOX1_H