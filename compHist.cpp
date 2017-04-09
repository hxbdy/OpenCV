#include "common.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	try {
		Mat src[2],hist[2];
		double r;
		const int histSize = 256;
		const float range[] = { 0,256 };
		const float* histRange = { range };

		if (argc < 3) {
			throw("few parameters");
		}
		
		for (int i = 0; i < 2; i++) {
			imread(argv[i + 1], IMREAD_GRAYSCALE).copyTo(src[i]);
		}
		for (int i = 0; i < 2; i++) {
			calcHist(&src[i], 1, 0, Mat(), hist[i], 1, &histSize, &histRange, true, false);
		}
		r = compareHist(hist[0], hist[1], CV_COMP_CORREL);
		cout << "CV_COMP_CORREL       =" << r << endl;
		r = compareHist(hist[0], hist[1], CV_COMP_CHISQR);
		cout << "CV_COMP_CHISQR       =" << r << endl;
		r = compareHist(hist[0], hist[1], CV_COMP_INTERSECT);
		cout << "CV_COMP_INTERSECT    =" << r << endl;
		r = compareHist(hist[0], hist[1], CV_COMP_BHATTACHARYYA);
		cout << "CV_COMP_BHATTACHARYYA=" << r << endl;
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}