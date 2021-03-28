
#ifndef __MATERIAL_H_666_
#define __MATERIAL_H_666_


#include "../practice31/colour.h"
#include <stdio.h>

/*=====================================================================
����
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
		m_ambient,//�����ⷴ��ϵ��
		m_diffuse,//������ϵ��
		m_specular;//���淴��ϵ��
	float m_ns;//���淴�����

};


#endif //__MATERIAL_H_666_




