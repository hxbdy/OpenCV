#include "common.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	try {
		Mat dst,dst32f,sumRow;
		vector<Mat> mat;
		double dMin = FLT_MAX, r;
		int pos = 0;

		if (argc < 3) {
			throw("few parameters");
		}
		
		mat.resize(argc - 1);
		for (int i = 0; i < mat.size(); i++) {
			imread(argv[i + 1], IMREAD_GRAYSCALE).copyTo(mat[i]);
		}
		for (int i = 1; i < mat.size(); i++) {
			absdiff(mat[0], mat[i], dst);
			r = sum(dst)[0];
			cout << argv[i + 1] << ",r=" << r << endl;
			if (dMin > r) {
				dMin = r;
				pos = i + 1;
			}
		}
		cout << "[" << argv[1] << "] best " << argv[pos] << endl;
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}