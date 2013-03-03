/********************************************************************
filename:   FairyAnimationSound.h
created:    2005/10/5
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    the ribbon that attach to the bone or a locator.
*********************************************************************/

#ifndef _AnimationSound_H_
#define _AnimationSound_H_

#include "Core/FairyPrerequisites.h"

#include <OgreStringInterface.h>

namespace Fairy	{

	class AnimationSound : public Ogre::StringInterface
	{
	public:
		class CmdAttachTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdSoundName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		AnimationSound(void);

		void setAttachTime(float time)
		{
			mAttachTime = time;
		}
		float getAttachTime(void) const
		{
			return mAttachTime;
		}

		void setSoundName( const Ogre::String &name )
		{
			mSoundName = name;
		}
		const Ogre::String& getSoundName(void) const
		{
			return mSoundName;
		}

        /** 拷贝所有特效参数变量
        @remarks 由于原来用Ogre::StringInterface::copyParametersTo这个函数
        来进行参数的拷贝速度太慢（因为里面有一些字符串的操作），所以
        用一个函数来复制所有的参数变量，每加一个参数，都要在这个函数
        中添加相对项
        */
        void copyParameters(AnimationSound& newInfo) const;

	public:

		/// the handle of sound, will set by play sound function.
		int mSoundHandle;
		bool mPlayed;

	protected:

		float mAttachTime;
		String mSoundName;


	//////////////////////////////////////////////////////////////////////////
		static CmdAttachTime msAttachTimeCmd;
		static CmdSoundName msSoundNameCmd;
	};
}

#endif