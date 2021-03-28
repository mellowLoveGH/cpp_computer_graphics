
#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
#include "../practice31/vec3.h"

class Vec3;
class Object;
class Light;
class Ray;
class Colour;

/*====================================================================
World
�ṩ�����ļ��㣨���߸��٣���������ԭ��Ĺ����ȣ�
====================================================================*/
class World
{
public:
	World();
	~World();

	//ͨ�����߸��ٻ������ray����ɫ
	//��ray����������������ôray��������������ɫ����colour_out
	void getColourForRay(const Ray& ray, Colour& colour_out, int traceNum);


	//����һ������
	void insertObject(Object* ob);

	//����һ����Դ
	void insertLight(Light* light);

	//���ظ��ٴ���
	int getTraceNum() { return m_traceNum; }

	//���ø��ٴ���
	void setTraceNum(int traceNum);

	//�����������͹�Դ
	void clear();


private:

	//������ray�ཻ�������Object,����ray����뽻��ľ������dist����û��Object��ray�ཻ������NULL,distΪ����
	Object* closestObject(const Ray& ray, float& dist);

	std::vector<Object*> m_objects;//����

	std::vector<Light*> m_lights;//��Դ

	int m_traceNum;//���ٴ���
};


#endif //__WORLD_H__