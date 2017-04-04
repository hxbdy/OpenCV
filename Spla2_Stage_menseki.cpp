//Splatoon2�X�e�[�W�̃X�N�V������C���N�h��ʐς��v�Z�X����
//�X�e�[�W�S�̂�100%�Ƃ��Ȃ��ŁA���h��ʐς�100%�Ƃ��Ă郈
//���s���@�̓^�[�~�i���ňȉ��̒ʂ�_��
//OpenCV3.0 <�X�e�[�W�̃}�X�N�摜> <�X�N�V���摜>
//Ver 1.0.2

#include "common.h"
#define GRAY_UP  1.7 //BGR��̏�͈�
#define GRAY_SUM 110 //BGR���a��臒l
#define V_THR    120 //HSV��V��臒l

using namespace cv;
using namespace std;

typedef struct Ink {
	float per;              //�S�̓h��ʐς̊���
	float men;              //�h�葍�ʐ�
	Mat   ink;              //���w�C���N�F�݂̂𔽉f�������摜����
	int   BGR[3];           //���摜����̎擾BGR
	Scalar color[2];        //���w�C���N�F
	float H;                //HSV��H
	float HRange[2];        //H�͈͂̉��A��
};

//BGR��HSV��H�ɕϊ�����
float HSV_H(int BGR[]) {
	float H;
	int max, min;
	max = MAX(MAX(BGR[0], BGR[1]), BGR[2]);
	min = MIN(MIN(BGR[0], BGR[1]), BGR[2]);
	if (max == BGR[0]) H = float(60.0 * ((BGR[2] - BGR[1]) / (max - min)) + 240);
	else if (max == BGR[1]) H = float(60.0 * ((BGR[0] - BGR[2]) / (max - min)) + 120);
	else if (max == BGR[2]) H = float(60.0 * ((BGR[1] - BGR[0]) / (max - min)));
	if (H < 0) H += 360.0;
	return H;
}

//BGR��HSV��V�ɕϊ�����
int HSV_V(int BGR[]) {
	return MAX(MAX(BGR[0], BGR[1]), BGR[2]);
}

