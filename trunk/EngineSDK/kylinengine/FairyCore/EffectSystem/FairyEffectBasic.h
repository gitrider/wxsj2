/********************************************************************
filename:   FairyEffectBasic.h
created:    2005/10/5
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    the basic of effect and effect element.
*********************************************************************/

#ifndef __EffectBasic_H__
#define __EffectBasic_H__

// fairy header
#include "Core/FairyPrerequisites.h"

// ogre header
#include <OgreVector3.h>
#include <OgreColourValue.h>

namespace Ogre	{
	class SceneNode;
}

namespace Fairy	{
	class EffectManager;
	class System;
}

namespace Fairy	{

	/// the transform info of this effect, this will set the position and orienetation of 
	/// the effect every frame
	struct TransformInfo
	{
		TransformInfo( Ogre::Vector3 pos = Ogre::Vector3::ZERO,
			Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY ) :
		mPosition(pos), mRotation(rotation)
		{
		}

		Ogre::Vector3 mPosition;
		Ogre::Quaternion mRotation;
	};

	typedef std::vector<TransformInfo> TransformInfos;

	class EffectBasic
	{
	public:

		/**
			constructor
		@param system the fairy system, so we can get the info of the scene.
		*/
		EffectBasic( System *system );
		virtual ~EffectBasic();		

		void setSceneNode( Ogre::SceneNode *node )
		{
			assert (node);
			mBasicNode = node;
		}

		/**	delete the scene node of this effect */
		void deleteSceneNode(void);	

		Ogre::SceneNode *getBasicNode(void)	{	return mBasicNode;	}

		/** dump the scene node info ,for debug */
		void dumpSceneNode(Ogre::SceneNode *node, bool create = true) const;

		/** ����Ч�Ƿ��Կɼ�����������ϵͳ�������ⷢ�䣩
		*/
		virtual bool existVisualElements(void)	{	return false;	}

		/// ����λ����Ϣ����ת��Ϣ
		virtual void setTransformInfo( const TransformInfo &info ) = 0;

        /// ������Ч����ʾ������
        virtual void setVisible(bool visible);

		virtual bool getVisible(){return mVisible;}

        virtual void setColour( const Ogre::ColourValue& colour ) { }

        /// ����visible flag
        virtual void setVisibleFlag(uint32 flags) = 0;

	protected:

		/// the scene node of the effect
		Ogre::SceneNode *mBasicNode;

		System *mSystem;

        bool mVisible;
	};
}
#endif