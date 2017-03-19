#include "common.h"
#include "opencv2\stitching.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	try {
		if (argc < 3) throw "few paramater\n";
		Mat dst;
		vector<Mat> srcs;
		for (int i = 0; i < argc - 1; i++) {
			Mat src;
			imread(argv[i + 1]).copyTo(src);
			srcs.push_back(src);
			imshow("src" + to_string(i), src);
		}
		Stitcher stt = Stitcher::createDefault();
		Stitcher::Status status = stt.stitch(srcs, dst);
		if (status != Stitcher::OK) {
			throw "faild at stitch";
		}
		imshow("dst", dst);
		imwrite("dst.jpg", dst);
		waitKey(0);
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}