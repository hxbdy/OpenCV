#include <opencv2\opencv.hpp>
#pragma comment(lib,"opencv_world310.lib")

int main(int argc,char* argv[]) {
	if (argc < 2) {
		std::cerr << "no parameter." << std::endl;
		return -1;
	}
	cv::Mat src = cv::imread(argv[1]);
	cv::imshow("src", src);
	cv::waitKey(0);
	return 0;
}