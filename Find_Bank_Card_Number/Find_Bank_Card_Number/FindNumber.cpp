#include "FindNumber.h"
#include<vector>
using namespace std;

FindNumber::FindNumber() { }

FindNumber::FindNumber(Mat img) {
	colorfulImg = img;
	LoadDeal();
	ä����С��Χ();
	UseKmeans();
	GetEachColor();
	FindRowPeak();
	CalculatRowSize();
	CutRow();
	//������();

	//Myimwrite("���Կ���ͼ�� ", ���ԵĿ���ͼ��);
	////Myimwrite("����Kmeansͼ�� ", kmeansImg);
	//Myimwrite("������̬ѧ�ݶ�ͼ�� ", kmeansErodeImg);

	Myimwrite("��ȷ����ͼ��", preciseRowImg);
}

void FindNumber::LoadDeal() {
	resize(colorfulImg, colorfulImg, NUM_SIZE);//�޸�ͼ���С
	cvtColor(colorfulImg, grayImg, COLOR_RGB2GRAY);//�����Ҷ�ͼ
	width = colorfulImg.cols;
	height = colorfulImg.rows;
	//Myimwrite("��׼��С�����п�", colorfulImg);
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

	//�������ͼƬ��ȷ��k=3
	kmeans(data, 4, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0),
		3, KMEANS_RANDOM_CENTERS);

	int n = 0;
	//��ʾ����������ͬ������ò�ͬ����ɫ��ʾ
	for (int i = 0; i < kmeansImg.rows; i++)
		for (int j = 0; j < kmeansImg.cols; j++) {
			int clusterIdx = labels.at<int>(n);
			kmeansImg.at<Vec3b>(i, j) = kmeansColorTab[clusterIdx];
			n++;
		}

	morphologyEx(kmeansImg, kmeansErodeImg, MORPH_GRADIENT,
		getStructuringElement(MORPH_RECT, Size(2, 2)));

	////Myimshow("Kmeans����", kmeansImg);
	//Myimwrite("Kmeans����", kmeansImg);
	//Myimwrite("��̬ѧ�ݶ�", kmeansErodeImg);
}

void FindNumber::ä����С��Χ() {
	int y1 = colorfulImg.rows * 0.4;
	int y2 = colorfulImg.rows * 0.7;
	roughlyNumRect = Rect(0, y1, colorfulImg.cols, y2 - y1);
	roughlyNumImg = Mat(colorfulImg, roughlyNumRect).clone();
	//Myimwrite("���ԵĿ���λ��", ���ԵĿ���ͼ��);
}

void FindNumber::������() {
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

#pragma region Sobel����ʹ�÷���
	/*
	1-2���������Mat
	3�����ͼ����ȣ�Ӧ����CV_16S��ȡCV_8U
	4��dx��x�����ϵĲ�ֽ���
	5��dy��y�����ϵĲ�ֽ���
	6��ksize��Sobel�ں˵Ĵ�С������ȡ1,3,5,7,
	7��scale��Ĭ��ȡ 1
	8����ѡ��deltaֵ��Ĭ��ȡ 0
	9��borderType���߽�ģʽ��Ĭ��ȡBORDER_DEFAULT
	*/
	//�ھ����������Ҫ��convertScaleAbs()��������ת��ԭ����uint8��ʽ�������޷���ʾͼ�񣬶�ֻ��һ����ɫ�Ĵ���
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
	//��ʼ��
	Mat EachColor[8];
	for (int i = 0; i < 8; i++)
		EachColor[i] = Mat(kmeansErodeImg.size(), CV_8UC3, Scalar(0, 0, 0));
	//����ɸѡ
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
	int rowHeightMaxValue = 25;//�涨��������Ŀ�
	int fuzzyValue = 5;//�涨ģ��ֵ
	struct Location {
		int rowStart;
		int rowEnd;
		int size;
	};
	vector<Location> hasFoundsL;
	int picNo = -1;
	for (auto pic : rowPeak) {//�������е�ͼƬ
		picNo++;
		int peakNo = -1;
		for (auto peak : pic) {//�������еķ�ֵ
			Location temp{ peak , 0 , 0 };
			peakNo++;
			int i = 0;
				if (peakNo != pic.size() - 1) {
					for (; peak + i < pic[peakNo + 1]; i++) { //����ƽ�����
						try {
							temp.size += eachColorP[picNo].rowStat[peak + i];
						}
						catch (...) {}
					}
					temp.rowEnd = pic[peakNo + 1];
				}
				else {
					for (; peak + i < height; i++) { //����ƽ�����
						try {
							temp.size += eachColorP[picNo].rowStat[peak + i];
						}
						catch (...) {}
					}
					temp.rowEnd = height;
				}
			temp.size = temp.size / i;
			if (picNo > 0) {//����ǵ�һ���Ժ��ͼƬ
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
