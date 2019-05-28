#include "FindNumber.h"
#include<vector>
#include<cmath>
using std::abs;
using namespace std;

FindNumber::FindNumber() { }

FindNumber::FindNumber(Mat img) {

	colorfulImg = img;
	LoadDeal();
	盲猜缩小范围();
	//Myimwrite("分割用原图", roughlyNumImg);
	UseKmeans();
	GetEachColor();
	FindRowPeak();
	FindRow();
	CutRow();
	//找数字();

	//Myimwrite("粗略卡号图像 ", 粗略的卡号图像);
	////Myimwrite("卡号Kmeans图像 ", kmeansImg);
	//Myimwrite("卡号形态学梯度图像 ", kmeansErodeImg);
	//for (int i = 0; i < eachColorP.size(); i++) {
	//	Myimwrite("第" + to_string(i) + "张图片的原图", eachColorP[i].srcImg);
	//	Myimwrite("第" + to_string(i) + "张图片的row投影", eachColorP[i].rowPImgWithAverage);
	//	Myimwrite("第" + to_string(i) + "张图片的col投影", eachColorP[i].colPImgWithAverage);
	//}
	//vector<Projection> test;
	//for (auto i : eachColorP) {
	//	test.push_back(Projection(i.srcImg(preciseRowRect)));
	//}
	//for (int i = 0; i < test.size(); i++) {
	//	Myimwrite("第" + to_string(i) + "张图片的原图", test[i].srcImg);
	//	//Myimwrite("第" + to_string(i) + "张图片的row投影", test[i].rowPImgWithAverage);
	//	Myimwrite("第" + to_string(i) + "张图片的col投影", test[i].colPImgWithAverage);
	//}
	Myimwrite("精确卡号图像", preciseRowImg);
}

void FindNumber::LoadDeal() {
	resize(colorfulImg, colorfulImg, NUM_SIZE);//修改图像大小
	cvtColor(colorfulImg, grayImg, COLOR_RGB2GRAY);//创建灰度图
	//width = colorfulImg.cols;
	//height = colorfulImg.rows;
	//Myimwrite("标准大小的银行卡", colorfulImg);
}

void FindNumber::UseKmeans() {
	const int MAX_CLUSTERS = 5;

	Mat data, labels;
	BCmodify(roughlyNumImg, kmeansImg, 1.27, 0);
	medianBlur(kmeansImg, kmeansImg, 3);
	for (int i = 0; i < kmeansImg.rows; i++)
		for (int j = 0; j < kmeansImg.cols; j++) {
			Vec3b point = kmeansImg.at<Vec3b>(i, j);
			Mat tmp = (Mat_<float>(1, 3) << point[0], point[1], point[2]);
			data.push_back(tmp);
		}

	//根据浏览图片，确定k=3
	kmeans(data, 4, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0),
		1, KMEANS_PP_CENTERS);

	int n = 0;
	//显示聚类结果，不同的类别用不同的颜色显示
	for (int i = 0; i < kmeansImg.rows; i++)
		for (int j = 0; j < kmeansImg.cols; j++) {
			int clusterIdx = labels.at<int>(n);
			kmeansImg.at<Vec3b>(i, j) = kmeansColorTab[clusterIdx];
			n++;
		}

	morphologyEx(kmeansImg, kmeansErodeImg, MORPH_GRADIENT,
		getStructuringElement(MORPH_RECT, Size(3, 3)));//原来是4,4

}

void FindNumber::盲猜缩小范围() {
	int y1 = colorfulImg.rows * 0.4;
	int y2 = colorfulImg.rows * 0.7;
	roughlyNumRect = Rect(0, y1, colorfulImg.cols, y2 - y1);
	roughlyNumImg = Mat(colorfulImg, roughlyNumRect).clone();
	//Myimwrite("粗略的卡号位置", 粗略的卡号图像);
}

void FindNumber::找数字() {
	struct NumY {
		int weight;
		int y;
		int height;
		int area;
	};
	int minArea = 10000;
	vector<NumY> EachY;
}

Mat FindNumber::UseSobel(Mat srcImg, int xy) {

#pragma region Sobel算子使用方法
	/*
	1-2：输入输出Mat
	3：输出图像深度，应该是CV_16S，取CV_8U
	4：dx，x方向上的差分阶数
	5：dy，y方向上的差分阶数
	6：ksize，Sobel内核的大小，必须取1,3,5,7,
	7：scale，默认取 1
	8：可选的delta值，默认取 0
	9：borderType，边界模式，默认取BORDER_DEFAULT
	*/
	//在经过处理后，需要用convertScaleAbs()函数将其转回原来的uint8形式，否则将无法显示图像，而只是一副灰色的窗口
#pragma endregion
	
	Mat sobel_x, sobel_y;
	Mat sobelAbsX;
	Mat sobelAbsY;
	Mat sobelAbsXY;
	switch (xy) {
	case(0):
		Sobel(srcImg, sobel_x, CV_16S, 0, 1);
		convertScaleAbs(sobel_x, sobelAbsX);
		threshold(sobelAbsX, sobelAbsX, 80, 255, THRESH_BINARY);
		return sobelAbsX;
	case(1):
		Sobel(srcImg, sobel_y, CV_16S, 1, 0);
		convertScaleAbs(sobel_y, sobelAbsY);
		threshold(sobelAbsY, sobelAbsY, 80, 255, THRESH_BINARY);
		return sobelAbsY;
	case(2):
		Sobel(srcImg, sobel_x, CV_16S, 0, 1);
		convertScaleAbs(sobel_x, sobelAbsX);
		Sobel(srcImg, sobel_y, CV_16S, 1, 0);
		convertScaleAbs(sobel_y, sobelAbsY);
		addWeighted(sobelAbsX, 0.5, sobelAbsY, 0.5, 0, sobelAbsXY);
		threshold(sobelAbsXY, sobelAbsXY, 80, 255, THRESH_BINARY);
		return sobelAbsXY;
	default:
		cout << "xy Wrong!" << endl;
		return Mat();
	}

	//Myimwrite("sobelAbsX ", sobelAbsX);
	//Myimwrite("sobelAbsY ", sobelAbsY);
	//Myimwrite("sobelAbsXY ", sobelAbsXY);
}

