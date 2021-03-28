

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



//返回与ray相交的最近的Object,并将ray起点与交点的距离存入dist；若没有Object与ray相交，返回NULL,dist为负数
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

//获取射线ray的颜色，存入colour_out，最多跟踪traceNum次
void World::getColourForRay(const Ray& ray, Colour& colour_out, int traceNum)
{
	//向量：见(maths/vec3.h)
		//Vec3 v(x,y,z)：定义Vec3对象(可表示点或方向等),float x/y/z分别是其分量(值域[-∞,+∞])
		//若有 Vec3 v,v1,v2,常用计算:v=v1,v=v1*float_number,v=v1+v2,v=v1-v2,v=v1*v2,v=normalise(v1),float dot_product=v1.dot(v2)

	//射线：见(raytracer/ray.h)
		//Ray xray(point,dir)：定义Ray对象,Vec3 point,Vec3 dir分别是起点,方向

	//颜色：见(raytracer/colour.h)
		//Colour colour(r,g,b); 定义Colour对象,float r/g/b分别是红/绿/蓝分量(值域[0.0f,1.0f]);
		//若有Colour c,c1,c2,常用计算:c=c1,c=c1*float_number,c+=c1,c-=c1,c=c1+c2,c=c1-c2,c=c1*c2

	//材质：见(raytracer/material/material.h)

	//形状：见(raytracer/geometry/geometry.h),平面(raytracer/geometry/rayplane.h),球面(raytracer/geometry/raysphere.h)

	//物体：见(raytracer/object.h).
		//Object* object=NULL; 定义Object指针

	//光源：见(raytracer/light.h)

	//---------------------------------------------------------------------

	//可能用到的函数:
	//closestObject,world.h
	//normalise,length,dot,addMult,vec3.h
	//getMaterial,object.h
	//光源存储在m_lights里.m_lights.size(),返回光源个数.m_lights[i],第i个光源 ,0<=i<m_lights.size()
	//pow(x,y),x的y次方

	//---------------------------------------------------------------------

	//本程序不考虑折射

	//---------------------------------------------------------------------

	if (traceNum < 1) {
		colour_out = Colour::black();
		return;
	}

	//添加代码
	Colour
		ambient,//环境光反射系数
		diffuse,//漫反射系数
		specular;//镜面反射系数
	float ns;//镜面反射参数

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
