#include "common.h"

using namespace cv;
using namespace std;

static UMat makeMask(int width, int height) {
	Size sz(width, height);
	UMat mask(sz, CV_8UC1, Scalar(0));
	Point p0 = Point(width / 4, height / 4);
	Point p1 = Point(width * 3 / 4, height * 3 / 4);
	rectangle(mask, p0, p1, Scalar(255), -1);
	return mask;
}

Mat createCosMat(const int rows, const int cols) {
	Mat mat(rows, cols, CV_8UC3, Scalar(0));
	Point center = Point(rows / 2, cols / 2);
	double radius = sqrt(pow(center.x, 2) + pow(center.y, 2));

	for (int y = 0; y < mat.rows; y++) {
		for (int x = 0; x < mat.cols; x++) {
			double distance = sqrt(pow(center.x - x, 2) + pow(center.y - y, 2));
			double radian = (distance / radius)*(double)CV_PI;
			double Y = (cos(radian) + 1.0) / 2.0;
			mat.at<Vec3b>(y, x)[0] = mat.at<Vec3b>(y, x)[1] = mat.at<Vec3b>(y, x)[2] = (unsigned char)(Y*255.0f);
		}
	}

	return mat;
}

Mat mulMat(const Mat mat, const Mat table) {
	Mat dst, mat32f, table32f, dst32f;
	mat.convertTo(mat32f, CV_32FC3);
	table.convertTo(table32f, CV_32FC3);
	table32f /= 255.0f;
	multiply(mat32f, table32f, dst32f);
	dst32f.convertTo(dst, CV_8UC3);

	return dst;
}

int main(int argc,char* argv[]) {
	try {
		Mat src1,src2, intSrc1,intSrc2,dst;
		if (argc < 3) {
			throw("few parameter");
		}
		imread(argv[1]).copyTo(src1);
		imread(argv[2]).copyTo(src2);

		Mat weightMat = createCosMat(src1.rows, src2.cols);
		Mat iWeightMat = Scalar(255, 255, 255) - weightMat;

		intSrc1 = mulMat(src1, weightMat);
		intSrc2 = mulMat(src2, iWeightMat);
		add(intSrc1, intSrc2, dst);

		imshow("src1", src1);
		imshow("src2", src2);
		imshow("dst", dst);

		waitKey(0);
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}