int main(int argc, char* argv[]) {
	try {

		Ink mikata, teki;
		mikata.men = 0;
		teki.men = 0;

		int white = 0, cnt = 0, ink[3];
		float min, R, G, B, H_up, H_thr;
		Mat stage, src1, src2, dst, dst2;
		Mat teki_ink, mikata_ink;

		if (argc < 2) {
			throw "few para";
		}

		//�X�e�[�W����(������)
		//�X�e�[�W���ʐόv�Z
		imread(argv[1]).copyTo(src1);
		imread(argv[2]).copyTo(src2);
		bitwise_and(src1, src2, dst);
		cvtColor(dst, stage, COLOR_RGB2GRAY);
		threshold(stage, stage, 80, 210, THRESH_BINARY);
		for (int y = 0; y < stage.rows; y++) {
			for (int x = 0; x < stage.cols; x++) {
				if (stage.at<unsigned char>(y, x)) white++;
			}
		}
		mikata_ink = src1.clone();
		teki_ink = src1.clone();

		//�V���K�[���_
		//�G�C���NH
		bool flg = true;
		for (int y = 0; y < dst.rows; y++) {
			for (int x = 0; x < dst.cols; x++) {
				ink[0] = dst.at<Vec3b>(y, x)[0];
				ink[1] = dst.at<Vec3b>(y, x)[1];
				ink[2] = dst.at<Vec3b>(y, x)[2];
				if (ink[0] == 0) ink[0] = 1;
				if (ink[1] == 0) ink[1] = 1;
				if (ink[2] == 0) ink[2] = 1;

				if (HSV_V(ink) < V_THR) continue;

				min = float(MIN(MIN(ink[0], ink[1]), ink[2]));
				B = ink[0] / min;
				G = ink[1] / min;
				R = ink[2] / min;
				if (ink[0] + ink[1] + ink[2] > GRAY_SUM) {
					if (B == 1.000) {
						if (GRAY_UP < G || GRAY_UP < R) {
							if (flg) {
								teki.H = HSV_H(ink);
								flg = false;
							}
						}
					}
					else if (G == 1.000) {
						if (GRAY_UP < B || GRAY_UP < R) {
							if (flg) {
								teki.H = HSV_H(ink);
								flg = false;
							}
						}
					}
					else if (R == 1.000) {
						if (GRAY_UP < G || GRAY_UP < B) {
							if (flg) {
								teki.H = HSV_H(ink);
								flg = false;
							}
						}
					}
				}
				if (!flg) break;
			}
			if (!flg) break;
		}

		//�����C���NH
		flg = true;
		for (int y = dst.rows; y >0; y--) {
			for (int x = dst.cols; x > 0; x--) {
				ink[0] = dst.at<Vec3b>(y, x)[0];
				ink[1] = dst.at<Vec3b>(y, x)[1];
				ink[2] = dst.at<Vec3b>(y, x)[2];
				if (ink[0] == 0) ink[0] = 1;
				if (ink[1] == 0) ink[1] = 1;
				if (ink[2] == 0) ink[2] = 1;

				if (HSV_V(ink) < V_THR) continue;

				min = float(MIN(MIN(ink[0], ink[1]), ink[2]));
				B = ink[0] / min;
				G = ink[1] / min;
				R = ink[2] / min;
				if (ink[0] + ink[1] + ink[2] > GRAY_SUM) {
					if (B == 1.000) {
						if (GRAY_UP < G || GRAY_UP < R) {
							if (flg) {
								mikata.H = HSV_H(ink);
								flg = false;
							}
						}
					}
					else if (G == 1.000) {
						if (GRAY_UP < B || GRAY_UP < R) {
							if (flg) {
								mikata.H = HSV_H(ink);
								flg = false;
							}
						}
					}
					else if (R == 1.000) {
						if (GRAY_UP < G || GRAY_UP < B) {
							if (flg) {
								mikata.H = HSV_H(ink);
								flg = false;
							}
						}
					}
				}
				if (!flg) break;
			}
			if (!flg) break;
		}
		cout << "teki BGR=" << ink[0] << " " << ink[1] << " " << ink[2] << "\n";
		cout << "teki H=" << teki.H << "\n";
		cout << "mikata BGR=" << ink[0] << " " << ink[1] << " " << ink[2] << "\n";
		cout << "mikata H=" << mikata.H << "\n";

		//�C���N臒l�v�Z
		H_thr = (mikata.H + teki.H) / 2;
		H_up = H_thr + 180;
		cout << "H_low=" << H_thr << " " << "H_up=" << H_up << "\n";
		if (H_thr <= teki.H && teki.H <= H_up) {
			teki.HRange[0] = H_thr;
			teki.HRange[1] = H_up;
		}
		else {
			teki.HRange[0] = H_up;
			teki.HRange[1] = H_up + 180;
		}
		if (H_thr <= mikata.H && mikata.H <= H_up) {
			mikata.HRange[0] = H_thr;
			mikata.HRange[1] = H_up;
		}
		else {
			mikata.HRange[0] = H_up;
			mikata.HRange[1] = H_up + 180;
		}

		//�ʐόv�Z
		for (int y = 0; y < dst.rows; y++) {
			for (int x = 0; x < dst.cols; x++) {
				ink[0] = dst.at<Vec3b>(y, x)[0];
				ink[1] = dst.at<Vec3b>(y, x)[1];
				ink[2] = dst.at<Vec3b>(y, x)[2];
				if (ink[0] == 0) ink[0] = 1;
				if (ink[1] == 0) ink[1] = 1;
				if (ink[2] == 0) ink[2] = 1;

				if (HSV_V(ink) < V_THR) continue;

				min = float(MIN(MIN(ink[0], ink[1]), ink[2]));
				B = ink[0] / min;
				G = ink[1] / min;
				R = ink[2] / min;
				if (ink[0] + ink[1] + ink[2] > GRAY_SUM) {
					if (B == 1.000) {
						if (GRAY_UP < G || GRAY_UP < R) {
						if (mikata.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= mikata.HRange[1] ||
							mikata.HRange[0] <= HSV_H(ink) + 360 && HSV_H(ink) + 360 <= mikata.HRange[1]) {
							mikata.men++;
							mikata_ink.at<Vec3b>(y, x)[0] = ink[0];
							mikata_ink.at<Vec3b>(y, x)[1] = ink[1];
							mikata_ink.at<Vec3b>(y, x)[2] = ink[2];

						}
						else if (teki.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= teki.HRange[1] ||
							teki.HRange[0] <= HSV_H(ink) + 360 && HSV_H(ink) + 360 <= teki.HRange[1]) {
							teki.men++;
							teki_ink.at<Vec3b>(y, x)[0] = ink[0];
							teki_ink.at<Vec3b>(y, x)[1] = ink[1];
							teki_ink.at<Vec3b>(y, x)[2] = ink[2];

						}
						}
					}
					else if (G == 1.000) {
						if (GRAY_UP < B || GRAY_UP < R) {
						if (mikata.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= mikata.HRange[1] ||
							mikata.HRange[0] <= HSV_H(ink) + 360 && HSV_H(ink) + 360 <= mikata.HRange[1]) {
							mikata.men++;
							mikata_ink.at<Vec3b>(y, x)[0] = ink[0];
							mikata_ink.at<Vec3b>(y, x)[1] = ink[1];
							mikata_ink.at<Vec3b>(y, x)[2] = ink[2];
						}
						else if (teki.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= teki.HRange[1] ||
							teki.HRange[0] <= HSV_H(ink) + 360 && HSV_H(ink) + 360 <= teki.HRange[1]) {
							teki.men++;
							teki_ink.at<Vec3b>(y, x)[0] = ink[0];
							teki_ink.at<Vec3b>(y, x)[1] = ink[1];
							teki_ink.at<Vec3b>(y, x)[2] = ink[2];

						}
						}
					}
					else if (R == 1.000) {
						if (GRAY_UP < G || GRAY_UP < B) {
						if (mikata.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= mikata.HRange[1] ||
							mikata.HRange[0] <= HSV_H(ink) + 360 && HSV_H(ink) + 360 <= mikata.HRange[1]) {
							mikata.men++;
							mikata_ink.at<Vec3b>(y, x)[0] = ink[0];
							mikata_ink.at<Vec3b>(y, x)[1] = ink[1];
							mikata_ink.at<Vec3b>(y, x)[2] = ink[2];
						}
						else if (teki.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= teki.HRange[1] ||
							teki.HRange[0] <= HSV_H(ink) + 360 && HSV_H(ink) + 360 <= teki.HRange[1]) {
							teki.men++;
							teki_ink.at<Vec3b>(y, x)[0] = ink[0];
							teki_ink.at<Vec3b>(y, x)[1] = ink[1];
							teki_ink.at<Vec3b>(y, x)[2] = ink[2];

						}
						}
					}
				}
			}
		}

		imshow("teki_ink", teki_ink);
		imshow("mikata_ink", mikata_ink);
		imwrite("ink2.png", teki_ink);
		imwrite("ink1.png", mikata_ink);
		cout << "mikata=" << mikata.men << "\n";
		cout << "teki=" << teki.men << "\n";
		cout << "zentai=" << mikata.men + teki.men << "\n";

		cout << (mikata.men / (mikata.men + teki.men)) * 100 << "% vs " << (teki.men / (mikata.men + teki.men)) * 100 << "%\n";
		waitKey(0);
	}
	catch (const char* str) {
		cerr << str << endl;
	}
	return 0;
}