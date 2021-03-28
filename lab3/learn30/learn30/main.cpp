
#include <windows.h>
#include <stdlib.h>
#include <glut.h>
#include "transformation.h"
#include "common.h"
#include "letter.h"
#include "line.h"
#include "scan.h"
#include <iostream>
using namespace std;
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

//�ӿڴ�С
#define defSCREEN_WIDTH 700								
#define defSCREEN_HEIGHT 700								

//��Ļ��������
GLubyte g_abyScreenImage[defSCREEN_HEIGHT][defSCREEN_WIDTH][3];
float ZB[defSCREEN_HEIGHT][defSCREEN_WIDTH];//��Ȼ���

#define PI 3.1415926

//��ɫ
typedef struct COLOR {
	COLOR(GLubyte nr = 255, GLubyte ng = 255, GLubyte nb = 255) :r(nr), g(ng), b(nb) {}
	GLubyte r, g, b;
}COLOR;

//������ɫ
COLOR g_color(255, 255, 255);

float g_vertex[10][3] = {
	{0,0,-50},{0,0,50},
	{0,200,0},
	{200,0,0},
	{0,-200,0},
	{-200,0,0},

	{50,50,0},
	{50,-50,0},
	{-50,-50,0},
	{-50,50,0}
};//����
float g_pvertex[10][3];//�任��Ķ���
float Txyz[4][4], Rxyz[4][4], Sxyz[4][4], ComT[4][4]; //ƽ��,��ת,����,���ϱ任���� 



//�Զ��庯��---------------------------------------------------------------------------

//����任
void VerTransfer(float pv[][3], float v[][3], int n, float m[4][4])
{
	static float tpv[4], tv[4];

	for (int i = 0; i < n; i++) {
		tv[0] = v[i][0];
		tv[1] = v[i][1];
		tv[2] = v[i][2];
		tv[3] = 1;
		MVMul(tpv, m, tv);
		pv[i][0] = tpv[0];
		pv[i][1] = tpv[1];
		pv[i][2] = tpv[2];
	}

}

//-------------------------------------------------------------------------------------

//��ʼ��
void MyInit()
{
	int i;
	float r[4][4];

	SetIdentity(Txyz);
	SetIdentity(Rxyz);
	SetIdentity(Sxyz);
	SetIdentity(ComT);
	SetIdentity(r);

	matTxyz(Txyz, 0, 0, 0);
	matSxyz(Sxyz, 1, 1, 1);

	matRx(r, PI / 6);//��x����ת30��
	MatMul(Rxyz, r, Rxyz);
	matRy(r, PI / 6);//��y����ת30��
	MatMul(Rxyz, r, Rxyz);
	matRz(r, PI / 6);//��z����ת30��
	MatMul(Rxyz, r, Rxyz);

	//�󸴺ϱ任����
	MatMul(ComT, Rxyz, ComT);
	MatMul(ComT, Sxyz, ComT);
	MatMul(ComT, Txyz, ComT);

	for (i = 0; i < 10; i++)
	{
		g_pvertex[i][0] = g_vertex[i][0];
		g_pvertex[i][1] = g_vertex[i][1];
		g_pvertex[i][2] = g_vertex[i][2];
	}
	VerTransfer(g_pvertex, g_vertex, 10, ComT);

}

//��������
void SetPixel(int x, int y)
{
	if (0 <= x && x < defSCREEN_WIDTH && 0 <= y && y < defSCREEN_HEIGHT) {
		g_abyScreenImage[y][x][0] = g_color.r;
		g_abyScreenImage[y][x][1] = g_color.g;
		g_abyScreenImage[y][x][2] = g_color.b;
	}
}
//�������
void SetDepth(int x, int y, float depth = 1.0)
{
	if (depth <= 1.0&&depth >= 0.0) {
		if (0 <= x && x < defSCREEN_WIDTH && 0 <= y && y < defSCREEN_HEIGHT) {
			ZB[y][x] = depth;
		}
	}

}
//������ɫ
inline void SetColor(GLubyte r, GLubyte g, GLubyte b)
{
	g_color.r = r;
	g_color.g = g;
	g_color.b = b;
}
//��ȡ���
float GetDepth(int x, int y)
{
	if (0 <= x && x < defSCREEN_WIDTH && 0 <= y && y < defSCREEN_HEIGHT) {
		return ZB[y][x];
	}
	return -1;
}
//�������ز��������Ϊ0.5
inline void SetPixelDepth(int x, int y) {
	SetPixel(x, y);
	SetDepth(x, y, 0.5);
}

//�ѵ��z����ֵת�������ֵ
inline float convertZToDepth(float z) {
	return (1000 - z) / 2000;
}

