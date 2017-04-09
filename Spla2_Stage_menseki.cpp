//Splatoon2ステージのスクショからインク塗り面積を計算スルヨ
//ステージ全体を100%としないで、総塗り面積を100%としてるヨ
//前提として、各ステージのマスク画像、ステージ推測用の2値画像が必要ダヨ(公開予定)
//インクの識別法が2つ実装されているけど、「誤差許容法」の使用を推奨するんのぉ!
//実行方法はターミナルで以下の通りダヨ
//OpenCV3.0 <スクショ画像> <インク色認識方法フラグ>
//Ver 2.0.0

#include <iostream>
#include "common.h"

#define GRAY_UP  1.7 //BGR比の上範囲
#define GRAY_SUM 110 //BGR総和の閾値
#define V_THR    120 //HSVのVの閾値
#define H_LU     60.0//Hの許容範囲(誤差許容法のとき使用)

//インク色認識方法フラグ
#define BISECTION 0 //HSV二分割法
#define TOLERANCE 1 //誤差許容法

//ステージ識別用
#define STAGE              2 //ステージ数
#define BATTERASTREET      0 //バッテラストリート
#define FUJITSUBOSPORTCLUB 1 //フジツボスポーツクラブ
#define CONBUTRACK         2 //コンブトラック

using namespace cv;
using namespace std;

typedef struct Ink {
	double per;      //全体塗り面積の割合
	double men;      //塗り総面積
	Mat   ink;       //自陣インク色のみを反映させた画像生成
	int   BGR[3];    //元画像からの取得BGR
	Scalar color[2]; //自陣インク色
	double H;        //HSVのH
	double HRange[2];//H範囲の下、上
};

//BGRをHSVのHに変換する
double HSV_H(int intBGR[]) {
	double H, BGR[3];
	double max, min;
	BGR[0] = double(intBGR[0]);
	BGR[1] = double(intBGR[1]);
	BGR[2] = double(intBGR[2]);
	max = double(MAX(MAX(BGR[0], BGR[1]), BGR[2]));
	min = double(MIN(MIN(BGR[0], BGR[1]), BGR[2]));
	if (max == BGR[0])    	H = 60.0 * ((BGR[2] - BGR[1]) / (max - min)) + 240;
	else if (max == BGR[1]) H = 60.0 * ((BGR[0] - BGR[2]) / (max - min)) + 120;
	else if (max == BGR[2]) H = 60.0 * ((BGR[1] - BGR[0]) / (max - min));
	if (H < 0) H += 360.0;
	return H;
}

//BGRをHSVのVに変換する
int HSV_V(int BGR[]) {
	return MAX(MAX(BGR[0], BGR[1]), BGR[2]);
}

