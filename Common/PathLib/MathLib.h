#ifndef __MathLib_H__
#define __MathLib_H__

#include <assert.h>
#include <math.h>

#include <numeric>
#include <limits>
#include <vector>

namespace PathLib
{
    class Vector2;
    class Vector3;

    /**
    copy from ogre
    */
    class Math
    {
    public:

        static inline int IAbs (int iValue) { return ( iValue >= 0 ? iValue : -iValue ); }
        static inline int ICeil (float fValue) { return int(ceil(fValue)); }
        static inline int IFloor (float fValue) { return int(floor(fValue)); }
        static int ISign (int iValue);

        static inline float Abs (float fValue) { return float(fabs(fValue)); }
       
        static inline float Ceil (float fValue) { return float(ceil(fValue)); }

        static inline float Exp (float fValue) { return float(exp(fValue)); }

        static inline float Floor (float fValue) { return float(floor(fValue)); }

        static inline float Log (float fValue) { return float(log(fValue)); }

        static inline float Pow (float fBase, float fExponent) { return float(pow(fBase,fExponent)); }

        static float Sign (float fValue);       

        static inline float Sqr (float fValue) { return fValue*fValue; }

        static inline float Sqrt (float fValue) { return float(sqrt(fValue)); }

        /** Checks whether a given point is inside a triangle, in a
        2-dimensional (Cartesian) space.
        @remarks
        The vertices of the triangle must be given in either
        trigonometrical (anticlockwise) or inverse trigonometrical
        (clockwise) order.
        @param
        p The point.
        @param
        a The triangle's first vertex.
        @param
        b The triangle's second vertex.
        @param
        c The triangle's third vertex.
        @returns
        If the point resides in the triangle, <b>true</b> is
        returned.
        @par
        If the point is outside the triangle, <b>false</b> is
        returned.
        */
        static bool pointInTri2D(const Vector2& p, const Vector2& a, 
            const Vector2& b, const Vector2& c);

        static float squareDistance(const Vector2& pos1, const Vector2& pos2);
        static float distance(const Vector2& pos1, const Vector2& pos2);

        /** Compare 2 reals, using tolerance for inaccuracies.
        */
        static bool RealEqual(float a, float b,
            float tolerance = std::numeric_limits<float>::epsilon());

        static const float POS_INFINITY;
        static const float NEG_INFINITY;
        static const float PI;
        static const float TWO_PI;
        static const float HALF_PI;
        static const float fDeg2Rad;
        static const float fRad2Deg;
    };

    /**
    copy from ogre
    */
    class Vector2
    {
    public:
        float x, y;

    public:
        inline Vector2()
        {
        }

        inline Vector2(const float fX, const float fY )
            : x( fX ), y( fY )
        {
        }

        inline explicit Vector2( const float scaler )
            : x( scaler), y( scaler )
        {
        }

        inline explicit Vector2( const float afCoordinate[2] )
            : x( afCoordinate[0] ),
            y( afCoordinate[1] )
        {
        }

        inline explicit Vector2( const int afCoordinate[2] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
        }

        inline explicit Vector2( float* const r )
            : x( r[0] ), y( r[1] )
        {
        }

        inline Vector2( const Vector2& rkVector )
            : x( rkVector.x ), y( rkVector.y )
        {
        }

        inline float operator [] ( const size_t i ) const
        {
            assert( i < 2 );

            return *(&x+i);
        }

        inline float& operator [] ( const size_t i )
        {
            assert( i < 2 );

            return *(&x+i);
        }

        /// Pointer accessor for direct copying
        inline float* ptr()
        {
            return &x;
        }
        /// Pointer accessor for direct copying
        inline const float* ptr() const
        {
            return &x;
        }

        /** Assigns the value of the other vector.
        @param
        rkVector The other vector
        */
        inline Vector2& operator = ( const Vector2& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;

            return *this;
        }

        inline Vector2& operator = ( const float fScalar)
        {
            x = fScalar;
            y = fScalar;

            return *this;
        }

        inline bool operator == ( const Vector2& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y );
        }

