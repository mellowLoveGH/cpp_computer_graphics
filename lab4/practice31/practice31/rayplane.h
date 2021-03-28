
#ifndef __RAYPLANE_H__
#define __RAYPLANE_H__

#include "geometry.h"
#include "../practice31/plane.h"

/*====================================================================
����ƽ��
====================================================================*/

class RayPlane : public Geometry
{
public:
	//���캯��
	RayPlane(const Plane& plane);

	~RayPlane() {}

	//���ش�������������ߵ���ƽ��ľ��룬�����ߺ�ƽ��û�н��㣬���ظ�ֵ
	//ע�⣬�����ƽ���з������ߴ�ƽ�桰���󡱴���ƽ�棬����Ϊû�н���
	//���߰�����ƽ�汻��Ϊû�н���
	virtual float getDistanceUntilHit(const Ray& ray) const;

	//���ط���������Ϊ��ƽ�棬����Ҫ����pos
	virtual const Vec3 getNormalForPos(const Vec3& pos) const {
		return m_plane.getNormal();
	}

private:
	//ƽ��
	Plane m_plane;
};


#endif //__RAYPLANE_H__