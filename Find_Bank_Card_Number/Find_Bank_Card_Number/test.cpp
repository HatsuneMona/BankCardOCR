//#include<opencv2/opencv.hpp>
//using namespace cv;
//
//int main(int argc, char* argv[]) {
//
//	srand(time(0));
//	if (argc <= 1) {
//		std::cout << "please input file path!" << std::endl;
//		return 1;
//	}
//	try {
//		int i;
//		for (i = 1; i < argc; i++) {
//			Mat srcImg = imread(argv[i]);
//			resize(srcImg, srcImg, Size(552, 46));
//			imwrite("E:\\¾ÖÓòÍø¹²Ïí\\552x46\\" + std::to_string(rand()) + ".jpg", srcImg);
//		}
//	}
//	catch (std::invalid_argument e) {
//		std::cout << "exception: " << e.what() << std::endl;
//	}
//	catch (...) {
//		std::cout << "unexcept exception!" << std::endl;
//	}
//
//	return 0;
//}