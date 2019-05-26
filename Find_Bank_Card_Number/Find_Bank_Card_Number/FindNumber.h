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
	void CalculatRowSize();
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
	vector<vector<int>> colPeak;
	vector<vector<int>> rowPeak;
	int y1, y2;

	Rect preciseRowRect;
	Mat preciseRowImg;

	FindNumber();
	FindNumber(Mat img);
	~FindNumber();
};

#define ROWPLUSNUM 20
//#define COLPLUSNUM 20
//
//class NumProjectionDeal {
//private:
//	void UsePlus();
//	void FindRowPeak();
//	void CalculatRowSize();
//	double variance(double a[]);//��� 
//public:
//	vector<Projection> eachColorP;
//	vector<vector<int>> colPeak;
//	vector<vector<int>> rowPeak;
//
//	vector<int> rowSize;
//
//	int width, height;
//	int x1, x2, y1, y2;
//	vector<Mat> colPImgWithAverage;
//	vector<Mat> rowPImgWithAverage;
//
//	NumProjectionDeal();
//	NumProjectionDeal(vector<Projection> src);
//	~NumProjectionDeal();
//};