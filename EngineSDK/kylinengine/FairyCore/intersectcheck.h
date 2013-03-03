
#ifndef __FAIRY_INTERSECT_CHECK_H__
#define __FAIRY_INTERSECT_CHECK_H__

namespace Ogre
{
	class MovableObject;
}

namespace Fairy
{
	bool KYLIN_IntersectRayWithEntity( const Ogre::Vector3 &vStart, const Ogre::Vector3 &vEnd, System *pFRSystem, 
							  Ogre::Vector3 &pos, Ogre::Vector3 &normal );

	bool KYLIN_GetIntersectEntities( const Ogre::Vector3 &vStart, const Ogre::Vector3 &vEnd, System *pFRSystem , std::vector< Ogre::MovableObject *> &objArray );
}

#endif


