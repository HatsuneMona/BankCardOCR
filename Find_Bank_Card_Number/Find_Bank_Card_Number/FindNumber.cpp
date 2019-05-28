#include "FindNumber.h"
#include<vector>
#include<cmath>
using std::abs;
using namespace std;

FindNumber::FindNumber() { }

FindNumber::FindNumber(Mat img) {

	colorfulImg = img;
	LoadDeal();
	ä����С��Χ();
	//Myimwrite("�ָ���ԭͼ", roughlyNumImg);
	UseKmeans();
	GetEachColor();
	FindRowPeak();
	FindRow();
	CutRow();
	//������();

	//Myimwrite("���Կ���ͼ�� ", ���ԵĿ���ͼ��);
	////Myimwrite("����Kmeansͼ�� ", kmeansImg);
	//Myimwrite("������̬ѧ�ݶ�ͼ�� ", kmeansErodeImg);
	//for (int i = 0; i < eachColorP.size(); i++) {
	//	Myimwrite("��" + to_string(i) + "��ͼƬ��ԭͼ", eachColorP[i].srcImg);
	//	Myimwrite("��" + to_string(i) + "��ͼƬ��rowͶӰ", eachColorP[i].rowPImgWithAverage);
	//	Myimwrite("��" + to_string(i) + "��ͼƬ��colͶӰ", eachColorP[i].colPImgWithAverage);
	//}
	//vector<Projection> test;
	//for (auto i : eachColorP) {
	//	test.push_back(Projection(i.srcImg(preciseRowRect)));
	//}
	//for (int i = 0; i < test.size(); i++) {
	//	Myimwrite("��" + to_string(i) + "��ͼƬ��ԭͼ", test[i].srcImg);
	//	//Myimwrite("��" + to_string(i) + "��ͼƬ��rowͶӰ", test[i].rowPImgWithAverage);
	//	Myimwrite("��" + to_string(i) + "��ͼƬ��colͶӰ", test[i].colPImgWithAverage);
	//}
	Myimwrite("��ȷ����ͼ��", preciseRowImg);
}

void FindNumber::LoadDeal() {
	resize(colorfulImg, colorfulImg, NUM_SIZE);//�޸�ͼ���С
	cvtColor(colorfulImg, grayImg, COLOR_RGB2GRAY);//�����Ҷ�ͼ
	//width = colorfulImg.cols;
	//height = colorfulImg.rows;
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
		1, KMEANS_PP_CENTERS);

	int n = 0;
	//��ʾ����������ͬ������ò�ͬ����ɫ��ʾ
	for (int i = 0; i < kmeansImg.rows; i++)
		for (int j = 0; j < kmeansImg.cols; j++) {
			int clusterIdx = labels.at<int>(n);
			kmeansImg.at<Vec3b>(i, j) = kmeansColorTab[clusterIdx];
			n++;
		}

	morphologyEx(kmeansImg, kmeansErodeImg, MORPH_GRADIENT,
		getStructuringElement(MORPH_RECT, Size(3, 3)));//ԭ����4,4

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
	for (int i = 1; i < 8; i++) {
		cvtColor(EachColor[i], EachColor[i], COLOR_RGB2GRAY);
		//�ų�΢С����
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
		//Myimwrite("ԭʼͶӰ", pr.rowPImgWithAverage);

		auto tempRects = UseFindContours(pr.rowProjectionImg
			(Rect(0, 0, pr.width - pr.rowStatAverage + 3, pr.height)));
		//Myimwrite("ԭͼ ", pr.srcImg);
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
	//����ϸ���Ӱ
	morphologyEx(img, test, MORPH_DILATE,
		getStructuringElement(MORPH_RECT, Size(5,5)));
	//Myimwrite("Ч��ͼ", test);
	bitwise_not(img, img);

	vector<vector<Point>> contours = vector<vector<Point>>();//��������
	vector<Vec4i> hierarchy = vector<Vec4i>();//�����νṹ
	findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);// Ѱ������

	cvtColor(img, img, COLOR_GRAY2RGB);//�����Ҫ���ӻ�������������ҪRGB
	Scalar color(255, 255, 255);

	vector<Rect> rects;//����������
	if(contours.size()!=0)
		for (int index = 0; index >= 0; index = hierarchy[index][0]) {//��������
			//drawContours(img, contours, index, color, FILLED, 8, hierarchy);
			Rect tempRect = boundingRect(contours[index]);
			if (tempRect.height >= 10 && tempRect.area() > 100) {//���������
				rectangle(img, tempRect, Scalar(0, 0, 255), 3);//�����������Ӿ��ο�
				rects.push_back(tempRect);
			}
		}
	//Myimwrite("����ͼ ", img);
	return rects;
}


void FindNumber::CutRow() {
	preciseRowRect = Rect(0, y1, roughlyNumImg.cols, y2 - y1);
	preciseRowImg = roughlyNumImg(preciseRowRect).clone();
}
