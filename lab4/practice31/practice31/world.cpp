

#include "world.h"


#include "../practice31/object.h"
#include "../practice31/colour.h"
#include "../practice31/ray.h"
#include "../practice31/light.h"
#include "../practice31/geometry.h"
#include <assert.h>
#include <iostream>


const Colour ambient_lighting(0.5f, 0.5f, 0.5f);

World::World()
{
	m_traceNum = 5;
}

World::~World()
{
	for (unsigned int i = 0; i < m_objects.size(); ++i)
	{
		delete m_objects[i];
	}

	for (unsigned int z = 0; z < m_lights.size(); ++z)
	{
		delete m_lights[z];
	}
}


void World::insertObject(Object* ob)
{
	m_objects.push_back(ob);
}



void World::insertLight(Light* light)
{
	m_lights.push_back(light);
}



//������ray�ཻ�������Object,����ray����뽻��ľ������dist����û��Object��ray�ཻ������NULL,distΪ����
Object* World::closestObject(const Ray& ray, float& dist)
{
	Object* obj = NULL;
	float smallest_dist = 1e9f;

	for (unsigned int i = 0; i < m_objects.size(); i++)
	{
		const float tracedist = m_objects[i]->getGeometry().getDistanceUntilHit(ray);

		if (tracedist >= 0.0f && tracedist < smallest_dist)
		{
			smallest_dist = tracedist;
			obj = m_objects[i];
		}
	}
	if (obj == NULL)dist = -666;
	else dist = smallest_dist * 0.999f;
	return obj;
}

//��ȡ����ray����ɫ������colour_out��������traceNum��
void World::getColourForRay(const Ray& ray, Colour& colour_out, int traceNum)
{
	//��������(maths/vec3.h)
		//Vec3 v(x,y,z)������Vec3����(�ɱ�ʾ������),float x/y/z�ֱ��������(ֵ��[-��,+��])
		//���� Vec3 v,v1,v2,���ü���:v=v1,v=v1*float_number,v=v1+v2,v=v1-v2,v=v1*v2,v=normalise(v1),float dot_product=v1.dot(v2)

	//���ߣ���(raytracer/ray.h)
		//Ray xray(point,dir)������Ray����,Vec3 point,Vec3 dir�ֱ������,����

	//��ɫ����(raytracer/colour.h)
		//Colour colour(r,g,b); ����Colour����,float r/g/b�ֱ��Ǻ�/��/������(ֵ��[0.0f,1.0f]);
		//����Colour c,c1,c2,���ü���:c=c1,c=c1*float_number,c+=c1,c-=c1,c=c1+c2,c=c1-c2,c=c1*c2

	//���ʣ���(raytracer/material/material.h)

	//��״����(raytracer/geometry/geometry.h),ƽ��(raytracer/geometry/rayplane.h),����(raytracer/geometry/raysphere.h)

	//���壺��(raytracer/object.h).
		//Object* object=NULL; ����Objectָ��

	//��Դ����(raytracer/light.h)

	//---------------------------------------------------------------------

	//�����õ��ĺ���:
	//closestObject,world.h
	//normalise,length,dot,addMult,vec3.h
	//getMaterial,object.h
	//��Դ�洢��m_lights��.m_lights.size(),���ع�Դ����.m_lights[i],��i����Դ ,0<=i<m_lights.size()
	//pow(x,y),x��y�η�

	//---------------------------------------------------------------------

	//�����򲻿�������

	//---------------------------------------------------------------------

	if (traceNum < 1) {
		colour_out = Colour::black();
		return;
	}

	//��Ӵ���
	Colour
		ambient,//�����ⷴ��ϵ��
		diffuse,//������ϵ��
		specular;//���淴��ϵ��
	float ns;//���淴�����

	Colour local, reflect;
	Vec3 p0 = ray.m_startPos;
	Vec3 V = ray.m_unitDir;

	Object* obj;
	float dist;
	obj = closestObject(ray, dist);
	if (dist < 0) {
		//Colour bg = Colour::black();
		//bg = bg.operator*((m_lights[0]->getColour()).operator+(m_lights[1]->getColour()));
		//bg = bg.operator*(m_lights[1]->getColour());
		
		//colour_out = (m_lights[0]->getColour()).operator+(m_lights[1]->getColour());
		colour_out = Colour::black();			
		return;
	}
	else {
		//local
		Material mat = obj->getMaterial();
		Vec3 p1 = p0.operator+(V.operator*(dist));
		Vec3 N = obj->getGeometry().getNormalForPos(p1);
		N = N.normalise();

		ambient = mat.m_ambient;
		diffuse = mat.m_diffuse;
		specular = mat.m_specular;
		ns = mat.m_ns;
		

		Colour c[2];
		local = ambient;
		//local = ambient.operator*((m_lights[0]->getColour()).operator+(m_lights[1]->getColour()));
		Colour lightColour;
		Vec3 lightPos;
		Vec3 L;
		for (int i = 0; i < 2; i++)
		{
			lightColour = m_lights[i]->getColour();
			lightPos = m_lights[i]->getPos();
			L = lightPos.operator-(p1);
			L = L.normalise();

			//shadow
			const Ray shadowRay(p1, L);
			float d;
			Object* shadow = closestObject(shadowRay, d);
			if (d >= 0) {
				//shadow
				//c[i] = Colour::black();
			}
			else {
				//
				float cos = L.dot(N);
				if (cos < 0) {
					cos = 0;
				}
				Vec3 R = N.operator*(2).operator*(cos);
				R = R.operator-(L);
				//			
				float alpha = V.dot(R);
				alpha = pow(alpha, 36);
				//

				//Colour cc = diffuse.operator+(specular).operator+(ambient);
				Colour cc = (diffuse.operator*(cos)).operator+((specular.operator*(alpha)));
				//c[i] = lightColour.operator*(cc);
				local = local.operator+(lightColour.operator*(cc));
				//			
			}
			
		}
		//local = local.operator+(c[0].operator+(c[1]));
		//

		//reflection
		if (traceNum - 1 > 0) {
			Vec3 v = V.operator*(-1);
			v = v.normalise();
			float cos = v.dot(N);
			Vec3 r = N.operator*(2).operator*(cos);
			r = r.operator-(v);
			r = r.normalise();
			const Ray rRay(p1, r);
			float flag;
			Object* iterative = closestObject(rRay, flag);
			if (flag < 0) {

			}
			else {
				getColourForRay(rRay, reflect, traceNum - 1);
				specular = obj->getMaterial().m_specular;
				local = local.operator+(specular.operator*(reflect));
			}
		}
		
		
		//
		colour_out = local;
		//colour_out = local.operator+(reflect);
		//colour_out = local;
	}

}

void World::setTraceNum(int traceNum)
{
	m_traceNum = traceNum;
}

void World::clear()
{
	int i = m_objects.size();
	for (int j = 0; j < i; j++)
		delete m_objects[j];
	m_objects.clear();

	i = m_lights.size();
	for (int j = 0; j < i; j++)
		delete m_lights[j];
	m_lights.clear();
}
