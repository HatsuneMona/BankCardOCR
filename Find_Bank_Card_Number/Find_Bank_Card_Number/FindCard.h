#pragma once
#include<opencv2/opencv.hpp>
#include"AllUse.h"
#include"FindNumber.h"
#include"Projection.h"
using namespace cv;

class FindCard {
private:
	void LoadDealing();
	void UseKmeans();
	void FineTuning();
	void UseFindContours();
	
public:
	Mat colorfulImg;
	Mat grayImg;
	Mat kmeansImg;
	Rect bankCardArea;
	Mat bankCardAreaImage;
	Mat kmeansErodeImg;
	FindNumber Number;

	Projection cardPj;
	int x1, x2, y1, y2;

	FindCard(Mat img);
	~FindCard();
};

