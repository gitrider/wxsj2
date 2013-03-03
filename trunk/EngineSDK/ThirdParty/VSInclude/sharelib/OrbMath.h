#ifndef __ORBMATH__HEADER
#define __ORBMATH__HEADER
#include <cmath>
#include <limits>
#include <memory.h>
#include <iostream>

#ifndef ORB_EXPORT
#define ORB_EXPORT
#endif

//!  namespace orb
/*!
	 namespace orb is used throughout engine orb, as a subset of engine orb, this lib uses
	 namespace orb,too.
	 @date 05/12/2003
	 @version 1.0
	 @author Dan Tong
	 @author mail: Lythm@citiz.net
*/

#ifndef EPSILON
#define EPSILON 0.0001f
#endif

#ifndef PI
#define PI 3.1415926f
#endif

namespace orb
{
	// foward declare
	struct MATRIX44;
	struct MATRIX33;

	//!  Orb vector2
	/*!
		 This struct implemented a 2 dimension vector, with some member fucntions to
		 simplize vector operation.
		 @warning This struct does not have a virtual destructor, so don't derive from this struct
		 unless you know exactly what you are doing.
		 @date 05/12/2003
		 @version 1.0
		 @author Dan Tong
		 @author mail: Lythm@citiz.net
	*/
	struct ORB_EXPORT VECTOR2
	{
		//!  Default Constructor
		/*!
			 This function set all the member value to default value 0.0f.
			 @return no return 
		*/
		VECTOR2();
		
		//!  Constructor
		/*!
			 This function assign x and y value to the vector
			 @param _x x value of the vector
			 @param _y y value of the vector
			 @return no return 
		*/
		VECTOR2(float _x, float _y);

		//!  Operator ==
		/*!  @return if the vector is equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator==(const VECTOR2& other) const;
		
		//! Operator !=
		/*!  @return if the vector is not equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator!=(const VECTOR2& other) const;
		
		//! operator +
		/*!
			 Unary operator
			 @return the positive vector value			 
		*/
		VECTOR2 operator+() const;
		
		//! operator -
		/*!
			 Unary operator
			 @return the negative vector value
		*/
		VECTOR2 operator-() const;
		
		//! Vector length
		/*!
			 @return the module of the vector
		*/
		float Length() const;

		void Change()//得到二维向量的垂直向量
		{
			float m;
			m = x;
			x = -y;
			y = m;
		}

		float Dist(VECTOR2&p1 , VECTOR2&p2);//点到直线的距离

		float Dist(VECTOR2& vector)//两点之间的距离
		{
			return (float)sqrt((x-vector.x)*(x-vector.x)+(y-vector.y)*(y-vector.y));
		}
		

		float isInter(VECTOR2 p1 , VECTOR2 p2)//判断点到直线的投影是否在线段内
		{
			VECTOR2 p;
			float d = Dist(p1,p2);
			
			p.x = p2.x - p1.x;
			p.y = p2.y - p1.y;
			float x1 = p.x;
			p.Normalize();
			p.Change();
			p.x *= d;
			p.y *= d;
			float x2;
			x2 = x - p1.x - p.x;
			float t;
			t = x2 / x1;
			if((t>0)&&(t<1)) return true;
			return false;
		}

		float Dist_sqr(VECTOR2& vector)
		{
			return (x-vector.x)*(x-vector.x)+(y-vector.y)*(y-vector.y);
		}

		float Getradian()//得到向量的弧度
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
			return 0.0f;
		}
		void Rotate(float s)//旋转向量
		{
			{
				float x1,y1;
				x1 = x*(float)cos(s) - y*(float)sin(s);
				y1 = x*(float)sin(s) + y*(float)cos(s);
				x = x1;
				y = y1;
			}
		}
		
		//! Normalize operation
		/*!
			 This funtction normalize the vector
			 @return the const reference of the vector
		*/
		const VECTOR2& Normalize();

		//! GetNormalized operation
		/*!
			 @return a normalized vector of the vector
		*/
		VECTOR2 GetNormalized() const;


		const VECTOR2& operator +=(const VECTOR2& other);
		const VECTOR2& operator -=(const VECTOR2& other);
		const VECTOR2& operator *=(float s);
		const VECTOR2& operator /=(float s);

		// data member

