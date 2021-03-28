

#include "triangle.h"

#include "../practice31/ray.h"



Triangle::Triangle(const Triangle& triangle)
	: m_v1(triangle.m_v1), m_v2(triangle.m_v2), m_v3(triangle.m_v3)
{

}

float Triangle::getDistanceUntilHit(const Ray& ray) const
{

	Vec3 A = m_v2 - m_v1, B = m_v3 - m_v1, C = -1 * ray.m_unitDir, D = ray.m_startPos - m_v1;
	float a = A.x, b = B.x, c = C.x,
		d = A.y, e = B.y, f = C.y,
		g = A.z, h = B.z, i = C.z,
		j = D.x, k = D.y, l = D.z;
	float det = a * (e*i - f * h) - b * (d*i - f * g) + c * (d*h - e * g),
		det0 = j * (e*i - f * h) - b * (k*i - f * l) + c * (k*h - e * l),
		det1 = a * (k*i - f * l) - j * (d*i - f * g) + c * (d*l - k * g),
		det2 = a * (e*l - k * h) - b * (d*l - k * g) + j * (d*h - e * g);
	float U = det0 / det, V = det1 / det, T = det2 / det;

	if (T > 0 && 0 < U&&U < 1 && 0 < V&&V < 1 && 0 < U + V && U + V < 1)return T;
	else return -666.0f;

}
