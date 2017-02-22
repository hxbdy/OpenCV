#include <opencv2\opencv.hpp>
#pragma comment(lib,"opencv_world310.lib")

using namespace cv;
using namespace std;

int main(int argc,char* argv[]) {
	if (argc < 2) {
		cerr << "no parameter." << endl;
		return -1;
	}

	//Mat2UMat
	{
		Mat mat = imread(argv[1]);
		UMat umat;
		mat.copyTo(umat);
	}

	//Mat2UMat
	{
		UMat umat;
		imread(argv[1]).copyTo(umat);
	}

	//Mat2UMat
	{
		Mat mat = imread(argv[1]);
		UMat umat = mat.getUMat(ACCESS_RW);
	}

	//UMat2Mat
	{
		UMat umat;
		imread(argv[1]).copyTo(umat);
		Mat mat = umat.getMat(ACCESS_RW);
	}
	return 0;
}