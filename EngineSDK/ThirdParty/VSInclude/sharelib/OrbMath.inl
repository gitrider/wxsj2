#ifndef __ORB_MATH_INL__
#define __ORB_MATH_INL__

//================================================
// VECTOR2 inline implement
//================================================
inline 
VECTOR2::VECTOR2():x(0), y(0)
{
}
inline 
VECTOR2::VECTOR2(float _x, float _y):x(_x), y(_y)
{
}
inline 
bool VECTOR2::operator==(const VECTOR2& other) const
{
	return x == other.x && y == other.y;
}
inline
bool VECTOR2::operator!=(const VECTOR2& other) const
{
	return !operator==(other);
}
inline
VECTOR2 VECTOR2 ::operator+() const
{
	return *this;
}
inline
VECTOR2 VECTOR2 ::operator-() const
{
	return VECTOR2(-x, -y);
}
inline
float VECTOR2::Length() const
{
	return sqrtf(x * x + y * y);
}
inline
VECTOR2 VECTOR2::GetNormalized() const
{
	VECTOR2 v = *this;
	v.Normalize();
	return v;
}
inline
const VECTOR2& VECTOR2::operator +=(const VECTOR2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}
inline
const VECTOR2& VECTOR2::operator -=(const VECTOR2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}
inline
const VECTOR2& VECTOR2::operator *=(float s)
{
	x *= s;
	y *= s;
	return *this;
}
inline
const VECTOR2& VECTOR2::operator /=(float s)
{
	x /= s;
	y /= s;
	return *this;
}
//================================================

//================================================
// VECTOR3 inline implement
//================================================
inline
VECTOR3::VECTOR3():x(0), y(0), z(0)
{
}
inline
VECTOR3::VECTOR3(float _x, float _y, float _z):x(_x), y(_y), z(_z)
{
}
inline
bool VECTOR3::operator==(const VECTOR3& other) const
{
	return x == other.x && y == other.y && z == other.z;
}
inline
bool VECTOR3::operator!=(const VECTOR3& other) const 
{
	return !operator==(other);
}
inline 
VECTOR3 VECTOR3::operator+() const
{
	return *this;
}
inline
VECTOR3 VECTOR3::operator-() const
{
	return VECTOR3(-x, -y, -z);
}
inline
float VECTOR3::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}
inline
VECTOR3 VECTOR3::GetNormalized() const
{
	VECTOR3 v = *this;
	v.Normalize();
	return v;
}
inline
const VECTOR3& VECTOR3::operator +=(const VECTOR3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}
inline
const VECTOR3& VECTOR3::operator -=(const VECTOR3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}
inline
const VECTOR3& VECTOR3::operator *=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
inline
const VECTOR3& VECTOR3::operator /=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}
//================================================

//================================================
// VECTOR4 inline implement
//================================================
inline 
VECTOR4::VECTOR4():x(0), y(0), z(0), w(0)
{
}
inline
VECTOR4::VECTOR4(float _x, float _y, float _z, float _w):x(_x), y(_y), z(_z), w(_w)
{
}
inline
bool VECTOR4::operator==(const VECTOR4& other) const
{
	return x == other.x && y == other.y && z == other.z && w == other.w;
}
inline
bool VECTOR4::operator!=(const VECTOR4& other) const
{
	return !operator==(other);
}
inline
VECTOR4 VECTOR4::operator+() const
{
	return *this;
}
inline
VECTOR4 VECTOR4::operator-() const
{
	return VECTOR4(-x, -y, -z, -w);
}
inline
float VECTOR4::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}
inline 
VECTOR4 VECTOR4 ::GetNormalized() const
{
	VECTOR4 v = *this;
	v.Normalize();
	return v;
}
inline
const VECTOR4& VECTOR4::operator +=(const VECTOR4& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}
inline
const VECTOR4& VECTOR4::operator -=(const VECTOR4& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}
inline
const VECTOR4& VECTOR4::operator *=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}
inline
const VECTOR4& VECTOR4::operator /=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}
//================================================


