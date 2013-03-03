#include "MathLib.h"

namespace PathLib
{
    const float Math::POS_INFINITY = std::numeric_limits<float>::infinity();
    const float Math::NEG_INFINITY = -std::numeric_limits<float>::infinity();
    const float Math::PI = float( 4.0 * atan( 1.0 ) );
    const float Math::TWO_PI = float( 2.0 * PI );
    const float Math::HALF_PI = float( 0.5 * PI );
    const float Math::fDeg2Rad = PI / float(180.0);
    const float Math::fRad2Deg = float(180.0) / PI;
    //-----------------------------------------------------------------------
    int Math::ISign (int iValue)
    {
        return ( iValue > 0 ? +1 : ( iValue < 0 ? -1 : 0 ) );
    }
    
    //-----------------------------------------------------------------------
    float Math::Sign (float fValue)
    {
        if ( fValue > 0.0 )
            return 1.0;

        if ( fValue < 0.0 )
            return -1.0;

        return 0.0;
    }
    float Math::squareDistance(const Vector2& pos1, const Vector2& pos2)
    {
        return (pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y); 
    }
    float Math::distance(const Vector2& pos1, const Vector2& pos2)
    {
        return Sqrt( squareDistance(pos1, pos2) );
    }
    //-----------------------------------------------------------------------
    bool Math::pointInTri2D(const Vector2& p, const Vector2& a, 
        const Vector2& b, const Vector2& c)
    {
        // Winding must be consistent from all edges for point to be inside
        Vector2 v1, v2;
        float dot[3];
        bool zeroDot[3];

        v1 = b - a;
        v2 = p - a;

        // Note we don't care about normalisation here since sign is all we need
        // It means we don't have to worry about magnitude of cross products either
        dot[0] = v1.crossProduct(v2);
        zeroDot[0] = Math::RealEqual(dot[0], 0.0f, 1e-3f);


        v1 = c - b;
        v2 = p - b;

        dot[1] = v1.crossProduct(v2);
        zeroDot[1] = Math::RealEqual(dot[1], 0.0f, 1e-3f);

        // Compare signs (ignore colinear / coincident points)
        if(!zeroDot[0] && !zeroDot[1] 
        && Math::Sign(dot[0]) != Math::Sign(dot[1]))
        {
            return false;
        }

        v1 = a - c;
        v2 = p - c;

        dot[2] = v1.crossProduct(v2);
        zeroDot[2] = Math::RealEqual(dot[2], 0.0f, 1e-3f);
        // Compare signs (ignore colinear / coincident points)
        if((!zeroDot[0] && !zeroDot[2] 
        && Math::Sign(dot[0]) != Math::Sign(dot[2])) ||
            (!zeroDot[1] && !zeroDot[2] 
            && Math::Sign(dot[1]) != Math::Sign(dot[2])))
            {
                return false;
            }


            return true;
    }    
    //-----------------------------------------------------------------------
    bool Math::RealEqual( float a, float b, float tolerance )
    {
        if (fabs(b-a) <= tolerance)
            return true;
        else
            return false;
    }

    //const Vector2 Vector2::ZERO( 0, 0);

    const Vector2 Vector2::UNIT_X( 1, 0);
    const Vector2 Vector2::UNIT_Y( 0, 1);
    const Vector2 Vector2::NEGATIVE_UNIT_X( -1,  0);
    const Vector2 Vector2::NEGATIVE_UNIT_Y(  0, -1);
    const Vector2 Vector2::UNIT_SCALE(1, 1);

    //const Vector3 Vector3::ZERO( 0, 0, 0 );

    const Vector3 Vector3::UNIT_X( 1, 0, 0 );
    const Vector3 Vector3::UNIT_Y( 0, 1, 0 );
    const Vector3 Vector3::UNIT_Z( 0, 0, 1 );
    const Vector3 Vector3::NEGATIVE_UNIT_X( -1,  0,  0 );
    const Vector3 Vector3::NEGATIVE_UNIT_Y(  0, -1,  0 );
    const Vector3 Vector3::NEGATIVE_UNIT_Z(  0,  0, -1 );
    const Vector3 Vector3::UNIT_SCALE(1, 1, 1);

    
}