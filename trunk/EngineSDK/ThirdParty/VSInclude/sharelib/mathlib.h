//mathlib.h
#ifndef _MATHLIB_H_11
#define _MATHLIB_H_11
#include <math.h>
#include <assert.h>
#ifndef EPSILON
#define EPSILON 0.0001f
#endif
#ifndef PI
#define PI 3.1415926f
#endif
//vector3d
struct vector2d
{
	union
	{
		struct 
		{
			float x , y;
		};
		float v[2];
	};
	vector2d(){}
	~vector2d(){}
	vector2d(float _x , float _y):x(_x),y(_y){}
	vector2d(float *f):x(f[0]),y(f[1]){}
	vector2d(const vector2d& vector):x(vector.x),y(vector.y){}
	float mag()
	{
		return (float)sqrt(x*x+y*y);
	}
	float mag_sqr()
	{
		return x*x + y*y;
	}
	float dist(vector2d& vector)//两点之间的距离
	{
		return (float)sqrt((x-vector.x)*(x-vector.x)+(y-vector.y)*(y-vector.y));
	}
	float dot(vector2d& vector)
	{
		return x*vector.x + y*vector.y;
	}
	void change()//得到二维向量的垂直向量
	{
		float m;
		m = x;
		x = -y;
		y = m;
	}
	float dist(vector2d&p1 , vector2d&p2)//点到直线的距离
	{
		vector2d p(p2.x - p1.x , p2.y - p1.y);
		p.normalize();
		p.change();
		return p.dot(vector2d(x-p1.x,y-p1.y));
	}
	float isInter(vector2d p1 , vector2d p2)//判断点到直线的投影是否在线段内
	{
		vector2d p;
		float d = dist(p1,p2);
		
		p.x = p2.x - p1.x;
		p.y = p2.y - p1.y;
		float x1 = p.x;
		p.normalize();
		p.change();
		p.x *= d;
		p.y *= d;
		float x2;
		x2 = x - p1.x - p.x;
		float t;
		t = x2 / x1;
		if((t>0)&&(t<1)) return true;
		return false;
	}
	float dist_sqr(vector2d& vector)
	{
		return (x-vector.x)*(x-vector.x)+(y-vector.y)*(y-vector.y);
	}
	void set(float _x , float _y)
	{
		x = _x;
		y = _y;
	}
	
	void normalize()
	{
		float d = mag();
		x /= d;
		y /= d;
	}
	void operator=(vector2d vector)
	{
		x = vector.x;
		y = vector.y;
	}
	void operator +=(vector2d& vector)
	{
		x += vector.x;
		y += vector.y;
	}
	void operator -=(vector2d& vector)
	{
		x -= vector.x;
		y -= vector.y;
	}
	vector2d operator +(vector2d& vector)
	{
		vector2d rel(x+vector.x ,y + vector.y);
		return rel;
	}
	vector2d operator -(vector2d& vector)
	{
		vector2d rel(x-vector.x , y-vector.y);
		return rel;
	}
	vector2d operator *(float s)
	{
		vector2d rel(x*s, y*s);
		return rel;
	}
	vector2d operator /(float& s)
	{
		vector2d rel(x/s,y/s);
		return rel;
	}
	float getradian()//得到向量的弧度
	{
		if(fabs(x)<=EPSILON&&y>0)return 90.0f;
		if(fabs(x)<=EPSILON&&y<0)return 270.0f;
		if(fabs(y)<=EPSILON&&x>0)return 0.0f;
		if(fabs(y)<=EPSILON&&x<0)return 180.0f;
		float rel = (float)asinf((float)fabs(y));
		rel = rel*180.0f / PI;
		if(x>0&&y>0)return rel;
		if(x<0&&y>0)return 180.0f - rel;
		if(x<0&&y<0)return 180.0f + rel;
		if(x>0&&y<0)return 360.0f - rel;
	}
	void rotate(float s)//旋转向量
	{
		{
			float x1,y1;
			x1 = x*(float)cos(s) - y*(float)sin(s);
			y1 = x*(float)sin(s) + y*(float)cos(s);
			x = x1;
			y = y1;
		}
	}
};
struct vector3d
{
	union
	{
		struct
		{
			float x , y ,z ;
		};
		float v[3];
	};
	
	vector3d(){}
	~vector3d(){}
	vector3d(float _x , float _y , float _z):x(_x),y(_y),z(_z){}
	vector3d(float *f):x(f[0]),y(f[1]),z(f[2]){}
	vector3d(const vector3d& vector):x(vector.x),y(vector.y),z(vector.z){}
	float mag()
	{
		return (float)sqrt(x*x + y*y + z*z);
	}
	float mag_sqr()
	{
		return x*x + y*y + z*z;
	}
	
	float dist(vector3d& vector)
	{
		return (float)sqrt((x-vector.x)*(x-vector.x)+(y-vector.y)*(y-vector.y)+(z-vector.z)*(z-vector.z));
	}
	float dist_sqr(vector3d& vector)
	{
		return ((x-vector.x)*(x-vector.x)+(y-vector.y)*(y-vector.y)+(z-vector.z)*(z-vector.z));
	}