		float x;	/*!< x member of the vector */
		float y;	/*!< y member of the vector */
	};

	//!  Orb vector3
	/*!
		 This struct implemented a 3 dimension vector, with some member fucntions to
		 simplize vector operation.
		 @warning This struct does not have a virtual destructor, so don't derive from this struct
		 unless you know exactly what you are doing.
		 @date 05/12/2003
		 @version 1.0
		 @author Dan Tong
		 @author mail: Lythm@citiz.net
	*/
	struct ORB_EXPORT VECTOR3
	{
		//!  Default Constructor
		/*!
			 This function set all the member value to default value 0.0f.
			 @return no return
		*/
		VECTOR3();

		//!  Constructor
		/*!
			 This function assign x, y and z value to the vector
			 @param _x x value of the vector
			 @param _y y value of the vector
			 @param _z z value of the vector
			 @return no return
		*/
		VECTOR3(float _x, float _y, float _z);

		//!  Operator ==
		/*!  @return if the vector is equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator==(const VECTOR3& other) const;

		//!  Operator !=
		/*!  @return if the vector is not equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator!=(const VECTOR3& other) const;

		//! operator +
		/*!
			 Unary operator
			 Sign operation, return the positive vector value
			 @return the positive vector value
		*/
		VECTOR3 operator+() const;
		
		//! operator -
		/*!
			 Unary operator
			 Sign operation, return the negative vector value
			 @return the negative vector value
		*/
		VECTOR3 operator-() const;
		
		//! Vector length
		/*!
			 @return the module of the vector
		*/
		float Length() const;


		float Mag()
		{
			return (float)sqrt(x*x + y*y + z*z);
		}
		float Mag_sqr()
		{
			return x*x + y*y + z*z;
		}
		
		float Dist(VECTOR3& vector)
		{
			return (float)sqrt((x-vector.x)*(x-vector.x)+(y-vector.y)*(y-vector.y)+(z-vector.z)*(z-vector.z));
		}

		float Dist_sqr(VECTOR3& vector)
		{
			return ((x-vector.x)*(x-vector.x)+(y-vector.y)*(y-vector.y)+(z-vector.z)*(z-vector.z));
		}

		float Dist_sqrxz(VECTOR3 &vector )
		{
			return ((x-vector.x)*(x-vector.x)+(z-vector.z)*(z-vector.z));
		}


		inline void makeFloor( const VECTOR3& cmp )
		{
			if( cmp.x < x ) x = cmp.x;
			if( cmp.y < y ) y = cmp.y;
			if( cmp.z < z ) z = cmp.z;
		}


		inline void makeCeil( const VECTOR3& cmp )
		{
			if( cmp.x > x ) x = cmp.x;
			if( cmp.y > y ) y = cmp.y;
			if( cmp.z > z ) z = cmp.z;
		}

		//! Normalize operation
		/*!
			 This funtction normalize the vector
			 @return the const refernce of the vector.
		*/
		const VECTOR3& Normalize();

		//! GetNormalized operation
		/*!
			 @return the normalized vector
		*/
		VECTOR3 GetNormalized() const;

		const VECTOR3& operator +=(const VECTOR3& other);
		const VECTOR3& operator -=(const VECTOR3& other);
		const VECTOR3& operator *=(float s);
		const VECTOR3& operator /=(float s);

		// data member

		float x;			/*!< x member of the vector */
		float y;			/*!< y member of the vector */
		float z;			/*!< z member of the vector */
	};

	//!  Orb vector4
	/*!
		 This struct implemented a 4 dimension vector, with some member fucntions to
		 simplize vector operation.
		 @warning This struct does not have a virtual destructor, so don't derive from this struct
		 unless you know exactly what you are doing.
		 @date 05/12/2003
		 @version 1.0
		 @author Dan Tong
		 @author mail: Lythm@citiz.net
	*/
	struct ORB_EXPORT VECTOR4
	{
		//!  Default Constructor
		/*!
			 This function set all the member value to default value 0.0f.
			 @return no return.
		*/
		VECTOR4();

		//!  Constructor
		/*!
			 This function assign x, y, z and w value to the vector
			 @param _x x value of the vector
			 @param _y y value of the vector
			 @param _z z value of the vector
			 @param _w w value of the vector
			 @return no return
		*/
		VECTOR4(float _x, float _y, float _z, float _w);

		//!  Operator ==
		/*!  @return if the vector is equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator==(const VECTOR4& other) const;

		//!  Operator !=
		/*!  @return if the vector is not equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator!=(const VECTOR4& other) const;

		//! operator +
		/*!
			 Unary operator
			 Sign operation, return the positive vector value
			 @return the positive vector value
		*/
		VECTOR4 operator+() const;

		//! operator -
		/*!
			 Unary operator
			 Sign operation, return the negative vector value
			 @return the negative vector value
		*/
		VECTOR4 operator-() const;

		//! Vector length
		/*!
			 @return the module of the vector
		*/
		float Length() const;

		//! Normalize operation
		/*!
			 This funtction normalize the vector
			 @return the const reference of the vector.
		*/
		const VECTOR4& Normalize();

		//! GetNormalized operation
		/*!
			 @return the normalized vecor
		*/
		VECTOR4 GetNormalized() const;

		const VECTOR4& operator +=(const VECTOR4& other);
		const VECTOR4& operator -=(const VECTOR4& other);
		const VECTOR4& operator *=(float s);
		const VECTOR4& operator /=(float s);


		// data member
		float x;			/*!< x member of the vector */
		float y;			/*!< y member of the vector */
		float z;			/*!< z member of the vector */
		float w;			/*!< w member of the vector */
	};

	//!  Orb quaternion
	/*!
		 This struct implemented a quaternion, with some member fucntions to
		 simplize quaternion operation.
		 @warning This struct does not have a virtual destructor, so don't derive from this struct
		 unless you know exactly what you are doing.
		 @date 05/12/2003
		 @version 1.0
		 @author Dan Tong
		 @author mail: Lythm@citiz.net
	*/
	struct ORB_EXPORT QUATERNION
	{
		//!  Default Constructor
		/*!
			 This function set all the member value to default value 0.0f.
			 @return no return.
		*/
		QUATERNION();

		//!  Constructor
		/*!
			 This function assign x, y, z and w value to the quaternion
			 @param _x x value of the quaternion
			 @param _y y value of the quaternion
			 @param _z z value of the quaternion
			 @param _w w value of the quaternion
			 @return no return.
		*/
		QUATERNION(float _x, float _y, float _z, float _w);
		
		//! Constructor
		/*!
			 This function construct a quaternion from a MATRIX33
			 @param mat matrix to construct quaternion
			 @return no return.
		*/
		explicit QUATERNION(const MATRIX33& mat);

		//! Constructor
		/*!
			 This function construct a quaternion from a MATRIX44
			 @param mat matrix to construct quaternion
			 @return no return.
		*/
		explicit QUATERNION(const MATRIX44& mat);
		
		//!  Operator ==
		/*!  @return if the vector is equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator == (const QUATERNION& other) const;

		//!  Operator !=
		/*!  @return if the vector is not equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator != (const QUATERNION& other) const;

		//! operator +
		/*!
			 Unary operator
			 Sign operation, return the negative quaternion value
			 @return the negative quaternion value
		*/
		QUATERNION operator + () const;

		//! operator -
		/*!
			 Unary operator
			 Sign operation, return the negative quaternion value
			 @return the negative quaternion value
		*/
		QUATERNION operator - () const;
		
		//! operator +=
		/*!
			 This operator add another quaternion to itself.
			 @return the reference of *this.
		*/
		QUATERNION& operator += (const QUATERNION& other);

		//! operator +=
		/*!
			 This operator subtract another quaternion from itself.
			 @return the reference of *this.
		*/
		QUATERNION& operator -= (const QUATERNION& other);

		//! operator *=
		/*!
			 This operator multiply another quaternion to itself.
			 @return the reference of *this.
		*/
		QUATERNION& operator *= (const QUATERNION& other);

		//! operator *=
		/*!
			 This operator multiply a float to itself.
			 @return the reference of *this.
		*/
		QUATERNION& operator *= (float f);

		//! operator /=
		/*!
			 This operator divide a float from itself.
			 @return the reference of *this.
		*/
		QUATERNION& operator /= ( float f );

		//! Quaternion length
		/*!
			 @return the module of the quaternion
		*/
		float Length() const;
	
		//!  IsIdentity
		/*!
			 @return If the quaternion is a identity one, this function returns @b true.
			 Otherwise, it returns @b false.
		*/
		bool IsIdentity() const;

		//!  SetIdentity
		/*!
			 This function initialize the quaternion with identity value
			 @return the const reference of the quaternion.
		*/
		const QUATERNION& SetIdentity();

		//! Normalize operation
		/*!
			 This funtction normalize the quaternion
			 @return the const reference of the quaternion.
		*/
		const QUATERNION& Normalize();

		//! Normalize operation
		/*!
			 @return the normalized quaternion
		*/
		QUATERNION GetNormalize() const;

		// data member

		float x;			/*!< x member of the quaternion */
		float y;			/*!< y member of the quaternion */
		float z;			/*!< z member of the quaternion */
		float w;			/*!< w member of the quaternion */
	};

	//!  Orb 3x3 rotation Matrix
	/*!
		 This struct implemented a 3x3 matrix, with some member fucntions to
		 simplize matrix operation.
		 @warning This struct does not have a virtual destructor, so don't derive from this struct
		 unless you know exactly what you are doing.
		 @date 05/12/2003
		 @version 1.0
		 @author Dan Tong
		 @author mail: Lythm@citiz.net
	*/
	struct ORB_EXPORT MATRIX33
	{
		//!  Default Constructor
		/*!
			 This function set all the member value to default value 0.0f.
			 @return no return.
		*/
		MATRIX33();

		//!  Constructor
		/*!
			 This function construct the matrix with a float[9] array.
			 @return no return.
		*/
		explicit MATRIX33(float _m[9]);
		
		//!  Constructor
		/*!
			 This function construct the matrix with 9 float parameters
			 @return no return.
		*/
		MATRIX33(	float _m11, float _m12, float _m13,
			float _m21, float _m22, float _m23, 
			float _m31, float _m32, float _m33);

		//!  Constructor
		/*!
			 This function construct the matrix with quaternion.
			 @param quat the unit quaternion to construct the matrix
			 @return no return.
			 @warning the quaternion must be a unit one
		*/
		explicit MATRIX33(const QUATERNION& quat);

		//!  float* extractor
		/*!
			 This function turn the matrix to a quaternion.
			 @return the quaternion value of this matrix.
		*/
		operator float*() const;
		//!  Operator()
		/*!
			 This function return the reference to the sub value of the matrix, 
			 which locate at [row, col].
			 @param row the row
			 @param col the column
			 @return The reference to the sub value of the matrix.
		*/
		float& operator()(int row, int col);

		//!  Operator()
		/*!
			 This is a const version of the operator().
			 @see float& operator()(int row, int col)
		*/
		const float& operator()(int row, int col) const;
	
		//!  Operator+=
		/*!
			 This function add another matrix to itself.
			 @param other another matrix
			 @return the the const reference of *this
		*/
		const MATRIX33& operator+=(const MATRIX33& other);

		//!  Operator[]
		/*!
			 This function return the reference to the sub value of the matrix, 
			 which locate at [nIndex].
			 @param nIndex the nIndex
			 @return The reference to the sub value of the matrix.
		*/
		float& operator[](int nIndex);

		//!  Operator[]
		/*!
			 This is a const version of the operator().
			 @see float& operator[](int nIndex);
		*/
		const float& operator[](int nIndex) const;

		//!  Operator-=
		/*!
			 This function subtract another matrix from itself.
			 @param other another matrix
			 @return the the const reference of *this
		*/
		const MATRIX33& operator-=(const MATRIX33& other);

		//!  Operator*=
		/*!
			 This function multiply another matrix to itself.
			 @param other another matrix
			 @return the the const reference of *this
		*/
		const MATRIX33& operator*=(const MATRIX33& other);

		//!  Operator==
		/*!  @return if the vector is equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator==(const MATRIX33& other) const;

		//!  Operator!=
		/*!  @return if the vector is not equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator!=(const MATRIX33& other) const;

		//!  SetIdentity
		/*!
			 This function initialize the matrix with identity value.
			 @return the const reference of the matrix.
		*/
		const MATRIX33& SetIdentity();

		//!  IsIdentity
		/*!
			 @return If the matrix is a identity one, this function returns @b true.
			 Otherwise, it returns @b false.
		*/
		bool isIdentity() const;

		//!  Transpose
		/*!
			 Transpose the matrix.
			 @return the const reference of the matrix.
		*/
		const MATRIX33& Transpose();

		//!  GetTranspose
		/*!
			 Get the Transpose of the matrix.
			 @return the tranpose of the matrix.
		*/
		MATRIX33 GetTranspose() const;

		//!  Invert
		/*!
			 Inverse the matrix.
			 @return the const reference of the matrix
		*/
		const MATRIX33& Invert();

		//!  GetInverse
		/*!
			 Get the inverse of the matrix.
			 @return the inverse of the matrix
		*/
		MATRIX33 GetInverse() const;

		//!  Determinant
		/*!
			 @return the Determinant of this matrix
		*/
		float Determinant() const;
		
		//!  GetRow
		/*!
			 Get a row of the matrix
			 @return the specified row vector.
		*/
		VECTOR3 GetRow(int row) const;

		//!  GetLine
		/*!
			 Get a Line of the matrix
			 @return the specified line vector.
		*/
		VECTOR3 GetLine(int line) const;

		//!  SetLine
		/*!
			 Set a Line of the matrix
			 @return no return.
		*/
		void SetLine(int line, const VECTOR3& v);

		//!  SetRow
		/*!
			 Set a row of the matrix
			 @return no return.
		*/
		void SetRow(int row, const VECTOR3& v);

		private:
		union
		{
			float	m[9];							/*!< matrix member int float[16] format */
			
			struct	
			{
				float	m11,						/*!< matrix member [1,1]*/
						m12,						/*!< matrix member [1,2]*/
						m13,						/*!< matrix member [1,3]*/
						m21, 						/*!< matrix member [2,1]*/
						m22, 						/*!< matrix member [2,2]*/
						m23, 						/*!< matrix member [2,3]*/
						m31, 						/*!< matrix member [3,1]*/
						m32, 						/*!< matrix member [3,2]*/
						m33; 						/*!< matrix member [3,3]*/
			};										
		};
	};

	//!  Orb 4x4 Matrix
	/*!
		 This struct implemented a 4x4 matrix, with some member fucntions to
		 simplize matrix operation.
		 @warning This struct does not have a virtual destructor, so don't derive from this struct
		 unless you know exactly what you are doing.
		 @date 05/12/2003
		 @version 1.0
		 @author Dan Tong
		 @author mail: Lythm@citiz.net
	*/
	struct ORB_EXPORT MATRIX44
	{
		//!  Default Constructor
		/*!
			 This function set all the member value to default value 0.0f.
			 @return no return.
		*/
		MATRIX44();

		//!  Constructor
		/*!
			 This function construct the matrix with a 3x3 matrix.
			 @return no return.
		*/
		MATRIX44(const MATRIX33& other);


		//!  Constructor
		/*!
			 This function construct the matrix with a float[16] array.
			 @return no return.
		*/
		explicit MATRIX44(float _m[16]);
		
		//!  Constructor
		/*!
			 This function construct the matrix with 16 float parameters
			 @return no return.
		*/
		MATRIX44(	float _m11, float _m12, float _m13, float _m14, 
			float _m21, float _m22, float _m23, float _m24,
			float _m31, float _m32, float _m33, float _m34,
			float _m41, float _m42, float _m43, float _m44);

		//!  Constructor
		/*!
			 This function construct the matrix with quaternion.
			 @param quat the unit quaternion to construct the matrix
			 @return no return.
			 @warning the quaternion must be a unit one
		*/
		explicit MATRIX44(const QUATERNION& quat);

		//!  float* extractor
		/*!
			 This function turn the matrix to a quaternion.
			 @return the quaternion value of this matrix.
		*/
		operator float*() const;

		//!  Operator()
		/*!
			 This function return the reference to the sub value of the matrix, 
			 which locate at [row, col].
			 @param row the row
			 @param col the column
			 @return The reference to the sub value of the matrix.
		*/
		float& operator()(int row, int col);

		//!  Operator()
		/*!
			 This is a const version of the operator().
			 @see float& operator()(int row, int col)
		*/
		const float& operator()(int row, int col) const;
	
		//!  Operator+=
		/*!
			 This function add another matrix to itself.
			 @param other another matrix
			 @return the the const reference of *this
		*/
		const MATRIX44& operator+=(const MATRIX44& other);

		//!  Operator[]
		/*!
			 This function return the reference to the sub value of the matrix, 
			 which locate at [nIndex].
			 @param nIndex the nIndex
			 @return The reference to the sub value of the matrix.
		*/
		float& operator[](int nIndex);

		//!  Operator[]
		/*!
			 This is a const version of the operator().
			 @see float& operator[](int nIndex);
		*/
		const float& operator[](int nIndex) const;

		//!  Operator-=
		/*!
			 This function subtract another matrix from itself.
			 @param other another matrix
			 @return the the const reference of *this
		*/
		const MATRIX44& operator-=(const MATRIX44& other);

		//!  Operator*=
		/*!
			 This function multiply another matrix to itself.
			 @param other another matrix
			 @return the the const reference of *this
		*/
		const MATRIX44& operator*=(const MATRIX44& other);

		//!  Operator==
		/*!  @return if the vector is equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator==(const MATRIX44& other) const;

		//!  Operator!=
		/*!  @return if the vector is not equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator!=(const MATRIX44& other) const;

		//!  SetIdentity
		/*!
			 This function initialize the matrix with identity value.
			 @return the const reference of the matrix.
		*/
		const MATRIX44& SetIdentity();

		//!  IsIdentity
		/*!
			 @return If the matrix is a identity one, this function returns @b true.
			 Otherwise, it returns @b false.
		*/
		bool isIdentity() const;

		//!  Transpose
		/*!
			 Transpose the matrix.
			 @return the const reference of the matrix.
		*/
		const MATRIX44& Transpose();

		//!  GetTranspose
		/*!
			 Get the Transpose of the matrix.
			 @return the tranpose of the matrix.
		*/
		MATRIX44 GetTranspose() const;

		//!  Invert
		/*!
			 Inverse the matrix.
			 @return the const reference of the matrix
		*/
		const MATRIX44& Invert();

		//!  GetInverse
		/*!
			 Get the inverse of the matrix.
			 @return the inverse of the matrix
		*/
		MATRIX44 GetInverse() const;

		//!  Determinant
		/*!
			 @return the Determinant of this matrix
		*/
		float Determinant() const;
		
		//!  GetRow
		/*!
			 Get a row of the matrix
			 @return the specified row vector
		*/
		VECTOR4 GetRow(int row) const;

		//!  GetLine
		/*!
			 Get a line of the matrix
			 @return the specified line vector
		*/
		VECTOR4 GetLine(int line) const;

		//!  SetLine
		/*!
			 Set a line of the matrix
			 @return no return
		*/
		void SetLine(int line, const VECTOR4& v);

		//!  SetRow
		/*!
			 Set a row of the matrix
			 @return no return
		*/
		void SetRow(int row, const VECTOR4& v);

		private:
		union
		{
			float	m[16];							/*!< matrix member int float[16] format */
			
			struct	
			{
				float	m11,						/*!< matrix member [1,1]*/
						m12,						/*!< matrix member [1,2]*/
						m13,						/*!< matrix member [1,3]*/
						m14,						/*!< matrix member [1,4]*/
						m21, 						/*!< matrix member [2,1]*/
						m22, 						/*!< matrix member [2,2]*/
						m23, 						/*!< matrix member [2,3]*/
						m24,						/*!< matrix member [2,4]*/
						m31, 						/*!< matrix member [3,1]*/
						m32, 						/*!< matrix member [3,2]*/
						m33, 						/*!< matrix member [3,3]*/
						m34,						/*!< matrix member [3,4]*/
						m41, 						/*!< matrix member [4,1]*/
						m42, 						/*!< matrix member [4,2]*/
						m43, 						/*!< matrix member [4,3]*/
						m44;						/*!< matrix member [4,4]*/
			};										
		};
	};

	struct LINE2D
	{
		VECTOR2 p1 , p2;
		bool IsIntersect(LINE2D& line)
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
		VECTOR2 GetIntersectPoint(LINE2D& line);
	
	};


	struct CIRCLE2D
	{
	public:
		VECTOR2 pos;
		float radius;
		bool bintersect;
	public:
		bool isIntersect(VECTOR2& p1 , VECTOR2& p2);//判断线段和圆是否相交
		void Set(VECTOR2 &v , float& r);
		VECTOR2 getPoint(VECTOR2& p1 , VECTOR2& p2);//得到圆相对于p1的切点
		void operator =(CIRCLE2D c)
		{
			pos = c.pos;
			radius = c.radius;
			bintersect = c.bintersect;
		}
		CIRCLE2D();
		virtual ~CIRCLE2D();
		
	};

	//!  Orb PLANE
	/*!
		 This struct implemented a Plane.
		 
		 @warning This struct does not have a virtual destructor, so don't derive from this struct
		 unless you know exactly what you are doing.
		 @date 05/28/2003
		 @version 1.0
		 @author Dan Tong
		 @author mail: Lythm@citiz.net
	*/
	struct ORB_EXPORT PLANE
	{
		//!  Default Constructor
		/*!
			 This function set all the member value to default value 0.0f.
			 @return no return.
		*/
		PLANE();

		//!  Copy Constructor
		PLANE(const PLANE& other);

		//!  Constructor
		/*!
			 Construct the plane with four coefficient.
			 AX + BY + CZ + D = 0
			 @return no return.
		*/
		PLANE(float A, float B, float C, float D);

		//!  Constructor
		/*!
			 Construct the plane from three points
			 @return no return.
		*/
		PLANE(const VECTOR3& p1, const VECTOR3& p2, const VECTOR3& p3);

		//!  Constructor
		/*!
			 Construct the plane from a normal and a point
			 @return no return.
		*/
		PLANE(const VECTOR3& n, const VECTOR3& p);

		//!  Normalize
		/*!
			 Normalize the plane.
			 @return const refernce of *this
		*/
		const PLANE& Normalize();

		//!  GetNormalized
		/*!
			 @return the normalized plane of this plane
		*/
		PLANE GetNormalized() const;

		//!  PointDistance
		/*!
			 Calculate the distance between the given point and this plane
			 @return distance
		*/
		float PointDistance(const VECTOR3& point) const;

		//!  Operator ==
		/*!  @return if the plane is equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator==(const PLANE& other) const;

		//!  Operator !=
		/*!  @return if the plane is not equal to parameter @b other, the function 
			 returns @b true, otherwise, it returns @b false.
		*/
		bool operator!=(const PLANE& other) const;

		//!  Operator float*
		operator float* ();
		
		union
		{
			struct 
			{
				float a, b, c, d;				/*!< plane formular coefficient---- AX + BY +CZ +D = 0*/
			};
			struct 
			{
				VECTOR3	normal;					/*!< plane face normal*/
				float	d;						/*!< value of any a point in the plane dot normal*/
			};
			float v[4];
		};
		
	};

	//!  planeDotVec
	/*!
		 Calculate a vector4 and a plane's dot production
		 @return the dot production
	*/
	ORB_EXPORT float planeDotVec(const PLANE& plane, const VECTOR4& v);

	//!  planeDotVec
	/*!
		 Calculate a vector3 and a plane's dot production, which assume the w of 
		 the vector is 1
		 @return the dot production
	*/
	ORB_EXPORT float planeDotVec(const PLANE& plane, const VECTOR3& v);

	//!  planeDotNormal
	/*!
		 Calculate a vector3 and a plane's dot production, which assume the w of 
		 the vector is 0
		 @return the dot production
	*/
	ORB_EXPORT float planeDotNormal(const PLANE& plane, const VECTOR3& v);

	//!  planeTransform
	/*!
		 Transform the plane by a matrix
		 @return the const reference of the parameter 1
	*/
	ORB_EXPORT const PLANE& planeTransform(PLANE& plane, const MATRIX44& m);
	
	//!  planeLineIntersection
	/*!
		 Calculate the intersection of a plane and a line
		 @return the intersection
	*/
	ORB_EXPORT VECTOR3 planeLineIntersection(const PLANE& plane, const VECTOR3& start, const VECTOR3& end);

	//!  matBuildLookAtLH
	/*!
		 Build a 4x4 view matrix in the left-handed system
		 @param eye the eye pos
		 @param at  the at pos
		 @param up  the up direction
		 @return the view matrix
	*/
	ORB_EXPORT MATRIX44 matBuildLookAtLH(const VECTOR3& eye, const VECTOR3& at, const VECTOR3& up);

	//!  matBuildLookAtRH
	/*!
		 Build a 4x4 view matrix in the right-handed system
		 @param eye the eye pos
		 @param at  the at pos
		 @param up  the up direction
		 @return the view matrix
	*/
	ORB_EXPORT MATRIX44 matBuildLookAtRH(const VECTOR3& eye, const VECTOR3& at, const VECTOR3& up);

	//!  matBuildPerspectiveLH
	/*!
		 Build a 4x4 perspective matrix for left-handed coord system
		 @param nvp the z of the near view-plane
		 @param fvp the z of the far view-plane
		 @param fov field of view, in radians.
		 @param aspect Aspect ratio.
		 @return the perspective matrix
	*/
	ORB_EXPORT MATRIX44 matBuildPerspectiveLH(float nvp, float fvp, float fov, float aspect);

	//!  matBuildPerspectiveRH
	/*!
		 Build a 4x4 perspective matrix for right-handed coord system
		 @param nvp the z of the near view-plane
		 @param fvp the z of the far view-plane
		 @param fov field of view, in radians.
		 @param aspect Aspect ratio.
		 @return the perspective matrix
	*/
	ORB_EXPORT MATRIX44 matBuildPerspectiveRH(float nvp, float fvp, float fov, float aspect);


	//!  matBuildRotationAxis
	/*!
		 Build a 3x3 matrix from rotation against a specified axis(a vector3).
		 @param	axis a vector3 specified the axis that the rotation against. 
			The axis vector must be normalized before passed in.
		 @param radian the angle to rotate in radian
		 @return the rotation matrix
	*/
	ORB_EXPORT MATRIX33 matBuildRotationAxis(const VECTOR3& axis, float radian);
	

	//!  matBuildRotationX
	/*!
		 Build a 3x3 matrix from rotation against X axis.
		 @param radian the angle to rotate in radian
		 @return the rotation matrix
	*/
	ORB_EXPORT MATRIX33 matBuildRotationX(float radian);

	//!  matBuildRotationY
	/*!
		 Build a 3x3 matrix from rotation against Y axis.
		 @param radian the angle to rotate in radian
		 @return the rotation matrix
	*/
	ORB_EXPORT	MATRIX33 matBuildRotationY(float radian);

	//!  matBuildRotationZ
	/*!
		 Build a 3x3 matrix from rotation against Z axis.
		 @param radian the angle to rotate in radian
		 @return the rotation matrix
	*/
	ORB_EXPORT MATRIX33 matBuildRotationZ(float radian);

	//!  matBuildScale
	/*!
		 Build a 4x4 scaling matrix.
		 @param sx scale of the x axis
		 @param sy scale of the y axis
		 @param sz scale of the z axis
		 @return the scaling matrix
	*/
	ORB_EXPORT MATRIX44 matBuildScale(float sx, float sy, float sz);

	//!  matBuildTranslation
	/*!
		 Build a 4x4 translation matrix.
		 @param x offset of the x axis
		 @param y offset of the y axis
		 @param z offset of the z axis
		 @return the translation matrix
	*/
	ORB_EXPORT MATRIX44 matBuildTranslation(float x, float y, float z);

	//!  vector2 operator+
	/*!
		 Add two vector2 and return the result.
		 @param v1 a vector
		 @param v2 a vector
		 @return the result of the addition
	*/
	ORB_EXPORT VECTOR2 operator+(const VECTOR2& v1, const VECTOR2& v2);

	//!  vector3 operator+
	/*!
		 Add two vector3 and return the result.
		 @param v1 a vector
		 @param v2 a vector
		 @return the result of the addition
	*/
	ORB_EXPORT VECTOR3 operator+(const VECTOR3& v1, const VECTOR3& v2);

	//!  vector4 operator+
	/*!
		 Add two vector4 and return the result.
		 @param v1 a vector
		 @param v2 a vector
		 @return the result of the addition
	*/
	ORB_EXPORT VECTOR4 operator+(const VECTOR4& v1, const VECTOR4& v2);

	//!  quaternion operator+
	/*!
		 Add two quaternion and return the result.
		 @param q1 a quaternion
		 @param q2 a quaternion
		 @return the result of the addition
	*/
	ORB_EXPORT QUATERNION operator+(const QUATERNION& q1, const QUATERNION& q2);

	//!  vector2 operator-
	/*!
		 Subtract two vector2 and return the result.
		 @param v1 a vector
		 @param v2 a vector
		 @return the result of the subtraction
	*/
	ORB_EXPORT VECTOR2 operator-(const VECTOR2& v1, const VECTOR2& v2);

	//!  vector3 operator-
	/*!
		 Subtract two vector3 and return the result.
		 @param v1 a vector
		 @param v2 a vector
		 @return the result of the subtraction
	*/
	ORB_EXPORT VECTOR3 operator-(const VECTOR3& v1, const VECTOR3& v2);

	//!  vector4 operator-
	/*!
		 Subtract two vector4 and return the result.
		 @param v1 a vector
		 @param v2 a vector
		 @return the result of the subtraction
	*/
	ORB_EXPORT VECTOR4 operator-(const VECTOR4& v1, const VECTOR4& v2);

	//!  quaternion operator-
	/*!
		 Subtract two quaternion and return the result.
		 @param q1 a quaternion
		 @param q2 a quaternion
		 @return the result of the subtraction
	*/
	ORB_EXPORT QUATERNION operator-(const QUATERNION& q1, const QUATERNION& q2);

	//!  vector2 operator*
	/*!
		 Multiply a vector2 with a float
		 @param v a vector
		 @param s a float
		 @return the result of the multiplication
	*/
	ORB_EXPORT VECTOR2 operator*(const VECTOR2& v, float s);

	//!  vector2 operator*
	/*!
		 Multiply a float with a vector2.
		 @param s a float
		 @param v a vector
		 @return the result of the multiplication
	*/
	ORB_EXPORT VECTOR2 operator*(float s, const VECTOR2& v);
	
	//!  vector3 operator*
	/*!
		 Multiply a vector3 with a float
		 @param v a vector
		 @param s a float
		 @return the result of the multiplication
	*/
	ORB_EXPORT VECTOR3 operator*(const VECTOR3& v, float s);

	//!  vector3 operator*
	/*!
		 Multiply a float with a vector3.
		 @param s a float
		 @param v a vector
		 @return the result of the multiplication
	*/
	ORB_EXPORT VECTOR3 operator*(float s, const VECTOR3& v);

	//!  vector4 operator*
	/*!
		 Multiply a vector4 with a float
		 @param v a vector
		 @param s a float
		 @return the result of the multiplication
	*/
	ORB_EXPORT VECTOR4 operator*(const VECTOR4& v, float s);

	//!  vector4 operator*
	/*!
		 Multiply a float with a vector4.
		 @param s a float
		 @param v a vector
		 @return the result of the multiplication
	*/
	ORB_EXPORT VECTOR4 operator*(float s, const VECTOR4& v);

	//!  quaternion operator*
	/*!
		 Multiply a quaternion with a float
		 @param q a quaternion to multiply
		 @param s a float to multiply
		 @return the result of the multiplication
	*/
	ORB_EXPORT QUATERNION operator*(const QUATERNION& q, float s);

	//!  quaternion operator*
	/*!
		 Multiply a float with a quaternion.
		 @param s a float to multiply
		 @param q a quaternion to multiply
		 @return the result of the multiplication
	*/
	ORB_EXPORT QUATERNION operator*(float s, const QUATERNION& q);

	//!  matrix operator*
	/*!
		 Multiply a matrix with a float.
		 @param mat a matrix to multiply
		 @param s a float to multiply
		 @return the result of the multiplication
	*/
	ORB_EXPORT MATRIX44 operator*(const MATRIX44& mat, float s);

	//!  matrix operator*
	/*!
		 Multiply a matrix with a float.
		 @param mat a matrix to multiply
		 @param s a float to multiply
		 @return the result of the multiplication
	*/
	ORB_EXPORT MATRIX33 operator*(const MATRIX33& mat, float s);

	//!  matrix operator*
	/*!
		 Multiply a float with a matrix.
		 @param s a float to multiply
		 @param mat a matrix to multiply
		 @return the result of the multiplication
	*/
	ORB_EXPORT MATRIX44 operator*(float s, const MATRIX44& mat);

	//!  matrix operator*
	/*!
		 Multiply a float with a matrix.
		 @param s a float to multiply
		 @param mat a matrix to multiply
		 @return the result of the multiplication
	*/
	ORB_EXPORT MATRIX33 operator*(float s, const MATRIX33& mat);
	
	//!  vector2 dot production
	/*!
		 @param v1 a vector
		 @param v2 a vector
		 @return the result of dot production
	*/
	ORB_EXPORT float vecDot(const VECTOR2& v1, const VECTOR2& v2);

	//!  vector3 dot production
	/*!
		 @param v1 a vector
		 @param v2 a vector
		 @return the result of dot production
	*/
	ORB_EXPORT float vecDot(const VECTOR3& v1, const VECTOR3& v2);

	//!  vector4 dot production
	/*!
		 @param v1 a vector
		 @param v2 a vector
		 @return the result of dot production
	*/
	ORB_EXPORT float vecDot(const VECTOR4& v1, const VECTOR4& v2);

	//!  quaternion dot production
	/*!
		 @param v1 a quaternion
		 @param v2 a quaternion
		 @return the result of dot production
	*/
	ORB_EXPORT float quatDot(const QUATERNION& q1, const QUATERNION& q2);

	//!  vector3 cross production
	/*!
		 @param v1 a vector
		 @param v2 a vector
		 @return the result of cross production
	*/
	ORB_EXPORT VECTOR3 vecCross(const VECTOR3& v1, const VECTOR3& v2);

