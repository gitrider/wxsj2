#include "FairyPostFilterManager.h"
#include "FairyPostFilterFactory.h"
#include "FairyPostFilter.h"

#include <OgreException.h>
#include <OgreString.h>
#include <OgreCompositorManager.h>

using namespace Fairy;

PostFilterManager::PostFilterManager(void)
{
}

PostFilterManager::~PostFilterManager()
{
    destroyAllPostFilter();
}

void PostFilterManager::_notifyViewportSizeChanged(Ogre::Viewport* vp)
{
   PostFilterList::const_iterator i;
    for (i = mPostFilters.begin(); i != mPostFilters.end(); ++i)
    {
        PostFilter* filter = i->second;
        if (filter->getViewport() == vp)
        {
            filter->_notifyViewportSizeChanged();
        }
    }
 }

PostFilter* PostFilterManager::createPostFilter(const Ogre::String& name, const Ogre::String& type, Ogre::Viewport* vp)
{
    if (mPostFilters.find(name) != mPostFilters.end())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
            "PostFilter with name '" + name + "' already exists.",
            "PostFilterManager::createPostFilter");
    }

    PostFilter* filter = getFactory(type)->createInstance(name);
    mPostFilters.insert(PostFilterList::value_type(name, filter));

    filter->_init(vp);
    return filter;
}

PostFilter* PostFilterManager::createPostFilter(const Ogre::String& type, Ogre::Viewport* vp)
{
    return createPostFilter(type, type, vp);
}

PostFilter* PostFilterManager::getPostFilter(const Ogre::String& name) const
{
    PostFilterList::const_iterator i = mPostFilters.find(name);
    if (i == mPostFilters.end())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
            "PostFilter with name '" + name + "' doesn't exists.",
            "PostFilterManager::getPostFilter");
    }

    return i->second;
}

bool PostFilterManager::hasPostFilter(const Ogre::String& name) const
{
    return mPostFilters.find(name) != mPostFilters.end();
}

void PostFilterManager::destroyPostFilter(PostFilter* filter)
{
    PostFilterList::iterator i;
    for (i = mPostFilters.begin(); i != mPostFilters.end(); ++i)
    {
        if (i->second == filter)
        {
            getFactory(filter->getType())->destroyInstance(filter);
            mPostFilters.erase(i);
            return;
        }
    }
}

void PostFilterManager::destroyPostFilter(const Ogre::String& name)
{
    PostFilterList::iterator i = mPostFilters.find(name);
    if (i == mPostFilters.end())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
            "PostFilter with name '" + name + "' doesn't exists.",
            "PostFilterManager::destroyPostFilter");
    }

    getFactory(i->second->getType())->destroyInstance(i->second);
    mPostFilters.erase(i);
}

void PostFilterManager::destroyAllPostFilter(void)
{
    PostFilterList::const_iterator i;
    for (i = mPostFilters.begin(); i != mPostFilters.end(); ++i)
    {
        getFactory(i->second->getType())->destroyInstance(i->second);
    }
    mPostFilters.clear();
}

void PostFilterManager::setPostFilterParameter(const Ogre::String& filterName, const Ogre::String& parameterName, const Ogre::String& value)
{
    getPostFilter(filterName)->setParameter(parameterName, value);
}

String PostFilterManager::getPostFilterParameter(const Ogre::String& filterName, const Ogre::String& parameterName)
{
    return getPostFilter(filterName)->getParameter(parameterName);
}

void PostFilterManager::setPostFilterEnabled(const Ogre::String& filterName, bool enabled)
{
    getPostFilter(filterName)->setEnabled(enabled);
}

bool PostFilterManager::getPostFilterEnabled(const Ogre::String& filterName) const
{
    return getPostFilter(filterName)->getEnabled();
}

void PostFilterManager::registerFactory(PostFilterFactory* factory)
{
    assert(factory);
    const Ogre::String& type = factory->getType();

    std::pair<PostFilterFactoryList::iterator, bool> inserted =
        mPostFilterFactories.insert(PostFilterFactoryList::value_type(type, factory));
    assert(inserted.second);

    factory->_init();
}

PostFilterFactory* PostFilterManager::getFactory(const Ogre::String& type) const
{
    PostFilterFactoryList::const_iterator it = mPostFilterFactories.find(type);
    if (it != mPostFilterFactories.end())
        return it->second;

    OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
        "Couldn't found post filter factory '" + type + "'",
        "PostFilterManager::getFactory");
    return NULL;
}
