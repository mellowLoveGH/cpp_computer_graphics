#pragma once

#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <lib3ds.h>
//#include <GL/glut.h>
#include <glut.h>
#include "common.h"
#include "transformation.h"
#include "component.h"

//
#include <string.h>

#define defSCREEN_WIDTH 700								
#define defSCREEN_HEIGHT 700								

#define PI  3.141592653589793
#define PI2 6.283185307179586

//���ڿ���
static int gl_width;
static int gl_height;

//ģ��
static Lib3dsFile *g_model = 0;

//����
static GLuint *texName;

//����
static float ArmComT[4][4], HandComT[4][4]; //�첲,�ֵı任����
static float RobotT[4][4], RobotR[4][4], RobotS[4][4];//�����ƫ��,��ת,���ž���

//�����ָ����
static vec2f g_pixel;

//��ʼ�����ϸ�����
void MyInit();

//��������Ϊname�Ľڵ�
Lib3dsNode* GetNodeByName(Lib3dsNode*node, const char*name);

//��ȡ�ڵ����
bool GetNodeMatrix(float(*M)[4], Lib3dsNode *node);

//���ýڵ����
bool SetNodeMatrix(float(*M)[4], Lib3dsNode *node);

//���½ڵ�ľ���
void UpdateMatrix(Lib3dsNode *node);

//���ƽڵ�
void RenderNode(Lib3dsNode *node);

//����ģ��
void Draw(Lib3dsFile *model);

//��Ӧ���̰���
void Keyboard(unsigned char uchKey, int iXPos, int iYPos);

//��Ӧ���ⰴ��
void Special(int iKey, int iXPos, int iYPos);

//��Ӧ��갴��
void Mouse(int iButton, int iState, int iXPos, int iYPos);

//��Ӧ����һ����갴ť�ƶ����
void Motion(int iXPos, int iYPos);

//��Ӧ������״�ı�
void Reshape(int iWidth, int iHeight);

//��ʼ��
void Init();

//���ƻص�����
void Display();

#endif