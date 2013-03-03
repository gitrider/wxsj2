#include "FairyBulletHelixOperator.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
#include "FairyBulletSystem.h"
#include <OgreStringConverter.h>
namespace Fairy
{
	BulletHelixOperator::CmdAmplitude		BulletHelixOperator::ms_amplitudeCmd;
	BulletHelixOperator::CmdFrequency		BulletHelixOperator::ms_frequencyCmd;

	BulletHelixOperator::BulletHelixOperator(BulletEventSystem* eventSystem) : BulletOperator()
	{
		m_type = "helix";
		m_parent = eventSystem;
		m_amplitude = 0.0;
		m_frequency = 0.0;

		if (createParamDictionary("BulletHelixOperator"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();

			dict->addParameter(Ogre::ParameterDef("amplitude", 
				"The amplitude of bullet helix operator.",
				Ogre::PT_REAL),&ms_amplitudeCmd);	

			dict->addParameter(Ogre::ParameterDef("frequency", 
				"The frequency of bullet helix operator.",
				Ogre::PT_REAL),&ms_frequencyCmd);	

		}
	}
	BulletHelixOperator::~BulletHelixOperator()
	{
	}
	void BulletHelixOperator::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletHelixOperator* newSpeedOperator =  dynamic_cast<BulletHelixOperator*>(&newOperator);
		if(newSpeedOperator)
		{
			newSpeedOperator->m_amplitude = m_amplitude;
			newSpeedOperator->m_frequency = m_frequency;
		}
    }
	void BulletHelixOperator::operateBulletEventSystem(Real timeElapsed)
	{
		BulletEventSystem::BulletSystemVecotor::iterator it =  m_parent->getActiveBulletSystem().begin();
		while(it != m_parent->getActiveBulletSystem().end())
		{
			BulletSystem* pBulletSystem = *it;
			if(pBulletSystem && pBulletSystem->getIsCreated())
			{
                // ���裺[6/2/2010 �¾���]
				//      1.���㳯�� 
				//      2.��ʱ��ӳ�䵽���ߺ���Sin ��ӳ�乫ʽ�� (2 * PI) / (1 / Frequency) = x / Age  ��
				//      3.��������������ת�����תƫ����
				//        ������������ת��ʽ��v' = (v - (v ��n) ��n) * cosx + (v * n) * sinx + (v ��n)��n 
				//        ���е� �������� * �����ˣ�v��Ҫ��ת��������n����ת�ᣬx����ת�ĽǶȣ� 
				//      4.���ݵ�Ŀ��İٷֱ���������������ӵ�λ��
				static Ogre::Vector3 startpos = pBulletSystem->getCurrentPosition();
				Ogre::Vector3 direction = pBulletSystem->getTargetPosition() - startpos;
				direction.normalise();

				Real fCumulateTime = pBulletSystem->getAge();
				Real sinvalue = Ogre::Math::Sin(fCumulateTime * Ogre::Math::TWO_PI * m_frequency);
				Real cosvalue = Ogre::Math::Cos(fCumulateTime * Ogre::Math::TWO_PI * m_frequency);
				Ogre::Vector3 absoffset, vdelta;
				vdelta = Ogre::Vector3::UNIT_Y;//�˴�����ΪY�ᣬҲ��������Ϊ����
				// v' = (v - (v ��n) ��n) * cosx + (v * n) * sinx + (v ��n)��n
				absoffset = (vdelta - (vdelta.dotProduct(direction)) * direction) * cosvalue 
					+ (vdelta.crossProduct(direction)) * sinvalue 
					+ (vdelta.dotProduct(direction)) *direction;

				Real oridistance = startpos.distance(pBulletSystem->getTargetPosition());
				Real curdistance = pBulletSystem->getCurrentPosition().distance(pBulletSystem->getTargetPosition());
				Real percent = Ogre::Math::RealEqual(oridistance, 0) ? 1 : curdistance/oridistance;

				TransformInfo info;
				info = pBulletSystem->getTransformInfo();
				Ogre::Vector3 vBulletPosition = info.mPosition;
				vBulletPosition += (m_amplitude * absoffset * percent);

				info.mPosition = vBulletPosition;
				pBulletSystem->setTransformInfo(info);
				pBulletSystem->setPosition(info.mPosition);
			}
			it ++;
		}
	}

	String BulletHelixOperator::CmdAmplitude::doGet(const void* source) const
	{
		const BulletHelixOperator* object = static_cast<const BulletHelixOperator*>(source);
		return Ogre::StringConverter::toString(object->getAmplitude());
	}
	void BulletHelixOperator::CmdAmplitude::doSet(void* target, const String& val)
	{
		BulletHelixOperator* object = static_cast<BulletHelixOperator*>(target);
		object->setAmplitude(Ogre::StringConverter::parseReal(val));
	}
	String BulletHelixOperator::CmdFrequency::doGet(const void* source) const
	{
		const BulletHelixOperator* object = static_cast<const BulletHelixOperator*>(source);
		return Ogre::StringConverter::toString(object->getFrequency());
	}
	void BulletHelixOperator::CmdFrequency::doSet(void* target, const String& val)
	{
		BulletHelixOperator* object = static_cast<BulletHelixOperator*>(target);
		object->setFrequency(Ogre::StringConverter::parseReal(val));
	}

}