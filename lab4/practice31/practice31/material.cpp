

#include "../practice31/material.h"

#include <assert.h>
#include <stdio.h>

Material::Material(const Colour ambient, const Colour diffuse, const Colour specular, float specularFactor)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_ns = specularFactor;
}