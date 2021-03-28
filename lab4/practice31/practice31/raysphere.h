
#ifndef __RAYSPHERE_H__
#define __RAYSPHERE_H__

#include "geometry.h"
#include "../practice31/vec3.h"


/*====================================================================
��������
====================================================================*/
class RaySphere : public Geometry
{
public:
	//���캯��
	RaySphere(const Vec3& pos_, float radius_)
	{
		m_centerpos = pos_;
		m_radius = radius_;
		m_radiusSquared = radius_ * radius_;
		m_recipRadius = 1.0f / m_radius;
	}

	~RaySphere() {}

	//���ش�������������ߵ�������ľ��룬�����ߺ�����û�н��㣬���ظ�ֵ
	//ע�⣬�������ڲ���������������棬����Ϊû�н���
	virtual float getDistanceUntilHit(const Ray& ray) const;

	//����������pos���ķ�����
	virtual const Vec3 getNormalForPos(const Vec3& pos) const;

	//����
	Vec3 m_centerpos;

private:

	float m_radius;//�뾶

	float m_radiusSquared;//�뾶��ƽ��

	float m_recipRadius;//�뾶�ĵ���
};





#endif //__RAYSPHERE_H__