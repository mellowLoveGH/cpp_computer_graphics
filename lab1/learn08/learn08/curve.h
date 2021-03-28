#pragma once
#ifndef CURVE_H
#define CURVE_H
#include<stdlib.h>
#include "common.h"


/*=====================================================================
三维直角坐标系中的曲线
计算机中的曲线是由多条直线首尾相连近似得到。
同样长度的曲线，构成它的直线越多，越短，曲线越逼真
=====================================================================*/
class Curve {

public:

	//构造函数
	Curve(wcPt3D *pPoints, int iPntNum) {
		m_pPoints = pPoints;
		m_iPntNum = iPntNum;
	}
	Curve() {
		m_iPntNum = 0;
		m_pPoints = 0;
	}

	//返回点数
	int GetPntNum() {
		return m_iPntNum;
	}

	//返回点集
	wcPt3D* GetPoints() {
		return m_pPoints;
	}

	//设置点集
	void SetPoints(wcPt3D *pPoints, int iPntNum) {
		m_pPoints = pPoints;
		m_iPntNum = iPntNum;
	}

private:

	wcPt3D *m_pPoints;//曲线点集，相邻两个点构成一条直线
	int m_iPntNum;//点的个数

};


/*=====================================================================
bezier曲线类,继承自curve类.
有一组控制点，用来控制曲线的形状
=====================================================================*/
class BezierCurve :public Curve {

	//成员函数
public:

	//构造函数
	BezierCurve(wcPt3D *pCtrlPoints, int iCtrlPntNum, wcPt3D *pPoints, int iPntNum) :Curve(pPoints, iPntNum) {//构造函数
		m_pCtrlPoints = pCtrlPoints;
		m_iCtrlPntNum = iCtrlPntNum;
	}
	BezierCurve() :Curve() {
		m_iCtrlPntNum = 0;
		m_pCtrlPoints = 0;
	}

	//返回控制点数
	int GetCtrlPntNum() {
		return m_iCtrlPntNum;
	}

	//返回控制点集
	wcPt3D* GetCtrlPoints() {
		return m_pCtrlPoints;
	}

	//设置控制点集
	void SetCtrlPoints(wcPt3D *pCtrlPoints, int iCtrlPntNum) {
		m_pCtrlPoints = pCtrlPoints;
		m_iCtrlPntNum = iCtrlPntNum;
	}

	//计算bezier曲线
	void bezier(wcPt3D *ctrlPts, int nCtrlPts, wcPt3D *bezCurvePts, int nBezCurvePts);

private:

	//计算n!/(k!(n-k)!),存储到c[k],其中k=0,1,...,n
	void binomiaCoeffs(int n, float *c);

	//Bezier曲线中,起点为B(0),终点为B(1),该函数计算点B(u)的坐标，其中0<=u<=1;
	void computeBezPt(float u, wcPt3D *bezPt, int nCtrlPts, wcPt3D *ctrlPts, float *C);


	//成员变量
private:

	wcPt3D *m_pCtrlPoints;//控制点集
	int m_iCtrlPntNum;//控制点个数,不能超过30
};

#endif