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
	ͶӰ();

	//������();

	//Myimwrite("���Կ���ͼ�� ", ���ԵĿ���ͼ��);
	////Myimwrite("����Kmeansͼ�� ", kmeansImg);
	//Myimwrite("������̬ѧ�ݶ�ͼ�� ", kmeansErodeImg);
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
	BCmodify(���ԵĿ���ͼ��, kmeansImg, 1.27, 0);
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
	���ԵĿ���λ�� = Rect(0, y1, colorfulImg.cols, y2 - y1);
	���ԵĿ���ͼ�� = Mat(colorfulImg, ���ԵĿ���λ��).clone();
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
	for (auto i : channalsProjectionBGR) {
		if (i.rowUseful) {
			Mat img = i.rowProjectionImg(Rect(0, 0, i.width - i.rowStatAverage - 10, i.height)).clone();
			//Mat img = i.rowPImgWithAverage(Rect(0, 0, i.width - i.rowStatAverage, i.height)).clone();
			//Mat img = i.rowProjectionImg.clone();
			//img = img(Rect(0, 0, height, i.width - i.rowStatAverage));
			//���ҡ���������
			bitwise_not(img, img);//�������ͼƬ��ת
			vector<vector<Point>> contours = vector<vector<Point>>();//��������
			vector<Vec4i> hierarchy = vector<Vec4i>();//�����νṹ
			findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);// Ѱ������
			cvtColor(img, img, COLOR_GRAY2RGB);
			Scalar color(255, 255, 255);
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {//��������
				drawContours(img, contours, index, color, FILLED, 8, hierarchy);
				Rect tempRect = boundingRect(contours[index]);
				if (tempRect.area() <= minArea) {//����С���
					minArea = tempRect.area();
				}
				int tempWeight = tempRect.area() / minArea;
				NumY eachRow = { tempWeight, tempRect.y, tempRect.height, tempRect.area() };
				EachY.push_back(eachRow);
				rectangle(img, tempRect, Scalar(0, 0, 255), 2);//���������Ӿ��ο�
				//rectangle(img, boundingRect(contours[index]), Scalar(0, 0, 255), 3);//�����������Ӿ��ο�
			}
			int maxWeight = -1;
			NumY result;
			for (auto i : EachY) {
				if (i.weight >= maxWeight) {
					result = i;
				}
			}
			//Mat resultImg = ���ԵĿ���ͼ��.clone();
			//rectangle(resultImg, Rect(5, result.y, resultImg.cols - 10, result.height)
			//	, Scalar(0, 0, 255), 3);//�Ӿ��ο�
			if (result.height < 15)
				continue;
			//Myimshow("ͶӰ��λ������", img);
			//Myimshow("��λ������ ", resultImg);
			//Myimwrite("ͶӰ��λ������", img);
			//Myimwrite("��λ������ ", resultImg);

			//�洢���
			//bankCardArea = maxRect;
			//bankCardAreaImage = Mat(colorfulImg, bankCardArea).clone();
		}
	}
}

void FindNumber::������ͨ��(Mat srcImg) {
	Mat img = kmeansErodeImg.clone();
	vector<Mat> channels;
	split(img, channels);//�����������ͨ��ͼ��
	for (int i = 0; i < 3; i++) {
		//Myimwrite("��ͨ�� ", channels[i]);
		channalsBGR[i] = channels[i];
	}
}

void FindNumber::ͶӰ() {
	for (int i = 0; i < 8; i++) {
		if (EachUseful) {
			eachColorP[i] = Projection(EachColor[i]);
		}
	}
	
	//Myimwrite("��ͨ��Bͼ�� ", channalsBGR[0]);
	//Myimwrite("��ͨ��Gͼ�� ", channalsBGR[1]);
	//Myimwrite("��ͨ��Rͼ�� ", channalsBGR[2]);
	//Myimwrite("��ͨ��BͶӰrow ", channalsProjectionBGR[0].rowPImgWithAverage);
	//Myimwrite("��ͨ��GͶӰrow ", channalsProjectionBGR[1].rowPImgWithAverage);
	//Myimwrite("��ͨ��RͶӰrow ", channalsProjectionBGR[2].rowPImgWithAverage);
	//Myimwrite("��ͨ��BͶӰcol ", channalsProjectionBGR[0].colPImgWithAverage);
	//Myimwrite("��ͨ��GͶӰcol ", channalsProjectionBGR[1].colPImgWithAverage);
	//Myimwrite("��ͨ��RͶӰcol ", channalsProjectionBGR[2].colPImgWithAverage);
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
	for (int i = 0; i < 8; i++) 
		if (colorStat[i] > 100) {
			cvtColor(EachColor[i], EachColor[i], COLOR_RGB2GRAY);
			threshold(EachColor[i], EachColor[i], 1, 255, THRESH_BINARY);
			EachUseful[i] = true;
		}
}


FindNumber::~FindNumber() {
}
