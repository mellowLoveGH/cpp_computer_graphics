
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "geometry.h"
#include "../practice31/vec3.h"

/*====================================================================
三角形
====================================================================*/

class Triangle : public Geometry
{
public:
	//构造函数
	Triangle(const Vec3 v1, const Vec3 v2, const Vec3 v3) :m_v1(v1), m_v2(v2), m_v3(v3) {}
	Triangle(const Triangle& triangle);

	~Triangle() {}

	//返回从射线起点沿射线到达三角形的距离，若射线和三角形没有交点，返回负值
	virtual float getDistanceUntilHit(const Ray& ray) const;

	//返回法相量。因为是三角形，不考虑pos
	virtual const Vec3 getNormalForPos(const Vec3& pos) const {
		Vec3 A = m_v2 - m_v1, B = m_v3 - m_v1;
		float a = A.x, b = A.y, c = A.z, d = B.x, e = B.y, f = B.z;
		return normalise(Vec3(b*f - c * e, c*d - a * f, a*e - b * d));
	}

private:
	Vec3 m_v1, m_v2, m_v3;
};


#endif 