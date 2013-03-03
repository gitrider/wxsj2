#include "FairyBulletSwayOperator.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
#include "FairyBulletSystem.h"
#include <OgreStringConverter.h>
namespace Fairy
{
	BulletSwayOperator::CmdAmplitude		BulletSwayOperator::ms_amplitudeCmd;
	BulletSwayOperator::CmdFrequency		BulletSwayOperator::ms_frequencyCmd;

	BulletSwayOperator::BulletSwayOperator(BulletEventSystem* eventSystem) : BulletOperator()
	{
		m_type = "sway";
		m_parent = eventSystem;

		m_amplitude = 0.0;
		m_frequency = 0.0;

		if (createParamDictionary("BulletSwayOperator"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();

			dict->addParameter(Ogre::ParameterDef("amplitude", 
				"The amplitude of bullet sway operator.",
				Ogre::PT_REAL),&ms_amplitudeCmd);	

			dict->addParameter(Ogre::ParameterDef("frequency", 
				"The frequency of bullet sway operator.",
				Ogre::PT_REAL),&ms_frequencyCmd);	

		}
	}
	BulletSwayOperator::~BulletSwayOperator()
	{
	}
	void BulletSwayOperator::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletSwayOperator* newSpeedOperator =  dynamic_cast<BulletSwayOperator*>(&newOperator);
		if(newSpeedOperator)
		{
			newSpeedOperator->m_amplitude = m_amplitude;
			newSpeedOperator->m_frequency = m_frequency;
		}
    }
	void BulletSwayOperator::operateBulletEventSystem(Real timeElapsed)
	{
		BulletEventSystem::BulletSystemVecotor::iterator it =  m_parent->getActiveBulletSystem().begin();
		while(it != m_parent->getActiveBulletSystem().end())
		{
			BulletSystem* pBulletSystem = *it;
			if(pBulletSystem && pBulletSystem->getIsCreated())
			{
                // 步骤：[6/2/2010 陈军龙]
				//      1.计算朝向 
				//      2.计算摇摆方向（与朝向垂直，且与地面平行） 
				//      3.将时间映射到曲线函数Sin （映射公式： (2 * PI) / (1 / Frequency) = x / Age  ）
				//      4.更新子弹位置
                static Ogre::Vector3 startpos = pBulletSystem->getCurrentPosition();
				Ogre::Vector3 direction = pBulletSystem->getTargetPosition() - startpos;
				direction.normalise();

				Real fCumulateTime = pBulletSystem->getAge();
				Ogre::Vector2 swaydir(direction.z, -direction.x);
				Real offset = m_amplitude * Ogre::Math::Sin(fCumulateTime * Ogre::Math::TWO_PI * m_frequency);
				Ogre::Vector2 absoffset = swaydir * offset;

				TransformInfo info;
				info = pBulletSystem->getTransformInfo();
				Ogre::Vector3 vBulletPosition = info.mPosition;
				vBulletPosition.x += absoffset.x;
				vBulletPosition.z += absoffset.y;

				info.mPosition = vBulletPosition;
				pBulletSystem->setTransformInfo(info);
				pBulletSystem->setPosition(info.mPosition);
			}
			it ++;
		}
	}

	String BulletSwayOperator::CmdAmplitude::doGet(const void* source) const
	{
		const BulletSwayOperator* object = static_cast<const BulletSwayOperator*>(source);
		return Ogre::StringConverter::toString(object->getAmplitude());
	}
	void BulletSwayOperator::CmdAmplitude::doSet(void* target, const String& val)
	{
		BulletSwayOperator* object = static_cast<BulletSwayOperator*>(target);
		object->setAmplitude(Ogre::StringConverter::parseReal(val));
	}
	String BulletSwayOperator::CmdFrequency::doGet(const void* source) const
	{
		const BulletSwayOperator* object = static_cast<const BulletSwayOperator*>(source);
		return Ogre::StringConverter::toString(object->getFrequency());
	}
	void BulletSwayOperator::CmdFrequency::doSet(void* target, const String& val)
	{
		BulletSwayOperator* object = static_cast<BulletSwayOperator*>(target);
		object->setFrequency(Ogre::StringConverter::parseReal(val));
	}

}