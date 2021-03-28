
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "../practice31/material.h"
class Geometry;
class Vec3;
class World;

/*===================================================================
����,����״�Ͳ��ʹ���
====================================================================*/
class Object
{
public:

	//���캯��
	Object(const Material& material, Geometry* geometry);
	~Object();


	//���ز���
	const Material& getMaterial() const { return m_material; }

	//������״
	Geometry& getGeometry() { return *m_geometry; }

private:
	Material m_material;//����
	Geometry* m_geometry;//��״

};



#endif //__OBJECT_H__
