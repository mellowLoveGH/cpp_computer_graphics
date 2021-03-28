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

//窗口宽，高
static int gl_width;
static int gl_height;

//模型
static Lib3dsFile *g_model = 0;

//纹理
static GLuint *texName;

//矩阵
static float ArmComT[4][4], HandComT[4][4]; //胳膊,手的变换矩阵
static float RobotT[4][4], RobotR[4][4], RobotS[4][4];//身体的偏移,旋转,缩放矩阵

//光标所指像素
static vec2f g_pixel;

//初始化以上各变量
void MyInit();

//查找名字为name的节点
Lib3dsNode* GetNodeByName(Lib3dsNode*node, const char*name);

//获取节点矩阵
bool GetNodeMatrix(float(*M)[4], Lib3dsNode *node);

//设置节点矩阵
bool SetNodeMatrix(float(*M)[4], Lib3dsNode *node);

//更新节点的矩阵
void UpdateMatrix(Lib3dsNode *node);

//绘制节点
void RenderNode(Lib3dsNode *node);

//绘制模型
void Draw(Lib3dsFile *model);

//响应键盘按键
void Keyboard(unsigned char uchKey, int iXPos, int iYPos);

//响应特殊按键
void Special(int iKey, int iXPos, int iYPos);

//响应鼠标按键
void Mouse(int iButton, int iState, int iXPos, int iYPos);

//响应按下一个鼠标按钮移动鼠标
void Motion(int iXPos, int iYPos);

//响应窗口形状改变
void Reshape(int iWidth, int iHeight);

//初始化
void Init();

//绘制回调函数
void Display();

#endif