//mathlib.cpp
#include "stdafx.h"
#include "mathlib.h"
vector3d operator-(vector3d const &a , vector3d const &b)
{
		return vector3d(a.x - b.x , a.y - b.y , a.z - b.z);
}
vector3d operator+(vector3d const &a, vector3d const &b)
{
		return vector3d(a.x+b.x ,a.y+b.y,a.z+b.z);
}
vector3d operator*(vector3d const &a,float const &s )
{
	return vector3d(s*a.x , s*a.y , s*a.z);
}
float dot(const vector3d& p1 , const vector3d& p2)
{
	return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}

CIRCLE::CIRCLE()
{

}

CIRCLE::~CIRCLE()
{

}

void CIRCLE::Set(vector2d &v, float& r)
{

}
vector2d CIRCLE::getpoint(vector2d&p1 , vector2d&p2)
{
	float d = pos.dist(p1,p2);
	float l = pos.dist(p1);
	l = radius / l;
	if(l > 1.0f)l= 1.0f;
	if(l < -1.0f)l=-1.0f;
	float s = (float)asin(l);
	if(d > 0) s = -s;
	vector2d p;
	p = pos - p1;
	p.normalize();
	p.rotate(s);
	d = sqrt(fabs(pos.dist(p1)*pos.dist(p1) - radius * radius));
	p = p1 + vector2d(p.x*d,p.y*d);
	return p;
}
bool CIRCLE::isIntersect(vector2d&p1, vector2d&p2)
{
	vector2d p(pos.x,pos.y);		
	float d;
    d = fabs(p.dist(p1,p2));
	if(d > radius)
		return false;
	else
	{
		if(pos.isInter(p1,p2))
			return true;
		else
			return false;
	}
}


