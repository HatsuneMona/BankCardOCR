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

	//��ʼ��ͶӰͼ
	colProjectionImg = Mat(srcImg.size(), CV_8U, Scalar(255));
	rowProjectionImg = Mat(srcImg.size(), CV_8U, Scalar(255));

	//���㣨ͳ�ƣ�ͶӰ��
	for (int row = 0; row < width; row++)
		for (int col = 0; col < height; col++)
			if (srcImg.at<uchar>(col, row) == 255) {
				rowStat[col]++;
				colStat[row]++;
				statAverageTemp++;
			}

	//����ͶӰƽ��ֵ
	colStatAverage = statAverageTemp / width;
	rowStatAverage = statAverageTemp / height;

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

void Projection::UsefulJudge() {
	std::cout << std::endl;
	std::cout << "�� ƽ����ռ��:" << 1.000 * colStatAverage / width << std::endl;
	std::cout << "�� ���ֵռ��:" << 1.000 * rowMax / width << std::endl;
	if (colStatAverage < width * 0.04 && rowMax < width * 0.6) {
		std::cout << "�� ƽ���߹��ͣ������ֵС��40%" << std::endl;
		rowUseful = false;
	}
}

vector<int> Projection::rowTanPeak(double tanMinValue, int precisionValue, int passValue) {
	vector<int> results;
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
			results.push_back(i + 2);
			i = i + passValue;
		}
	}
	return results;
}

vector<int> Projection::colTanPeak(double tanMinValue, int precisionValue, int passValue) {
	vector<int> results;
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
			results.push_back(i + 2);
			i = i + passValue;
		}
	}
	return results;
}