//================================================
// QUATERNION inline implement
//================================================
inline
QUATERNION::QUATERNION():x(0), y(0), z(0), w(0)
{
}

inline
QUATERNION::QUATERNION(float _x, float _y, float _z, float _w)
		:x(_x), y(_y), z(_z), w(_w)
{
}

inline
bool QUATERNION::operator == (const QUATERNION& other) const
{
	return x == other.x && y == other.y && z == other.z && w == other.w;
}

inline
bool QUATERNION::operator != (const QUATERNION& other) const
{
	return !operator==(other);
}

inline
QUATERNION QUATERNION::operator + () const
{
	return *this;
}

inline
QUATERNION QUATERNION::operator - () const
{
	return QUATERNION(-x, -y, -z, -w);
}

inline
QUATERNION& QUATERNION::operator += (const QUATERNION& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

inline
QUATERNION& QUATERNION::operator -= (const QUATERNION& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

inline
QUATERNION& QUATERNION::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

inline
QUATERNION& QUATERNION::operator /= ( float f )
{
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	w *= fInv;
	return *this;
}

inline
float QUATERNION::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

inline
bool QUATERNION::IsIdentity() const
{
	return x == 0.0f && y == 0.0f && z == 0.0f && w == 1.0f;
}
inline
const QUATERNION& QUATERNION::SetIdentity()
{
	x = y = z = 0.0f;
	w = 1.0f;
	return *this;
}
inline
QUATERNION QUATERNION ::GetNormalize() const
{
	QUATERNION q = *this;
	q.Normalize();
	return q;
}
//================================================


//================================================
// MATRIX33 inline implement
//================================================
inline
MATRIX33::MATRIX33()
{
	memset(this, 0, sizeof(MATRIX33));
}
inline
MATRIX33::MATRIX33(float _m[9])
{
	memcpy(m, _m, 9 * sizeof(float));
}

inline
MATRIX33::MATRIX33(	float _m11, float _m12, float _m13,
	float _m21, float _m22, float _m23,
	float _m31, float _m32, float _m33)
	
{
	m11 = _m11, m12 = _m12, m13 = _m13, 
	m21 = _m21, m22 = _m22, m23 = _m23, 
	m31 = _m31, m32 = _m32, m33 = _m33;
		
}

inline
float& MATRIX33::operator()(int row, int col)
{
	return m[3 * (row) + col ];
}

inline
const float& MATRIX33::operator()(int row, int col) const
{
	return m[3 * (row) + col];
}
inline
float& MATRIX33::operator[](int nIndex)
{
	return m[nIndex];
}
inline
const float& MATRIX33::operator[](int nIndex) const
{
	return m[nIndex];
}
inline
const MATRIX33& MATRIX33::operator+=(const MATRIX33& other)
{
	for(int i = 0; i < 9; ++i)
	{
		m[i] += other.m[i];
	}
	return *this;
}

inline
const MATRIX33& MATRIX33::operator-=(const MATRIX33& other)
{
	for(int i = 0; i < 9; ++i)
	{
		m[i] -= other.m[i];
	}
	return *this;
}

inline
bool MATRIX33::operator==(const MATRIX33& other) const
{
	for(int i = 0; i < 9; ++i)
	{
		if(m[i] != other.m[i])
			return false;
	}
	return true;
}

inline
bool MATRIX33::operator!=(const MATRIX33& other) const
{
	return !operator==(other);
}

inline
const MATRIX33& MATRIX33::SetIdentity()
{
	memset(this, 0, sizeof(MATRIX33));
	m11 = m22 = m33;
	return *this;
}

inline
bool MATRIX33::isIdentity() const
{
	return	m11 == 1.0f && m12 == 0.0f && m13 == 0.0f && 
			m21 == 0.0f && m22 == 1.0f && m23 == 0.0f && 
			m31 == 0.0f && m32 == 0.0f && m33 == 1.0f;
			
}
inline
float MATRIX33::Determinant() const
{
	return ((m11 * ((m22 * m33) - (m23 * m32))) -
            (m12 * ((m21 * m33) - (m23 * m31))) +
            (m13 * ((m21 * m32) - (m22 * m31))));
}
inline 
MATRIX33 MATRIX33::GetTranspose() const
{
	MATRIX33 ret = *this;
	ret.Transpose();
	return ret;
}
inline 
MATRIX33 MATRIX33::GetInverse() const
{
	MATRIX33 ret = *this;
	ret.Invert();
	return ret;
}
inline
MATRIX33::operator float*() const
{
	return (float*)m;
}
inline
MATRIX33::MATRIX33(const QUATERNION& quat)
{
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	x2 = quat.x + quat.x;
	y2 = quat.y + quat.y;
	z2 = quat.z + quat.z;
	xx = quat.x * x2;
	xy = quat.x * y2;
	xz = quat.x * z2;
	yy = quat.y * y2;
	yz = quat.y * z2;
	zz = quat.z * z2;
	wx = quat.w * x2;
	wy = quat.w * y2;
	wz = quat.w * z2;

	m11 = 1.0f - (yy + zz);
	m21 = xy - wz;
	m31 = xz + wy;
	
	m12 = xy + wz;
	m22 = 1.0f - (xx + zz);
	m32 = yz - wx;
	
	m13 = xz - wy;
	m23 = yz + wx;
	m33 = 1.0f - (xx + yy);
}
inline
VECTOR3 MATRIX33::GetRow(int row) const
{
	VECTOR3 v;
	v.x = (*this)(0, row);
	v.y = (*this)(1, row);
	v.z = (*this)(2, row);
	return v;
}
inline
VECTOR3 MATRIX33::GetLine(int line) const
{
	VECTOR3 v;
	v.x = (*this)(line, 0);
	v.y = (*this)(line, 1);
	v.z = (*this)(line, 2);
	return v;
}
inline
void MATRIX33::SetLine(int line, const VECTOR3& v)
{
	(*this)(line, 0) = v.x;
	(*this)(line, 1) = v.y;
	(*this)(line, 2) = v.z;
}
inline
void MATRIX33::SetRow(int row, const VECTOR3& v)
{
	(*this)(0, row) = v.x;
	(*this)(1, row) = v.y;
	(*this)(2, row) = v.z;
}
//================================================


//================================================
// MATRIX44 inline implement
//================================================
inline
MATRIX44::MATRIX44()
{
	memset(this, 0, sizeof(MATRIX44));
}
inline
MATRIX44::MATRIX44(float _m[16])
{
	memcpy(m, _m, 16* sizeof(float));
}
inline
MATRIX44::MATRIX44(const MATRIX33& other)
{
	m11 = other(0, 0);
	m12 = other(0, 1);
	m13 = other(0, 2);
	m14 = 0.0f;

	m21 = other(1, 0);
	m22 = other(1, 1);
	m23 = other(1, 2);
	m24 = 0.0f;

	m31 = other(2, 0);
	m32 = other(2, 1);
	m33 = other(2, 2);
	m34 = 0.0f;

	m41 = m42 = m43 = 0.0f;
	m44 = 1.0f;

}
inline
MATRIX44::MATRIX44(	float _m11, float _m12, float _m13, float _m14, 
	float _m21, float _m22, float _m23, float _m24,
	float _m31, float _m32, float _m33, float _m34,
	float _m41, float _m42, float _m43, float _m44)
{
	m11 = _m11, m12 = _m12, m13 = _m13, m14 = _m14, 
		m21 = _m21, m22 = _m22, m23 = _m23, m24 = _m24,
		m31 = _m31, m32 = _m32, m33 = _m33, m34 = _m34,
		m41 = _m41, m42 = _m42, m43 = _m43, m44 = _m44;
}

inline
float& MATRIX44::operator()(int row, int col)
{
	return m[4 * (row) + col ];
}

inline
const float& MATRIX44::operator()(int row, int col) const
{
	return m[4 * (row) + col];
}
inline
float& MATRIX44::operator[](int nIndex)
{
	return m[nIndex];
}
inline
const float& MATRIX44::operator[](int nIndex) const
{
	return m[nIndex];
}
inline
const MATRIX44& MATRIX44::operator+=(const MATRIX44& other)
{
	for(int i = 0; i < 16; ++i)
	{
		m[i] += other.m[i];
	}
	return *this;
}

inline
const MATRIX44& MATRIX44::operator-=(const MATRIX44& other)
{
	for(int i = 0; i < 16; ++i)
	{
		m[i] -= other.m[i];
	}
	return *this;
}

inline
bool MATRIX44::operator==(const MATRIX44& other) const
{
	for(int i = 0; i < 16; ++i)
	{
		if(m[i] != other.m[i])
			return false;
	}
	return true;
}

inline
bool MATRIX44::operator!=(const MATRIX44& other) const
{
	return !operator==(other);
}

inline
const MATRIX44& MATRIX44::SetIdentity()
{
	memset(this, 0, sizeof(MATRIX44));
	m11 = m22 = m33 = m44 = 1.0f;
	return *this;
}

inline
bool MATRIX44::isIdentity() const
{
	return	m11 == 1.0f && m12 == 0.0f && m13 == 0.0f && m14 == 0.0f &&
			m21 == 0.0f && m22 == 1.0f && m23 == 0.0f && m24 == 0.0f &&
			m31 == 0.0f && m32 == 0.0f && m33 == 1.0f && m34 == 0.0f &&
			m41 == 0.0f && m42 == 0.0f && m43 == 0.0f && m44 == 1.0f;
}
inline
float MATRIX44::Determinant() const
{
	return ((m11 * ((m22 * m33) - (m23 * m32))) -
            (m12 * ((m21 * m33) - (m23 * m31))) +
            (m13 * ((m21 * m32) - (m22 * m31))));
}
inline 
MATRIX44 MATRIX44::GetTranspose() const
{
	MATRIX44 ret = *this;
	ret.Transpose();
	return ret;
}
inline 
MATRIX44 MATRIX44::GetInverse() const
{
	MATRIX44 ret = *this;
	ret.Invert();
	return ret;
}
inline
MATRIX44::operator float*() const
{
	return (float*)m;
}
inline
VECTOR4 MATRIX44::GetRow(int row) const
{
	VECTOR4 v;
	v.x = (*this)(0, row);
	v.y = (*this)(1, row);
	v.z = (*this)(2, row);
	v.w = (*this)(3, row);
	return v;
}
inline
VECTOR4 MATRIX44::GetLine(int line) const
{
	VECTOR4 v;
	v.x = (*this)(line, 0);
	v.y = (*this)(line, 1);
	v.z = (*this)(line, 2);
	v.w = (*this)(line, 3);
	return v;
}
inline
void MATRIX44::SetLine(int line, const VECTOR4& v)
{
	(*this)(line, 0) = v.x;
	(*this)(line, 1) = v.y;
	(*this)(line, 2) = v.z;
	(*this)(line, 3) = v.w;
}
inline
void MATRIX44::SetRow(int row, const VECTOR4& v)
{
	(*this)(0, row) = v.x;
	(*this)(1, row) = v.y;
	(*this)(2, row) = v.z;
	(*this)(3, row) = v.w;
}
//================================================

//================================================
// PLANE inline implement
//================================================
inline
PLANE PLANE::GetNormalized() const
{
	return PLANE(*this).Normalize();
}
inline
PLANE::PLANE()
{
	a = b = c = d = 0;
}
inline
PLANE::PLANE(const PLANE& other)
{
	a = other.a;
	b = other.b;
	c = other.c;
	d = other.d;
}
inline
PLANE::PLANE(float A, float B, float C, float D)
{
	a = A;
	b = B;
	c = C;
	d = D;
}
inline
PLANE::PLANE(const VECTOR3& n, const VECTOR3& p)
{
	 normal = n;
	 d = -vecDot(normal, p);
}
inline
PLANE::PLANE(const VECTOR3& p1, const VECTOR3& p2, const VECTOR3& p3)
{
	normal = vecCross((p2 -p1).Normalize(), (p3 - p1).Normalize());
	d = -vecDot(normal.Normalize(), p1);
}
inline
bool PLANE::operator==(const PLANE& other) const
{
	return (other.a == a && other.b == b && other.c== c && other.d == d);
}
inline
bool PLANE::operator!=(const PLANE& other) const
{
	return !operator==(other);
}
inline
PLANE::operator float* ()
{
	return v;
}
inline
float PLANE::PointDistance(const VECTOR3& point) const
{
	return d + vecDot(normal, point);
}
//================================================


//================================================
// Helper functions inline implement
//================================================

inline 
VECTOR2 operator+(const VECTOR2& v1, const VECTOR2& v2)
{
	return VECTOR2(v1.x + v2.x, v1.y + v2.y);
}

inline 
VECTOR3 operator+(const VECTOR3& v1, const VECTOR3& v2)
{
	return VECTOR3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline 
VECTOR4 operator+(const VECTOR4& v1, const VECTOR4& v2)
{
	return VECTOR4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

inline
QUATERNION operator+(const QUATERNION& v1, const QUATERNION& v2)
{
	return QUATERNION(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

inline 
VECTOR2 operator-(const VECTOR2& v1, const VECTOR2& v2)
{
	return VECTOR2(v1.x - v2.x, v1.y - v2.y);
}

inline 
VECTOR3 operator-(const VECTOR3& v1, const VECTOR3& v2)
{
	return VECTOR3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline 
VECTOR4 operator-(const VECTOR4& v1, const VECTOR4& v2)
{
	return VECTOR4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

inline 
QUATERNION operator-(const QUATERNION& v1, const QUATERNION& v2)
{
	return QUATERNION(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

inline 
VECTOR2 operator*(const VECTOR2& v, float s)
{
	return VECTOR2(v.x * s, v.y * s);
}

inline 
VECTOR2 operator*(float s, const VECTOR2& v)
{
	return v * s;
}

inline 
VECTOR3 operator*(const VECTOR3& v, float s)
{
	return VECTOR3(v.x *s , v.y*s, v.z *s);
}

inline 
VECTOR3 operator*(float s, const VECTOR3& v)
{
	return v * s;
}

inline 
VECTOR4 operator*(const VECTOR4& v, float s)
{
	return VECTOR4(v.x *s , v.y*s, v.z *s, v.w * s);
}

inline 
VECTOR4 operator*(float s, const VECTOR4& v)
{
	return v * s;
}

inline 
QUATERNION operator*(const QUATERNION& v, float s)
{
	return QUATERNION(v.x *s , v.y*s, v.z *s, v.w * s);
}

inline 
QUATERNION operator*(float s, const QUATERNION& v)
{
	return v * s;
}
inline
MATRIX44 operator*(const MATRIX44& mat, float s)
{
	MATRIX44 ret;
	for(int i = 0; i < 16; ++i)
		ret[i] = mat[i] * s;
	return ret;
}
inline
MATRIX33 operator*(const MATRIX33& mat, float s)
{
	MATRIX33 ret;
	for(int i = 0; i < 9; ++i)
		ret[i] = mat[i] * s;
	return ret;
}
inline
MATRIX44 operator*(float s, const MATRIX44& mat)
{
	return mat * s;
}

inline
MATRIX33 operator*(float s, const MATRIX33& mat)
{
	return mat * s;
}

inline 
float vecDot(const VECTOR2& v1, const VECTOR2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

inline 
float vecDot(const VECTOR3& v1, const VECTOR3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline 
float vecDot(const VECTOR4& v1, const VECTOR4& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

inline 
float quatDot(const QUATERNION& q1, const QUATERNION& q2)
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

inline 
VECTOR3 vecCross(const VECTOR3& v1, const VECTOR3& v2)
{
	return VECTOR3(v1.y * v2.z - v1.z * v2.y, 
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}

inline
MATRIX44 operator+(const MATRIX44& m1, const MATRIX44& m2)
{
	MATRIX44 ret;
	for(int i = 0; i < 16; ++i)
	{
		ret[i] = m1[i] + m2[i];
	}
	return ret;
}
inline
MATRIX33 operator+(const MATRIX33& m1, const MATRIX33& m2)
{
	MATRIX33 ret;
	for(int i = 0; i < 9; ++i)
	{
		ret[i] = m1[i] + m2[i];
	}
	return ret;
}

inline
MATRIX44 operator-(const MATRIX44& m1, const MATRIX44& m2)
{
	MATRIX44 ret;
	for(int i = 0; i < 16; ++i)
	{
		ret[i] = m1[i] - m2[i];
	}
	return ret;
}
inline
MATRIX33 operator-(const MATRIX33& m1, const MATRIX33& m2)
{
	MATRIX33 ret;
	for(int i = 0; i < 9; ++i)
	{
		ret[i] = m1[i] - m2[i];
	}
	return ret;
}

inline
std::ostream& operator<<(std::ostream& o, const MATRIX44& m)
{
	using namespace std;

	o.setf(ios_base::fixed, ios_base::floatfield);

	o << m[0]  <<"\t" << m[1]  << "\t" << m[2]  << "\t" << m[3]  << endl;
	o << m[4]  <<"\t" << m[5]  << "\t" << m[6]  << "\t" << m[7]  << endl;
	o << m[8]  <<"\t" << m[9]  << "\t" << m[10] << "\t" << m[11] << endl;
	o << m[12] <<"\t" << m[13] << "\t" << m[14] << "\t" << m[15];
	
	o.setf(ios_base::fmtflags(0), ios_base::floatfield);
	return o;
}
inline
std::ostream& operator<<(std::ostream& o, const VECTOR2& v)
{
	using namespace std;

	o.setf(ios_base::fixed, ios_base::floatfield);

	o << "[" << v.x << "," << v.y << "]";

	o.setf(ios_base::fmtflags(0), ios_base::floatfield);

	return o;
}
inline
std::ostream& operator<<(std::ostream& o, const VECTOR3& v)
{
	using namespace std;

	o.setf(ios_base::fixed, ios_base::floatfield);

	o << "[" << v.x << "," << v.y << "," << v.z << "]";

	o.setf(ios_base::fmtflags(0), ios_base::floatfield);

	return o;
}
inline
std::ostream& operator<<(std::ostream& o, const VECTOR4& v)
{
	using namespace std;

	o.setf(ios_base::fixed, ios_base::floatfield);

	o << "[" << v.x << "," << v.y << "," << v.z << "," << v.w << "]";

	o.setf(ios_base::fmtflags(0), ios_base::floatfield);

	return o;
}
inline
std::ostream& operator<<(std::ostream& o, const QUATERNION& q)
{
	using namespace std;

	o.setf(ios_base::fixed, ios_base::floatfield);

	o << "[" << q.w << ",(" << q.x << "," << q.y << "," << q.z << ")]";

	o.setf(ios_base::fmtflags(0), ios_base::floatfield);
	
	return o;
}
inline
std::ostream& operator<<(std::ostream& o, const PLANE& p)
{
	using namespace std;

	o.setf(ios_base::fixed, ios_base::floatfield);

	o << "[" << p.a << "," << p.b << "," << p.c << "," << p.d << "]";

	o.setf(ios_base::fmtflags(0), ios_base::floatfield);
	
	return o;
}
inline
std::ostream& operator<<(std::ostream& o, const MATRIX33& m)
{
	using namespace std;

	o.setf(ios_base::fixed, ios_base::floatfield);

	o << m[0]  <<"\t" << m[1]  << "\t" << m[2]  << endl;
	o << m[3]  <<"\t" << m[4]  << "\t" << m[5]  << endl;
	o << m[6]  <<"\t" << m[7]  << "\t" << m[8]  << endl;
		
	o.setf(ios_base::fmtflags(0), ios_base::floatfield);
	return o;
}
//================================================

#endif

