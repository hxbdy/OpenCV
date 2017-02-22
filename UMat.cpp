#include <opencv2\opencv.hpp>
#pragma comment(lib,"opencv_world310.lib")

using namespace cv;
using namespace std;

int main(int argc,char* argv[]) {
	UMat src, dst;
	if (argc < 2) {
		cerr << "no parameter." << endl;
		return -1;
	}
	imread(argv[1]).copyTo(src);
	cvtColor(src, dst, COLOR_RGB2GRAY);
	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}