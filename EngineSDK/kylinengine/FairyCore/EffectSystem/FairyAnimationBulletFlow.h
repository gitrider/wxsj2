
/********************************************************************
	created:	2008/03/03
	created:	3:3:2008   17:05
	file base:	FairyAnimationBulletFlow
	file ext:	h
	author:		姜威
	
	purpose:	为原有Skill系统加入释放BulletFlow功能
*********************************************************************/
#ifndef FairyAnimationBulletFlow_h__
#define FairyAnimationBulletFlow_h__

#include <OgreStringVector.h>
#include <OgreStringInterface.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>

#include "Core/FairyPrerequisites.h"

namespace Fairy	{

	class AnimationBulletFlow : public Ogre::StringInterface
	{
	public:

		class CmdAttachTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
			virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdAttachPoint : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
			virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdBulletFlowTemplateName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
			virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdOffsetPos : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
			virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdOffsetRotation : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
			virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdAttach : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
			virtual void doCopy(void* target, const void* source);
#endif
		};
		
		//////////////////////////////////////////////////////////////////////////

		AnimationBulletFlow();
		~AnimationBulletFlow();

		bool initParamDictionary(void);

		void setAttachTime(float time)
		{
			m_attachTime = time;
		}
		float getAttachTime(void) const
		{
			return m_attachTime;
		}

		void setAttachPoint(const String &point)
		{
			m_attachPoint = point;
		}
		const String& getAttachPoint(void) const
		{
			return m_attachPoint;
		}

		void setBulletFlowTemplateName(const String &name)
		{
			m_bulletFlowTemplateName = name;
		}
		const String& getBulletFlowTemplateName(void) const
		{
			return m_bulletFlowTemplateName;
		}

		void setOffsetPos(const Ogre::Vector3 &pos)
		{
			m_offsetPos = pos;
		}
		const Ogre::Vector3& getOffsetPos(void) const
		{
			return m_offsetPos;
		}

		void setOffsetRotation(const Ogre::Quaternion &rotation)
		{
			m_offsetRotation = rotation;
		}
		const Ogre::Quaternion& getOffsetRotation(void) const
		{
			return m_offsetRotation;
		}

		const bool& getAttach() const
		{
			return m_isAttach;
		}
		void setAttach(const bool& val)
		{
			m_isAttach = val;
		}

		/** 拷贝所有特效参数变量
		@remarks 由于原来用Ogre::StringInterface::copyParametersTo这个函数
		来进行参数的拷贝速度太慢（因为里面有一些字符串的操作），所以
		用一个函数来复制所有的参数变量，每加一个参数，都要在这个函数
		中添加相对项
		*/
		void copyParameters(AnimationBulletFlow& newInfo) const;

	protected:

		float					m_attachTime;
		String					m_attachPoint;
		String					m_bulletFlowTemplateName;
		Ogre::Vector3			m_offsetPos;
		Ogre::Quaternion		m_offsetRotation;
		bool					m_isAttach;

		//////////////////////////////////////////////////////////////////////////
		static CmdAttachTime				ms_attachTimeCmd;
		static CmdAttachPoint				ms_attachPointCmd;
		static CmdBulletFlowTemplateName	ms_bulletFlowTemplateNameCmd;
		static CmdOffsetPos					ms_offsetPosCmd;
		static CmdOffsetRotation			ms_offsetRotationCmd;
		static CmdAttach					ms_attachCmd;
	};
}
#endif // FairyAnimationBulletFlow_h__