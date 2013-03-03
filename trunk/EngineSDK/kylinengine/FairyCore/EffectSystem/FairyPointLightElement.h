/********************************************************************
filename:   FairyPointLightElement.h

purpose:    点光源特效元素，对Ogre的Light进行封装，可以使发出的特效对
			周围的物体进行影响，如发出的火球可以照亮地面
*********************************************************************/

#ifndef _POINTLIGHTELEMENT_H_
#define _POINTLIGHTELEMENT_H_

// fairy header
#include "FairyEffectElement.h"
#include "Core/FairyPrerequisites.h"

#include <OgreColourValue.h>

namespace Ogre	{
	class Light;
}

namespace Fairy	{

	class PointLightElement : public EffectElement
	{
	public:

        class CmdDiffuse : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };

        class CmdSpecular : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdRange : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdAttenuationConstant : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdAttenuationLinear : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdAttenuationQuadric : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

		/////////////////////////////////////////////////////////////
		PointLightElement(const Ogre::String &type, System *system);
		~PointLightElement();

		virtual bool createRenderInstance(void);
		virtual void destroyRenderInstance(void);

        void setDiffuse(const Ogre::ColourValue& diffuse);
        Ogre::ColourValue getDiffuse(void) const
        {
            return mDiffuse;
        }

        void setSpecular(const Ogre::ColourValue& specular);
        Ogre::ColourValue getSpecular(void) const
        {
            return mSpecular;
        }

        void setRange(Ogre::Real range);
        Ogre::Real getRange(void) const
        {
            return mRange;
        }

        void setAttenuationConstant(Ogre::Real constant);
        Ogre::Real getAttenuationConstant(void) const
        {
            return mAttenuationConstant;
        }

        void setAttenuationLinear(Ogre::Real linear);
        Ogre::Real getAttenuationLinear(void) const
        {
            return mAttenuationLinear;
        }

        void setAttenuationQuadric(Ogre::Real quadric);
        Ogre::Real getAttenuationQuadric(void) const
        {
            return mAttenuationQuadric;
        }

        /// 设置visible flag
        virtual void setVisibleFlag(uint32 flags);
//laim 版本差异代码 
/*
		void update( Real time, TransformInfo &info );
		void OnParamChanged();
*/
	protected:

		bool _createLight(void);
		void _destroyLight(void);

	protected:

		Ogre::Light* mLight;

        Ogre::ColourValue mDiffuse;
        Ogre::ColourValue mSpecular;

        Ogre::Real mRange;
        Ogre::Real mAttenuationConstant;
        Ogre::Real mAttenuationLinear;
        Ogre::Real mAttenuationQuadric;

		////////////////////////////////////////////////////////////////
        static CmdDiffuse  msDiffuseCmd;
        static CmdSpecular msSpecularCmd;
        static CmdRange msRangeCmd;
        static CmdAttenuationConstant msAttenuationConstantCmd;
        static CmdAttenuationLinear msAttenuationLinearCmd;
        static CmdAttenuationQuadric msAttenuationQuadricCmd;
       //LAIM
/*  版本差异代码
		public:
			Ogre::ColourValue __getColourValueByTime( float nTime );

			Ogre::ColourValue			mColourStart;
			Ogre::ColourValue			mColourEnd;
			Ogre::ColourValue			mColourDelta1;
			Ogre::ColourValue			mColourDelta2;
			float						mTimePass;
			float						mTimeChange;
*/

	};
}

#endif