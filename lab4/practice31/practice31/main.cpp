
#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include "letter.h"

#include "../practice31/colour.h"
#include "../practice31/material.h"
#include "../practice31/rayplane.h"
#include "../practice31/raysphere.h"
#include "../practice31/triangle.h"
#include "../practice31/object.h"
#include "../practice31/light.h"
#include "scene.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define defSCREEN_WIDTH 700								
#define defSCREEN_HEIGHT 700		

float lx = 50;

//��Ļ��������
GLubyte g_abyScreenImage[defSCREEN_HEIGHT][defSCREEN_WIDTH][3];

Scene s(defSCREEN_WIDTH, defSCREEN_HEIGHT);

void createScene() {
	//�������͹�Դ
	s.clear();
	s.setCamPos(0, 3, 40);
	Object*sphere;
	Light*light;
	float planedist = -7;

	//������
	{
		Material mat(Colour(0.2f, 0.2f, 0.2f), Colour(0.5f, 0.5f, 0.5f), Colour(0.5f, 0.5f, 0.5f), 100.0f);

		Triangle* triangle = new Triangle(Vec3(-15, -6, -45), Vec3(20, 2, -40), Vec3(12, 16, -36));

		Object* trg = new Object(mat, triangle);

		s.insertObject(trg);
	}
	//������
	float par0 = 0.6;
	float par = 0.01;
	float par1 = 1.0;
	{
		Material mat(Colour(0.2f, 0.2f, 0.2f).operator*(par0), Colour(0.5f, 0.5f, 0.5f).operator*(par), Colour(0.5f, 0.5f, 0.5f).operator*(par1), 100.0f);

		Triangle* triangle = new Triangle(Vec3(30, -9, 30), Vec3(-30, -9, 30), Vec3(30, 0, -100));

		Object* trg = new Object(mat, triangle);

		s.insertObject(trg);
	}
	//������
	{
		Material mat(Colour(0.2f, 0.2f, 0.2f).operator*(par0), Colour(0.5f, 0.5f, 0.5f).operator*(par), Colour(0.5f, 0.5f, 0.5f).operator*(par1), 100.0f);

		Triangle* triangle = new Triangle(Vec3(-30, -9, 30), Vec3(-30, 0, -100), Vec3(30, 0, -100));

		Object* trg = new Object(mat, triangle);

		s.insertObject(trg);
	}
	//����
	{

		Material mat(Colour(0.2f, 0.0f, 0.0f), Colour(0.6f, 0.0f, 0.0f), Colour(0.6f, 0.5, 0.5f), 100.0f);

		RaySphere* geom = new RaySphere(Vec3(2, planedist + 8, -35), 3);

		sphere = new Object(mat, geom);

		s.insertObject(sphere);
	}

	//����
	{

		Material mat(Colour(0.2f, 0.2f, 0.0f), Colour(0.6f, 0.6f, 0.0f), Colour(0.6f, 0.6f, 0.5f), 100.0f);

		RaySphere* geom = new RaySphere(Vec3(0, planedist + 3, -20), 3.2);

		sphere = new Object(mat, geom);

		s.insertObject(sphere);
	}

	//����
	{
		Material mat(Colour(0.0f, 0.0f, 0.2f), Colour(0.0f, 0.0f, 0.6f), Colour(0.5f, 0.5f, 0.6f), 100.0f);

		RaySphere* geom = new RaySphere(Vec3(10, planedist + 9.5, -30), 4);

		sphere = new Object(mat, geom);

		s.insertObject(sphere);
	}
	//��Դ
	light = new Light(Vec3(lx, 100, 20), Colour(1, 1, 1));
	s.insertLight(light);
	light = new Light(Vec3(0, 3, 20), Colour(0.4, 0.4, 0.4));
	s.insertLight(light);

}


void myInit() {
	s.setTraceNum(4);
	createScene();
	s.draw();
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
		}
	}
}

//�ı���Ļ��������
void ModifyScreenImage()
{
	for (int i = 0; i < defSCREEN_HEIGHT; i++) {
		for (int j = 0; j < defSCREEN_WIDTH; j++) {
			g_abyScreenImage[i][j][0] = s.m_screenBuffer[i][j][0];
			g_abyScreenImage[i][j][1] = s.m_screenBuffer[i][j][1];
			g_abyScreenImage[i][j][2] = s.m_screenBuffer[i][j][2];
		}
	}
}

//��Ӧ���̰���
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	if (uchKey == 27) {
		exit(0);
	}
	else if (uchKey <= '9'&&uchKey >= '0') {
		printf("%d\n", uchKey - '0' + 0);
		s.setTraceNum(uchKey - '0' + 0);
		s.draw();
		glutPostRedisplay();
	}
	else if (uchKey == 'a' || uchKey == 'A') {
		s.shutdown();
		lx = -50;
		createScene();
		s.draw();
		glutPostRedisplay();
	}
	else if (uchKey == 's' || uchKey == 'S') {
		s.shutdown();
		lx = 50;
		createScene();
		s.draw();
		glutPostRedisplay();
	}
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
	createScene();
}

//���ƻص�����
void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//InitScreenImage();
	ModifyScreenImage();
	glRasterPos2f(-1, -1);
	glDrawPixels(defSCREEN_WIDTH, defSCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, g_abyScreenImage);
	float y = 1, d = 40 / (float)defSCREEN_HEIGHT;
	y -= d;
	glRasterPos2f(-1, y);
	drawCNString("���ּ�ѡ����ٴ���");
	y -= d;
	glRasterPos2f(-1, y);
	drawCNString("a,s���ƹ�Դλ��");

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	myInit();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(defSCREEN_WIDTH, defSCREEN_HEIGHT);
	glutCreateWindow("���߸���");
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

