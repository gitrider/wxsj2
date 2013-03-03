#ifndef __FAIRYPOSTFILTER_H__
#define __FAIRYPOSTFILTER_H__

#include "FairyPostFilterPrerequisites.h"

namespace Fairy {

class PostFilter
{
public:
    PostFilter(const Ogre::String& name, const Ogre::String& compositorName);
    virtual ~PostFilter();

public:

    virtual const Ogre::String& getType(void) const = 0;

    const Ogre::String& getName(void) const
    {
        return mName;
    }

    const Ogre::String& getCompositorName(void) const
    {
        return mCompositorName;
    }

    Ogre::Viewport* getViewport(void) const
    {
        return mViewport;
    }

    Ogre::CompositorInstance* getCompositorInstance(void) const
    {
        return mCompositorInstance;
    }

    void setEnabled(bool enable);
    bool getEnabled(void) const;

    virtual void setParameter(const Ogre::String& name, const Ogre::String& value);
    virtual Ogre::String getParameter(const Ogre::String& name) const;

    virtual void _init(Ogre::Viewport* vp);

    virtual void _notifyViewportSizeChanged(void);

protected:
   Ogre::String mName;
   Ogre::String mCompositorName;
    Ogre::Viewport* mViewport;
    Ogre::CompositorInstance* mCompositorInstance;
};

extern PostFilterFactory* getHeatVisionPostFilterFactory(void);
extern PostFilterFactory* getBloomPostFilterFactory(void);
extern PostFilterFactory* getFloodlightingPostFilterFactory(void);
extern PostFilterFactory* getFadePostFilterFactory(void);
extern PostFilterFactory* getMotionBlurPostFilterFactory(void);

extern void registerAllPostFilterFactories(PostFilterManager* postFilterManager);

}



#endif 
