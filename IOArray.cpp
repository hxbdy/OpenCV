#include <opencv2\opencv.hpp>
#pragma comment(lib,"opencv_world310.lib")

using namespace cv;
using namespace std;

static void
add1mat(const Mat& mat) {
	Mat dst = mat + 1;
	cout << "dst=" << dst << endl << endl;
}

static void
add1array(const InputArray inputSrc_) {
	Mat src = inputSrc_.getMat();
	Mat dst = src + 1;
	cout << "dst=" << dst << endl << endl;
}

int main(int argc, char* argv[]) {
	Mat mat = (Mat_<float>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
	cout << "mat=" << mat << endl << endl;
	add1mat(mat);
	add1array(mat);
	return 0;
}