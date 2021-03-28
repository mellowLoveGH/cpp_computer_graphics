

#ifndef __GEOMETRY_H_666_
#define __GEOMETRY_H_666_


class Ray;
class Vec3;
class World;
class Colour;

/*=====================================================================
������
--------
�ӿڣ�����object����״
=====================================================================*/
class Geometry
{
public:

	virtual ~Geometry() {}


	//���ش�������������ߵ��Ｘ����ľ��룬�����ߺͼ�����û�н��㣬���ظ�ֵ
	virtual float getDistanceUntilHit(const Ray& ray) const = 0;

	//���ؼ�������pos���ķ�����
	virtual const Vec3 getNormalForPos(const Vec3& pos) const = 0;
};



#endif //__GEOMETRY_H_666_




