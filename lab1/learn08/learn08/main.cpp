
#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include "common.h"
#include "letter.h"
#include "line.h"
#include "curve.h"
#include<iostream>
using namespace std;

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

//视口大小
#define defSCREEN_WIDTH 700								
#define defSCREEN_HEIGHT 700								

//像素颜色
static GLubyte g_iColorRed, g_iColorGreen, g_iColorBlue;

//屏幕像素数组
GLubyte g_abyScreenImage[defSCREEN_HEIGHT][defSCREEN_WIDTH][3];

int g_iCtrlPntNum,//bezier曲线的控制点个数
g_iPntNum;//构成bezier曲线的点的个数
wcPt3D *g_pCtrlPoints,//控制点
*g_pPoints;//曲线上的点

BezierCurve g_curve;//bezier曲线
bool g_bSelectPoint;//按下空格键选择一个控制点，该变量被置为true
bool g_bFinish;//按下'm'键，结束控制点的选择，该变量被置为true

int iSrcX, iSrcY, iDestX, iDestY;//直线起点，终点像素坐标

unsigned short g_lineType;//线条类型
static int tp;

//初始化
void MyInit(void)
{
	g_iCtrlPntNum = 0;
	g_iPntNum = 0;

	g_pCtrlPoints = (wcPt3D*)malloc(50 * sizeof(wcPt3D));

	g_pPoints = (wcPt3D*)malloc(200 * sizeof(wcPt3D));

	g_curve.SetPoints(g_pPoints, g_iPntNum);
	g_curve.SetCtrlPoints(g_pCtrlPoints, g_iCtrlPntNum);

	g_bFinish = false;
	g_bSelectPoint = false;

	g_lineType = SOLID_LINE;
	tp = 0;
}

//颜色
void setColour(int r, int g, int b)
{
	g_iColorRed = r;
	g_iColorGreen = g;
	g_iColorBlue = b;
}

//像素绘制
void setPixel(int i, int j)
{
	static unsigned short t = 0x8000;
	static unsigned short t1 = t;
	static int nb = 8 * sizeof(unsigned short);
	static int tx, ty;

	t1 = t >> tp;
	tp = (tp + 1) % nb;
	if ((g_lineType & t1) != 0)
	{
		tx = i;
		ty = j;

		if (0 <= ty && ty < defSCREEN_HEIGHT && 0 <= tx && tx < defSCREEN_WIDTH) {
			g_abyScreenImage[ty][tx][0] = g_iColorRed;
			g_abyScreenImage[ty][tx][1] = g_iColorGreen;
			g_abyScreenImage[ty][tx][2] = g_iColorBlue;
		}
	}
}

//线条类型
void setLineType(unsigned short type)
{
	g_lineType = type;
	tp = 0;
}

//初始化屏幕像素数组为全黑
void InitScreenImage()
{
	int iHeight, iWidth;
	for (iHeight = 0; iHeight < defSCREEN_HEIGHT; iHeight++) {
		for (iWidth = 0; iWidth < defSCREEN_WIDTH; iWidth++) {
			g_abyScreenImage[iHeight][iWidth][0] = 0;
			g_abyScreenImage[iHeight][iWidth][1] = 0;
			g_abyScreenImage[iHeight][iWidth][2] = 0;
		}
	}
}

//修改图像
void ModifyScreenImage()
{
	static int i;
	static int dsx, dsy;

	//白色虚线
	setColour(255, 255, 255);
	setLineType(DASH_LINE);

	//绘制控制图
	for (i = 0; i < g_curve.GetCtrlPntNum() - 1; i++) {

		//这里为简单起见，三维空间坐标系中的点转换到平面坐标系的方法是：忽略点的z坐标，并将x，y坐标转换为最接近它们的整数
		iSrcX = ROUND(g_pCtrlPoints[i].x);
		iSrcY = ROUND(g_pCtrlPoints[i].y);
		iDestX = ROUND(g_pCtrlPoints[i + 1].x);
		iDestY = ROUND(g_pCtrlPoints[i + 1].y);

		DDA(iSrcX, iSrcY, iDestX, iDestY, setPixel);
		//Bres(iSrcX,iSrcY,iDestX,iDestY,setPixel);
	}

	//红色实线
	setColour(255, 0, 0);
	setLineType(SOLID_LINE);

	//绘制bezier曲线
	for (i = 0; i < g_curve.GetPntNum() - 1; i++) {

		iSrcX = ROUND(g_pPoints[i].x);
		iSrcY = ROUND(g_pPoints[i].y);
		iDestX = ROUND(g_pPoints[i + 1].x);
		iDestY = ROUND(g_pPoints[i + 1].y);

		DDA(iSrcX, iSrcY, iDestX, iDestY, setPixel);
		//Bres(iSrcX,iSrcY,iDestX,iDestY,setPixel);
	}
}

