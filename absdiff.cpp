#include "common.h"

using namespace cv;
using namespace std;

int main(int argc,char* argv[]) {
	try {
		Mat src1,src2, dst;
		if (argc < 3) {
			throw("few parameter");
		}
		imread(argv[1]).copyTo(src1);
		imread(argv[2]).copyTo(src2);

		absdiff(src1, src2, dst);

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