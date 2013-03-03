#include "FairyPostFilter.h"

#include <OgreException.h>

#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include "FairyPostFilterManager.h"

namespace Fairy {


PostFilter::PostFilter(const Ogre::String& name, const Ogre::String& compositorName)
    : mName(name)
    , mCompositorName(compositorName)
    , mViewport()
    , mCompositorInstance()
{
}

PostFilter::~PostFilter()
{
    if (mCompositorInstance && mViewport)
    {
        Ogre::CompositorManager::getSingleton().removeCompositor(mViewport, mCompositorName);
    }
}

void PostFilter::setEnabled(bool enable)
{
    if (mCompositorInstance)
    {
        mCompositorInstance->setEnabled(enable);
    }
}

bool PostFilter::getEnabled(void) const
{
    return mCompositorInstance && mCompositorInstance->getEnabled();
}

void PostFilter::setParameter(const Ogre::String& name, const Ogre::String& value)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
        "Parameter '" + name + "' doesn't exists.",
        "PostFilter::setParameter");
}

String PostFilter::getParameter(const Ogre::String& name) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
        "Parameter '" + name + "' doesn't exists.",
        "PostFilter::getParameter");
}

void PostFilter::_init(Ogre::Viewport* vp)
{
    mViewport = vp;
    mCompositorInstance = Ogre::CompositorManager::getSingleton().addCompositor(vp, mCompositorName);
}

void PostFilter::_notifyViewportSizeChanged(void)
{
    if (mCompositorInstance)
    {
        if (mCompositorInstance->getEnabled())
        {
            mCompositorInstance->setEnabled(false);
            mCompositorInstance->setEnabled(true);
        }
    }
}

extern void registerAllPostFilterFactories( PostFilterManager* postFilterManager)
{
    assert(postFilterManager);

    postFilterManager->registerFactory(getHeatVisionPostFilterFactory());
    postFilterManager->registerFactory(getBloomPostFilterFactory());
    postFilterManager->registerFactory(getFloodlightingPostFilterFactory());
    postFilterManager->registerFactory(getFadePostFilterFactory());
    postFilterManager->registerFactory(getMotionBlurPostFilterFactory());
}


};