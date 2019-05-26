#include "FindCard.h"
#include <vector>
using std::vector;

FindCard::FindCard(Mat img) {
	colorfulImg = img;
	LoadDealing();//Ԥ����ͼƬ
	UseKmeans();//ʹ�� K-means���о���

	//�Ծ���������ͶӰ����
	cardPj = Projection(kmeansErodeImg);
	if (cardPj.rowStat[0] == 0
		&& cardPj.rowStat[cardPj.height - 1] == 0
		&& cardPj.colStat[0] == 0
		&& cardPj.colStat[cardPj.width - 1] == 0) {
		UseFindContours();
		//Myimwrite("������ľ�������", kmeansErodeImg);
	}
	else {
		FineTuning();
	}
	//���ݾ���Ľ�����вü�ͼƬ
	bankCardAreaImage = Mat(colorfulImg, bankCardArea).clone();

	//Myimwrite("�����ݶ�", kmeansErodeImg);
	//Myimwrite("����ͶӰ", cardPj.rowPImgWithAverage);
	//Myimwrite("����ͶӰ", cardPj.colPImgWithAverage);
	
	//Myimwrite("���п�ͼƬ���� ", kmeansImg);
	//Myimwrite("���п�ͼƬ ", bankCardAreaImage);

	//Myimshow("Kmeans���� ", kmeansImg);
	Number = FindNumber(bankCardAreaImage);//���е���һ�����ҿ��š�
}

void FindCard::LoadDealing() {
	resize(colorfulImg, colorfulImg, IMG_SIZE);//�޸�ͼ���С
	cvtColor(colorfulImg, grayImg, COLOR_RGB2GRAY);//�����Ҷ�ͼ
}

void FindCard::UseKmeans() {
	kmeansImg = colorfulImg.clone();
	Mat data, labels;
	BCmodify(kmeansImg, kmeansImg, 1.27, 0);//ԭʼ 1.27,0
	medianBlur(kmeansImg, kmeansImg, 5);
	//Myimwrite("���п����� ", kmeansImg);

	for (int i = 0; i < kmeansImg.rows; i++)
		for (int j = 0; j < kmeansImg.cols; j++) {
			Vec3b point = kmeansImg.at<Vec3b>(i, j);
			Mat tmp = (Mat_<float>(1, 3) << point[0], point[1], point[2]);
			data.push_back(tmp);
		}

	//ʹ��kmeans��2��
	kmeans(data, 2, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0),
		1, KMEANS_RANDOM_CENTERS);

	int n = 0;
	//��ʾ����������ͬ������ò�ͬ����ɫ��ʾ
	for (int i = 0; i < kmeansImg.rows; i++)
		for (int j = 0; j < kmeansImg.cols; j++) {
			int clusterIdx = labels.at<int>(n);
			kmeansImg.at<Vec3b>(i, j) = kmeansColorTab[clusterIdx];
			n++;
		}
	//Myimwrite("���п������� ", kmeansImg);
	cvtColor(kmeansImg, kmeansImg, COLOR_RGB2GRAY);
	//��̬ѧ�ݶȣ�����Ѱ������
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
	//	cvtColor(img, img, COLOR_RGB2GRAY);//�������ͼƬתΪ�Ҷ�ͼ

	morphologyEx(img, img, MORPH_DILATE,
		getStructuringElement(MORPH_RECT, Size(13, 13)));

	vector<vector<Point>> contours = vector<vector<Point>>();//��������
	vector<Vec4i> hierarchy = vector<Vec4i>();//�����νṹ
	findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);// Ѱ������

	//cvtColor(img, img, COLOR_GRAY2RGB);
	//Scalar color(255, 255, 255);

	Rect maxRect = Rect(0, 0, 100, 200);//������С����
	for (int index = 0; index >= 0; index = hierarchy[index][0]) {//��������
		//drawContours(img, contours, index, color, FILLED, 8, hierarchy);
		if (boundingRect(contours[index]).area() >= maxRect.area()) {//���������
			maxRect = boundingRect(contours[index]);
		}
		//rectangle(img, boundingRect(contours[index]), Scalar(0, 0, 255), 3);//�����������Ӿ��ο�
	}

	//΢��������Ƭλ�ã��ܹ�����10px����
	maxRect.x = maxRect.x + 7;
	maxRect.y = maxRect.y + 6;
	maxRect.width = maxRect.width - 16;
	maxRect.height = maxRect.height - 15;
	//rectangle(img, maxRect, Scalar(0, 0, 255), 3);//���������Ӿ��ο�
	//Myimshow("��λ���п�", img);

	//�洢���
	bankCardArea = maxRect;
}


FindCard::~FindCard() {
}
