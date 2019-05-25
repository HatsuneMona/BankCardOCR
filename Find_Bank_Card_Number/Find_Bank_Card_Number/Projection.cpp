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

	//Myimwrite("水平投影", rowProjectionImg);
	//Myimwrite("水平投影带平均值 ", rowPImgWithAverage);
	//std::cout << rowStatAverage << std::endl;
}

Projection::Projection() {
}

Projection::~Projection() {
}

void Projection::ProjectionMain() {
	//height = (int)(0.3 * IMG_HEIGHT);
	//width = IMG_WIDTH;

	//初始化投影图
	colProjectionImg = Mat(srcImg.size(), CV_8U, Scalar(255));
	rowProjectionImg = Mat(srcImg.size(), CV_8U, Scalar(255));

	//计算（统计）投影量
	for (int row = 0; row < width; row++)
		for (int col = 0; col < height; col++)
			if (srcImg.at<uchar>(col, row) == 255) {
				rowStat[col]++;
				colStat[row]++;
				statAverageTemp++;
			}

	//计算投影平均值
	colStatAverage = statAverageTemp / width;
	rowStatAverage = statAverageTemp / height;

	//绘制可视化投影图、统计最大值
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

	//补充绘制可视化平均值投影图
	cvtColor(rowProjectionImg, rowPImgWithAverage, COLOR_GRAY2RGB);
	cvtColor(colProjectionImg, colPImgWithAverage, COLOR_GRAY2RGB);
	line(rowPImgWithAverage, Point(width - rowStatAverage, 0), 
		Point(width - rowStatAverage, height), Scalar(0, 0, 255));
	line(colPImgWithAverage, Point(0, height - colStatAverage), 
		Point(width, height - colStatAverage), Scalar(0, 0, 255));
}

void Projection::UsefulJudge() {
	std::cout << std::endl;
	std::cout << "行 平均线占比:" << 1.000 * colStatAverage / width << std::endl;
	std::cout << "行 最大值占比:" << 1.000 * rowMax / width << std::endl;
	if (colStatAverage < width * 0.04 && rowMax < width * 0.6) {
		std::cout << "行 平均线过低，且最高值小于40%" << std::endl;
		rowUseful = false;
	}
}

vector<int> Projection::FindRowPeak(double value) {
	vector<int> results;
	int precision = 3;//设置精度
	int truePass = 10;//找到后跳过多少数据
	for (int i = 0; i < height - precision; i = i + precision) {
		double tan = 1.000 * (rowStat[i] - rowStat[i + precision]) / precision;
		if (tan < 0) tan = -tan;
		if (tan > value) {
			results.push_back(i + 2);
			i = i + truePass;
		}
	}
	return results;
}

vector<int> Projection::FindColPeak(double value) {
	vector<int> results;
	int precision = 3;//设置精度
	int truePass = 10;//找到后跳过多少数据
	for (int i = 0; i < width - precision; i = i + precision) {
		double tan = 1.000 * (colStat[i] - colStat[i + precision]) / precision;
		if (tan < 0) tan = -tan;
		if (tan > value) {
			results.push_back(i + 2);
			i = i + truePass;
		}
	}
	return results;
}