int g_f = 5;//曲线点数与控制点数的比例因子
//响应键盘按键
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	if (iXPos < 0 || iXPos >= defSCREEN_WIDTH || iYPos < 0 || iYPos >= defSCREEN_HEIGHT)
		return;

	if (uchKey == 27/*Esc*/) {
		exit(0);
	}
	else if (uchKey >= '1'&&uchKey <= '5')
	{
		g_f = int(uchKey - '0');
		g_curve.SetPoints(g_pPoints, (g_curve.GetCtrlPntNum() - 1)*g_f + 1);
		g_curve.bezier(g_curve.GetCtrlPoints(), g_curve.GetCtrlPntNum(), g_curve.GetPoints(), g_curve.GetPntNum());
		glutPostRedisplay();
	}

}

//响应特殊按键
void Special(int iKey, int iXPos, int iYPos)
{

}

//响应鼠标按键
void Mouse(int iButton, int iState, int iXPos, int iYPos)
{
	//cout << g_curve.GetPntNum() << endl;
	if (iButton == GLUT_LEFT_BUTTON && iState == GLUT_UP)
	{

		int i;
		for (i = 0; i < g_curve.GetCtrlPntNum() - 1; i++) {
			//cout << g_pCtrlPoints[i].x << ", " << g_pCtrlPoints[i].y << endl;
			//cout << g_pCtrlPoints[i + 1].x << ", " << g_pCtrlPoints[i + 1].y << endl;
		}


		if (g_bFinish) {//当所有控制点都已选择完，鼠标左键按下，重新选择控制点集
			g_iCtrlPntNum = 0;
			g_bFinish = false;
		}

		if (g_iCtrlPntNum >= 30)//过多的控制点，会导致计算值过大，甚至超出计算机所能表示的数的上限
			return;

		//设置新的控制点坐标
		g_pCtrlPoints[g_iCtrlPntNum].x = iXPos;
		g_pCtrlPoints[g_iCtrlPntNum].y = defSCREEN_HEIGHT - iYPos;
		g_pCtrlPoints[g_iCtrlPntNum].z = 0;
		g_iCtrlPntNum++;//控制点数加1

		g_iPntNum = (g_iCtrlPntNum - 1)*g_f + 1;//曲线上的点数与控制点数成正比
		g_curve.SetPoints(g_pPoints, g_iPntNum);//重新设置曲线点集和点数
		g_curve.SetCtrlPoints(g_pCtrlPoints, g_iCtrlPntNum);//重新设置控制点集和点数

		g_bSelectPoint = true;
	}
	else if (iButton == GLUT_RIGHT_BUTTON && iState == GLUT_UP)
	{
		if (!g_bFinish) {
			//控制点数不变，重新设置曲线相关属性值
			g_iPntNum = (g_iCtrlPntNum - 1)*g_f + 1;
			g_curve.SetPoints(g_pPoints, g_iPntNum);
			g_curve.SetCtrlPoints(g_pCtrlPoints, g_iCtrlPntNum);

			g_bFinish = true;
		}
	}
	g_curve.bezier(g_curve.GetCtrlPoints(), g_curve.GetCtrlPntNum(), g_curve.GetPoints(), g_curve.GetPntNum());

}

//响应移动鼠标
void PassiveMotionFunc(int iXPos, int iYPos);

//响应按下一个鼠标按钮移动鼠标
void Motion(int iXPos, int iYPos)
{
	PassiveMotionFunc(iXPos, iYPos);
}

//响应移动鼠标
void PassiveMotionFunc(int iXPos, int iYPos)
{
	if (!g_bFinish) {//如果没有完成控制点的选择

		//实时保存光标位置
		g_pCtrlPoints[g_iCtrlPntNum].x = iXPos;
		g_pCtrlPoints[g_iCtrlPntNum].y = defSCREEN_HEIGHT - iYPos;
		g_pCtrlPoints[g_iCtrlPntNum].z = 0;

		if (g_bSelectPoint) {//如果刚刚选择了一个控制点,当前的光标位置还不能成为新的控制点，但应该显示到屏幕上，以实现"拖动"的效果

			//另加上一个控制点
			g_iPntNum = g_iCtrlPntNum * g_f + 1;
			g_curve.SetPoints(g_pPoints, g_iPntNum);
			g_curve.SetCtrlPoints(g_pCtrlPoints, g_iCtrlPntNum + 1);
			g_bSelectPoint = false;

		}
		g_curve.bezier(g_curve.GetCtrlPoints(), g_curve.GetCtrlPntNum(), g_curve.GetPoints(), g_curve.GetPntNum());

	}
	glutPostRedisplay();

}

//响应窗口大小改变
void Reshape(int iWidth, int iHeight)
{
	glutReshapeWindow(defSCREEN_WIDTH, defSCREEN_HEIGHT);

}

//初始化
void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	InitScreenImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

//绘制回调函数
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
	drawCNString("单击鼠标左键选择控制点(不超过30个)，单击右键结束");
	y -= d;
	glRasterPos2f(-1, y);
	drawCNString("1,2,3,4,5 控制平滑度");

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	MyInit();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(defSCREEN_WIDTH, defSCREEN_HEIGHT);
	glutCreateWindow("Bezier曲线(2010)");
	Init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutMainLoop();
	return 0;
}


