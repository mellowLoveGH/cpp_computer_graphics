
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "geometry.h"
#include "../practice31/vec3.h"

/*====================================================================
������
====================================================================*/

class Triangle : public Geometry
{
public:
	//���캯��
	Triangle(const Vec3 v1, const Vec3 v2, const Vec3 v3) :m_v1(v1), m_v2(v2), m_v3(v3) {}
	Triangle(const Triangle& triangle);

	~Triangle() {}

	//���ش�������������ߵ��������εľ��룬�����ߺ�������û�н��㣬���ظ�ֵ
	virtual float getDistanceUntilHit(const Ray& ray) const;

	//���ط���������Ϊ�������Σ�������pos
	virtual const Vec3 getNormalForPos(const Vec3& pos) const {
		Vec3 A = m_v2 - m_v1, B = m_v3 - m_v1;
		float a = A.x, b = A.y, c = A.z, d = B.x, e = B.y, f = B.z;
		return normalise(Vec3(b*f - c * e, c*d - a * f, a*e - b * d));
	}

private:
	Vec3 m_v1, m_v2, m_v3;
};


#endif 