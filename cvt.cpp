#include <opencv2\opencv.hpp>
#pragma comment(lib,"opencv_world310.lib")

using namespace cv;
using namespace std;

static void
cvt(InputArray inputSrc_,OutputArray _outputDst) {
	_outputDst.create(inputSrc_.size(), inputSrc_.type());
	Mat src = inputSrc_.getMat();
	Mat dst = _outputDst.getMat();
	flip(src, dst, 1);
}

int main(int argc, char* argv[]) {
	Mat src,dst;
	
	if (argc < 2) {
		cerr << "no parameter." << endl;
		return -1;
	}

	src = imread(argv[1]);
	//dst.create(src.size(), src.type());
	cvt(src, dst);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);

	return 0;
}