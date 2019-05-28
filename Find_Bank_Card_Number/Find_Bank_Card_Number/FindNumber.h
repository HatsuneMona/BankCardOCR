#pragma once
#include<opencv2/opencv.hpp>
#include"AllUse.h"
#include"Projection.h"
using namespace cv;

#define COLORSTAT_MIN 350 //��ɫͳ��ɸѡ�������

class NumProjection;

class FindNumber {
private:
	void LoadDeal();
	void UseKmeans();
	void ä����С��Χ();
	void ������();
	Mat UseSobel(Mat srcImg, int xy = 3);
	void GetEachColor();
	void FindRowPeak();
	void FindRow();
	vector<Rect> UseFindContours(Mat srcImg);
	void CutRow();
public:
	//int width, height;
	Mat grayImg;//�洢�Ҷ�ͼ
	Mat colorfulImg;//�洢ԭͼ
	Mat kmeansImg;
	Rect roughlyNumRect;
	Mat roughlyNumImg;
	Mat kmeansErodeImg;
	vector<Projection> eachColorP;
	vector<vector<pt>> colPeak;
	vector<vector<pt>> rowPeak;
	int y1, y2;

	Rect preciseRowRect;
	Mat preciseRowImg;

	FindNumber();
	FindNumber(Mat img);
	~FindNumber();
};
