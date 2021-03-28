
#ifndef __MATERIAL_H_666_
#define __MATERIAL_H_666_


#include "../practice31/colour.h"
#include <stdio.h>

/*=====================================================================
材质
=====================================================================*/
class Material
{
public:

	Material(const Colour ambient = Colour(0.2f, 0.2f, 0.2f), const Colour diffuse = Colour(0.8f, 0.8f, 0.8f), const Colour specular = Colour(0.0f, 0.0f, 0.0f), float specularFactor = 5.0f);

	Material(const Material& m) :m_ambient(m.m_ambient), m_diffuse(m.m_diffuse), m_specular(m.m_specular), m_ns(m.m_ns) {

	}

	inline Material& operator=(const Material m) {
		m_ambient = m.m_ambient;
		m_diffuse = m.m_diffuse;
		m_specular = m.m_specular;
		m_ns = m.m_ns;

	}

	~Material() {}

	Colour
		m_ambient,//环境光反射系数
		m_diffuse,//漫反射系数
		m_specular;//镜面反射系数
	float m_ns;//镜面反射参数

};


#endif //__MATERIAL_H_666_




