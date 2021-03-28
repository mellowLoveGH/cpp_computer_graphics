
#ifndef __SCENE_H__
#define __SCENE_H__

#include "../practice31/world.h"
#include "../practice31/ray.h"
#include "../practice31/colour.h"
#include "../practice31/material.h"
#include "../practice31/rayplane.h"
#include "../practice31/raysphere.h"
#include "../practice31/object.h"
#include "../practice31/light.h"

/*=====================================================================
场景类
=====================================================================*/
class Scene
{
public:

	//构造函数，width宽度，height高度
	Scene(int width, int height);

	~Scene(void);


	//设置摄像机位置
	void setCamPos(float x, float y, float z) { m_campos = Vec3(x, y, z); }

	//设置跟踪次数
	void setTraceNum(int traceNum) { m_world->setTraceNum(traceNum); }

	//加入物体
	void insertObject(Object* ob);

	//加入光源
	void insertLight(Light* light);

	//清除物体和光源
	void clear() { m_world->clear(); }


	//绘制场景
	void draw();

	//用于释放不用的内存空间
	void shutdown();


	unsigned char***m_screenBuffer;//像素数组

private:

	//返回经过坐标为（x，y）的像素的射线方向(单位化)
	Vec3 getUnitDirForImageCoords(int x, int y);

	//将坐标为（x，y）的像素颜色设置为colour
	void drawPixel(int x, int y, Colour colour);

	int m_iWidth, m_iHeight;//像素数组宽，高

	World*	m_world;//m_world实现光线跟踪

	Vec3 m_campos;//摄像机位置
};
#endif