//����
void Draw(float v[][3])
{
	int i, j, a1;
	static int halWidth = defSCREEN_WIDTH / 2, halHeight = defSCREEN_HEIGHT / 2;
	static int faceindex[4][3] = { {0,1,2},{0,1,3},{0,1,4},{0,1,5} };
	static int faceindex1[1][8] = { {2,6,3,7,4,8,5,9} };
	static int sx, sy, ex, ey;
	static int coord[8][2];
	static float depth[8];
	COLOR c[4] = { COLOR(255,0,0),COLOR(0,255,0),COLOR(0,0,255),COLOR(255,255,0) };


	SetColor(100, 100, 100);
	//x��
	for (i = 20; i <= defSCREEN_WIDTH - 20; i++) {
		SetPixel(i, halHeight);
		SetDepth(i, halHeight, 0.5);
	}
	DDA(defSCREEN_WIDTH - 20, halHeight, defSCREEN_WIDTH - 40, halHeight + 20, SetPixelDepth);
	DDA(defSCREEN_WIDTH - 20, halHeight, defSCREEN_WIDTH - 40, halHeight - 20, SetPixelDepth);

	//y��
	for (j = 20; j <= defSCREEN_HEIGHT - 20; j++) {
		SetPixel(halWidth, j);
		SetDepth(halWidth, j, 0.5);
	}
	DDA(halWidth, defSCREEN_HEIGHT - 20, halWidth - 20, defSCREEN_HEIGHT - 40, SetPixelDepth);
	DDA(halWidth, defSCREEN_HEIGHT - 20, halWidth + 20, defSCREEN_HEIGHT - 40, SetPixelDepth);

	//z��
	DDA(halWidth, halHeight, halWidth + 300, halHeight + 300, SetPixelDepth);

	//ͼ��
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 3; j++)
		{
			a1 = faceindex[i][j];

			coord[j][0] = ROUND(g_pvertex[a1][0]) + halWidth;
			coord[j][1] = ROUND(g_pvertex[a1][1]) + halHeight;
			depth[j] = convertZToDepth(g_pvertex[a1][2]);
		}
		SetColor(c[i].r, c[i].g, c[i].b);
		//
		//cout << "draw triangle" << endl;
		linefill(coord, depth, 3);
	}
	for (i = 0; i < 1; i++)
	{
		for (j = 0; j < 8; j++)
		{
			a1 = faceindex1[i][j];

			coord[j][0] = ROUND(g_pvertex[a1][0]) + halWidth;
			coord[j][1] = ROUND(g_pvertex[a1][1]) + halHeight;
			depth[j] = convertZToDepth(g_pvertex[a1][2]);
		}
		SetColor(255, 255, 255);
		//
		//cout << "draw polygon" << endl;
		linefill(coord, depth, 8);
	}
}

//��ʼ����Ļ��������Ϊȫ��
void InitScreenImage()
{
	int iHeight, iWidth;
	int iColorRed, iColorGreen, iColorBlue;
	for (iHeight = 0; iHeight < defSCREEN_HEIGHT; iHeight++) {
		for (iWidth = 0; iWidth < defSCREEN_WIDTH; iWidth++) {
			iColorRed = 0;
			iColorGreen = 0;
			iColorBlue = 0;
			g_abyScreenImage[iHeight][iWidth][0] = (GLubyte)iColorRed;
			g_abyScreenImage[iHeight][iWidth][1] = (GLubyte)iColorGreen;
			g_abyScreenImage[iHeight][iWidth][2] = (GLubyte)iColorBlue;
			ZB[iHeight][iWidth] = 1.0;
		}
	}
}

//�ı���Ļ��������
void ModifyScreenImage()
{
	Draw(g_pvertex);
}


//��Ӧ���̰���
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	static float t[4][4];
	SetIdentity(t);
	if (uchKey == 27) {
		exit(0);
	}
	switch (uchKey) {
		//ƽ��
	case 'd':
	case 'D'://x����
		matTxyz(t, 4, 0, 0);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;
	case 'a':
	case 'A'://x��С
		matTxyz(t, -4, 0, 0);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;
	case 'w':
	case 'W'://y����
		matTxyz(t, 0, 4, 0);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;
	case 's':
	case 'S'://y��С
		matTxyz(t, 0, -4, 0);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;
	case 'q':
	case 'Q'://z����
		matTxyz(t, 0, 0, 4);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;
	case 'e':
	case 'E'://z��С
		matTxyz(t, 0, 0, -4);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;

		//����
	case 'z':
	case 'Z'://�Ŵ�
		matSxyz(t, 1.05, 1.05, 1.05);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;
	case 'x':
	case 'X'://��С
		matSxyz(t, 0.95, 0.95, 0.95);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;

		//��ת
	case 'l':
	case 'L'://��x��
		matRx(t, 0.1);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;
	case 'i':
	case 'I'://��y��
		matRy(t, 0.1);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;
	case 'j':
	case 'J'://��z��
		matRz(t, 0.1);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;
	case 'k':
	case 'K'://��ָ����
		matR(t, 0.1, 1, 1, 0);
		MatMul(ComT, t, ComT);
		VerTransfer(g_pvertex, g_pvertex, 10, t);
		break;
	}
	glutPostRedisplay();
}
//��Ӧ���ⰴ��
void Special(int iKey, int iXPos, int iYPos)
{

}

//��Ӧ��갴��
void Mouse(int iButton, int iState, int iXPos, int iYPos)
{

}

//��Ӧ����һ����갴ť�ƶ����
void Motion(int iXPos, int iYPos)
{

}

void Reshape(int iWidth, int iHeight)
{
	glutReshapeWindow(defSCREEN_WIDTH, defSCREEN_HEIGHT);
}

//��ʼ��
void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	InitScreenImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

//���ƻص�����
void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	InitScreenImage();
	ModifyScreenImage();
	glRasterPos2f(-1, -1);
	glDrawPixels(defSCREEN_WIDTH, defSCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, g_abyScreenImage);
	float y = 1, d = 40 / (float)defSCREEN_HEIGHT;
	y -= d;
	glRasterPos2f(-1, y);
	drawCNString("ƽ��(w,a,s,d,q,e)");
	y -= d;
	glRasterPos2f(-1, y);
	drawCNString("��ת(l��x��,i��y��,j��z��,k��ָ����)");
	y -= d;
	glRasterPos2f(-1, y);
	drawCNString("����(z,x)");

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	MyInit();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(defSCREEN_WIDTH, defSCREEN_HEIGHT);
	glutCreateWindow("ɨ�������+����(2010)");
	Init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
	return 0;
}