//	VECTOR4 vecCross(const VECTOR4& v1, const VECTOR4& v2, const VECTOR4& v3);

	//!  quaternion operator*
	/*!
		 Multiply two quaternion and return the result.
		 @param q1 the quaternion to multiply
		 @param q2 the quaternion to multiply
		 @return the result of the multiplication
	*/
	ORB_EXPORT QUATERNION operator * ( const QUATERNION& q1 , const QUATERNION& q2);

	//!  quatSlerp
	/*!
		 Calculate the slerp between the two quaternion, and return the result.
		 @param qs start quaternion
		 @param qf end quaternion
		 @param s  Slerp scale
		 @return the slerp result quaternion between the two quaternion
	*/
	ORB_EXPORT QUATERNION quatSlerp(const QUATERNION& qs , const QUATERNION& qf, float s);

	//!  quatBuildRotationAxis
	/*!
		 Build a quaternion from rotation against a specified axis(a vector3).
		 @param	axis a vector3 specified the axis that the rotation against.
		 @param radian the angle to rotate in radian
		 @return the rotation quaternion
	*/
	ORB_EXPORT QUATERNION quatBuildRotationAxis(const VECTOR3& axis, float radian);
		
	//!  matrix operator+
	/*!
		 Add two matrix and return the result.
		 @param m1 the matrix to add
		 @param m2 the matrix to add
		 @return the result matrix of the addition
	*/
	ORB_EXPORT MATRIX44 operator+(const MATRIX44& m1, const MATRIX44& m2);

	//!  matrix operator+
	/*!
		 Add two matrix and return the result.
		 @param m1 the matrix to add
		 @param m2 the matrix to add
		 @return the result matrix of the addition
	*/
	ORB_EXPORT MATRIX33 operator+(const MATRIX33& m1, const MATRIX33& m2);
	
	//!  matrix operator-
	/*!
		 Subtract two matrix and return the result.
		 @param m1 the matrix to subtract from
		 @param m2 the matrix to subtract
		 @return the result matrix of the subtraction
	*/
	ORB_EXPORT MATRIX44 operator-(const MATRIX44& m1, const MATRIX44& m2);

	//!  matrix operator-
	/*!
		 Subtract two matrix and return the result.
		 @param m1 the matrix to subtract from
		 @param m2 the matrix to subtract
		 @return the result matrix of the subtraction
	*/
	ORB_EXPORT MATRIX33 operator-(const MATRIX33& m1, const MATRIX33& m2);
	
	//!  matrix operator*
	/*!
		 Multiply two matrix and return the result.
		 @param m1 the matrix to multiply
		 @param m2 the matrix to multiply
		 @return the result matrix of the multiplication
	*/
	ORB_EXPORT MATRIX44 operator*(const MATRIX44& m1, const MATRIX44& m2);

	//!  matrix operator*
	/*!
		 Multiply two matrix and return the result.
		 @param m1 the matrix to multiply
		 @param m2 the matrix to multiply
		 @return the result matrix of the multiplication
	*/
	ORB_EXPORT MATRIX33 operator*(const MATRIX33& m1, const MATRIX33& m2);

	//!  vecTransform
	/*!
		 Transform a vector 2 by a matrix
		 @return the const reference of the vector
	*/
	ORB_EXPORT const VECTOR2& vecTransform(VECTOR2& v, const MATRIX44& m);

	//!  vecTransform
	/*!
		 Transform a vector 2 by a matrix
		 @return the const reference of the vector
	*/
	ORB_EXPORT const VECTOR2& vecTransform(VECTOR2& v, const MATRIX33& m);

	//!  vecTransform
	/*!
		 Transform a vector 3 by a matrix
		 @return the const reference of the vector
	*/
	ORB_EXPORT const VECTOR3& vecTransform(VECTOR3& v, const MATRIX44& m);

	//!  vecTransform
	/*!
		 Transform a vector 3 by a matrix
		 @return the const reference of the vector
	*/
	ORB_EXPORT const VECTOR3& vecTransform(VECTOR3& v, const MATRIX33& m);

	//!  vecTransform
	/*!
		 Transform a vector 4 by a matrix
		 @return the const reference of the vector
	*/
	ORB_EXPORT const VECTOR4& vecTransform(VECTOR4& v, const MATRIX44& m);

	//!  output to stream
	ORB_EXPORT std::ostream& operator<<(std::ostream& o, const MATRIX44& m);

	//!  output to stream
	ORB_EXPORT std::ostream& operator<<(std::ostream& o, const MATRIX33& m);
	
	//!  output to stream
	ORB_EXPORT std::ostream& operator<<(std::ostream& o, const VECTOR2& v);

	//!  output to stream
	ORB_EXPORT std::ostream& operator<<(std::ostream& o, const VECTOR3& v);

	//!  output to stream
	ORB_EXPORT std::ostream& operator<<(std::ostream& o, const VECTOR4& v);

	//!  output to stream
	ORB_EXPORT std::ostream& operator<<(std::ostream& o, const QUATERNION& q);

	//!  output to stream
	ORB_EXPORT std::ostream& operator<<(std::ostream& o, const PLANE& p);

#include "OrbMath.inl"
}
#endif
