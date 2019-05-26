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
	FindRowPeak();
	CalculatRowSize();
	CutRow();
	//找数字();

	//Myimwrite("粗略卡号图像 ", 粗略的卡号图像);
	////Myimwrite("卡号Kmeans图像 ", kmeansImg);
	//Myimwrite("卡号形态学梯度图像 ", kmeansErodeImg);

	Myimwrite("精确卡号图像", preciseRowImg);
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
	for (int i = 1; i < 8; i++) 
		if (colorStat[i] > COLORSTAT_MIN) {
			cvtColor(EachColor[i], EachColor[i], COLOR_RGB2GRAY);
			threshold(EachColor[i], EachColor[i], 1, 255, THRESH_BINARY);
			eachColorP.push_back(Projection(EachColor[i]));
		}
}


FindNumber::~FindNumber() {
}

void FindNumber::FindRowPeak() {
	for (auto i : eachColorP) {
		vector<int> rowPeakTemp = i.rowTanPeak(3.3, 3, 10);
		rowPeak.push_back(rowPeakTemp);
	}
	//for (int pic = 0; pic < srcNumP.size(); pic++) {
	//	vector<int> rowPeakTemp = srcNumP[pic].rowTanPeak(15, 3, 15);
	//	rowPeak.push_back(rowPeakTemp);
	//}
}

void FindNumber::CalculatRowSize() {
	int rowHeightMaxValue = 25;//规定数字区域的宽
	int fuzzyValue = 5;//规定模糊值
	struct Location {
		int rowStart;
		int rowEnd;
		int size;
	};
	vector<Location> hasFoundsL;
	int picNo = -1;
	for (auto pic : rowPeak) {//对于所有的图片
		picNo++;
		int peakNo = -1;
		for (auto peak : pic) {//对于所有的峰值
			Location temp{ peak , 0 , 0 };
			peakNo++;
			int i = 0;
				if (peakNo != pic.size() - 1) {
					for (; peak + i < pic[peakNo + 1]; i++) { //计算平均面积
						try {
							temp.size += eachColorP[picNo].rowStat[peak + i];
						}
						catch (...) {}
					}
					temp.rowEnd = pic[peakNo + 1];
				}
				else {
					for (; peak + i < height; i++) { //计算平均面积
						try {
							temp.size += eachColorP[picNo].rowStat[peak + i];
						}
						catch (...) {}
					}
					temp.rowEnd = height;
				}
			temp.size = temp.size / i;
			if (picNo > 0) {//如果是第一张以后的图片
				bool flag = false;
				for (auto &i : hasFoundsL) {
					if (i.rowStart - temp.rowStart <= fuzzyValue &&
						i.rowStart - temp.rowStart >= -fuzzyValue&&
						i.rowEnd - temp.rowEnd <= fuzzyValue &&
						i.rowEnd - temp.rowEnd >= -fuzzyValue) {
						flag = true;
						i.size += temp.size;
						if (temp.rowStart < i.rowStart) 
							i.rowStart = temp.rowStart;
						if (temp.rowEnd > i.rowEnd)
							i.rowEnd = temp.rowEnd;
					}
				}
				if (!flag) {
					hasFoundsL.push_back(temp);
				}
			}
			else {
				hasFoundsL.push_back(temp);
			}
		}
	}
	Location realL{ 0 , 0 , 0 };
	for (auto i : hasFoundsL) {
		if (i.size > realL.size) {
			realL = i;
		}
	}
	y1 = realL.rowStart - 7;
	y2 = realL.rowStart + rowHeightMaxValue + 8;
	if (y1 < 0)y1 = 0;
	if (y2 > roughlyNumImg.rows)y2 = roughlyNumImg.rows - 1;
	int i = 1;
}
void FindNumber::CutRow() {
	preciseRowRect = Rect(0, y1, width, y2 - y1);
	preciseRowImg = roughlyNumImg(preciseRowRect).clone();
}
//
//NumProjectionDeal::NumProjectionDeal() {
//}
//
//NumProjectionDeal::NumProjectionDeal(vector<Projection> src) {
//	eachColorP = src;
//	FindRowPeak();
//	CalculatRowSize();
//}
//
//NumProjectionDeal::~NumProjectionDeal() {
//}
