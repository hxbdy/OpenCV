#include "common.h"

using namespace cv;
using namespace std;

int main(int argc,char* argv[]) {
	try {
		int wait = 1;
		VideoCapture capture;
		if (argc>1 && _strcmpi(argv[1],"camera")!=0) {
			capture = VideoCapture(argv[1]);
			wait = 33;
		}
		else {
			capture = VideoCapture(0);
		}
		double threshold1 = 40.0, threshold2 = 200.0;
		if (argc >= 3) threshold1 = atof(argv[2]);
		if (argc >= 4) threshold2 = atof(argv[3]);
		int width = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
		int height = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));
		cout << "frame size=" << width << "x" << height << endl;

		double fps = capture.get(CV_CAP_PROP_FPS);
		fps = fps > 0.0 ? fps : 30.0;
		VideoWriter save("dst.avi", CV_FOURCC_DEFAULT,fps,Size(width,height),true);
		if (!save.isOpened()) {
			throw "VideoWRiter ERR\n";
		}
		int fCounter = 0;
		const int fLimit = 30 * 60;
		const char* srcwName = "src";
		const char* dstwName = "dst";
		UMat src,gray,canny;
		Mat  dst;
		namedWindow(srcwName, CV_WINDOW_AUTOSIZE);
		namedWindow(dstwName, CV_WINDOW_AUTOSIZE);
		while (true) {
			capture >> src;
			if (src.empty()) break;

			imshow(srcwName, src);

			cvtColor(src, gray, COLOR_RGB2GRAY);
			Canny(gray, canny, threshold1, threshold2);
			canny.copyTo(dst);
			imshow(dstwName, dst);
			cvtColor(dst, dst, COLOR_GRAY2RGB);
			save << dst;
			if (fCounter++ > fLimit) break;
			if (waitKey(1) >= 0) break;
		}
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}