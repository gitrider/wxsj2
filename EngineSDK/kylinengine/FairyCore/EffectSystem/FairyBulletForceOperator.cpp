#include "FairyBulletForceOperator.h"
#include "FairyBulletSystem.h"
#include <OgreStringConverter.h>
namespace Fairy
{

	BulletForceOperator::CmdForceVector			BulletForceOperator::ms_forceVectorCmd;

	BulletForceOperator::BulletForceOperator(BulletEventSystem* eventSystem) : BulletOperator()
	{
		m_type = "force";
		m_parent = eventSystem;

		m_isOperated = false;
		m_forceVector = Ogre::Vector3::ZERO;

		if (createParamDictionary("BulletForceOperator"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();
			dict->addParameter(Ogre::ParameterDef("forceVector", 
				"The mode of bullet speed operator.",
				Ogre::PT_VECTOR3),&ms_forceVectorCmd);	  
		}
	}
	BulletForceOperator::~BulletForceOperator()
	{
	}

	void BulletForceOperator::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletForceOperator* newForceOperator =  dynamic_cast<BulletForceOperator*>(&newOperator);
		if(newForceOperator)
		{
			newForceOperator->m_forceVector = m_forceVector;
		}
    }

	String BulletForceOperator::CmdForceVector::doGet(const void* source) const
	{
		const BulletForceOperator* object = static_cast<const BulletForceOperator*>(source);
		return Ogre::StringConverter::toString(object->getForceVector());
	}

	void BulletForceOperator::CmdForceVector::doSet(void* target, const String& val)
	{
		BulletForceOperator* object = static_cast<BulletForceOperator*>(target);
		object->setForceVector(Ogre::StringConverter::parseVector3(val));
	}

	void BulletForceOperator::operateBulletEventSystem(Real timeElapsed)
	{
		BulletEventSystem::BulletSystemVecotor::iterator it =  m_parent->getActiveBulletSystem().begin();
		while(it != m_parent->getActiveBulletSystem().end())
		{
			BulletSystem* pBulletSystem = *it;
			if(pBulletSystem && pBulletSystem->getIsCreated() && !pBulletSystem->findBulletOperator(this))
			{

				Real fCumulateTime = pBulletSystem->getAge();

				TransformInfo info;
				info = pBulletSystem->getTransformInfo();

				Ogre::Vector3 vBulletPosition = info.mPosition;
				vBulletPosition += m_forceVector;

				info.mPosition = vBulletPosition;
				pBulletSystem->setTransformInfo(info);
				pBulletSystem->setPosition(info.mPosition);
				pBulletSystem->addBulletOperator(this);

			}
			it ++;
		}
	}



}