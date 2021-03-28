#include "common.h"
#include "math.h"
#include "scan.h"
#include <iostream>
using namespace std;
//��ɨ����Y=y���AB�Ľ����x���꣬����޽��㣬����-99999
int linejoin(int A[2], int B[2], int y)
{
	float t;
	if (B[1] == A[1])
		return -99999;

	t = (y - A[1]) / float(B[1] - A[1]);
	if (t >= 0.0 && t <= 1.0) {
		return ROUND(A[0] + (B[0] - A[0])*t);
	}
	else
		return -99999;
}

//�����õ��ĺ���
void SetPixel(int x, int y);
float GetDepth(int x, int y);
void SetDepth(int x, int y, float depth);


int isJointVertex(int x, int y) {

	return 0;
}


//���������㷨,Zbuffer�㷨
//coord ��������
//depth ���ض�Ӧ�����
//n ���ظ���
void linefill(int coord[][2], float depth[], int n)
{	
	
	int miny, maxy;
	int X[10];
	float A[3], B[3], C[3], D[3], E[3], R[3];

	//ƽ�淽��P(u,w)=A+uB+wC
	for (int i = 0; i < 2; i++) {
		A[i] = coord[0][i];
		B[i] = coord[1][i] - coord[0][i];
		C[i] = coord[2][i] - coord[0][i];
	}
	A[2] = depth[0];
	B[2] = depth[1] - depth[0];
	C[2] = depth[2] - depth[0];

	//ֱ�߷���Q(t)=(x0,y0,0)+(0,0,1)t 
	D[0] = D[1] = 0;//���ø�ֱ��ʱ,D[0],D[1]��Ҫ���¸�ֵ
	D[2] = 0;
	E[0] = E[1] = 0;
	E[2] = 1;

	// �ҳ�y����������С�Ķ��� 
	miny = coord[0][1]; maxy = coord[0][1];
	for (int i = 0; i < n; i++) {
		if (coord[i][1] < miny)
			miny = coord[i][1];
		if (coord[i][1] > maxy)
			maxy = coord[i][1];
	}

	// ʹɨ���ߴ�y������С�ĵ���1��y���Ķ��� 
	for (int y = miny; y <= maxy; y++)
	{
		//��Ӵ���
		//get all vertices
		int index = 0;
		for (int i = 0; i < n; i++) {
			int x = linejoin(coord[i], coord[(i + 1) % n], y);
			//
			if (coord[i][1] == coord[(i + 1) % n][1]) {
				
			}
			else {
				//rule that whether the vertex is counted or not
				if (y < coord[i][1] || y < coord[(i + 1) % n][1]) {
					//
					if (x != -99999) {
						X[index] = x;
						index++;						
					}
				}
			}			
		}

		//sort vertices
		sort(X, index);
		//cout << X[0] << ", " << X[1] << endl;
		if (index % 2 == 1) {
			cout << "problem! " << endl;
		}

		//draw
		for (int k = 0; k <= index / 2 - 1; k++) {
			for (int x = X[2 * k]; x <= X[2 * k + 1]; x++) {
				D[0] = x;
				D[1] = y;
				int flag = facejoin(R, A, B, C, D, E);
				if (flag == 1) {
					float d = R[2];
					if (d < GetDepth(x, y) && d >= 0.0 && d <= 1.0) {
						SetPixel(x, y);
						SetDepth(x, y, d);
					}
				}
				else {
					
				}
			}
		}
	}
}

