#include <opencv2\opencv.hpp>
#pragma comment(lib,"opencv_world310.lib")

using namespace cv;
using namespace std;

int main(int argc,char* argv[]) {
	Mat src, dst;
	if (argc < 2) {
		cerr << "no parameter." << endl;
		return -1;
	}
	src = imread(argv[1]);
	cvtColor(src, dst, COLOR_RGB2GRAY);
	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}