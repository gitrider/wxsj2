/********************************************************************
	created:	2008/03/17
	created:	17:3:2008   16:53
	file base:	FairyBulletCameraShakeOperator
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletCameraShakeOperator_h__
#define FairyBulletCameraShakeOperator_h__

#include "FairyBulletOperator.h"
#include "Core/FairyPrerequisites.h"
#include "OgreVector3.h"

namespace Fairy
{
	class BulletEventSystem;
	class BulletCameraShakeOperator : public BulletOperator
	{
	public:
		BulletCameraShakeOperator(BulletEventSystem* eventSystem);
		virtual ~BulletCameraShakeOperator();

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		class CmdAmplitude : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		class CmdDuration : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		class CmdFrequencyName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		Fairy::Real getAmplitude() const { return m_amplitude; }
		void setAmplitude(const Fairy::Real val) { m_amplitude = val; }

		Fairy::Real getDuration() const { return m_duration; }
		void setDuration(const Fairy::Real val) { m_duration = val; }

		void setFrequency( const Fairy::Real val ){ m_frequency = val; }
		Fairy::Real getFrequency(void) const { return m_frequency; }

		static CmdAmplitude ms_amplitudeCmd;
		static CmdDuration	ms_durationCmd;
		static CmdFrequencyName msFrequencyCmd;
	protected:
		Real	m_amplitude;
		Real	m_duration;
		Real	m_frequency;
		Real	m_age;
		bool	m_isStart;
	private:
	};
}

#endif // FairyBulletCameraShakeOperator_h__