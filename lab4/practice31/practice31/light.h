/*=====================================================================
��Դ
=====================================================================*/
#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "colour.h"
#include "../practice31/vec3.h"



class Light
{
public:
	//���캯��
	Light(const Vec3& pos_, const Colour& c)
		: pos(pos_),
		colour(c)
	{}

	~Light() {}

	//��ȡ��Դλ��
	inline const Vec3& getPos() const { return pos; }

	//��ȡ����ɫ
	inline const Colour& getColour() const { return colour; }

	Vec3 pos;
	Colour colour;
};


#endif //__LIGHT_H__