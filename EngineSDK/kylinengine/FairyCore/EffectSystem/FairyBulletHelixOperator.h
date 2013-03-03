/********************************************************************
	created:	2010/06/02
	created:	6:2:2010   13:20
	file base:	FairyBulletHelixOperator
	file ext:	h
	author:		³Â¾üÁú
	email:		pizi0475@163.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletHelixOperator_h__
#define FairyBulletHelixOperator_h__

#include "FairyBulletOperator.h"
#include "Core/FairyPrerequisites.h"
#include "OgreVector3.h"

namespace Fairy
{
	class BulletEventSystem;
	class BulletHelixOperator : public BulletOperator
	{
	public:
		BulletHelixOperator(BulletEventSystem* eventSystem);
		virtual ~BulletHelixOperator();

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		class CmdAmplitude : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* source) const;
			void doSet(void* target, const String& val);
		};

		class CmdFrequency : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* source) const;
			void doSet(void* target, const String& val);
		};

		const Fairy::Real&			getAmplitude() const { return m_amplitude; }
		void						setAmplitude(const Fairy::Real& val) {m_amplitude = val;}

		const Fairy::Real&			getFrequency() const { return m_frequency; }
		void						setFrequency(const Fairy::Real& val) {m_frequency = val;}

		static CmdAmplitude		ms_amplitudeCmd;
		static CmdFrequency		ms_frequencyCmd;

	protected:

		/// Õñ·ù
		Real	m_amplitude;
		/// ÆµÂÊ
		Real	m_frequency;

	private:
	};
}
#endif // FairyBulletHelixOperator_h__