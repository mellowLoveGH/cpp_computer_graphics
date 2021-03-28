
#include "curve.h"
#include <math.h>
#include <glut.h>
#include<iostream>
using namespace std;

//计算n!/(k!(n-k)!),存储到c[k],其中k=0,1,...,n
void BezierCurve::binomiaCoeffs(int n, float *c) {

	int k, i;

	for (k = 0; k <= n; k++) {
		//计算n!/(k!(n-k)!)
		c[k] = 1;
		for (i = n; i >= k + 1; i--)
			c[k] *= i;
		for (i = n - k; i >= 2; i--)
			c[k] /= i;
	}
}


//Bezier曲线中,起点为B(0),终点为B(1),该函数计算点B(u)的坐标，其中0<=u<=1;
//bezPt 指向B(u)的指针,
//nCtrlPts 控制点个数
//ctrlPts 控制点内存起始地址
void BezierCurve::computeBezPt(float u, wcPt3D *bezPt, int nCtrlPts, wcPt3D *ctrlPts, float *C) {
	//
	wcPt3D t;
	t.x = 0.0;
	t.y = 0.0;
	t.z = 0.0;

	int j;
	for (j = 0; j < nCtrlPts; j++) {
		float m = C[j] * powf(u, j) * powf(1 - u, nCtrlPts - 1 - j);
		t.x = t.x + m * ctrlPts[j].x;
		t.y = t.y + m * ctrlPts[j].y;
		//cout << nCtrlPts << endl;
		//cout << "m, " << m << endl;
	}

	bezPt[0] = t;
}


//计算bezier曲线
//ctrlPts 控制点数组内存起始地址
//nCtrlPts 控制点个数
//bezCurvePts 曲线点数组内存起始地址
//nBezCurvePts 曲线点个数
void BezierCurve::bezier(wcPt3D *ctrlPts, int nCtrlPts, wcPt3D *bezCurvePts, int nBezCurvePts) {

	if (nCtrlPts == 0) {
		return;
	}


	if (nBezCurvePts > 1) {

		float *C = (float*)malloc((nCtrlPts + 1) * sizeof(float));
		binomiaCoeffs(nCtrlPts - 1, C);
		//
		float delta = 1.0 / (nBezCurvePts - 1);
		cout << delta << endl;
		float u = 0.0;
		//		
		//
		wcPt3D *t = (wcPt3D*)malloc(1 * sizeof(wcPt3D));
		int i;
		for (i = 0; i < nBezCurvePts; i++) {
			//cout << nBezCurvePts << " , " << i << " , " << u << endl;

			/*
			//
			wcPt3D t;
			t.x = 0.0;
			t.y = 0.0;
			t.z = 0.0;
			//
			int j;
			for (j = 0; j < nCtrlPts; j++) {
				float m = C[j] * powf(u, j) * powf(1 - u, nCtrlPts - 1 - j);
				t.x = t.x + m * ctrlPts[j].x;
				t.y = t.y + m * ctrlPts[j].y;
				//cout << nCtrlPts << endl;
				//cout << "m, " << m << endl;
			}
			//
			//cout << " x, " << t.x << " y, " << t.y << endl;
			*/
			
			computeBezPt(u, t, nCtrlPts, ctrlPts, C);

			bezCurvePts[i] = t[0];
			u = u + delta;

		}

	}

}
