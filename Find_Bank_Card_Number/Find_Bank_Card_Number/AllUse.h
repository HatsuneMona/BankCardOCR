#pragma once
#include<opencv2/opencv.hpp>
using namespace cv;
//原图片大小归一化后的长宽
#define IMG_WIDTH     400
#define IMG_HEIGHT    300
#define IMG_SIZE      Size(IMG_WIDTH, IMG_HEIGHT)

#define NUM_WIDTH     300
#define NUM_HEIGHT    200
#define NUM_SIZE      Size(NUM_WIDTH, NUM_HEIGHT)
////原图片大小归一化后的长宽
//#define IMG_WIDTH     640
//#define IMG_HEIGHT    480
//#define IMG_SIZE      Size(IMG_WIDTH, IMG_HEIGHT)
struct pt {
	int location;
	double tan;
};

extern Vec3b kmeansColorTab[];
extern Vec3b gradientColorTab[8];
extern String tempOutPath;

//显示窗口，窗口名字，要显示的图片
void Myimshow(String name, Mat img);

//修改图像对比度，亮度
void BCmodify(Mat srcimg, Mat &dstImg, double k, double b);

//输出图像文件，窗口名字，要输出的图片
void Myimwrite(String name, Mat img);

//根据直方图动态调整对比度
void autoCmodify(Mat srcImg, Mat& dstImg);

//根据直方图动态调整亮度
void autoBmodify(Mat srcImg, Mat& dstImg);