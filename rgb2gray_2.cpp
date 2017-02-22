#include <opencv2\opencv.hpp>
#pragma comment(lib,"opencv_world310.lib")

using namespace cv;
using namespace std;

static void
cvt(InputArray inputSrc_) {
	int type = inputSrc_.kind() & _InputArray::KIND_MASK;
	switch (type) {
	case _InputArray::MAT:
	{
		Mat src = inputSrc_.getMat();
		Mat dst;
		flip(src, dst, 1);

		imshow("src", src);
		imshow("dst", dst);
	}
	break;
	case _InputArray::UMAT:
	{
		UMat usrc = inputSrc_.getUMat();
		UMat udst;

		cvtColor(usrc, udst, COLOR_RGB2GRAY);

		imshow("usrc", usrc);
		imshow("udst", udst);
	}
	break;
	}

}

int main(int argc, char* argv[]) {
	Mat src;
	UMat usrc;
	
	if (argc < 2) {
		cerr << "no parameter." << endl;
		return -1;
	}

	src = imread(argv[1]);
	cvt(src);

	waitKey(0);
	destroyAllWindows();

	src.copyTo(usrc);
	cvt(usrc);

	waitKey(0);

	return 0;
}