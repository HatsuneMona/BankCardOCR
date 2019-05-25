#include "FindNumber.h"
#include<vector>
using namespace std;

FindNumber::FindNumber() { }

FindNumber::FindNumber(Mat img) {
	colorfulImg = img;
	LoadDeal();
	盲猜缩小范围();
	UseKmeans();
	GetEachColor();
	投影();

	//找数字();

	//Myimwrite("粗略卡号图像 ", 粗略的卡号图像);
	////Myimwrite("卡号Kmeans图像 ", kmeansImg);
	//Myimwrite("卡号形态学梯度图像 ", kmeansErodeImg);
}

void FindNumber::LoadDeal() {
	resize(colorfulImg, colorfulImg, NUM_SIZE);//修改图像大小
	cvtColor(colorfulImg, grayImg, COLOR_RGB2GRAY);//创建灰度图
	width = colorfulImg.cols;
	height = colorfulImg.rows;
	//Myimwrite("标准大小的银行卡", colorfulImg);
}

void FindNumber::UseKmeans() {
	const int MAX_CLUSTERS = 5;

	Mat data, labels;
	BCmodify(粗略的卡号图像, kmeansImg, 1.27, 0);
	medianBlur(kmeansImg, kmeansImg, 3);
	for (int i = 0; i < kmeansImg.rows; i++)
		for (int j = 0; j < kmeansImg.cols; j++) {
			Vec3b point = kmeansImg.at<Vec3b>(i, j);
			Mat tmp = (Mat_<float>(1, 3) << point[0], point[1], point[2]);
			data.push_back(tmp);
		}

	//根据浏览图片，确定k=3
	kmeans(data, 4, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0),
		3, KMEANS_RANDOM_CENTERS);

	int n = 0;
	//显示聚类结果，不同的类别用不同的颜色显示
	for (int i = 0; i < kmeansImg.rows; i++)
		for (int j = 0; j < kmeansImg.cols; j++) {
			int clusterIdx = labels.at<int>(n);
			kmeansImg.at<Vec3b>(i, j) = kmeansColorTab[clusterIdx];
			n++;
		}

	morphologyEx(kmeansImg, kmeansErodeImg, MORPH_GRADIENT,
		getStructuringElement(MORPH_RECT, Size(2, 2)));

	////Myimshow("Kmeans聚类", kmeansImg);
	//Myimwrite("Kmeans聚类", kmeansImg);
	//Myimwrite("形态学梯度", kmeansErodeImg);
}

void FindNumber::盲猜缩小范围() {
	int y1 = colorfulImg.rows * 0.4;
	int y2 = colorfulImg.rows * 0.7;
	粗略的卡号位置 = Rect(0, y1, colorfulImg.cols, y2 - y1);
	粗略的卡号图像 = Mat(colorfulImg, 粗略的卡号位置).clone();
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
	for (auto i : channalsProjectionBGR) {
		if (i.rowUseful) {
			Mat img = i.rowProjectionImg(Rect(0, 0, i.width - i.rowStatAverage - 10, i.height)).clone();
			//Mat img = i.rowPImgWithAverage(Rect(0, 0, i.width - i.rowStatAverage, i.height)).clone();
			//Mat img = i.rowProjectionImg.clone();
			//img = img(Rect(0, 0, height, i.width - i.rowStatAverage));
			//查找、绘制轮廓
			bitwise_not(img, img);//将待检测图片反转
			vector<vector<Point>> contours = vector<vector<Point>>();//定义轮廓
			vector<Vec4i> hierarchy = vector<Vec4i>();//定义层次结构
			findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);// 寻找轮廓
			cvtColor(img, img, COLOR_GRAY2RGB);
			Scalar color(255, 255, 255);
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {//绘制轮廓
				drawContours(img, contours, index, color, FILLED, 8, hierarchy);
				Rect tempRect = boundingRect(contours[index]);
				if (tempRect.area() <= minArea) {//绑定最小面积
					minArea = tempRect.area();
				}
				int tempWeight = tempRect.area() / minArea;
				NumY eachRow = { tempWeight, tempRect.y, tempRect.height, tempRect.area() };
				EachY.push_back(eachRow);
				rectangle(img, tempRect, Scalar(0, 0, 255), 2);//对外轮廓加矩形框
				//rectangle(img, boundingRect(contours[index]), Scalar(0, 0, 255), 3);//对所有轮廓加矩形框
			}
			int maxWeight = -1;
			NumY result;
			for (auto i : EachY) {
				if (i.weight >= maxWeight) {
					result = i;
				}
			}
			//Mat resultImg = 粗略的卡号图像.clone();
			//rectangle(resultImg, Rect(5, result.y, resultImg.cols - 10, result.height)
			//	, Scalar(0, 0, 255), 3);//加矩形框
			if (result.height < 15)
				continue;
			//Myimshow("投影定位数字行", img);
			//Myimshow("定位数字行 ", resultImg);
			//Myimwrite("投影定位数字行", img);
			//Myimwrite("定位数字行 ", resultImg);

			//存储结果
			//bankCardArea = maxRect;
			//bankCardAreaImage = Mat(colorfulImg, bankCardArea).clone();
		}
	}
}

void FindNumber::分离三通道(Mat srcImg) {
	Mat img = kmeansErodeImg.clone();
	vector<Mat> channels;
	split(img, channels);//分离成三个单通道图像
	for (int i = 0; i < 3; i++) {
		//Myimwrite("单通道 ", channels[i]);
		channalsBGR[i] = channels[i];
	}
}

void FindNumber::投影() {
	for (int i = 0; i < 8; i++) {
		if (EachUseful) {
			eachColorP[i] = Projection(EachColor[i]);
		}
	}
	
	//Myimwrite("单通道B图像 ", channalsBGR[0]);
	//Myimwrite("单通道G图像 ", channalsBGR[1]);
	//Myimwrite("单通道R图像 ", channalsBGR[2]);
	//Myimwrite("单通道B投影row ", channalsProjectionBGR[0].rowPImgWithAverage);
	//Myimwrite("单通道G投影row ", channalsProjectionBGR[1].rowPImgWithAverage);
	//Myimwrite("单通道R投影row ", channalsProjectionBGR[2].rowPImgWithAverage);
	//Myimwrite("单通道B投影col ", channalsProjectionBGR[0].colPImgWithAverage);
	//Myimwrite("单通道G投影col ", channalsProjectionBGR[1].colPImgWithAverage);
	//Myimwrite("单通道R投影col ", channalsProjectionBGR[2].colPImgWithAverage);
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
	for (int i = 0; i < 8; i++) 
		if (colorStat[i] > 100) {
			cvtColor(EachColor[i], EachColor[i], COLOR_RGB2GRAY);
			threshold(EachColor[i], EachColor[i], 1, 255, THRESH_BINARY);
			EachUseful[i] = true;
		}
}


FindNumber::~FindNumber() {
}
