#ifndef __FAIRYPOSTFILTERMANAGER_H__
#define __FAIRYPOSTFILTERMANAGER_H__

#include "FairyPostFilterPrerequisites.h"

namespace Fairy {

// This should be an Singleton, but you
// need to take into account its destruction, whereas
// the singleton pattern can let's you forget about this
// So, consider singleton-ize it in your framework
class PostFilterManager
{
public:
    PostFilterManager(void);
    ~PostFilterManager();

public:
    void _notifyViewportSizeChanged(Ogre::Viewport* vp);

    PostFilter* createPostFilter(const Ogre::String& name, const Ogre::String& type, Ogre::Viewport* vp);
    PostFilter* createPostFilter(const Ogre::String& type, Ogre::Viewport* vp);
    PostFilter* getPostFilter(const Ogre::String& name) const;
    bool hasPostFilter(const Ogre::String& name) const;
    void destroyPostFilter(PostFilter* filter);
    void destroyPostFilter(const Ogre::String& name);
    void destroyAllPostFilter(void);

    void setPostFilterParameter(const Ogre::String& filterName, const Ogre::String& parameterName, const Ogre::String& value);
   Ogre::String getPostFilterParameter(const Ogre::String& filterName, const Ogre::String& parameterName);

    void setPostFilterEnabled(const Ogre::String& filterName, bool enabled);
    bool getPostFilterEnabled(const Ogre::String& filterName) const;

    typedef std::map<String, PostFilter*> PostFilterList;

    const PostFilterList& getPostFilters(void) const
    {
        return mPostFilters;
    }

    void registerFactory(PostFilterFactory* factory);
    PostFilterFactory* getFactory(const Ogre::String& type) const;

    typedef std::map<String, PostFilterFactory*> PostFilterFactoryList;

    const PostFilterFactoryList& getFactoryList(void) const
    {
        return mPostFilterFactories;
    }

private:
    PostFilterFactoryList mPostFilterFactories;
    PostFilterList mPostFilters;
};

}

#endif 
