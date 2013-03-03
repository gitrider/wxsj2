/********************************************************************
filename:   FairyAnimationRibbon.cpp

purpose:    the ribbon that attach to the bone or a locator.
*********************************************************************/

#include "FairyAnimationLine.h"
#include "FairyEffectManager.h"

#include "Core/FairySystem.h"

#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include <OgreRenderSystem.h>
#include <OgreRoot.h>
#include <OgreHardwareBufferManager.h>

namespace Fairy	{

	AnimationLine::CmdAttachTime	AnimationLine::msAttachTimeCmd;
	AnimationLine::CmdAttachPoint		AnimationLine::msAttachPointCmd;
	AnimationLine::CmdOffsetPos	 AnimationLine::msOffsetPosCmd;
	AnimationLine::CmdOffsetRotation	AnimationLine::msOffsetRotationCmd;


	AnimationLine::CmdColour AnimationLine::msColourCmd;
	AnimationLine::CmdMaterial AnimationLine::msMaterialCmd;
	AnimationLine::CmdWidth AnimationLine::msWidthCmd;
	AnimationLine::CmdLifeTime AnimationLine::msLifeTimeCmd;

	//////////////////////////////////////////////////////////////////////////

	AnimationLine::AnimationLine( System *system) :
        mLifeTime(0.0f), 
        mWidth(40.0f),
		mColour(Ogre::ColourValue::White), 
		mStartAlphaValue(1.0f), 
        mEndAlphaValue(1.0f), 
        mCreateInstance(false), 
        mCurrentLifeTime(0.0f),
		mAttachTime(0.0f), 
        mAttachPoint(""), 
        mBasicNode(NULL),
		mManualObject(NULL),
        mSystem(system),
		mOffsetPos(Ogre::Vector3::ZERO), 
        mOffsetRotation(Ogre::Quaternion::IDENTITY),
        mBuffersCreated(false),
		mTargetPoint(Ogre::Vector3::ZERO)
	{

		if (createParamDictionary("AnimationLine"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();

			dict->addParameter(Ogre::ParameterDef("AttachTime", 
				"the time that showing the ribbon.",
				Ogre::PT_REAL),&msAttachTimeCmd);	  

			dict->addParameter(Ogre::ParameterDef("AttachPoint", 
				"the attached bone or locator.",
				Ogre::PT_STRING),&msAttachPointCmd);	  

			dict->addParameter(Ogre::ParameterDef("OffsetPos", 
				"offset position to the attach point.",
				Ogre::PT_VECTOR3),&msOffsetPosCmd);	  

			dict->addParameter(Ogre::ParameterDef("OffsetRotation", 
				"offset orientation to the attach point.",
				Ogre::PT_QUATERNION),&msOffsetRotationCmd);	  


			dict->addParameter(Ogre::ParameterDef("LifeTime", 
				"the life time of ribbon vertex.",
				Ogre::PT_REAL),&msLifeTimeCmd);

			dict->addParameter(Ogre::ParameterDef("Width", 
				"the width of ribbon.",
				Ogre::PT_REAL),&msWidthCmd);

			dict->addParameter(Ogre::ParameterDef("Colour", 
				"the colour of head point.",
				Ogre::PT_COLOURVALUE),&msColourCmd);


			dict->addParameter(Ogre::ParameterDef("Material", 
				"name of ribbon material.",
				Ogre::PT_STRING),&msMaterialCmd);	 


		}
	}
    //-----------------------------------------------------------------------
	AnimationLine::~AnimationLine()
	{
		destroyRenderInstance();
	}
    //-----------------------------------------------------------------------
	bool AnimationLine::createRenderInstance(void)
	{	
		if (mCreateInstance)
		{
            // 如果已经创建，就马上返回
			mBasicNode->setVisible(true);

			mCurrentLifeTime = 0.0f;


			return true;
		}
		else
		{
			// create scene node
			Ogre::SceneNode *baseNode = mSystem->getBaseSceneNode();

			mBasicNode = baseNode->createChildSceneNode();

			Ogre::SceneNode* parent = mBasicNode;

			if (parent)
			{
				mManualObject = mSystem->getSceneManager()->createManualObject(mBasicNode->getName());

				mBasicNode->attachObject(mManualObject);

                mManualObject->setCastShadows(false);

				mManualObject->clear();
				mManualObject->begin(mMaterialName);
				mManualObject->position(mPosition.x,mPosition.y - mWidth/2.0f,mPosition.z);
				mManualObject->textureCoord(0.0f,0.0f);
				mManualObject->position(mPosition.x,mPosition.y + mWidth/2.0f,mPosition.z);
				mManualObject->textureCoord(0.0f,1.0f);
				mManualObject->position(mTargetPoint.x,mTargetPoint.y - mWidth/2.0f,mTargetPoint.z);
				mManualObject->textureCoord(1.0f,0.0f);
				mManualObject->position(mTargetPoint.x,mTargetPoint.y + mWidth/2.0f,mTargetPoint.z);

				mManualObject->index(0);
				mManualObject->index(2);
				mManualObject->index(1);
				mManualObject->index(2);
				mManualObject->index(3);
				mManualObject->index(1);
				mManualObject->end();

				mCreateInstance = true;

				return true;
			}

			return false;
		}		
	}
    //-----------------------------------------------------------------------
	void AnimationLine::destroyRenderInstance(void)
	{
		if(mManualObject)
		{
			mBasicNode->getCreator()->destroyManualObject(mManualObject);
			mManualObject = NULL;
		}

		// delete scene node
		if (mBasicNode)
		{
			mBasicNode->getCreator()->destroySceneNode(mBasicNode);
			mBasicNode = NULL;
		}	

		mCreateInstance = false;
	}
    //-----------------------------------------------------------------------
	void AnimationLine::updateRenderInstance( float time, TransformInfo &transform )
	{
		mPosition = transform.mPosition;
		if (mCreateInstance)
		{
			mManualObject->clear();
			mManualObject->begin(mMaterialName);
			mManualObject->position(mPosition.x,mPosition.y - mWidth/2.0f,mPosition.z);
			mManualObject->textureCoord(0.0f,0.0f);
			mManualObject->position(mPosition.x,mPosition.y + mWidth/2.0f,mPosition.z);
			mManualObject->textureCoord(0.0f,1.0f);
			mManualObject->position(mTargetPoint.x,mTargetPoint.y - mWidth/2.0f,mTargetPoint.z);
			mManualObject->textureCoord(1.0f,0.0f);
			mManualObject->position(mTargetPoint.x,mTargetPoint.y + mWidth/2.0f,mTargetPoint.z);

			mManualObject->index(0);
			mManualObject->index(2);
			mManualObject->index(1);
			mManualObject->index(2);
			mManualObject->index(3);
			mManualObject->index(1);
			mManualObject->end();

			mCreateInstance = true;
		}

	} 

    //-----------------------------------------------------------------------
	void AnimationLine::_createBuffer( size_t vertexCount )
	{


		mBuffersCreated = true;
	}
    //-----------------------------------------------------------------------
	void AnimationLine::_destroyBuffer(void)
	{

		mBuffersCreated = false;
	}
 
	//-----------------------------------------------------------------------
	void AnimationLine::setMaterialName(const Ogre::String &matName)
	{
		mMaterialName = matName;

		if (mManualObject)
			mManualObject->setMaterialName(0,mMaterialName);
	}
    //-----------------------------------------------------------------------
    void AnimationLine::copyParameters(AnimationLine& newInfo) const
    {        
        newInfo.mColour = mColour;
        newInfo.mStartAlphaValue = mStartAlphaValue;
        newInfo.mEndAlphaValue = mEndAlphaValue;
        newInfo.mMaterialName = mMaterialName;
        newInfo.mWidth = mWidth;
        newInfo.mLifeTime = mLifeTime;
        newInfo.mAttachTime = mAttachTime;
        newInfo.mAttachPoint = mAttachPoint;
        newInfo.mOffsetPos = mOffsetPos;
        newInfo.mOffsetRotation = mOffsetRotation;
    }

    //-----------------------------------------------------------------------
	String AnimationLine::CmdWidth::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationLine *>(target)->getWidth() );
	}
	void AnimationLine::CmdWidth::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationLine *>(target)->setWidth(Ogre::StringConverter::parseReal(val));
	}

    //-----------------------------------------------------------------------
	String AnimationLine::CmdColour::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationLine *>(target)->getColour() );
	}
	void AnimationLine::CmdColour::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationLine *>(target)->setColour(Ogre::StringConverter::parseColourValue(val));
	}


    //-----------------------------------------------------------------------
	String AnimationLine::CmdMaterial::doGet(const void* target) const
	{
		return static_cast<const AnimationLine *>(target)->getMaterialName();
	}
	void AnimationLine::CmdMaterial::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationLine *>(target)->setMaterialName(val);
	}

    //-----------------------------------------------------------------------
	String AnimationLine::CmdLifeTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationLine *>(target)->getLifeTime() );
	}
	void AnimationLine::CmdLifeTime::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationLine *>(target)->setLifeTime(Ogre::StringConverter::parseReal(val));
	}

    //-----------------------------------------------------------------------
	String AnimationLine::CmdAttachTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationLine *>(target)->getAttachTime() );
	}
	void AnimationLine::CmdAttachTime::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationLine *>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
	}

    //-----------------------------------------------------------------------
	String AnimationLine::CmdAttachPoint::doGet(const void* target) const
	{
		return static_cast<const AnimationLine *>(target)->getAttachPoint();
	}
	void AnimationLine::CmdAttachPoint::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationLine *>(target)->setAttachPoint(val);
	}

    //-----------------------------------------------------------------------
	String AnimationLine::CmdOffsetPos::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationLine *>(target)->getOffsetPos() );
	}
	void AnimationLine::CmdOffsetPos::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationLine *>(target)->setOffsetPos(Ogre::StringConverter::parseVector3(val));
	}

    //-----------------------------------------------------------------------
	String AnimationLine::CmdOffsetRotation::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationLine *>(target)->getOffsetRotation() );
	}
	void AnimationLine::CmdOffsetRotation::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationLine *>(target)->setOffsetRotation(Ogre::StringConverter::parseQuaternion(val));
	}



}