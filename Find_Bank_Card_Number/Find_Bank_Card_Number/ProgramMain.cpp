#include"FindCard.h"
#include"AllUse.h"
#include<thread>
using namespace std;
//聚类所需要的颜色
Vec3b kmeansColorTab[] = {
	Vec3b(0, 0, 0),
	Vec3b(255, 255, 255),
	Vec3b(255, 0, 0),
	Vec3b(0, 0, 255),
	Vec3b(0, 255, 0)
};
//梯度化后可能的颜色
Vec3b gradientColorTab[8] = {
	Vec3b(0, 0, 0),//黑色
	Vec3b(255, 255, 255),//白色
	Vec3b(255, 0, 0),
	Vec3b(255, 255, 0),
	Vec3b(255, 0, 255),
	Vec3b(0, 255, 0),
	Vec3b(0, 255, 255),
	Vec3b(0, 0, 255),
};

//临时输出文件夹（需手动创建）格式："imwrite\\test2"
String tempOutPath = "imwrite\\20190527";
void MultiThread(int i);
void ShowTime();


int main(int argc, char* argv[]) {
	srand(time(0));
	if (argc == 2) {
		//单线程处理
		std::thread a(ShowTime);
		a.detach();
		cout << "单线程处理" << endl;
		//while (1)
			for (int i = 7; i <= 7; i++) {
				cout << "第 " << i << " 张图片开始处理 ";
				String path = "testImages\\allcard\\";
				//String path = "testImages\\test2\\";
				path += std::to_string(i);
				path += ".jpg";
				Mat m = imread(path);
				if (m.empty()) continue;
				//FindNumber number = FindNumber(m);
				FindCard card = FindCard(m);
				cout << " 处理完毕" << endl;
			}
		cout << "关闭所有窗口退出，正在等待退出";
		//getchar();
		waitKey();
	}
	else if (argc == 1) {
		//多线程处理
		for (int i = 1; i <= 18; i++) {
			this_thread::sleep_for(0.5s);
			std::thread a(MultiThread, i);
			a.detach();
		}
		cout << "线程马上结束，请不要着急退出" << endl;
		getchar();
		cout << "再次按下任意键退出" << endl;
		getchar();
	}
}

void MultiThread(int i) {
	cout << "-----线程 " << i << " 开始-----" << endl;
	srand(time(0));
	String path = "testImages\\20190524\\";
	path += std::to_string(i);
	path += ".jpg";
	Mat m = imread(path);
	if (m.empty()) return;
	FindNumber number = FindNumber(m);
	//FindCard card = FindCard(m);
	cout << "-----线程 " << i << " 结束-----" << endl;
	waitKey();
}

void ShowTime() {
	while (1) {
		this_thread::sleep_for(0.2s);
		cout << ".";
	}
}
