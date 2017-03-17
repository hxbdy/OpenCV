#include "common.h"

using namespace cv;
using namespace std;

int main(int argc,char* argv[]) {
	try {
		int wait=1;
		VideoCapture capture;
		
		if (argc == 2) {
			capture = VideoCapture(argv[1]);
			wait = 33;
		}
		else {
			capture = VideoCapture(0);
		}
		if (!capture.open(0)) {
			throw "camera not found";
		}
		int width = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
		int height = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));
		cout << "frame size=" << width << "x" << height << endl;
		double threshold1 = 40.0, threshold2 = 200.0;
		if (argc >= 3) {
			threshold1 = atof(argv[2]);
		}
		if (argc >= 4) {
			threshold2 = atof(argv[3]);
		}
		const char* wName2 = "src";
		const char* wName = "dst";
		UMat src,dst,gray,canny;
		namedWindow(wName, CV_WINDOW_AUTOSIZE);
		while (true) {
			capture >> src;
			if (src.empty()) {
				break;
			}
			cvtColor(src, gray, COLOR_RGB2GRAY);
			Canny(gray, dst, threshold1, threshold2);
			imshow(wName2, src);
			imshow(wName, dst);
			if (waitKey(wait) >= 0) {
				break;
			}
		}
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}