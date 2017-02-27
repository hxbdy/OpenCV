#include "common.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	try {
		Mat mat = imread(argv[1]);
		if (mat.empty()) {
			throw("faild open file");
		}

		Point p = Point(50, mat.rows / 2 - 50);
		putText(mat, "Hello opencv", p, FONT_HERSHEY_TRIPLEX, 0.8, Scalar(250, 200, 200), 2, CV_AA);

		imshow("mat", mat);
		imwrite("mat.jpg", mat);

		waitKey(0);
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}