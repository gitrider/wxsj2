/********************************************************************
filename:   FairyAnimationRibbon.h
created:    2005/10/5
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    the ribbon that attach to the bone or a locator.
*********************************************************************/

#ifndef _FairyAnimationLine_H_
#define _FairyAnimationLine_H_

// ogre header
#include <OgreVector3.h>
#include <OgreColourValue.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreStringInterface.h>
#include <OgreStringVector.h>
#include <OgreQuaternion.h>
#include <OgreManualObject.h>

#include "FairyEffect.h"

namespace Ogre	{
	class SceneNode;
}

namespace Fairy	{

	class System;

	class AnimationLine :  public Ogre::StringInterface
	{
	
	public:

		class CmdLifeTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		class CmdColour : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};


		class CmdMaterial : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		class CmdWidth : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		class CmdAttachTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		class CmdAttachPoint : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		class CmdOffsetPos : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		class CmdOffsetRotation : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
		};

		/////////////////////////////////////////////////////////////
		AnimationLine( System *system);

		~AnimationLine();

		bool createRenderInstance(void);
		void destroyRenderInstance(void);
		void updateRenderInstance( float time, TransformInfo &transform );


		void setWidth( float width )
		{
			mWidth = width;
		}

		float getWidth(void) const
		{
			return mWidth;
		}

		void setColour( const Ogre::ColourValue &value )
		{
			mColour = value;
		}

		const Ogre::ColourValue & getColour(void) const
		{
			return mColour;
		}

		const Ogre::String& getMaterialName(void) const
		{
			return mMaterialName;
		}	

		float getLifeTime(void) const
		{
			return mLifeTime;
		}

		void setLifeTime(float time)
		{
			mLifeTime = time;
		}

		void setAttachTime(float time)
		{
			mAttachTime = time;
		}
		float getAttachTime(void) const
		{
			return mAttachTime;
		}

		void setAttachPoint(const Ogre::String &point)
		{
			mAttachPoint = point;
		}
		const Ogre::String& getAttachPoint(void) const
		{
			return mAttachPoint;
		}

		void setOffsetPos(const Ogre::Vector3 &pos)
		{
			mOffsetPos = pos;
		}
		const Ogre::Vector3& getOffsetPos(void) const
		{
			return mOffsetPos;
		}

		void setOffsetRotation(const Ogre::Quaternion &rotation)
		{
			mOffsetRotation = rotation;
		}
		const Ogre::Quaternion& getOffsetRotation(void) const
		{
			return mOffsetRotation;
		}


		void setInterpolationTimes(int times);

		void setMaterialName(const Ogre::String &matName);
		Ogre::String getMaterial(void) const
		{
			return mMaterialName;
		}

        /** 拷贝所有特效参数变量
        @remarks 由于原来用Ogre::StringInterface::copyParametersTo这个函数
        来进行参数的拷贝速度太慢（因为里面有一些字符串的操作），所以
        用一个函数来复制所有的参数变量，每加一个参数，都要在这个函数
        中添加相对项
        */
        void copyParameters(AnimationLine& newInfo) const;

	protected:

		void _createBuffer( size_t vertexCount );
		void _destroyBuffer(void);

	protected:

		Ogre::ManualObject* mManualObject;

		/// 所有billboard的颜色
		Ogre::ColourValue mColour;

		Ogre::String mMaterialName;

		float mLifeTime;
		// colour

		// alpha value
		float mStartAlphaValue;
		float mEndAlphaValue;

		float mWidth;

		Ogre::SceneNode *mBasicNode;

		bool mCreateInstance;

		float mCurrentLifeTime;

		System *mSystem;

		float mAttachTime;
		String mAttachPoint;

		Ogre::Vector3 mOffsetPos;
		Ogre::Quaternion mOffsetRotation;

		bool mBuffersCreated;

		Ogre::Vector3 mTargetPoint;
		Ogre::Vector3 mPosition;

		////////////////////////////////////////////////////////////////

		static CmdColour msColourCmd;
		static CmdMaterial msMaterialCmd;
		static CmdWidth msWidthCmd;
		static CmdLifeTime msLifeTimeCmd;
		static CmdAttachTime msAttachTimeCmd;
		static CmdAttachPoint msAttachPointCmd;
		static CmdOffsetPos msOffsetPosCmd;
		static CmdOffsetRotation msOffsetRotationCmd;
	};
}

#endif