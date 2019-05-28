#include "Projection.h"

Projection::Projection(Mat img) {
	if (img.channels() != 1) 
		cvtColor(img, srcImg, COLOR_RGB2GRAY);
	else
		srcImg = img.clone();
	height = srcImg.rows;
	width = srcImg.cols;
	ProjectionMain();

	//UsefulJudge();

	//Myimwrite("ˮƽͶӰ", rowProjectionImg);
	//Myimwrite("ˮƽͶӰ��ƽ��ֵ ", rowPImgWithAverage);
	//std::cout << rowStatAverage << std::endl;
}

Projection::Projection() {
}

Projection::~Projection() {
}

void Projection::ProjectionMain() {
	//height = (int)(0.3 * IMG_HEIGHT);
	//width = IMG_WIDTH;

	//���㣨ͳ�ƣ�ͶӰ��
	for (int row = 0; row < width; row++)
		for (int col = 0; col < height; col++)
			if (srcImg.at<uchar>(col, row) == 255) {
				rowStat[col]++;
				colStat[row]++;
				statAll++;
			}

	//����ͶӰƽ��ֵ
	colStatAverage = statAll / width;
	rowStatAverage = statAll / height;

	//���ƿ��ӻ�ͶӰ
	DrawProjection();
}

void Projection::UsefulJudge() {
	std::cout << std::endl;
	std::cout << "�� ƽ����ռ��:" << 1.000 * colStatAverage / width << std::endl;
	std::cout << "�� ���ֵռ��:" << 1.000 * rowMax / width << std::endl;
	if (colStatAverage < width * 0.04 && rowMax < width * 0.6) {
		std::cout << "�� ƽ���߹��ͣ������ֵС��40%" << std::endl;
		rowUseful = false;
	}
}

vector<pt> Projection::rowTanPeak(double tanMinValue, int precisionValue, int passValue) {
	vector<pt> results;
	//int precisionValue = 3;//���þ���
	//int passValue = 10;//�ҵ���������������
	for (int i = 0; i < height - precisionValue; i = i + precisionValue) {
		int max = 0, min = 10000;
		for (int j = i; j < i + 3; j++) {
			if (rowStat[j] > max) max = rowStat[j];
			if (rowStat[j] < min) min = rowStat[j];
		}
		double tan = 1.000 * (max - min) / precisionValue;
		if (tan < 0) tan = -tan;//ȡ����ֵ
		if (tan > tanMinValue) {
			results.push_back({ max,tan });
			i = i + passValue;
		}
	}
	return results;
}

vector<pt> Projection::colTanPeak(double tanMinValue, int precisionValue, int passValue) {
	vector<pt> results;
	//int precisionValue = 3;//���þ���
	//int truePass = 10;//�ҵ���������������
	for (int i = 0; i < width - precisionValue; i = i + precisionValue) {
		int max = 0, min = 10000;
		for (int j = i; j < i + 3; j++) {
			if (rowStat[j] > max) max = rowStat[j];
			if (rowStat[j] < min) min = rowStat[j];
		}
		double tan = 1.000 * (max - min) / precisionValue;
		if (tan < 0) tan = -tan;//ȡ����ֵ
		if (tan > tanMinValue) {
			results.push_back({ max,tan });
			i = i + passValue;
		}
	}
	return results;
}

void Projection::DrawProjection() {
	//��ʼ��ͼ��
	colProjectionImg = Mat(srcImg.size(), CV_8U, Scalar(255));
	rowProjectionImg = Mat(srcImg.size(), CV_8U, Scalar(255));
	//���ƿ��ӻ�ͶӰͼ��ͳ�����ֵ
	for (int row = 0; row < height; ++row) {
		if (rowStat[row] > rowMax)
			rowMax = rowStat[row];
		line(rowProjectionImg, Point(width - rowStat[row], row),
			Point(width - 1, row), Scalar::all(0));
	}
	for (int col = 0; col < width; col++) {
		if (colStat[col] > colMax)
			colMax = colStat[col];
		line(colProjectionImg, Point(col, height - colStat[col]),
			Point(col, height - 1), Scalar::all(0));
	}

	//������ƿ��ӻ�ƽ��ֵͶӰͼ
	cvtColor(rowProjectionImg, rowPImgWithAverage, COLOR_GRAY2RGB);
	cvtColor(colProjectionImg, colPImgWithAverage, COLOR_GRAY2RGB);
	line(rowPImgWithAverage, Point(width - rowStatAverage, 0),
		Point(width - rowStatAverage, height), Scalar(0, 0, 255));
	line(colPImgWithAverage, Point(0, height - colStatAverage),
		Point(width, height - colStatAverage), Scalar(0, 0, 255));
}
//
//void Projection::CutTip(int rowValue = 0, int colValue = 0) {
//	int choose = 0;
//	if (rowValue)choose += 1;
//	if (colValue)choose += 2;
//	int howlong = 0;
//	int howmany = 0;
//	switch (choose) {
//	case(3)://������ȥ��
//	case(1)://rowȥ��
//		for (int i = width - rowMax; i < width; i++) {
//			int& row = rowStat[i];
//			if (row >= howlong) {
//				howmany += howlong;
//				row = row - howlong;
//			}
//				
//		}
//		if (choose != 3)break;
//	case(2)://colȥ��
//		howlong = 0;
//		for (int i = 0; i < height; i++) {
//			int& row = rowStat[i];
//			if (row > 0 && row <= rowValue) {
//				howlong = width - i;
//				row = 0;
//			}
//		}
//		for (int i = 0; i < width; i++) {
//			int& col = colStat[i];
//			if (col >= howlong) {
//				howmany += howlong;
//				col = col - howlong;
//			}
//				
//		}
//		break;
//	default:
//		cout << "Projection::CutTip Do Nothing" << endl;
//		break;
//	}
//	statAll -= howmany;
//	//����ͶӰƽ��ֵ
//	colStatAverage = statAll / width;
//	rowStatAverage = statAll / height;
//	DrawProjection();
//}