        inline bool operator != ( const Vector2& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y  );
        }

        // arithmetic operations
        inline Vector2 operator + ( const Vector2& rkVector ) const
        {
            return Vector2(
                x + rkVector.x,
                y + rkVector.y);
        }

        inline Vector2 operator - ( const Vector2& rkVector ) const
        {
            return Vector2(
                x - rkVector.x,
                y - rkVector.y);
        }

        inline Vector2 operator * ( const float fScalar ) const
        {
            return Vector2(
                x * fScalar,
                y * fScalar);
        }

        inline Vector2 operator * ( const Vector2& rhs) const
        {
            return Vector2(
                x * rhs.x,
                y * rhs.y);
        }

        inline Vector2 operator / ( const float fScalar ) const
        {
            assert( fScalar != 0.0f );

            float fInv = 1.0f / fScalar;

            return Vector2(
                x * fInv,
                y * fInv);
        }

        inline Vector2 operator / ( const Vector2& rhs) const
        {
            return Vector2(
                x / rhs.x,
                y / rhs.y);
        }

        inline const Vector2& operator + () const
        {
            return *this;
        }

        inline Vector2 operator - () const
        {
            return Vector2(-x, -y);
        }

        // overloaded operators to help Vector2
        inline friend Vector2 operator * ( const float fScalar, const Vector2& rkVector )
        {
            return Vector2(
                fScalar * rkVector.x,
                fScalar * rkVector.y);
        }

        inline friend Vector2 operator / ( const float fScalar, const Vector2& rkVector )
        {
            return Vector2(
                fScalar / rkVector.x,
                fScalar / rkVector.y);
        }

        inline friend Vector2 operator + (const Vector2& lhs, const float rhs)
        {
            return Vector2(
                lhs.x + rhs,
                lhs.y + rhs);
        }

        inline friend Vector2 operator + (const float lhs, const Vector2& rhs)
        {
            return Vector2(
                lhs + rhs.x,
                lhs + rhs.y);
        }

        inline friend Vector2 operator - (const Vector2& lhs, const float rhs)
        {
            return Vector2(
                lhs.x - rhs,
                lhs.y - rhs);
        }

        inline friend Vector2 operator - (const float lhs, const Vector2& rhs)
        {
            return Vector2(
                lhs - rhs.x,
                lhs - rhs.y);
        }
        // arithmetic updates
        inline Vector2& operator += ( const Vector2& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;

            return *this;
        }

        inline Vector2& operator += ( const float fScaler )
        {
            x += fScaler;
            y += fScaler;

            return *this;
        }

        inline Vector2& operator -= ( const Vector2& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;

            return *this;
        }

        inline Vector2& operator -= ( const float fScaler )
        {
            x -= fScaler;
            y -= fScaler;

            return *this;
        }

        inline Vector2& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;

            return *this;
        }

        inline Vector2& operator *= ( const Vector2& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;

            return *this;
        }

        inline Vector2& operator /= ( const float fScalar )
        {
            assert( fScalar != 0.0f );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;

            return *this;
        }

        inline Vector2& operator /= ( const Vector2& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;

            return *this;
        }

        /** Returns the length (magnitude) of the vector.
        @warning
        This operation requires a square root and is expensive in
        terms of CPU operations. If you don't need to know the exact
        length (e.g. for just comparing lengths) use squaredLength()
        instead.
        */
        inline float length () const
        {
            return Math::Sqrt( x * x + y * y );
        }

        /** Returns the square of the length(magnitude) of the vector.
        @remarks
        This  method is for efficiency - calculating the actual
        length of a vector requires a square root, which is expensive
        in terms of the operations required. This method returns the
        square of the length of the vector, i.e. the same as the
        length but before the square root is taken. Use this if you
        want to find the longest / shortest vector without incurring
        the square root.
        */
        inline float squaredLength () const
        {
            return x * x + y * y;
        }

        /** Calculates the dot (scalar) product of this vector with another.
        @remarks
        The dot product can be used to calculate the angle between 2
        vectors. If both are unit vectors, the dot product is the
        cosine of the angle; otherwise the dot product must be
        divided by the product of the lengths of both vectors to get
        the cosine of the angle. This result can further be used to
        calculate the distance of a point from a plane.
        @param
        vec Vector with which to calculate the dot product (together
        with this one).
        @returns
        A float representing the dot product value.
        */
        inline float dotProduct(const Vector2& vec) const
        {
            return x * vec.x + y * vec.y;
        }

        /** Normalises the vector.
        @remarks
        This method normalises the vector such that it's
        length / magnitude is 1. The result is called a unit vector.
        @note
        This function will not crash for zero-sized vectors, but there
        will be no changes made to their components.
        @returns The previous length of the vector.
        */
        inline float normalise()
        {
            float fLength = Math::Sqrt( x * x + y * y);

            // Will also work for zero-sized vectors, but will change nothing
            if ( fLength > 1e-08 )
            {
                float fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
            }

            return fLength;
        }



        /** Returns a vector at a point half way between this and the passed
        in vector.
        */
        inline Vector2 midPoint( const Vector2& vec ) const
        {
            return Vector2(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f );
        }

        /** Returns true if the vector's scalar components are all greater
        that the ones of the vector it is compared against.
        */
        inline bool operator < ( const Vector2& rhs ) const
        {
            if( x < rhs.x && y < rhs.y )
                return true;
            return false;
        }

        /** Returns true if the vector's scalar components are all smaller
        that the ones of the vector it is compared against.
        */
        inline bool operator > ( const Vector2& rhs ) const
        {
            if( x > rhs.x && y > rhs.y )
                return true;
            return false;
        }

        /** Sets this vector's components to the minimum of its own and the
        ones of the passed in vector.
        @remarks
        'Minimum' in this case means the combination of the lowest
        value of x, y and z from both vectors. Lowest is taken just
        numerically, not magnitude, so -1 < 0.
        */
        inline void makeFloor( const Vector2& cmp )
        {
            if( cmp.x < x ) x = cmp.x;
            if( cmp.y < y ) y = cmp.y;
        }

        /** Sets this vector's components to the maximum of its own and the
        ones of the passed in vector.
        @remarks
        'Maximum' in this case means the combination of the highest
        value of x, y and z from both vectors. Highest is taken just
        numerically, not magnitude, so 1 > -3.
        */
        inline void makeCeil( const Vector2& cmp )
        {
            if( cmp.x > x ) x = cmp.x;
            if( cmp.y > y ) y = cmp.y;
        }

        /** Generates a vector perpendicular to this vector (eg an 'up' vector).
        @remarks
        This method will return a vector which is perpendicular to this
        vector. There are an infinite number of possibilities but this
        method will guarantee to generate one of them. If you need more
        control you should use the Quaternion class.
        */
        inline Vector2 perpendicular(void) const
        {
            return Vector2 (-y, x);
        }
        /** Calculates the 2 dimensional cross-product of 2 vectors, which results
        in a single floating point value which is 2 times the area of the triangle.
        */
        inline float crossProduct( const Vector2& rkVector ) const
        {
            return x * rkVector.y - y * rkVector.x;
        }

        /** Returns true if this vector is zero length. */
        inline bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y);
            return (sqlen < (1e-06 * 1e-06));

        }

        /** As normalise, except that this vector is unaffected and the
        normalised vector is returned as a copy. */
        inline Vector2 normalisedCopy(void) const
        {
            Vector2 ret = *this;
            ret.normalise();
            return ret;
        }

        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline Vector2 reflect(const Vector2& normal) const
        {
            return Vector2( *this - ( 2 * this->dotProduct(normal) * normal ) );
        }

        // special points
        //static const Vector2 ZERO;
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;
        static const Vector2 NEGATIVE_UNIT_X;
        static const Vector2 NEGATIVE_UNIT_Y;
        static const Vector2 UNIT_SCALE;
    };


    /** Standard 3-dimensional vector.
    @remarks
    A direction in 3D space represented as distances along the 3
    orthoganal axes (x, y, z). Note that positions, directions and
    scaling factors can be represented by a vector, depending on how
    you interpret the values.
    */
    class Vector3
    {
    public:
        float x, y, z;

    public:
        inline Vector3()
        {
        }

        inline Vector3( const float fX, const float fY, const float fZ )
            : x( fX ), y( fY ), z( fZ )
        {
        }

        inline explicit Vector3( const float afCoordinate[3] )
            : x( afCoordinate[0] ),
            y( afCoordinate[1] ),
            z( afCoordinate[2] )
        {
        }

        inline explicit Vector3( const int afCoordinate[3] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
            z = (float)afCoordinate[2];
        }

        inline explicit Vector3( float* const r )
            : x( r[0] ), y( r[1] ), z( r[2] )
        {
        }

        inline explicit Vector3( const float scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
        {
        }


        inline Vector3( const Vector3& rkVector )
            : x( rkVector.x ), y( rkVector.y ), z( rkVector.z )
        {
        }

        inline float operator [] ( const size_t i ) const
        {
            assert( i < 3 );

            return *(&x+i);
        }

        inline float& operator [] ( const size_t i )
        {
            assert( i < 3 );

            return *(&x+i);
        }
        /// Pointer accessor for direct copying
        inline float* ptr()
        {
            return &x;
        }
        /// Pointer accessor for direct copying
        inline const float* ptr() const
        {
            return &x;
        }

        /** Assigns the value of the other vector.
        @param
        rkVector The other vector
        */
        inline Vector3& operator = ( const Vector3& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;

            return *this;
        }

        inline Vector3& operator = ( const float fScaler )
        {
            x = fScaler;
            y = fScaler;
            z = fScaler;

            return *this;
        }

        inline bool operator == ( const Vector3& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
        }

        inline bool operator != ( const Vector3& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
        }

        // arithmetic operations
        inline Vector3 operator + ( const Vector3& rkVector ) const
        {
            return Vector3(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z);
        }

        inline Vector3 operator - ( const Vector3& rkVector ) const
        {
            return Vector3(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z);
        }

        inline Vector3 operator * ( const float fScalar ) const
        {
            return Vector3(
                x * fScalar,
                y * fScalar,
                z * fScalar);
        }

        inline Vector3 operator * ( const Vector3& rhs) const
        {
            return Vector3(
                x * rhs.x,
                y * rhs.y,
                z * rhs.z);
        }

        inline Vector3 operator / ( const float fScalar ) const
        {
            assert( fScalar != 0.0f );

            float fInv = 1.0f / fScalar;

            return Vector3(
                x * fInv,
                y * fInv,
                z * fInv);
        }

        inline Vector3 operator / ( const Vector3& rhs) const
        {
            return Vector3(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z);
        }

        inline const Vector3& operator + () const
        {
            return *this;
        }

        inline Vector3 operator - () const
        {
            return Vector3(-x, -y, -z);
        }

        // overloaded operators to help Vector3
        inline friend Vector3 operator * ( const float fScalar, const Vector3& rkVector )
        {
            return Vector3(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z);
        }

        inline friend Vector3 operator / ( const float fScalar, const Vector3& rkVector )
        {
            return Vector3(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z);
        }

        inline friend Vector3 operator + (const Vector3& lhs, const float rhs)
        {
            return Vector3(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs);
        }

        inline friend Vector3 operator + (const float lhs, const Vector3& rhs)
        {
            return Vector3(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z);
        }

        inline friend Vector3 operator - (const Vector3& lhs, const float rhs)
        {
            return Vector3(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs);
        }

        inline friend Vector3 operator - (const float lhs, const Vector3& rhs)
        {
            return Vector3(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z);
        }

        // arithmetic updates
        inline Vector3& operator += ( const Vector3& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;

            return *this;
        }

        inline Vector3& operator += ( const float fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            return *this;
        }

        inline Vector3& operator -= ( const Vector3& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;

            return *this;
        }

        inline Vector3& operator -= ( const float fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            return *this;
        }

        inline Vector3& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            return *this;
        }

        inline Vector3& operator *= ( const Vector3& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;

            return *this;
        }

        inline Vector3& operator /= ( const float fScalar )
        {
            assert( fScalar != 0.0f );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }

        inline Vector3& operator /= ( const Vector3& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;

            return *this;
        }


        /** Returns the length (magnitude) of the vector.
        @warning
        This operation requires a square root and is expensive in
        terms of CPU operations. If you don't need to know the exact
        length (e.g. for just comparing lengths) use squaredLength()
        instead.
        */
        inline float length () const
        {
            return Math::Sqrt( x * x + y * y + z * z );
        }

        /** Returns the square of the length(magnitude) of the vector.
        @remarks
        This  method is for efficiency - calculating the actual
        length of a vector requires a square root, which is expensive
        in terms of the operations required. This method returns the
        square of the length of the vector, i.e. the same as the
        length but before the square root is taken. Use this if you
        want to find the longest / shortest vector without incurring
        the square root.
        */
        inline float squaredLength () const
        {
            return x * x + y * y + z * z;
        }

        /** Returns the distance to another vector.
        @warning
        This operation requires a square root and is expensive in
        terms of CPU operations. If you don't need to know the exact
        distance (e.g. for just comparing distances) use squaredDistance()
        instead.
        */
        inline float distance(const Vector3& rhs) const
        {
            return (*this - rhs).length();
        }

        /** Returns the square of the distance to another vector.
        @remarks
        This method is for efficiency - calculating the actual
        distance to another vector requires a square root, which is
        expensive in terms of the operations required. This method
        returns the square of the distance to another vector, i.e.
        the same as the distance but before the square root is taken.
        Use this if you want to find the longest / shortest distance
        without incurring the square root.
        */
        inline float squaredDistance(const Vector3& rhs) const
        {
            return (*this - rhs).squaredLength();
        }

        /** Calculates the dot (scalar) product of this vector with another.
        @remarks
        The dot product can be used to calculate the angle between 2
        vectors. If both are unit vectors, the dot product is the
        cosine of the angle; otherwise the dot product must be
        divided by the product of the lengths of both vectors to get
        the cosine of the angle. This result can further be used to
        calculate the distance of a point from a plane.
        @param
        vec Vector with which to calculate the dot product (together
        with this one).
        @returns
        A float representing the dot product value.
        */
        inline float dotProduct(const Vector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        /** Calculates the absolute dot (scalar) product of this vector with another.
        @remarks
        This function work similar dotProduct, except it use absolute value
        of each component of the vector to computing.
        @param
        vec Vector with which to calculate the absolute dot product (together
        with this one).
        @returns
        A float representing the absolute dot product value.
        */
        inline float absDotProduct(const Vector3& vec) const
        {
            return Math::Abs(x * vec.x) + Math::Abs(y * vec.y) + Math::Abs(z * vec.z);
        }

        /** Normalises the vector.
        @remarks
        This method normalises the vector such that it's
        length / magnitude is 1. The result is called a unit vector.
        @note
        This function will not crash for zero-sized vectors, but there
        will be no changes made to their components.
        @returns The previous length of the vector.
        */
        inline float normalise()
        {
            float fLength = Math::Sqrt( x * x + y * y + z * z );

            // Will also work for zero-sized vectors, but will change nothing
            if ( fLength > 1e-08 )
            {
                float fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
                z *= fInvLength;
            }

            return fLength;
        }

        /** Calculates the cross-product of 2 vectors, i.e. the vector that
        lies perpendicular to them both.
        @remarks
        The cross-product is normally used to calculate the normal
        vector of a plane, by calculating the cross-product of 2
        non-equivalent vectors which lie on the plane (e.g. 2 edges
        of a triangle).
        @param
        vec Vector which, together with this one, will be used to
        calculate the cross-product.
        @returns
        A vector which is the result of the cross-product. This
        vector will <b>NOT</b> be normalised, to maximise efficiency
        - call Vector3::normalise on the result if you wish this to
        be done. As for which side the resultant vector will be on, the
        returned vector will be on the side from which the arc from 'this'
        to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z)
        = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
        This is because OGRE uses a right-handed coordinate system.
        @par
        For a clearer explanation, look a the left and the bottom edges
        of your monitor's screen. Assume that the first vector is the
        left edge and the second vector is the bottom edge, both of
        them starting from the lower-left corner of the screen. The
        resulting vector is going to be perpendicular to both of them
        and will go <i>inside</i> the screen, towards the cathode tube
        (assuming you're using a CRT monitor, of course).
        */
        inline Vector3 crossProduct( const Vector3& rkVector ) const
        {
            return Vector3(
                y * rkVector.z - z * rkVector.y,
                z * rkVector.x - x * rkVector.z,
                x * rkVector.y - y * rkVector.x);
        }

        /** Returns a vector at a point half way between this and the passed
        in vector.
        */
        inline Vector3 midPoint( const Vector3& vec ) const
        {
            return Vector3(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f,
                ( z + vec.z ) * 0.5f );
        }

        /** Returns true if the vector's scalar components are all greater
        that the ones of the vector it is compared against.
        */
        inline bool operator < ( const Vector3& rhs ) const
        {
            if( x < rhs.x && y < rhs.y && z < rhs.z )
                return true;
            return false;
        }

        /** Returns true if the vector's scalar components are all smaller
        that the ones of the vector it is compared against.
        */
        inline bool operator > ( const Vector3& rhs ) const
        {
            if( x > rhs.x && y > rhs.y && z > rhs.z )
                return true;
            return false;
        }

        /** Sets this vector's components to the minimum of its own and the
        ones of the passed in vector.
        @remarks
        'Minimum' in this case means the combination of the lowest
        value of x, y and z from both vectors. Lowest is taken just
        numerically, not magnitude, so -1 < 0.
        */
        inline void makeFloor( const Vector3& cmp )
        {
            if( cmp.x < x ) x = cmp.x;
            if( cmp.y < y ) y = cmp.y;
            if( cmp.z < z ) z = cmp.z;
        }

        /** Sets this vector's components to the maximum of its own and the
        ones of the passed in vector.
        @remarks
        'Maximum' in this case means the combination of the highest
        value of x, y and z from both vectors. Highest is taken just
        numerically, not magnitude, so 1 > -3.
        */
        inline void makeCeil( const Vector3& cmp )
        {
            if( cmp.x > x ) x = cmp.x;
            if( cmp.y > y ) y = cmp.y;
            if( cmp.z > z ) z = cmp.z;
        }

        /** Generates a vector perpendicular to this vector (eg an 'up' vector).
        @remarks
        This method will return a vector which is perpendicular to this
        vector. There are an infinite number of possibilities but this
        method will guarantee to generate one of them. If you need more
        control you should use the Quaternion class.
        */
        inline Vector3 perpendicular(void) const
        {
            static const float fSquareZero = 1e-06f * 1e-06f;

            Vector3 perp = this->crossProduct( Vector3::UNIT_X );

            // Check length
            if( perp.squaredLength() < fSquareZero )
            {
                /* This vector is the Y axis multiplied by a scalar, so we have
                to use another axis.
                */
                perp = this->crossProduct( Vector3::UNIT_Y );
            }

            return perp;
        }       

        

        /** Returns true if this vector is zero length. */
        inline bool isZeroLength(void) const
        {
            float sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-06 * 1e-06));

        }

        /** As normalise, except that this vector is unaffected and the
        normalised vector is returned as a copy. */
        inline Vector3 normalisedCopy(void) const
        {
            Vector3 ret = *this;
            ret.normalise();
            return ret;
        }

        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline Vector3 reflect(const Vector3& normal) const
        {
            return Vector3( *this - ( 2 * this->dotProduct(normal) * normal ) );
        }

        /** Returns whether this vector is within a positional tolerance
        of another vector.
        @param rhs The vector to compare with
        @param tolerance The amount that each element of the vector may vary by
        and still be considered equal
        */
        inline bool positionEquals(const Vector3& rhs, float tolerance = 1e-03) const
        {
            return Math::RealEqual(x, rhs.x, tolerance) &&
                Math::RealEqual(y, rhs.y, tolerance) &&
                Math::RealEqual(z, rhs.z, tolerance);

        }

        /** Returns whether this vector is within a positional tolerance
        of another vector, also take scale of the vectors into account.
        @param rhs The vector to compare with
        @param tolerance The amount (related to the scale of vectors) that distance
        of the vector may vary by and still be considered close
        */
        inline bool positionCloses(const Vector3& rhs, float tolerance = 1e-03f) const
        {
            return squaredDistance(rhs) <=
                (squaredLength() + rhs.squaredLength()) * tolerance;
        }

        // special points
        //static const Vector3 ZERO;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
        static const Vector3 NEGATIVE_UNIT_X;
        static const Vector3 NEGATIVE_UNIT_Y;
        static const Vector3 NEGATIVE_UNIT_Z;
        static const Vector3 UNIT_SCALE;
    };    

    typedef std::vector<Vector3> PointList;

    typedef std::vector<Vector2> Point2DList;

}

#endif __MathLib_H__