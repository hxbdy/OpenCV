#include "common.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	try {
		Mat mat = imread(argv[1]);
		if (mat.empty()) {
			throw("faild open file");
		}

		Point p0 = Point(mat.cols/8, mat.rows/8);
		Point p1 = Point(mat.cols*7/8, mat.rows*7/8);

		rectangle(mat, p0, p1, Scalar(0, 255, 0), 5, 8);

		waitKey(0);
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}