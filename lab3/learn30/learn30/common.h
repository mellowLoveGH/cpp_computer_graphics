#pragma once
#ifndef COMMON_H
#define COMMON_H
//#pragma comment( linker, "/subsystem:windows /entry:WinMainCRTStartup" )
//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
//#pragma comment( linker, "/subsystem:console /entry:mainCRTStartup" )
//#pragma comment( linker, "/subsystem:console /entry:WinMainCRTStartup" )
//��������
#define SOLID_LINE	0xffff//ʵ��
#define DASH_LINE	0xf8f8//����
#define DOT_LINE	0x8888//����

#define ROUND(a) ((int)(a+0.5))

typedef struct {
	int x, y;
}wcPt2D;

typedef struct {
	float x, y, z;
}wcPt3D;

//��������
inline void swap(int& ia, int& ib)
{
	ia = ia ^ ib;
	ib = ib ^ ia;
	ia = ia ^ ib;
}

//��������
template<class T>
void sort(T a[], int n)
{
	int i, j, k;
	T temp;
	for (i = 0; i < n - 1; i++) {
		k = i;
		for (j = i + 1; j < n; j++)
			if (a[j] < a[k])
				k = j;
		temp = a[i];
		a[i] = a[k];
		a[k] = temp;
	}
}

//�������
float dot(float A[3], float B[3]);

//�������.�����Ĳ���ķ���������������������ֱ��Ҳ��������������������ƽ��ķ�����
void cross(float R[3], float A[3], float B[3]);

//��ƽ��P(u,w)=A+uB+wC��ֱ��Q(t)=D+tE�Ľ���R 
//ע�⺯�������ĺ���
//A�ǵ������; B,C�ǲ�ƽ�еķ�������;  u,w��(-��,+��) ;A+uB+wCȷ��һ��ƽ��
//D�ǵ������; E�Ƿ�������; t��(-��,+��); D+tE ȷ��һ��ֱ��
int facejoin(float R[3], float A[3], float B[3], float C[3], float D[3], float E[3]);
#endif