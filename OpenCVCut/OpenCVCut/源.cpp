#include<iostream>
#include<fstream>
#include<io.h>
#include<Windows.h>
#include<ctime>

#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

//ԭͼƬ��С��һ����ĳ���
//Сͼ
#define IMG_WIDTH     120
#define IMG_HEIGHT    46
#define IMG_SIZE      Size(IMG_WIDTH, IMG_HEIGHT)

//Ĭ�ϴ����Ĵ�������
#define WINDOW_NAME "��"

//ȫ�ֱ���
Mat image;
String imagePath;
int numStat[100] = { 0 };
String dirNameTab[3];
String dirName;

//��������
void WriteImg(Mat img, char number);
Mat Enhance_GaryLin(Mat img, double k, double b);
void getFiles(string path, vector<string>& files);
void cut(Mat img);

int main(int argc, char* argv[]) {
	srand(time(0));
	for (int i = 0; i < 3; i++) {
		dirNameTab[i] = "";
		dirNameTab[i] += "E:\\����������\\CutFile";
		dirNameTab[i] += to_string(rand());
		dirNameTab[i] += "\\";
		CreateDirectory(dirNameTab[i].c_str(), NULL);
		for (int j = 0; j < 10; j++) {
			CreateDirectory((dirNameTab[i] + to_string(j) + "\\").c_str(), NULL);
		}
		CreateDirectory((dirNameTab[i] + "_\\").c_str(), NULL);
	}
	string Path = "D:\\����\\���п�ʶ������\\images";
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
			std::cout << "�� " << stat_all << " �����ݣ��Ѵ��� "
				<< stat << " ��ͼ����ɱȣ�"
				<< 1.000 * stat / stat_all << std::endl;
			std::cout << dirName << std::endl;
			image = imread(imagePath);//����ͼ��
			resize(image, image, IMG_SIZE);//�޸�ͼ���С
			////////////////////////////////////////////////////////////////////////
			//ͼ��������
			for (int blur_num = 1; blur_num <= 5; blur_num += 2) {//���룬3���ȼ�
				Mat blur_img;
				medianBlur(image, blur_img, blur_num);//���루ʹ����ֵ��ģ����3��
				for (double kk = 0.6; kk <= 1.85; kk += 0.5) {//�Աȶȣ�5���ȼ�
					for (double bb = -20; bb <= 20; bb += 10) {//���ȣ�4���ȼ�
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
			std::cout << "�� " << stat_all << " �����ݣ��Ѵ��� "
				<< stat << " ��ͼ����ɱȣ�"
				<< 1.000 * stat / stat_all << std::endl;
			std::cout << dirName << std::endl;
			image = imread(imagePath);//����ͼ��
			resize(image, image, IMG_SIZE);//�޸�ͼ���С
			////////////////////////////////////////////////////////////////////////
			//ͼ��������
			for (int blur_num = 1; blur_num <= 5; blur_num += 2) {//���룬3���ȼ�
				Mat blur_img;
				medianBlur(image, blur_img, blur_num);//���루ʹ����ֵ��ģ����3��
				for (double kk = 0.6; kk <= 1.85; kk += 0.5) {//�Աȶȣ�5���ȼ�
					for (double bb = -20; bb <= 20; bb += 10) {//���ȣ�4���ȼ�
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
			std::cout << "�� " << stat_all << " �����ݣ��Ѵ��� "
				<< stat << " ��ͼ����ɱȣ�"
				<< 1.000 * stat / stat_all << std::endl;
			std::cout << dirName << std::endl;
			image = imread(imagePath);//����ͼ��
			resize(image, image, IMG_SIZE);//�޸�ͼ���С
			////////////////////////////////////////////////////////////////////////
			//ͼ��������
			for (int blur_num = 1; blur_num <= 5; blur_num += 2) {//���룬3���ȼ�
				Mat blur_img;
				medianBlur(image, blur_img, blur_num);//���루ʹ����ֵ��ģ����3��
				for (double kk = 0.6; kk <= 1.85; kk += 0.5) {//�Աȶȣ�5���ȼ�
					for (double bb = -20; bb <= 20; bb += 10) {//���ȣ�4���ȼ�
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
				//ʹ��at����������ֹԽ��
				dstImage.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(k * (img.at<Vec3b>(i, j)[c]) + b);
			}
		}
	}
	//normalize(dstImage, dstImage, 0, 255, NORM_MINMAX);
	//convertScaleAbs(dstImage, dstImage);
	return dstImage;
}

void getFiles(string path, vector<string>& files) {
	//�ļ����
	intptr_t hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			//�����Ŀ¼,����֮
			//�������,�����б�
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
		//�и�ͼ�����
		resize(numberImg, numberImg, Size(39, IMG_HEIGHT));//�޸�ͼ���С
		int pos = imagePath.find_last_of('\\');
		String number(imagePath.substr(pos + 1, 4));
		WriteImg(numberImg, number[i]);
	}
}
