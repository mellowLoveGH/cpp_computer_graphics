
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
������
=====================================================================*/
class Scene
{
public:

	//���캯����width��ȣ�height�߶�
	Scene(int width, int height);

	~Scene(void);


	//���������λ��
	void setCamPos(float x, float y, float z) { m_campos = Vec3(x, y, z); }

	//���ø��ٴ���
	void setTraceNum(int traceNum) { m_world->setTraceNum(traceNum); }

	//��������
	void insertObject(Object* ob);

	//�����Դ
	void insertLight(Light* light);

	//�������͹�Դ
	void clear() { m_world->clear(); }


	//���Ƴ���
	void draw();

	//�����ͷŲ��õ��ڴ�ռ�
	void shutdown();


	unsigned char***m_screenBuffer;//��������

private:

	//���ؾ�������Ϊ��x��y�������ص����߷���(��λ��)
	Vec3 getUnitDirForImageCoords(int x, int y);

	//������Ϊ��x��y����������ɫ����Ϊcolour
	void drawPixel(int x, int y, Colour colour);

	int m_iWidth, m_iHeight;//�����������

	World*	m_world;//m_worldʵ�ֹ��߸���

	Vec3 m_campos;//�����λ��
};
#endif
