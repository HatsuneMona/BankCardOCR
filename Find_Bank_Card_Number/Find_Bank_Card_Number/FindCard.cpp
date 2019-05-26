#include "FindCard.h"
#include <vector>
using std::vector;

FindCard::FindCard(Mat img) {
	colorfulImg = img;
	LoadDealing();//预处理图片
	UseKmeans();//使用 K-means进行聚类

	//对聚类结果进行投影分析
	cardPj = Projection(kmeansErodeImg);
	if (cardPj.rowStat[0] == 0
		&& cardPj.rowStat[cardPj.height - 1] == 0
		&& cardPj.colStat[0] == 0
		&& cardPj.colStat[cardPj.width - 1] == 0) {
		UseFindContours();
		//Myimwrite("调整后的聚类轮廓", kmeansErodeImg);
	}
	else {
		FineTuning();
	}
	//根据聚类的结果进行裁剪图片
	bankCardAreaImage = Mat(colorfulImg, bankCardArea).clone();

	//Myimwrite("聚类梯度", kmeansErodeImg);
	//Myimwrite("横向投影", cardPj.rowPImgWithAverage);
	//Myimwrite("纵向投影", cardPj.colPImgWithAverage);
	
	//Myimwrite("银行卡图片聚类 ", kmeansImg);
	//Myimwrite("银行卡图片 ", bankCardAreaImage);

	//Myimshow("Kmeans聚类 ", kmeansImg);
	Number = FindNumber(bankCardAreaImage);//进行到下一步，找卡号。
}

void FindCard::LoadDealing() {
	resize(colorfulImg, colorfulImg, IMG_SIZE);//修改图像大小
	cvtColor(colorfulImg, grayImg, COLOR_RGB2GRAY);//创建灰度图
}

void FindCard::UseKmeans() {
	kmeansImg = colorfulImg.clone();
	Mat data, labels;
	BCmodify(kmeansImg, kmeansImg, 1.27, 0);//原始 1.27,0
	medianBlur(kmeansImg, kmeansImg, 5);
	//Myimwrite("银行卡处理 ", kmeansImg);

	for (int i = 0; i < kmeansImg.rows; i++)
		for (int j = 0; j < kmeansImg.cols; j++) {
			Vec3b point = kmeansImg.at<Vec3b>(i, j);
			Mat tmp = (Mat_<float>(1, 3) << point[0], point[1], point[2]);
			data.push_back(tmp);
		}

	//使用kmeans，2类
	kmeans(data, 2, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0),
		1, KMEANS_RANDOM_CENTERS);

	int n = 0;
	//显示聚类结果，不同的类别用不同的颜色显示
	for (int i = 0; i < kmeansImg.rows; i++)
		for (int j = 0; j < kmeansImg.cols; j++) {
			int clusterIdx = labels.at<int>(n);
			kmeansImg.at<Vec3b>(i, j) = kmeansColorTab[clusterIdx];
			n++;
		}
	//Myimwrite("银行卡聚类结果 ", kmeansImg);
	cvtColor(kmeansImg, kmeansImg, COLOR_RGB2GRAY);
	//形态学梯度，用于寻找轮廓
	morphologyEx(kmeansImg, kmeansErodeImg, MORPH_GRADIENT,
		getStructuringElement(MORPH_RECT, Size(10, 10)));
}

void FindCard::FineTuning() {

	auto colPeak = cardPj.colTanPeak(15.00, 3, 10);
	auto rowPeak = cardPj.rowTanPeak(20.00, 3, 10);

	for (x1 = 0; cardPj.rowStat[x1] < cardPj.rowStatAverage; ++x1) {
		line(kmeansErodeImg, Point(0, x1),
			Point(kmeansErodeImg.cols, x1), Scalar::all(0));
		if (x1 > rowPeak[0])break;
	}
	for (x2 = cardPj.height - 1; cardPj.rowStat[x2] < cardPj.rowStatAverage; --x2) {
		line(kmeansErodeImg, Point(0, x2),
			Point(kmeansErodeImg.cols, x2), Scalar::all(0));
		if (x2 < rowPeak[rowPeak.size() - 1])break;
	}
	for (y1 = 0; cardPj.colStat[y1] < cardPj.colStatAverage; ++y1) {
		line(kmeansErodeImg, Point(y1, 0),
			Point(y1, kmeansErodeImg.cols), Scalar::all(0));
		if (y1 > colPeak[0])break;
	}
	for (y2 = cardPj.width - 1; cardPj.colStat[y2] < cardPj.colStatAverage; --y2) {
		line(kmeansErodeImg, Point(y2, 0),
			Point(y2, kmeansErodeImg.cols), Scalar::all(0));
		if (y2 < colPeak[colPeak.size() - 1])break;
	}
	bankCardArea = Rect(y1, x1, y2 - y1, x2 - x1);
}

void FindCard::UseFindContours() {
	Mat img = kmeansErodeImg.clone();

	//if (img.channels() != 1)
	//	cvtColor(img, img, COLOR_RGB2GRAY);//将待检测图片转为灰度图

	morphologyEx(img, img, MORPH_DILATE,
		getStructuringElement(MORPH_RECT, Size(13, 13)));

	vector<vector<Point>> contours = vector<vector<Point>>();//定义轮廓
	vector<Vec4i> hierarchy = vector<Vec4i>();//定义层次结构
	findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);// 寻找轮廓

	//cvtColor(img, img, COLOR_GRAY2RGB);
	//Scalar color(255, 255, 255);

	Rect maxRect = Rect(0, 0, 100, 200);//定义最小轮廓
	for (int index = 0; index >= 0; index = hierarchy[index][0]) {//遍历轮廓
		//drawContours(img, contours, index, color, FILLED, 8, hierarchy);
		if (boundingRect(contours[index]).area() >= maxRect.area()) {//检测外轮廓
			maxRect = boundingRect(contours[index]);
		}
		//rectangle(img, boundingRect(contours[index]), Scalar(0, 0, 255), 3);//对所有轮廓加矩形框
	}

	//微调修正卡片位置，总共紧缩10px左右
	maxRect.x = maxRect.x + 7;
	maxRect.y = maxRect.y + 6;
	maxRect.width = maxRect.width - 16;
	maxRect.height = maxRect.height - 15;
	//rectangle(img, maxRect, Scalar(0, 0, 255), 3);//对外轮廓加矩形框
	//Myimshow("定位银行卡", img);

	//存储结果
	bankCardArea = maxRect;
}


FindCard::~FindCard() {
}
