#ifndef __TRIFUNC_H__
#define __TRIFUNC_H__

#include <OgreVector3.h>

// �Ƿ�ѡ��һ��������
bool IsSelTri(
				Ogre::Vector3 dir, 
				Ogre::Vector3 orig, 
				Ogre::Vector3 v1, 
				Ogre::Vector3 v2, 
				Ogre::Vector3 v3,
				float& t, 
				float& u, 
				float& v 
				);



#endif //__TRIFUNC_H__