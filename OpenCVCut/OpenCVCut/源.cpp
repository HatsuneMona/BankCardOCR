#include<iostream>
#include<fstream>
#include<io.h>
#include<Windows.h>
#include<ctime>

#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

//原图片大小归一化后的长宽
//小图
#define IMG_WIDTH     120
#define IMG_HEIGHT    46
#define IMG_SIZE      Size(IMG_WIDTH, IMG_HEIGHT)

//默认创建的窗口名字
#define WINDOW_NAME "喵"

//全局变量
Mat image;
String imagePath;
int numStat[100] = { 0 };
String dirNameTab[3];
String dirName;

//函数声明
void WriteImg(Mat img, char number);
Mat Enhance_GaryLin(Mat img, double k, double b);
void getFiles(string path, vector<string>& files);
void cut(Mat img);

int main(int argc, char* argv[]) {
	srand(time(0));
	for (int i = 0; i < 3; i++) {
		dirNameTab[i] = "";
		dirNameTab[i] += "E:\\局域网共享\\CutFile";
		dirNameTab[i] += to_string(rand());
		dirNameTab[i] += "\\";
		CreateDirectory(dirNameTab[i].c_str(), NULL);
		for (int j = 0; j < 10; j++) {
			CreateDirectory((dirNameTab[i] + to_string(j) + "\\").c_str(), NULL);
		}
		CreateDirectory((dirNameTab[i] + "_\\").c_str(), NULL);
	}
	string Path = "D:\\资料\\银行卡识别资料\\images";
	vector<string> files;
	getFiles(Path, files);
	int stat_all = files.size();
	stat_all = stat_all;
	int stat = 1;
	for (auto i : files) {
		if (stat < 200) {
			dirName = dirNameTab[0];
			imagePath = i;
			std::cout << imagePath << std::endl;
			std::cout << "共 " << stat_all << " 个数据，已处理 "
				<< stat << " 个图像，完成比："
				<< 1.000 * stat / stat_all << std::endl;
			std::cout << dirName << std::endl;
			image = imread(imagePath);//加载图像
			resize(image, image, IMG_SIZE);//修改图像大小
			////////////////////////////////////////////////////////////////////////
			//图像处理区域
			for (int blur_num = 1; blur_num <= 5; blur_num += 2) {//降噪，3个等级
				Mat blur_img;
				medianBlur(image, blur_img, blur_num);//降噪（使用中值法模糊，3）
				for (double kk = 0.6; kk <= 1.85; kk += 0.5) {//对比度，5个等级
					for (double bb = -20; bb <= 20; bb += 10) {//亮度，4个等级
						Mat resultImage = Enhance_GaryLin(blur_img, kk, bb);
						cut(resultImage);

					}
				}
			}
			////////////////////////////////////////////////////////////////////////
		}
		else if (stat < 400) {
			dirName = dirNameTab[1];
			imagePath = i;
			std::cout << imagePath << std::endl;
			std::cout << "共 " << stat_all << " 个数据，已处理 "
				<< stat << " 个图像，完成比："
				<< 1.000 * stat / stat_all << std::endl;
			std::cout << dirName << std::endl;
			image = imread(imagePath);//加载图像
			resize(image, image, IMG_SIZE);//修改图像大小
			////////////////////////////////////////////////////////////////////////
			//图像处理区域
			for (int blur_num = 1; blur_num <= 5; blur_num += 2) {//降噪，3个等级
				Mat blur_img;
				medianBlur(image, blur_img, blur_num);//降噪（使用中值法模糊，3）
				for (double kk = 0.6; kk <= 1.85; kk += 0.5) {//对比度，5个等级
					for (double bb = -20; bb <= 20; bb += 10) {//亮度，4个等级
						Mat resultImage = Enhance_GaryLin(blur_img, kk, bb);
						cut(resultImage);
					}
				}
			}
			////////////////////////////////////////////////////////////////////////
		}
		else {
			dirName = dirNameTab[2];
			imagePath = i;
			std::cout << imagePath << std::endl;
			std::cout << "共 " << stat_all << " 个数据，已处理 "
				<< stat << " 个图像，完成比："
				<< 1.000 * stat / stat_all << std::endl;
			std::cout << dirName << std::endl;
			image = imread(imagePath);//加载图像
			resize(image, image, IMG_SIZE);//修改图像大小
			////////////////////////////////////////////////////////////////////////
			//图像处理区域
			for (int blur_num = 1; blur_num <= 5; blur_num += 2) {//降噪，3个等级
				Mat blur_img;
				medianBlur(image, blur_img, blur_num);//降噪（使用中值法模糊，3）
				for (double kk = 0.6; kk <= 1.85; kk += 0.5) {//对比度，5个等级
					for (double bb = -20; bb <= 20; bb += 10) {//亮度，4个等级
						Mat resultImage = Enhance_GaryLin(blur_img, kk, bb);
						cut(resultImage);
					}
				}
			}
			////////////////////////////////////////////////////////////////////////
		}
		stat++;
	}

	//system("pause");
	return 0;
}

void WriteImg(Mat img, char number) {
	String fileNametemp = "";
	numStat[(int)number]++; 
	fileNametemp += dirName;
	fileNametemp += number;
	fileNametemp += "\\";
	fileNametemp += to_string(numStat[(int)number]);
	fileNametemp += ".jpg";
	
	imwrite(fileNametemp, img);
}

Mat Enhance_GaryLin(Mat img, double k, double b) {
	Mat dstImage = img.clone();

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			for (int c = 0; c < 3; c++) {
				//使用at操作符，防止越界
				dstImage.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(k * (img.at<Vec3b>(i, j)[c]) + b);
			}
		}
	}
	//normalize(dstImage, dstImage, 0, 255, NORM_MINMAX);
	//convertScaleAbs(dstImage, dstImage);
	return dstImage;
}

void getFiles(string path, vector<string>& files) {
	//文件句柄
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else {
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

int suiji(int a, int b) {
	int temp;
	b++;
	while (1) {
		temp = rand();
		if (temp % b >= a) {
			break;
		}
	}
	return temp % b;
}

void cut(Mat img) {
	int x = 0, y = 0;
	int w = suiji(37, 39);
	int h = IMG_HEIGHT;
	for (int i = 0; i < 4; i++) {
		Mat numberImg = img(Rect(x, y, w, h));
		x = x + w - 12;
		//切割图像完毕
		resize(numberImg, numberImg, Size(39, IMG_HEIGHT));//修改图像大小
		int pos = imagePath.find_last_of('\\');
		String number(imagePath.substr(pos + 1, 4));
		WriteImg(numberImg, number[i]);
	}
}
