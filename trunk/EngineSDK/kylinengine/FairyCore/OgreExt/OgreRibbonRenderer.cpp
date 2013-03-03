/********************************************************************
filename:   OgreRibbonRenderer.cpp

purpose:    ribbon particle renderer
*********************************************************************/
#include "OgreRibbonRenderer.h"
#include "OgreRibbonSet.h"

#include <OgreNode.h>
#include <OgreStringConverter.h>

namespace Ogre
{
   Ogre::String RibbonRendererType = "ribbon";

    RibbonRenderer::CmdRibbonHeadAlpha RibbonRenderer::msRibbonHeadAlphaCmd;
    RibbonRenderer::CmdRibbonTailAlpha RibbonRenderer::msRibbonTailAlphaCmd;
    RibbonRenderer::CmdRibbonHeadWidthScale RibbonRenderer::msRibbonHeadWidthScaleCmd;
    RibbonRenderer::CmdRibbonTailWidthScale RibbonRenderer::msRibbonTailWidthScaleCmd;
    RibbonRenderer::CmdRibbonElementCount RibbonRenderer::msRibbonElementCountCmd;

    RibbonRenderer::RibbonRenderer()
        : mRibbonSet(NULL)
        , mWorldSpace(false)
        , mNextRibbonIndex(0)
    {
        if (createParamDictionary("RibbonRenderer"))
        {
            ParamDictionary* dict = getParamDictionary();
            dict->addParameter(ParameterDef("head_alpha", 
                "the alpha value of the head of ribbon.",
                PT_REAL),
                &msRibbonHeadAlphaCmd);

            dict->addParameter(ParameterDef("tail_alpha", 
                "the alpha value of the tail of ribbon.",
                PT_REAL),
                &msRibbonTailAlphaCmd);

            dict->addParameter(ParameterDef("head_width_scale", 
                "the width scale value of the head of ribbon.",
                PT_REAL),
                &msRibbonHeadWidthScaleCmd);

            dict->addParameter(ParameterDef("tail_width_scale", 
                "the width scale value of the tail of ribbon.",
                PT_REAL),
                &msRibbonTailWidthScaleCmd);

            dict->addParameter(ParameterDef("element_count",
                "the element count of the ribbon.",
                PT_UNSIGNED_INT),
                &msRibbonElementCountCmd);
        }

        mRibbonSet = new RibbonSet("", 15);
    }
    //-----------------------------------------------------------------------
    RibbonRenderer::~RibbonRenderer()
    {
        delete mRibbonSet;
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::setRibbonHeadAlpha(Ogre::Real alpha)
    {
        mRibbonSet->setHeadAlpha(alpha);
    }
    //-----------------------------------------------------------------------
    Ogre::Real RibbonRenderer::getRibbonHeadAlpha(void) const
    {
        return mRibbonSet->getHeadAlpha();
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::setRibbonTailAlpha(Ogre::Real alpha)
    {
        mRibbonSet->setTailAlpha(alpha);
    }
    //-----------------------------------------------------------------------
    Ogre::Real RibbonRenderer::getRibbonTailAlpha(void) const
    {
        return mRibbonSet->getTailAlpha();
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::setRibbonHeadWidthScale(Ogre::Real width)
    {
        mRibbonSet->setHeadWidthScale(width);
    }
    //-----------------------------------------------------------------------
    Ogre::Real RibbonRenderer::getRibbonHeadWidthScale(void) const
    {
        return mRibbonSet->getHeadWidthScale();
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::setRibbonTailWidthScale(Ogre::Real width)
    {
        mRibbonSet->setTailWidthScale(width);
    }
    //-----------------------------------------------------------------------
    Ogre::Real RibbonRenderer::getRibbonTailWidthScale(void) const
    {
        return mRibbonSet->getTailWidthScale();
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::setRibbonElementCount(size_t count)
    {
        mRibbonSet->setMaxChainElements(count);
    }
    //-----------------------------------------------------------------------
    size_t RibbonRenderer::getRibbonElementCount(void) const
    {
        return mRibbonSet->getMaxChainElements();
    }
    //-----------------------------------------------------------------------
    const Ogre::String& RibbonRenderer::getType(void) const
    {
        return RibbonRendererType;
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::_updateRenderQueue(RenderQueue* queue, 
		std::list<Particle*>& currentParticles, bool cullIndividually)
    {
        mRibbonSet->_updateRenderQueue(queue);
    }
	void RibbonRenderer::visitRenderables(Renderable::Visitor* visitor, 
		bool debugRenderables)
	{
		mRibbonSet->visitRenderables(visitor, debugRenderables);
	}
    //-----------------------------------------------------------------------
    void RibbonRenderer::_setMaterial(MaterialPtr& mat)
    {
        mRibbonSet->setMaterialName(mat->getName());
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::_notifyCurrentCamera(Camera* cam)
    {
        mRibbonSet->_notifyCurrentCamera(cam);
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::_notifyParticleMoved(std::list<Particle*>& currentParticles)
    {
        for (std::list<Particle*>::iterator i = currentParticles.begin();
            i != currentParticles.end(); ++i)
        {
            Particle* p = *i;

            RibbonParticleData* particleData = static_cast<RibbonParticleData*>(p->getVisualData());

            mRibbonSet->updateRibbon( particleData->getRibbonIndex(), p->position, mWorldSpace, p->colour, p->mWidth );
        }
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::_notifyParticleRotated(void)
    {
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::_notifyParticleResized(void)
    {
        mRibbonSet->_notifyResized();
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::_notifyParticleQuota(size_t quota)
    {
        mRibbonSet->setNumberOfChains(quota);
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::_notifyAttached(Node* parent, bool isTagPoint)
    {
        mRibbonSet->_notifyAttached(parent, isTagPoint);
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::_notifyDefaultDimensions(Ogre::Real width, Ogre::Real height)
    {
        mRibbonSet->setDefaultSize(width, height);
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::setRenderQueueGroup(uint8 queueID)
    {
        mRibbonSet->setRenderQueueGroup(queueID);
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::setKeepParticlesInLocalSpace(bool keepLocal)
    {
        mWorldSpace = !keepLocal;
    }
    //-----------------------------------------------------------------------
    Ogre::SortMode RibbonRenderer::_getSortMode(void) const
    {
        return Ogre::SM_DISTANCE;
    }
    //-----------------------------------------------------------------------
    ParticleVisualData* RibbonRenderer::_createVisualData(void)
    {
        assert (mNextRibbonIndex < mRibbonSet->getNumberOfChains());

        return new RibbonParticleData(mNextRibbonIndex++);
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::_destroyVisualData(ParticleVisualData* vis)
    {
        delete vis;
    }
    //-----------------------------------------------------------------------
    void RibbonRenderer::_notifyParticleExpired(Particle* particle)
    {
        RibbonParticleData* particleData = static_cast<RibbonParticleData*>(particle->getVisualData());

        mRibbonSet->resetRibbon(particleData->getRibbonIndex());
    }
    //-----------------------------------------------------------------------
   Ogre::String RibbonRenderer::CmdRibbonHeadAlpha::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const RibbonRenderer*>(target)->getRibbonHeadAlpha() );
    }
    void RibbonRenderer::CmdRibbonHeadAlpha::doSet(void* target, const Ogre::String& val)
    {
        static_cast<RibbonRenderer*>(target)->setRibbonHeadAlpha(
           Ogre::StringConverter::parseReal(val));
    }
    //-----------------------------------------------------------------------
   Ogre::String RibbonRenderer::CmdRibbonTailAlpha::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const RibbonRenderer*>(target)->getRibbonTailAlpha() );
    }
    void RibbonRenderer::CmdRibbonTailAlpha::doSet(void* target, const Ogre::String& val)
    {
        static_cast<RibbonRenderer*>(target)->setRibbonTailAlpha(
           Ogre::StringConverter::parseReal(val));
    }
    //-----------------------------------------------------------------------
   Ogre::String RibbonRenderer::CmdRibbonHeadWidthScale::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const RibbonRenderer*>(target)->getRibbonHeadWidthScale() );
    }
    void RibbonRenderer::CmdRibbonHeadWidthScale::doSet(void* target, const Ogre::String& val)
    {
        static_cast<RibbonRenderer*>(target)->setRibbonHeadWidthScale(
           Ogre::StringConverter::parseReal(val));
    }
    //-----------------------------------------------------------------------
   Ogre::String RibbonRenderer::CmdRibbonTailWidthScale::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const RibbonRenderer*>(target)->getRibbonTailWidthScale() );
    }
    void RibbonRenderer::CmdRibbonTailWidthScale::doSet(void* target, const Ogre::String& val)
    {
        static_cast<RibbonRenderer*>(target)->setRibbonTailWidthScale(
           Ogre::StringConverter::parseReal(val));
    }
    //-----------------------------------------------------------------------
   Ogre::String RibbonRenderer::CmdRibbonElementCount::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const RibbonRenderer*>(target)->getRibbonElementCount() );
    }
    void RibbonRenderer::CmdRibbonElementCount::doSet(void* target, const Ogre::String& val)
    {
        static_cast<RibbonRenderer*>(target)->setRibbonElementCount(
           Ogre::StringConverter::parseUnsignedInt(val));
    }
    //-----------------------------------------------------------------------
    const Ogre::String& RibbonRendererFactory::getType() const
    {
        return RibbonRendererType;
    }
    //-----------------------------------------------------------------------
    ParticleSystemRenderer* RibbonRendererFactory::createInstance( 
        const Ogre::String& name )
    {
        return new RibbonRenderer();
    }
    //-----------------------------------------------------------------------
    void RibbonRendererFactory::destroyInstance( 
        ParticleSystemRenderer* inst)
    {
        delete inst;
    }
}