void FindNumber::GetEachColor() {
	//初始化
	Mat EachColor[8];
	for (int i = 0; i < 8; i++)
		EachColor[i] = Mat(kmeansErodeImg.size(), CV_8UC3, Scalar(0, 0, 0));
	//进行筛选
	int colorStat[8] = { 0 };
	for(int row = 0; row<kmeansErodeImg.cols; row++)
		for (int col = 0; col < kmeansErodeImg.rows; col++) {
			int choose = 0;
			while (kmeansErodeImg.at<Vec3b>(col, row) != gradientColorTab[choose]
				&& choose < 8)choose++;
			if (choose > 7) {
				cout << "Choose Color Wrong! " << endl;
				continue;
			}
			else {
				EachColor[choose].at<Vec3b>(col, row) = gradientColorTab[choose];
				colorStat[choose]++;
			}
		}
	for (int i = 1; i < 8; i++) {
		cvtColor(EachColor[i], EachColor[i], COLOR_RGB2GRAY);
		//排除微小干扰
		morphologyEx(EachColor[i], EachColor[i], MORPH_GRADIENT,
			getStructuringElement(MORPH_RECT, Size(3, 3)));
		if (colorStat[i] > COLORSTAT_MIN) {
			threshold(EachColor[i], EachColor[i], 30, 255, THRESH_BINARY);
			auto temp = Projection(EachColor[i]);
			if (temp.statAll > COLORSTAT_MIN) {
				eachColorP.push_back(temp);
			}
		}
	}
}

FindNumber::~FindNumber() {
}

void FindNumber::FindRowPeak() {
	for (auto i : eachColorP) {
		vector<pt> rowPeakTemp = i.rowTanPeak(20, 3, 4);//5,3,15
		rowPeak.push_back(rowPeakTemp);
	}
}

void FindNumber::FindRow() {
	int fuzzyValue = 2;
	vector<Rect> foundRects;
	Rect realRect = Rect(0, 0, 1, 1);
	for (int PNo = 0; PNo < eachColorP.size(); PNo++) {
		auto pr = eachColorP[PNo];
		//Myimwrite("原始投影", pr.rowPImgWithAverage);

		auto tempRects = UseFindContours(pr.rowProjectionImg
			(Rect(0, 0, pr.width - pr.rowStatAverage + 3, pr.height)));
		//Myimwrite("原图 ", pr.srcImg);
		for (auto i : tempRects) {
			//double weight = 0.2 * pr.height / 2 - abs((i.y + i.height) / 2 - pr.height / 2);
			if (i.height >= 10 && i.height <= 30
				&& i.area() > realRect.area()){
				realRect = i;
			}
		}
	}
	y1 = realRect.y - 4;
	y2 = realRect.y + realRect.height + 8;
	if (y1 < 0)y1 = 0;
	if (y2 > roughlyNumImg.rows)y2 = roughlyNumImg.rows - 1;
}

vector<Rect> FindNumber::UseFindContours(Mat srcImg) {
	Mat img = srcImg.clone();
	Mat test;
	//消除细尖的影
	morphologyEx(img, test, MORPH_DILATE,
		getStructuringElement(MORPH_RECT, Size(5,5)));
	//Myimwrite("效果图", test);
	bitwise_not(img, img);

	vector<vector<Point>> contours = vector<vector<Point>>();//定义轮廓
	vector<Vec4i> hierarchy = vector<Vec4i>();//定义层次结构
	findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);// 寻找轮廓

	cvtColor(img, img, COLOR_GRAY2RGB);//如果需要可视化绘制轮廓则需要RGB
	Scalar color(255, 255, 255);

	vector<Rect> rects;//定义轮廓框
	if(contours.size()!=0)
		for (int index = 0; index >= 0; index = hierarchy[index][0]) {//遍历轮廓
			//drawContours(img, contours, index, color, FILLED, 8, hierarchy);
			Rect tempRect = boundingRect(contours[index]);
			if (tempRect.height >= 10 && tempRect.area() > 100) {//检测外轮廓
				rectangle(img, tempRect, Scalar(0, 0, 255), 3);//对所有轮廓加矩形框
				rects.push_back(tempRect);
			}
		}
	//Myimwrite("轮廓图 ", img);
	return rects;
}


void FindNumber::CutRow() {
	preciseRowRect = Rect(0, y1, roughlyNumImg.cols, y2 - y1);
	preciseRowImg = roughlyNumImg(preciseRowRect).clone();
}
