#pragma once
#include "AllUse.h"
#include <vector>
using std::vector;

class Projection {
private:
	void ProjectionMain();
	void UsefulJudge();
public:
	Mat srcImg;
	int height;
	int width;
	//ͳ��ϸ��
	int rowStat[IMG_HEIGHT] = { 0 };
	int colStat[IMG_WIDTH] = { 0 };
	//����ͳ�ƣ�ƽ���ߡ����ֵ
	int statAverageTemp = 0;
	int colStatAverage;
	int rowStatAverage;
	int rowMax = 0;
	int colMax = 0;
	//ͼ�����
	Mat colProjectionImg;
	Mat rowProjectionImg;
	Mat colPImgWithAverage;
	Mat rowPImgWithAverage;
	//�ж��Ƿ�Ϊ�����ü�ֵ��ͶӰ
	bool rowUseful = true;
	bool colUseful = true;

	vector<int> FindRowPeak(double value);
	vector<int> FindColPeak(double value);

	Projection(Mat img);
	Projection();
	~Projection();
};