int main(int argc, char* argv[]) {
	try {

		Ink mikata, teki;
		mikata.men = 0;
		teki.men = 0;

		int white = 0, cnt = 0, ink[3], method, min;
		double R, G, B, H_up, H_thr;
		Mat stage, src1, src2, dst, dst2;
		Mat teki_ink, mikata_ink;

		if (argc < 3) {
			throw "few para";
		}

		//画像、フラグ読み込み
		imread(argv[1]).copyTo(src2);
		method = atoi(argv[2]);

		//ステージ判別
		inRange(src2, Scalar(180, 180, 180), Scalar(255, 255, 255), dst2);

		Mat hist[2];
		vector<Mat> mat;
		double r;
		const int histSize = 256;
		const float range[] = { 0,256 };
		const float* histRange = { range };
		double dMax = DBL_MIN;
		int pos = 0;

		mat.resize(STAGE+1);
		dst2.copyTo(mat[0]);
		imread("battera_street.png", IMREAD_GRAYSCALE).copyTo(mat[1]);
		imread("fujitsubo_sport_club.png", IMREAD_GRAYSCALE).copyTo(mat[2]);
		calcHist(&mat[0], 1, 0, Mat(), hist[0], 1, &histSize, &histRange, true, false);

		for (int i = 1; i <=STAGE; i++) {
			calcHist(&mat[i], 1, 0, Mat(), hist[1], 1, &histSize, &histRange, true, false);
			r = compareHist(hist[0], hist[1], CV_COMP_CORREL);
			cout << "r=" << r << endl;
			if (dMax < r) {
				dMax = r;
				pos = i-1;
			}
		}
		cout << "hist=" << r <<endl;
		switch (pos)
		{
		case BATTERASTREET:
			cout << "stage is battera\n\n";
			imread("battera_street_mask.png").copyTo(src1);
			break;
		case FUJITSUBOSPORTCLUB:
			cout << "stage is fujitsubo\n\n";
			imread("fujitsubo_sport_club_mask.png").copyTo(src1);
			break;
		default:
			throw("stage serch error");
		}

		//ステージ総面積計算(未使用)
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

		//敵インクH
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

				min = MIN(MIN(ink[0], ink[1]), ink[2]);
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
		cout << "teki BGR=" << ink[0] << " " << ink[1] << " " << ink[2] << "\n";
		cout << "teki H=" << teki.H << "\n\n";

		//味方インクH
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

				min = MIN(MIN(ink[0], ink[1]), ink[2]);
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
		cout << "mikata BGR=" << ink[0] << " " << ink[1] << " " << ink[2] << "\n";
		cout << "mikata H=" << mikata.H << "\n\n";

		//インク閾値計算(BISECTION)
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
		cout << "teki H range=" << teki.HRange[0] << "~" << teki.HRange[1] << endl;
		cout << "mikata H range=" << mikata.HRange[0] << "~" << mikata.HRange[1] << "\n\n";

		//面積計算
		for (int y = 0; y < dst.rows; y++) {
			for (int x = 0; x < dst.cols; x++) {
				ink[0] = dst.at<Vec3b>(y, x)[0];
				ink[1] = dst.at<Vec3b>(y, x)[1];
				ink[2] = dst.at<Vec3b>(y, x)[2];
				if (ink[0] == 0) ink[0] = 1;
				if (ink[1] == 0) ink[1] = 1;
				if (ink[2] == 0) ink[2] = 1;

				if (HSV_V(ink) < V_THR) continue;

				min = MIN(MIN(ink[0], ink[1]), ink[2]);
				B = ink[0] / min;
				G = ink[1] / min;
				R = ink[2] / min;
				if (ink[0] + ink[1] + ink[2] > GRAY_SUM) {
					if (B == 1.000) {
						if (GRAY_UP < G || GRAY_UP < R) {
							switch (method) {
							case BISECTION:
								if (mikata.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= mikata.HRange[1] ||
									mikata.HRange[0] <= HSV_H(ink) + 360.0 && HSV_H(ink) + 360.0 <= mikata.HRange[1]) {
									mikata.men++;
									mikata_ink.at<Vec3b>(y, x)[0] = ink[0];
									mikata_ink.at<Vec3b>(y, x)[1] = ink[1];
									mikata_ink.at<Vec3b>(y, x)[2] = ink[2];

								}
								else if (teki.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= teki.HRange[1] ||
									teki.HRange[0] <= HSV_H(ink) + 360.0 && HSV_H(ink) + 360.0 <= teki.HRange[1]) {
									teki.men++;
									teki_ink.at<Vec3b>(y, x)[0] = ink[0];
									teki_ink.at<Vec3b>(y, x)[1] = ink[1];
									teki_ink.at<Vec3b>(y, x)[2] = ink[2];
								}
								break;
							case TOLERANCE:
								if (mikata.H - H_LU <= HSV_H(ink) && HSV_H(ink) <= mikata.H + H_LU) {
									mikata.men++;
									mikata_ink.at<Vec3b>(y, x)[0] = ink[0];
									mikata_ink.at<Vec3b>(y, x)[1] = ink[1];
									mikata_ink.at<Vec3b>(y, x)[2] = ink[2];
								}
								else if (teki.H - H_LU <= HSV_H(ink) && HSV_H(ink) <= teki.H + H_LU) {
									teki.men++;
									teki_ink.at<Vec3b>(y, x)[0] = ink[0];
									teki_ink.at<Vec3b>(y, x)[1] = ink[1];
									teki_ink.at<Vec3b>(y, x)[2] = ink[2];
								}
								break;
							}
						}
					}
					else if (G == 1.000) {
						if (GRAY_UP < B || GRAY_UP < R) {
							switch (method) {
							case BISECTION:
								if (mikata.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= mikata.HRange[1] ||
									mikata.HRange[0] <= HSV_H(ink) + 360.0 && HSV_H(ink) + 360.0 <= mikata.HRange[1]) {
									mikata.men++;
									mikata_ink.at<Vec3b>(y, x)[0] = ink[0];
									mikata_ink.at<Vec3b>(y, x)[1] = ink[1];
									mikata_ink.at<Vec3b>(y, x)[2] = ink[2];
								}
								else if (teki.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= teki.HRange[1] ||
									teki.HRange[0] <= HSV_H(ink) + 360.0 && HSV_H(ink) + 360.0 <= teki.HRange[1]) {
									teki.men++;
									teki_ink.at<Vec3b>(y, x)[0] = ink[0];
									teki_ink.at<Vec3b>(y, x)[1] = ink[1];
									teki_ink.at<Vec3b>(y, x)[2] = ink[2];

								}
								break;
							case TOLERANCE:
								if (mikata.H - H_LU <= HSV_H(ink) && HSV_H(ink) <= mikata.H + H_LU) {
									mikata.men++;
									mikata_ink.at<Vec3b>(y, x)[0] = ink[0];
									mikata_ink.at<Vec3b>(y, x)[1] = ink[1];
									mikata_ink.at<Vec3b>(y, x)[2] = ink[2];
								}
								else if (teki.H - H_LU <= HSV_H(ink) && HSV_H(ink) <= teki.H + H_LU) {
									teki.men++;
									teki_ink.at<Vec3b>(y, x)[0] = ink[0];
									teki_ink.at<Vec3b>(y, x)[1] = ink[1];
									teki_ink.at<Vec3b>(y, x)[2] = ink[2];
								}
								break;
							}
						}
					}
					else if (R == 1.000) {
						if (GRAY_UP < G || GRAY_UP < B) {
							switch (method) {
								if (mikata.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= mikata.HRange[1] ||
									mikata.HRange[0] <= HSV_H(ink) + 360.0 && HSV_H(ink) + 360.0 <= mikata.HRange[1]) {
									mikata.men++;
									mikata_ink.at<Vec3b>(y, x)[0] = ink[0];
									mikata_ink.at<Vec3b>(y, x)[1] = ink[1];
									mikata_ink.at<Vec3b>(y, x)[2] = ink[2];
								}
								else if (teki.HRange[0] <= HSV_H(ink) && HSV_H(ink) <= teki.HRange[1] ||
									teki.HRange[0] <= HSV_H(ink) + 360.0 && HSV_H(ink) + 360.0 <= teki.HRange[1]) {
									teki.men++;
									teki_ink.at<Vec3b>(y, x)[0] = ink[0];
									teki_ink.at<Vec3b>(y, x)[1] = ink[1];
									teki_ink.at<Vec3b>(y, x)[2] = ink[2];

								}
								break;
							case TOLERANCE:
								if (mikata.H - H_LU <= HSV_H(ink) && HSV_H(ink) <= mikata.H + H_LU) {
									mikata.men++;
									mikata_ink.at<Vec3b>(y, x)[0] = ink[0];
									mikata_ink.at<Vec3b>(y, x)[1] = ink[1];
									mikata_ink.at<Vec3b>(y, x)[2] = ink[2];
								}
								else if (teki.H - H_LU <= HSV_H(ink) && HSV_H(ink) <= teki.H + H_LU) {
									teki.men++;
									teki_ink.at<Vec3b>(y, x)[0] = ink[0];
									teki_ink.at<Vec3b>(y, x)[1] = ink[1];
									teki_ink.at<Vec3b>(y, x)[2] = ink[2];
								}
								break;
							}
						}
					}
				}
			}
		}

		imshow("teki_ink", teki_ink);
		imshow("mikata_ink", mikata_ink);
		imwrite("ink1.png", teki_ink);
		imwrite("ink0.png", mikata_ink);
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