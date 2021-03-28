
#include "object.h"
#include "../practice31/geometry.h"
#include "../practice31/vec3.h"
#include "world.h"


Object::Object(const Material& material_, Geometry* geometry_)
	: m_material(material_),
	m_geometry(geometry_)
{

}

Object::~Object()
{
	delete m_geometry;
}



