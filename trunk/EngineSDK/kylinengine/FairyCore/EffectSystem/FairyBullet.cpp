
#include "FairyBullet.h"
#include <OgreSceneManager.h>
namespace Fairy
{
	Bullet::Bullet():m_velocityWorld(Ogre::Vector3::ZERO),m_accelerationWorld(Ogre::Vector3::ZERO),
		m_sceneNode(NULL),m_age(0.0f),m_clusterIndex(0)
	{

	}
	Bullet::~Bullet()
	{
		if(m_sceneNode)
		{
			m_sceneNode->getCreator()->destroySceneNode(m_sceneNode);
			m_sceneNode=NULL;
		}
	}
	const Ogre::Vector3& Bullet::getPosition() const
	{
		if(m_sceneNode)
		   return m_sceneNode->getPosition();
		return Ogre::Vector3::ZERO;
	}
	void Bullet::setPosition(const Ogre::Vector3& val)
	{
		if(m_sceneNode)
			m_sceneNode->setPosition(val);
	}

	const Ogre::Vector3& Bullet::getVelocityWorld() const
	{
		return m_velocityWorld;
	}
	void Bullet::setVelocityWorld(const Ogre::Vector3& val)
	{
		m_velocityWorld = val;
	}
	const Ogre::Vector3& Bullet::getAccelerationWorld() const
	{
		return m_accelerationWorld;
	}
	void Bullet::setAccelerationWorld(const Ogre::Vector3& val)
	{
		m_accelerationWorld = val;
	}

	const Real& Bullet::getAge() const
	{
		return m_age;
	}
	void Bullet::setAge(const Real& val)
	{
		m_age = val;
	}

	unsigned int Bullet::getClusterIndex() const
	{
		return m_clusterIndex;
	}
	void Bullet::setClusterIndex(unsigned int val)
	{
		m_clusterIndex = val;
	}
}
