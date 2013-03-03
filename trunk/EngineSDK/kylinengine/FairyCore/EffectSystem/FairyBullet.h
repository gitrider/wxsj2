/********************************************************************
	created:	2008/03/03
	created:	3:3:2008   17:21
	file base:	FairyBullet
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/

#ifndef FairyBullet_h__
#define FairyBullet_h__

#include "OgreVector3.h"
#include "OgreSceneNode.h"
#include "Core/FairyPrerequisites.h"

namespace Fairy
{
	class Bullet
	{
	public:
		Bullet();
		virtual ~Bullet();

		const Ogre::Vector3& getPosition() const;
		void setPosition(const Ogre::Vector3& val);

		const Ogre::Vector3& getVelocityWorld() const;
		void setVelocityWorld(const Ogre::Vector3& val);

		const Ogre::Vector3& getAccelerationWorld() const;
		void setAccelerationWorld(const Ogre::Vector3& val);

		const Real& getAge() const;
		void setAge(const Real& val);

		unsigned int getClusterIndex() const;
		void setClusterIndex(unsigned int val);

	protected:
		Ogre::Vector3		m_velocityWorld;
		Ogre::Vector3		m_accelerationWorld;
		Ogre::SceneNode*	m_sceneNode;
		Real				m_age;
		unsigned int		m_clusterIndex;
		
	private:
	};
}
#endif // FairyBullet_h__