#pragma once
#include "AllUse.h"
#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;

class Projection {
private:
	void ProjectionMain();
	void UsefulJudge();
public:
	Mat srcImg;
	int height;
	int width;
	//统计细节
	int rowStat[IMG_HEIGHT] = { 0 };
	int colStat[IMG_WIDTH] = { 0 };
	//其他统计：平均线、最大值
	int statAll = 0;
	int colStatAverage;
	int rowStatAverage;
	int rowMax = 0;
	int colMax = 0;
	//图像输出
	Mat colProjectionImg;
	Mat rowProjectionImg;
	Mat colPImgWithAverage;
	Mat rowPImgWithAverage;
	//判断是否为有利用价值的投影
	bool rowUseful = true;
	bool colUseful = true;

	vector<pt> rowTanPeak(double tanMinValue, int precisionValue, int passValue);
	vector<pt> colTanPeak(double tanMinValue, int precisionValue, int passValue);
	void DrawProjection();
	//void CutTip(int rowValue, int colValue);//不能用
	Projection(Mat img);
	Projection();
	~Projection();
};
