#include "common.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	try {
		Mat mat = imread(argv[1]);
		if (mat.empty()) {
			throw("faild open file");
		}

		int x0 = mat.cols / 4;
		int x1 = mat.cols * 3 / 4;
		int y0 = mat.rows / 4;
		int y1 = mat.rows * 3 / 4;

		Point p0 = Point(x0, y0);
		Point p1 = Point(x1, y1);
		line(mat, p0, p1, Scalar(0, 0, 255), 3, 4);

		p0.y = y1;
		p1.y = y0;

		line(mat, p0, p1, Scalar(255, 0, 0), 3, 4);

		imshow("mat", mat);
		imwrite("mat.jpg", mat);

		waitKey(0);
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}