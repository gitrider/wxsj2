#ifndef __FAIRYSCENE_H__
#define __FAIRYSCENE_H__

#include "FairyObject.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>

namespace Fairy
{
    struct by_sequenced;
    struct by_identity;
    struct by_category;
    struct by_type;
    struct by_name;

    // Type hiding technique to reduction of symbol name lengths

    struct ObjectSetIndices :
        boost::multi_index::indexed_by<
            boost::multi_index::sequenced<
                boost::multi_index::tag<by_sequenced>
            >,
            boost::multi_index::ordered_unique<
                boost::multi_index::tag<by_identity>,
                boost::multi_index::identity<ObjectPtr>
            >,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<by_category>,
                BOOST_MULTI_INDEX_CONST_MEM_FUN(Object, const Ogre::String&, getCategory)
            >,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<by_type>,
                BOOST_MULTI_INDEX_CONST_MEM_FUN(Object, const Ogre::String&, getType)
            >,
            boost::multi_index::ordered_unique<
                boost::multi_index::tag<by_name>,
                BOOST_MULTI_INDEX_MEMBER(Object,Ogre::String, mName)
            >
        >
    {};

    class Scene
    {
    public:
       Ogre::String mName;
       Ogre::String mAuthor;
       Ogre::String mCopyright;
       Ogre::String mDescription;
       Ogre::String mTerrainFilename;
	   Ogre::Real mTerrainMinHeight;
	   Ogre::Real mTerrainMaxHeight;

        struct Objects :
            boost::multi_index::multi_index_container<
                ObjectPtr,
                ObjectSetIndices
            >
        {};

        typedef std::map<String,Ogre::String> Attributes;

    protected:
        Objects mObjects;
        Attributes mAttributes;

       Ogre::String mAutoNamePrefix;
        size_t mAutoNameIndex;

    public:
        static const Ogre::String DEFAULT_AUTO_NAME_PREFIX;

        typedef Objects::index<by_type>::type::iterator ObjectsByTypeIterator;
        typedef std::pair<ObjectsByTypeIterator, ObjectsByTypeIterator> ObjectsByTypeRange;
        typedef Objects::index<by_category>::type::iterator ObjectsByCategoryIterator;
        typedef std::pair<ObjectsByCategoryIterator, ObjectsByCategoryIterator> ObjectsByCategoryRange;

        Scene();
        ~Scene();

        void setAutoNamePrefix(const Ogre::String& newAutoNamePrefix);
        const Ogre::String& getAutoNamePrefix(void) const         { return mAutoNamePrefix; }

       Ogre::String generateAutoName(const ObjectPtr &object);
        bool isAutoName(const Ogre::String &name) const;

        void reset();

        const Objects& getObjects(void) const
        {
            return mObjects;
        }

        void addObject(const ObjectPtr& object);
        void removeObject(const Ogre::String& name);
        void removeObject(const ObjectPtr& object);
        void renameObject(const Ogre::String& oldName, const Ogre::String& newName);
        void renameObject(const ObjectPtr& object, const Ogre::String& newName);

        const ObjectPtr& findObjectByName(const Ogre::String& name) const;
        ObjectsByTypeRange findObjectsByType(const Ogre::String& type) const;
        ObjectsByCategoryRange findObjectsByCategory(const Ogre::String& category) const;

        void broadcastMessage(const Ogre::String& msg, const Variant& params);

        const Attributes& getAttributes(void) const
        {
            return mAttributes;
        }

        bool existAttribute(const Ogre::String& name);
        const Ogre::String& getAttribute(const Ogre::String& name) const;
        void setAttribute(const Ogre::String& name, const Ogre::String& value);
        void removeAttribute(const Ogre::String& name);
    };
}

#endif 
