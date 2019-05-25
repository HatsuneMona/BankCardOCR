#pragma once
#include<opencv2/opencv.hpp>
#include"AllUse.h"
#include"Projection.h"
using namespace cv;

class FindNumber {
private:
	void LoadDeal();
	void UseKmeans();
	void 盲猜缩小范围();
	void 找数字();
	void 分离三通道(Mat srcImg);
	void 投影();
	Mat UseSobel(Mat srcImg, int xy = 3);
	void GetEachColor();
public:
	int width, height;
	Mat grayImg;//存储灰度图
	Mat colorfulImg;//存储原图
	Mat kmeansImg;
	Rect 粗略的卡号位置;
	Mat 粗略的卡号图像;
	Mat kmeansErodeImg;
	Mat channalsBGR[3];
	Projection channalsProjectionBGR[3];
	Mat EachColor[8];
	bool EachUseful[8];
	Projection eachColorP[8];

	int x1, x2, y1, y2;

	FindNumber();
	FindNumber(Mat img);
	~FindNumber();
};

