/********************************************************************
filename:   FairyPointLightElement.cpp

purpose:    ���Դ��ЧԪ�أ���Ogre��Light���з�װ������ʹ��������Ч��
            ��Χ���������Ӱ�죬�緢���Ļ��������������
*********************************************************************/

#include "FairyPointLightElement.h"

#include "Core/FairySystem.h"

#include <OgreLight.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace Fairy	{

    PointLightElement::CmdDiffuse PointLightElement::msDiffuseCmd;
    PointLightElement::CmdSpecular PointLightElement::msSpecularCmd;
    PointLightElement::CmdRange PointLightElement::msRangeCmd;
    PointLightElement::CmdAttenuationConstant PointLightElement::msAttenuationConstantCmd;
    PointLightElement::CmdAttenuationLinear PointLightElement::msAttenuationLinearCmd;
    PointLightElement::CmdAttenuationQuadric PointLightElement::msAttenuationQuadricCmd;

    //////////////////////////////////////////////////////////////////////////
	PointLightElement::PointLightElement(const Ogre::String &type, System *system) :
	EffectElement(type, system),
	mLight(NULL),
    mDiffuse(Ogre::ColourValue::White),
    mSpecular(Ogre::ColourValue::Black),
    mRange(1000),
    mAttenuationConstant(1.0f),
    mAttenuationLinear(0.0f),
    mAttenuationQuadric(0.0f)
	{
        if ( initDefaults(mElementType) )
        {
            Ogre::ParamDictionary* dict = getParamDictionary();   

            dict->addParameter(Ogre::ParameterDef("Diffuse", 
                "diffuse colour of point light.",
                Ogre::PT_COLOURVALUE),&msDiffuseCmd);

            dict->addParameter(Ogre::ParameterDef("Specular", 
                "specular colour of point light.",
                Ogre::PT_COLOURVALUE),&msSpecularCmd);

            dict->addParameter(Ogre::ParameterDef("Range", 
                "range of point light.",
                Ogre::PT_REAL),&msRangeCmd);

            dict->addParameter(Ogre::ParameterDef("AttenuationConstant", 
                "attenuation constant of point light.",
                Ogre::PT_REAL),&msAttenuationConstantCmd);

            dict->addParameter(Ogre::ParameterDef("AttenuationLinear", 
                "attenuation linear of point light.",
                Ogre::PT_REAL),&msAttenuationLinearCmd);

            dict->addParameter(Ogre::ParameterDef("AttenuationQuadric", 
                "attenuation quadric of point light.",
                Ogre::PT_REAL),&msAttenuationQuadricCmd);
        }
	}
    //-----------------------------------------------------------------------
	PointLightElement::~PointLightElement()
	{
		destroyRenderInstance();
	}
    //-----------------------------------------------------------------------
	bool PointLightElement::createRenderInstance()
	{
		return _createLight();
	}
    //-----------------------------------------------------------------------
	void PointLightElement::destroyRenderInstance()
	{
		_destroyLight();

		EffectElement::destroyRenderInstance();
	}
    //-----------------------------------------------------------------------
	bool PointLightElement::_createLight(void)
	{
		assert ( NULL == mLight );
		
		if (mBasicNode)
		{
			mLight = mSystem->getSceneManager()->createLight( mBasicNode->getName() );
			assert (mLight);

            // ��ʼ����Դ��Ϣ
            mLight->setDiffuseColour(mDiffuse);
            mLight->setSpecularColour(mSpecular);
            mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);

            mLight->setCastShadows(false);

            mLight->setVisible(mVisible);

			mBasicNode->attachObject(mLight);

			mBasicNode->setPosition(mRelPos);
            mBasicNode->setOrientation(mRelOrientation);

			return true;
		}

		return false;
	}
    //-----------------------------------------------------------------------
	void PointLightElement::_destroyLight(void)
	{
		if (mLight )
		{
			mSystem->getSceneManager()->destroyLight(mLight->getName());
			mLight = NULL;
		}
	}
    //-----------------------------------------------------------------------
    void PointLightElement::setDiffuse(const Ogre::ColourValue& diffuse)
    {
        mDiffuse = diffuse;

        if (mLight)
            mLight->setDiffuseColour(mDiffuse);
    }
    //-----------------------------------------------------------------------
    void PointLightElement::setSpecular(const Ogre::ColourValue& specular)
    {
        mSpecular = specular;

        if (mLight)
            mLight->setSpecularColour(mSpecular);
    }
    //-----------------------------------------------------------------------
    void PointLightElement::setRange(Ogre::Real range)
    {
        mRange = range;

        if (mLight)
            mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
    }
    //-----------------------------------------------------------------------
    void PointLightElement::setAttenuationConstant(Ogre::Real constant)
    {
        mAttenuationConstant = constant;

        if (mLight)
            mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
    }
    //-----------------------------------------------------------------------
    void PointLightElement::setAttenuationLinear(Ogre::Real linear)
    {
        mAttenuationLinear = linear;

        if (mLight)
            mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
    }
    //-----------------------------------------------------------------------
    void PointLightElement::setAttenuationQuadric(Ogre::Real quadric)
    {
        mAttenuationQuadric = quadric;

        if (mLight)
            mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
    }
    //-----------------------------------------------------------------------
    void PointLightElement::setVisibleFlag(uint32 flags)
    {
        if (mLight)
            mLight->setVisibilityFlags(flags);
    }
    //////////////////////////////////////////////////////////////////////////
    //-----------------------------------------------------------------------
   Ogre::String PointLightElement::CmdDiffuse::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const PointLightElement*>(target)->getDiffuse() );
    }
    void PointLightElement::CmdDiffuse::doSet(void* target, const Ogre::String& val)
    {
        static_cast<PointLightElement*>(target)->setDiffuse(Ogre::StringConverter::parseColourValue(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void PointLightElement::CmdDiffuse::doCopy(void* target, const void* source)
    {
        static_cast<PointLightElement*>(target)->setDiffuse(
            static_cast<const PointLightElement*>(source)->getDiffuse() );
    }
#endif
    //-----------------------------------------------------------------------
   Ogre::String PointLightElement::CmdSpecular::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const PointLightElement*>(target)->getSpecular() );
    }
    void PointLightElement::CmdSpecular::doSet(void* target, const Ogre::String& val)
    {
        static_cast<PointLightElement*>(target)->setSpecular(Ogre::StringConverter::parseColourValue(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void PointLightElement::CmdSpecular::doCopy(void* target, const void* source)
    {
        static_cast<PointLightElement*>(target)->setSpecular(
            static_cast<const PointLightElement*>(source)->getSpecular() );
    }
#endif
    //-----------------------------------------------------------------------
   Ogre::String PointLightElement::CmdRange::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const PointLightElement*>(target)->getRange() );
    }
    void PointLightElement::CmdRange::doSet(void* target, const Ogre::String& val)
    {
        static_cast<PointLightElement*>(target)->setRange(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void PointLightElement::CmdRange::doCopy(void* target, const void* source)
    {
        static_cast<PointLightElement*>(target)->setRange(
            static_cast<const PointLightElement*>(source)->getRange() );
    }
#endif

    //-----------------------------------------------------------------------
   Ogre::String PointLightElement::CmdAttenuationConstant::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const PointLightElement*>(target)->getAttenuationConstant() );
    }
    void PointLightElement::CmdAttenuationConstant::doSet(void* target, const Ogre::String& val)
    {
        static_cast<PointLightElement*>(target)->setAttenuationConstant(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void PointLightElement::CmdAttenuationConstant::doCopy(void* target, const void* source)
    {
        static_cast<PointLightElement*>(target)->setAttenuationConstant(
            static_cast<const PointLightElement*>(source)->getAttenuationConstant() );
    }
#endif

    //-----------------------------------------------------------------------
   Ogre::String PointLightElement::CmdAttenuationLinear::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const PointLightElement*>(target)->getAttenuationLinear() );
    }
    void PointLightElement::CmdAttenuationLinear::doSet(void* target, const Ogre::String& val)
    {
        static_cast<PointLightElement*>(target)->setAttenuationLinear(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void PointLightElement::CmdAttenuationLinear::doCopy(void* target, const void* source)
    {
        static_cast<PointLightElement*>(target)->setAttenuationLinear(
            static_cast<const PointLightElement*>(source)->getAttenuationLinear() );
    }
#endif

    //-----------------------------------------------------------------------
   Ogre::String PointLightElement::CmdAttenuationQuadric::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const PointLightElement*>(target)->getAttenuationQuadric() );
    }
    void PointLightElement::CmdAttenuationQuadric::doSet(void* target, const Ogre::String& val)
    {
        static_cast<PointLightElement*>(target)->setAttenuationQuadric(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void PointLightElement::CmdAttenuationQuadric::doCopy(void* target, const void* source)
    {
        static_cast<PointLightElement*>(target)->setAttenuationQuadric(
            static_cast<const PointLightElement*>(source)->getAttenuationQuadric() );
    }
#endif
}