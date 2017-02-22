#include <opencv2\opencv.hpp>
#pragma comment(lib,"opencv_world310.lib")

using namespace cv;
using namespace std;

static void
cvt(InputArray inputSrc_) {
	Mat src = inputSrc_.getMat();
	Mat dst;

	cvtColor(src, dst, COLOR_RGB2GRAY);

	imshow("src", src);
	imshow("dst", dst);
}

int main(int argc, char* argv[]) {
	Mat src;
	UMat usrc;

	if (argc < 2) return -1;
	
	src = imread(argv[1]);
	cvt(src);

	waitKey(0);
	destroyAllWindows();
	
	src.copyTo(usrc);
	cvt(usrc);
	
	waitKey(0);
	return 0;
}