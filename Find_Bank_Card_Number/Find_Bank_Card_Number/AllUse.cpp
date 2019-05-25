#include "AllUse.h"
#include <vector>

void Myimshow(String name, Mat img) {
	name += std::to_string(rand());
	namedWindow(name, WINDOW_NORMAL);
	imshow(name, img);
}

void BCmodify(Mat srcImg, Mat &dstImg, double k, double b) {
	dstImg = srcImg.clone();
	if (dstImg.channels() == 1)//��ͨ�����ڰ�
		for (int i = 0; i < dstImg.rows; i++)
			for (int j = 0; j < dstImg.cols; j++)
				dstImg.at<uchar>(i, j) = saturate_cast<uchar>(k * (srcImg.at<uchar>(i, j)) + b);
	else if (dstImg.channels() == 3)//��ͨ������ɫ
		for (int i = 0; i < dstImg.rows; i++)
			for (int j = 0; j < dstImg.cols; j++) {
				dstImg.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(k * (srcImg.at<Vec3b>(i, j)[0]) + b);
				dstImg.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(k * (srcImg.at<Vec3b>(i, j)[1]) + b);
				dstImg.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(k * (srcImg.at<Vec3b>(i, j)[2]) + b);
			}
	else
		dstImg = Mat(Size(1, 1), CV_8U);
	#pragma region �Աȶȵ���˵��
		//(1). | k | > 1ʱ
		//��k > 1ʱ��������������ͼ��ĶԱȶȣ�
		//(2). | k | = 1ʱ
		//��k = 1ʱ����������³���������ͼ������ȣ�
		//(3).0 < | k | < 1ʱ
		//��ʱ�任��Ч��������k>1ʱ�෴����ͼ�������ԱȶȺ�Ч������������
		//(4).k < 0ʱ
		//����������£�Դͼ��ĻҶȻᷢ����ת��Ҳ����ԭͼ���н����������䰵�����ϰ������򽫻������
		//�ر�ģ���ʱ������k = -1, d = 255, ������ͼ��ʵ����ȫ��ת��Ч����
	#pragma endregion
}

void Myimwrite(String name, Mat img) {
	String fileName = tempOutPath + "//" + name + std::to_string(rand()) + ".jpg";
	imwrite(fileName, img);
}

void autoCmodify(Mat srcImg, Mat& dstImg) {
	//ֱ��ͼ��ǿ�Աȶ�
	dstImg = srcImg.clone();
	std::vector<Mat> splitBGR(srcImg.channels());
	//�ָ�ͨ�����洢��splitBGR��
	split(srcImg, splitBGR);
	//�Ը���ͨ���ֱ����ֱ��ͼ���⻯
	for (int i = 0; i < dstImg.channels(); i++)
		equalizeHist(splitBGR[i], splitBGR[i]);
	//�ϲ�ͨ��
	merge(splitBGR, dstImg);
	//Myimshow("ֱ��ͼ��ǿ�Աȶ� ", dstImg);
}

void autoBmodify(Mat srcImg, Mat& dstImg) {
	cvtColor(srcImg, dstImg, COLOR_BGR2YCrCb);
	// ����ͨ��
	std::vector<Mat> channels;
	split(dstImg, channels);
	// ����Yͨ��
	equalizeHist(channels[0], channels[0]);
	//// ��ֱ��ͼ���⻯Yͨ��
	merge(channels, dstImg);
	// ת����BGRͼ��ģʽ
	cvtColor(dstImg, dstImg, COLOR_YCrCb2BGR);
}
