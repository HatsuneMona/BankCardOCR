#include "AllUse.h"
#include <vector>

void Myimshow(String name, Mat img) {
	name += std::to_string(rand());
	namedWindow(name, WINDOW_NORMAL);
	imshow(name, img);
}

void BCmodify(Mat srcImg, Mat &dstImg, double k, double b) {
	dstImg = srcImg.clone();
	if (dstImg.channels() == 1)//单通道，黑白
		for (int i = 0; i < dstImg.rows; i++)
			for (int j = 0; j < dstImg.cols; j++)
				dstImg.at<uchar>(i, j) = saturate_cast<uchar>(k * (srcImg.at<uchar>(i, j)) + b);
	else if (dstImg.channels() == 3)//多通道，彩色
		for (int i = 0; i < dstImg.rows; i++)
			for (int j = 0; j < dstImg.cols; j++) {
				dstImg.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(k * (srcImg.at<Vec3b>(i, j)[0]) + b);
				dstImg.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(k * (srcImg.at<Vec3b>(i, j)[1]) + b);
				dstImg.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(k * (srcImg.at<Vec3b>(i, j)[2]) + b);
			}
	else
		dstImg = Mat(Size(1, 1), CV_8U);
	#pragma region 对比度调整说明
		//(1). | k | > 1时
		//当k > 1时，可以用来增加图像的对比度，
		//(2). | k | = 1时
		//当k = 1时，这种情况下常用来调节图像的亮度，
		//(3).0 < | k | < 1时
		//此时变换的效果正好与k>1时相反，即图像的整体对比度和效果都被削减了
		//(4).k < 0时
		//在这种情况下，源图像的灰度会发生反转，也就是原图像中较亮的区域会变暗，而较暗的区域将会变量。
		//特别的，此时我们令k = -1, d = 255, 可以令图像实现完全反转的效果。
	#pragma endregion
}

void Myimwrite(String name, Mat img) {
	String fileName = tempOutPath + "//" + name + std::to_string(rand()) + ".jpg";
	imwrite(fileName, img);
}

void autoCmodify(Mat srcImg, Mat& dstImg) {
	//直方图增强对比度
	dstImg = srcImg.clone();
	std::vector<Mat> splitBGR(srcImg.channels());
	//分割通道，存储到splitBGR中
	split(srcImg, splitBGR);
	//对各个通道分别进行直方图均衡化
	for (int i = 0; i < dstImg.channels(); i++)
		equalizeHist(splitBGR[i], splitBGR[i]);
	//合并通道
	merge(splitBGR, dstImg);
	//Myimshow("直方图增强对比度 ", dstImg);
}

void autoBmodify(Mat srcImg, Mat& dstImg) {
	cvtColor(srcImg, dstImg, COLOR_BGR2YCrCb);
	// 分离通道
	std::vector<Mat> channels;
	split(dstImg, channels);
	// 分离Y通道
	equalizeHist(channels[0], channels[0]);
	//// 用直方图均衡化Y通道
	merge(channels, dstImg);
	// 转换回BGR图像模式
	cvtColor(dstImg, dstImg, COLOR_YCrCb2BGR);
}