	float dist_sqrxz(vector3d &vector )
	{
		return ((x-vector.x)*(x-vector.x)+(z-vector.z)*(z-vector.z));
	}

	void set(float _x , float _y , float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	vector3d cross(const vector3d& p)
	{
		vector3d vector;
		vector.x = y * p.z - z * p.y;
		vector.y = z * p.x - x * p.z;
		vector.z = x * p.y - y * p.x;
		return vector;
	}
	void normalize()
	{
		float invmag = 1.0f / mag();
		x *= invmag;
		y *= invmag;
		z *= invmag;
	}

	void operator = (const vector3d& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
	}

	void operator +=(const vector3d& p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
	}

	void operator -=(const vector3d& p)
	{
		x -= p.x;
		y -= p.y;
		z -= p.z;
	}

	void operator *=(const float& s)
	{
		x *= s;
		y *= s;
		z *= s;
	}

	void operator /=(const float& s)
	{
		x /= s;
		y /= s;
		z /= s;
	}	

	inline void makeFloor( const vector3d& cmp )
    {
        if( cmp.x < x ) x = cmp.x;
        if( cmp.y < y ) y = cmp.y;
        if( cmp.z < z ) z = cmp.z;
    }


    inline void makeCeil( const vector3d& cmp )
    {
        if( cmp.x > x ) x = cmp.x;
        if( cmp.y > y ) y = cmp.y;
        if( cmp.z > z ) z = cmp.z;
    }
};




/////////////////////
vector3d operator-(vector3d const &a , vector3d const &b);
vector3d operator+(vector3d const &a, vector3d const &b);
vector3d operator*(vector3d const &a,float const &s );
float dot(const vector3d& p1 , const vector3d& p2);

struct line2d
{
	vector2d p1 , p2;
	bool IsIntersect(line2d& line)
	{
		float a1 , b1 , c1;
		float a2 , b2 , c2;
		a1 = p2.x - p1.x;
		b1 = line.p1.x - line.p2.x;
		c1 = line.p1.x - p1.x;

		a2 = p2.y - p1.y;
		b2 = line.p1.y - line.p2.y;
		c2 = line.p1.y - p1.y;
		if(fabs(a2*b1 - a1*b2)>0.0001f)
			return true;
		else
			return false;
	}
	vector2d GetIntersectPoint(line2d& line)
	{
		float a1 , b1 , c1;
		float a2 , b2 , c2;
		a1 = p2.x - p1.x;
		b1 = line.p1.x - line.p2.x;
		c1 = line.p1.x - p1.x;

		a2 = p2.y - p1.y;
		b2 = line.p1.y - line.p2.y;
		c2 = line.p1.y - p1.y;
		vector2d rel;
		float t;
		t = (a2 * c1 - a1 * c2) / (a2 * b1 - a1 * b2);
		rel = line.p1 + (line.p2 - line.p1) * t;
		return rel;
	}
};


struct Matrix4d
{
	
};

class CIRCLE
{
public:
	vector2d pos;
	float radius;
	bool bintersect;
public:
	bool isIntersect(vector2d& p1 , vector2d& p2);//判断线段和圆是否相交
	void Set(vector2d &v , float& r);
	vector2d getpoint(vector2d& p1 , vector2d& p2);//得到圆相对于p1的切点
	void operator =(CIRCLE c)
	{
		pos = c.pos;
		radius = c.radius;
		bintersect = c.bintersect;
	}
	CIRCLE();
	virtual ~CIRCLE();
	
};

struct PLANE_EQ
{
	float a , b , c , d;
};
/*
struct ray2d
{
	vector2d vbegin;
	vector2d orient;
};

struct sphere
{
	bool PtInSphere(vector3d& v);
	float GetIntersectPos(Ray ray)
	{
		vector3d v = center - ray.vbegin;
		float costheta = v.dot(ray.orient) / center.dist(ray.vbegin);
		float sintheta = sqrt(1 - costheta*costheta);
		float d = center.dist(ray.vbegin) * sintheta;
		sintheta = d / radius;
		costheta = sqrt(1 - sintheta * sintheta);
		return radius * costheta - v.dot(ray.orient);
	}
	bool IsIntersect(Ray ray)
	{
		vector3d v = center - ray.vbegin;
		float costheta = v.dot(ray.orient) / center.dist(ray.vbegin);
		if(costheta < 0.0f)
			return false;
		float d = center.dist(ray.vbegin) *sqrt(1 - costheta*costheta);
		if(d > radius)
			return false;
		return true;
	}
	vector3d center;
	float radius;
};
inline bool sphere::PtInSphere(vector3d& v)
{
	if(center.dist_sqr(v)<radius*radius)
		return true;
	else
		return false;
}
*/
#endif //_MATHLIB_H_11