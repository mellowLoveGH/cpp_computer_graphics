#include <math.h>
#include <float.H>
#include <stdio.h>
#include <iostream>
#include "transformation.h"
#include "line.h"

// 平移(tx,ty,tz)的矩阵Txyz 
void matTxyz(float Txyz[4][4], float tx, float ty, float tz)
{
	////添加代码
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Txyz[i][j] = (i == j ? 1 : 0);
	//
	Txyz[0][3] = tx;
	Txyz[1][3] = ty;
	Txyz[2][3] = tz;

}

// 绕x轴旋转theta角的矩阵Rx 
void matRx(float Rx[4][4], float theta)
{
	////添加代码
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Rx[i][j] = (i == j ? 1 : 0);
	//
	Rx[1][1] = cos(theta);
	Rx[2][2] = cos(theta);	
	Rx[1][2] = -sin(theta);
	Rx[2][1] = sin(theta);

}

// 绕y轴旋转theta角的矩阵Ry 
void matRy(float Ry[4][4], float theta)
{
	////添加代码
	////添加代码
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Ry[i][j] = (i == j ? 1 : 0);

	Ry[0][0] = cos(theta);
	Ry[0][2] = sin(theta);
	Ry[2][0] = -sin(theta);
	Ry[2][2] = cos(theta);
}

// 绕z轴旋转theta角的矩阵Rz 
void matRz(float Rz[4][4], float theta)
{
	////添加代码
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Rz[i][j] = (i == j ? 1 : 0);
	//
	Rz[0][0] = cos(theta);
	Rz[1][1] = cos(theta);
	Rz[0][1] = - sin(theta);
	Rz[1][0] = sin(theta);

}


//绕指定轴(起点(0,0,0),终点(x,y,z))旋转theta角的矩阵R
void matR(float R[4][4], float theta, float x, float y, float z)
{
	//normalized
	float L = sqrt(x * x + y * y + z * z);
	x = x / L;
	y = y / L;
	z = z / L;
	//std::cout << x << ", " << y << ", " << z;


	////添加代码
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			R[i][j] = (i == j ? 1 : 0);
	//
	float c = cos(theta);
	float s = sin(theta);
	float K = 1 - c;
	/*
	R[0][0] = x * x * K + c;
	R[0][1] = x * y * K - z * s;
	R[0][2] = x * z * K + y * s;

	R[1][0] = x * y * K + z * s;
	R[1][1] = y * y * K + c;
	R[1][2] = y * z * K - x * s;

	R[2][0] = x * z * K - y * s;
	R[2][1] = y * z * K + x * s;
	R[2][2] = z * z * K + c;
	*/

	//
	R[0][0] = x * x + (1 - x * x) * c;
	R[0][1] = x * y * K - z * s;
	R[0][2] = x * z * K + y * s;

	R[1][0] = x * y * K + z * s;
	R[1][1] = y * y + (1 - y * y) * c;
	R[1][2] = y * z * K - x * s;

	R[2][0] = x * z * K - y * s;
	R[2][1] = y * z * K + x * s;
	R[2][2] = z * z + (1 - z * z) * c;

}


// 缩放矩阵Sxyz
void matSxyz(float Sxyz[4][4], float sx, float sy, float sz)
{
	////添加代码
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Sxyz[i][j] = (i == j ? 1 : 0);
	//
	Sxyz[0][0] = sx;
	Sxyz[1][1] = sy;
	Sxyz[2][2] = sz;

}

// 4阶方阵相乘 R=A*B 
void MatMul(float R[4][4], float A[4][4], float B[4][4])
{
	int i, j, k;
	float TR[4][4];

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			TR[i][j] = 0;
			for (k = 0; k < 4; k++)
				TR[i][j] = TR[i][j] + A[i][k] * B[k][j];
		}
	}
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			R[i][j] = TR[i][j];
}

// 4阶方阵与4维向量相乘 PV=Mat*Vector 
void MVMul(float PV[], float Mat[][4], float Vector[])
{
	int i, j;
	for (i = 0; i < 4; i++) {
		PV[i] = 0;
		for (j = 0; j < 4; j++) {
			PV[i] = PV[i] + Mat[i][j] * Vector[j];
		}
	}
}

