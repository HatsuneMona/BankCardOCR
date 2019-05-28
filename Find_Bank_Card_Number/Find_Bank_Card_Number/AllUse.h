#pragma once
#include<opencv2/opencv.hpp>
using namespace cv;
//ԭͼƬ��С��һ����ĳ���
#define IMG_WIDTH     400
#define IMG_HEIGHT    300
#define IMG_SIZE      Size(IMG_WIDTH, IMG_HEIGHT)

#define NUM_WIDTH     300
#define NUM_HEIGHT    200
#define NUM_SIZE      Size(NUM_WIDTH, NUM_HEIGHT)
////ԭͼƬ��С��һ����ĳ���
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

//��ʾ���ڣ��������֣�Ҫ��ʾ��ͼƬ
void Myimshow(String name, Mat img);

//�޸�ͼ��Աȶȣ�����
void BCmodify(Mat srcimg, Mat &dstImg, double k, double b);

//���ͼ���ļ����������֣�Ҫ�����ͼƬ
void Myimwrite(String name, Mat img);

//����ֱ��ͼ��̬�����Աȶ�
void autoCmodify(Mat srcImg, Mat& dstImg);

//����ֱ��ͼ��̬��������
void autoBmodify(Mat srcImg, Mat& dstImg);