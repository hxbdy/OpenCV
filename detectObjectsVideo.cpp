#include "common.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	try {
		int wait = 1;
		if (argc < 3) throw "few paramater";
		VideoCapture capture;
		if (argc > 1 && strcmp(argv[1], "camera") != 0) {
			capture = VideoCapture(argv[1]);
			wait = 33;
		}
		else {
			capture = VideoCapture(0);
		}

		CascadeClassifier objDetecter(argv[2]);
		UMat img,gray,equalize;
		const char* wName = "dst";
		namedWindow(wName, CV_WINDOW_AUTOSIZE);

		while (true) {
			capture >> img;
			if (img.empty()) break;
			cvtColor(img, gray, COLOR_RGB2GRAY);
			equalizeHist(gray, equalize);

			vector<Rect> objs;
			objDetecter.detectMultiScale(equalize, objs, 1.2, 2, CV_HAAR_SCALE_IMAGE, Size(100, 100));
			vector<Rect>::const_iterator it = objs.begin();
			for (; it != objs.end(); ++it) {
				rectangle(img, Point(it->x, it->y), Point(it->x + it->width, it->y + it->height), Scalar(0, 0, 200), 2, CV_AA);
			}
			imshow(wName, img);
			if(waitKey(wait)>=0) break;
		}
		
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}