#include"FindCard.h"
#include"AllUse.h"
#include<thread>
using namespace std;
//��������Ҫ����ɫ
Vec3b kmeansColorTab[] = {
	Vec3b(0, 0, 0),
	Vec3b(255, 255, 255),
	Vec3b(255, 0, 0),
	Vec3b(0, 0, 255),
	Vec3b(0, 255, 0)
};
//�ݶȻ�����ܵ���ɫ
Vec3b gradientColorTab[8] = {
	Vec3b(0, 0, 0),//��ɫ
	Vec3b(255, 255, 255),//��ɫ
	Vec3b(255, 0, 0),
	Vec3b(255, 255, 0),
	Vec3b(255, 0, 255),
	Vec3b(0, 255, 0),
	Vec3b(0, 255, 255),
	Vec3b(0, 0, 255),
};

//��ʱ����ļ��У����ֶ���������ʽ��"imwrite\\test2"
String tempOutPath = "imwrite\\20190527";
void MultiThread(int i);
void ShowTime();


int main(int argc, char* argv[]) {
	srand(time(0));
	if (argc == 2) {
		//���̴߳���
		std::thread a(ShowTime);
		a.detach();
		cout << "���̴߳���" << endl;
		//while (1)
			for (int i = 7; i <= 7; i++) {
				cout << "�� " << i << " ��ͼƬ��ʼ���� ";
				String path = "testImages\\allcard\\";
				//String path = "testImages\\test2\\";
				path += std::to_string(i);
				path += ".jpg";
				Mat m = imread(path);
				if (m.empty()) continue;
				//FindNumber number = FindNumber(m);
				FindCard card = FindCard(m);
				cout << " �������" << endl;
			}
		cout << "�ر����д����˳������ڵȴ��˳�";
		//getchar();
		waitKey();
	}
	else if (argc == 1) {
		//���̴߳���
		for (int i = 1; i <= 18; i++) {
			this_thread::sleep_for(0.5s);
			std::thread a(MultiThread, i);
			a.detach();
		}
		cout << "�߳����Ͻ������벻Ҫ�ż��˳�" << endl;
		getchar();
		cout << "�ٴΰ���������˳�" << endl;
		getchar();
	}
}

void MultiThread(int i) {
	cout << "-----�߳� " << i << " ��ʼ-----" << endl;
	srand(time(0));
	String path = "testImages\\20190524\\";
	path += std::to_string(i);
	path += ".jpg";
	Mat m = imread(path);
	if (m.empty()) return;
	FindNumber number = FindNumber(m);
	//FindCard card = FindCard(m);
	cout << "-----�߳� " << i << " ����-----" << endl;
	waitKey();
}

void ShowTime() {
	while (1) {
		this_thread::sleep_for(0.2s);
		cout << ".";
	}
}
