#include "common.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	try {
		UMat src, dst;
		if (argc < 3) {
			throw ("few parameter, e.g <filename> <scaleW> [<scaleH>]");
		}
		float scaleW = static_cast<float>(atof(argv[2]));
		float scaleH = scaleW;
		if (argc == 4) {
			scaleH = static_cast<float>(atof(argv[3]));
		}
		imread(argv[1]).copyTo(src);
		if (src.empty()) {
			throw("faild open file");
		}
		int width = static_cast<int>(src.cols*scaleW);
		int height = static_cast<int>(src.rows*scaleH);
		resize(src, dst, Size(width, height));

		imshow("src", src);
		imshow("dst", dst);

		waitKey(0);
	}
	catch(const char* str){
		cerr << str << endl;
	}
	return 0;
}