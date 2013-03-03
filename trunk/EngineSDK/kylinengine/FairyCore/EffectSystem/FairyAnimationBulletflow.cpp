#include "FairyAnimationBulletflow.h"
#include "FairyEffectManager.h"
#include "Core/FairyPropertyInterface.h"
#include <OgreStringConverter.h>
namespace Fairy	{

	AnimationBulletFlow::CmdAttachTime				AnimationBulletFlow::ms_attachTimeCmd;
	AnimationBulletFlow::CmdAttachPoint				AnimationBulletFlow::ms_attachPointCmd;
	AnimationBulletFlow::CmdBulletFlowTemplateName	AnimationBulletFlow::ms_bulletFlowTemplateNameCmd;
	AnimationBulletFlow::CmdOffsetPos				AnimationBulletFlow::ms_offsetPosCmd;
	AnimationBulletFlow::CmdOffsetRotation			AnimationBulletFlow::ms_offsetRotationCmd;
	AnimationBulletFlow::CmdAttach					AnimationBulletFlow::ms_attachCmd;

	//////////////////////////////////////////////////////////////////////////
	AnimationBulletFlow::AnimationBulletFlow() :
	m_attachTime(0.0f), m_attachPoint(""), m_bulletFlowTemplateName(""),
		m_offsetPos(Ogre::Vector3::ZERO), m_offsetRotation(Ogre::Quaternion::IDENTITY), m_isAttach(false)
	{
		initParamDictionary();
	}
	//---------------------------------------------------------------------
	AnimationBulletFlow::~AnimationBulletFlow()
	{
	
	}
	//---------------------------------------------------------------------
	bool AnimationBulletFlow::initParamDictionary(void)
	{
		if (createParamDictionary("AnimationBulletFlow"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();			

			dict->addParameter(Ogre::ParameterDef("AttachTime", 
				"the time that showing the effect.",
				Ogre::PT_REAL),&ms_attachTimeCmd);	  

			dict->addParameter(Ogre::ParameterDef("AttachPoint", 
				"the attached bone or locator.",
				Ogre::PT_STRING),&ms_attachPointCmd);	  

			dict->addParameter(Ogre::ParameterDef("BulletFlowTemplateName", 
				"name of bullet flow template.",
				Ogre::PT_STRING),&ms_bulletFlowTemplateNameCmd);	  

			dict->addParameter(Ogre::ParameterDef("OffsetPos", 
				"offset position to the attach point.",
				Ogre::PT_VECTOR3),&ms_offsetPosCmd);	  

			dict->addParameter(Ogre::ParameterDef("OffsetRotation", 
				"offset orientation to the attach point.",
				Ogre::PT_QUATERNION),&ms_offsetRotationCmd);	  

			dict->addParameter(Ogre::ParameterDef("Attach", 
				"whether the effect will move along with the attach point.",
				Ogre::PT_BOOL),&ms_attachCmd);	  

			return true;
		}

		return false;
	}
	//---------------------------------------------------------------------
	void AnimationBulletFlow::copyParameters(AnimationBulletFlow& newBulletFlow) const
	{
		newBulletFlow.m_attachTime = m_attachTime;
		newBulletFlow.m_attachPoint = m_attachPoint;
		newBulletFlow.m_bulletFlowTemplateName = m_bulletFlowTemplateName;
		newBulletFlow.m_offsetPos = m_offsetPos;
		newBulletFlow.m_offsetRotation = m_offsetRotation;
		newBulletFlow.m_isAttach = m_isAttach;
	}
	//////////////////////////////////////////////////////////////////////////
	String AnimationBulletFlow::CmdAttachTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationBulletFlow *>(target)->getAttachTime() );
	}
	void AnimationBulletFlow::CmdAttachTime::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationBulletFlow *>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
	void AnimationBulletFlow::CmdAttachTime::doCopy(void* target, const void* source)
	{
		static_cast<AnimationBulletFlow *>(target)->setAttachTime(
			static_cast<const AnimationBulletFlow *>(source)->getAttachTime() );
	}
#endif

	String AnimationBulletFlow::CmdAttachPoint::doGet(const void* target) const
	{
		return static_cast<const AnimationBulletFlow *>(target)->getAttachPoint();
	}
	void AnimationBulletFlow::CmdAttachPoint::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationBulletFlow *>(target)->setAttachPoint(val);
	}
#if OGRE_STRING_INTERFACE_COPY
	void AnimationBulletFlow::CmdAttachPoint::doCopy(void* target, const void* source)
	{
		static_cast<AnimationBulletFlow *>(target)->setAttachPoint(
			static_cast<const AnimationBulletFlow *>(source)->getAttachPoint() );
	}
#endif

	String AnimationBulletFlow::CmdBulletFlowTemplateName::doGet(const void* target) const
	{
		return static_cast<const AnimationBulletFlow *>(target)->getBulletFlowTemplateName();
	}
	void AnimationBulletFlow::CmdBulletFlowTemplateName::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationBulletFlow *>(target)->setBulletFlowTemplateName(val);
	}
#if OGRE_STRING_INTERFACE_COPY
	void AnimationBulletFlow::CmdBulletFlowTemplateName::doCopy(void* target, const void* source)
	{
		static_cast<AnimationBulletFlow *>(target)->setBulletFlowTemplateName(
			static_cast<const AnimationBulletFlow *>(source)->getBulletFlowTemplateName() );
	}
#endif

	String AnimationBulletFlow::CmdOffsetPos::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationBulletFlow *>(target)->getOffsetPos() );
	}
	void AnimationBulletFlow::CmdOffsetPos::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationBulletFlow *>(target)->setOffsetPos(Ogre::StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
	void AnimationBulletFlow::CmdOffsetPos::doCopy(void* target, const void* source)
	{
		static_cast<AnimationBulletFlow *>(target)->setOffsetPos(
			static_cast<const AnimationBulletFlow *>(source)->getOffsetPos() );
	}
#endif

	String AnimationBulletFlow::CmdOffsetRotation::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationBulletFlow *>(target)->getOffsetRotation() );
	}
	void AnimationBulletFlow::CmdOffsetRotation::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationBulletFlow *>(target)->setOffsetRotation(Ogre::StringConverter::parseQuaternion(val));
	}
#if OGRE_STRING_INTERFACE_COPY
	void AnimationBulletFlow::CmdOffsetRotation::doCopy(void* target, const void* source)
	{
		static_cast<AnimationBulletFlow *>(target)->setOffsetRotation(
			static_cast<const AnimationBulletFlow *>(source)->getOffsetRotation() );
	}
#endif

	String AnimationBulletFlow::CmdAttach::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationBulletFlow *>(target)->getAttach() );
	}
	void AnimationBulletFlow::CmdAttach::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationBulletFlow *>(target)->setAttach(Ogre::StringConverter::parseBool(val));
	}
#if OGRE_STRING_INTERFACE_COPY
	void AnimationBulletFlow::CmdAttach::doCopy(void* target, const void* source)
	{
		static_cast<AnimationBulletFlow *>(target)->setAttach(
			static_cast<const AnimationBulletFlow *>(source)->getAttach() );
	}
#endif
}