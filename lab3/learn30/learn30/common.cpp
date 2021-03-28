
#include "common.h"

//�������
float dot(float A[3], float B[3]) {
	return(A[0] * B[0] + A[1] * B[1] + A[2] * B[2]);
}

//�������.�����Ĳ���ķ���������������������ֱ��Ҳ��������������������ƽ��ķ�����
void cross(float R[3], float A[3], float B[3]) {
	R[0] = A[1] * B[2] - A[2] * B[1];
	R[1] = A[2] * B[0] - A[0] * B[2];
	R[2] = A[0] * B[1] - A[1] * B[0];
}

//��ƽ��P(u,w)=A+uB+wC��ֱ��Q(t)=D+tE�Ľ���R 
//ע�⺯�������ĺ���
//A�ǵ������; B,C�ǲ�ƽ�еķ�������;  u,w��(-��,+��) ;A+uB+wCȷ��һ��ƽ��
//D�ǵ������; E�Ƿ�������; t��(-��,+��); D+tE ȷ��һ��ֱ��
int facejoin(float R[3], float A[3], float B[3], float C[3], float D[3], float E[3])
{
	float Temp[3], t;

	cross(Temp, B, C);

	if (dot(Temp, E) == 0)
		return -1;//���Ϊ0,E��ƽ�淨������ֱ,��ֱ����ƽ��ƽ��,�޽���(����ֱ�߱�ƽ�����)

	t = (dot(Temp, A) - dot(Temp, D)) / dot(Temp, E);

	R[0] = D[0] + t * E[0];
	R[1] = D[1] + t * E[1];
	R[2] = D[2] + t * E[2];
	return 1;
}
