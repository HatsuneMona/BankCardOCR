#pragma once
#include<opencv2/opencv.hpp>
#include"AllUse.h"
#include"Projection.h"
using namespace cv;

class FindNumber {
private:
	void LoadDeal();
	void UseKmeans();
	void ä����С��Χ();
	void ������();
	void ������ͨ��(Mat srcImg);
	void ͶӰ();
	Mat UseSobel(Mat srcImg, int xy = 3);
	void GetEachColor();
public:
	int width, height;
	Mat grayImg;//�洢�Ҷ�ͼ
	Mat colorfulImg;//�洢ԭͼ
	Mat kmeansImg;
	Rect ���ԵĿ���λ��;
	Mat ���ԵĿ���ͼ��;
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

