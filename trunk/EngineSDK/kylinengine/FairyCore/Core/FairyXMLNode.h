#ifndef __FAIRYXMLNODE_H__
#define __FAIRYXMLNODE_H__

#include "FairyXMLAttributes.h"
#include <OgreIteratorWrappers.h>

namespace Fairy
{

    class XMLNode
    {
        typedef std::vector<XMLNode*> XMLNodeVector;
    public:
        typedef Ogre::ConstVectorIterator<XMLNodeVector> ChildrenIterator;

        XMLNode(const Ogre::String& name);
        ~XMLNode();

        XMLNode* getParent(void) const
        {
            return mParent;
        }

        const Ogre::String& getName(void) const
        {
            return mName;
        }

        //////////////////////////////////////////////////////////////////////////

        const XMLAttributes& attributes(void) const
        {
            return mAttributes;
        }

        XMLAttributes& attributes(void)
        {
            return mAttributes;
        }

        template <class T>
        typename XMLAttributes::helper<T>::result_type getAttribute(const Ogre::String& name) const
        {
            return mAttributes.getValueAs<T>(name);
        }

        template <class T>
        typename XMLAttributes::helper<T>::result_type getAttribute(const Ogre::String& name, typename boost::call_traits<T>::param_type def) const
        {
            return mAttributes.getValueAs<T>(name, def);
        }

        template <class T>
        void setAttribute(const Ogre::String& name, T val)
        {
            mAttributes.setValueAs(name, val);
        }

        template <class T>
        void setAttribute(const Ogre::String& name, T val, typename boost::call_traits<T>::param_type def)
        {
            mAttributes.setValueAs(name, val, def);
        }

        void setAttribute(const Ogre::String& name, const Ogre::String& val)
        {
            mAttributes.setValueAs(name, val);
        }

        void setAttribute(const Ogre::String& name, const Ogre::String& val, const Ogre::String& def)
        {
            mAttributes.setValueAs(name, val, def);
        }

        //////////////////////////////////////////////////////////////////////////

        void setTextContent(const Ogre::String& textContent);
        const Ogre::String& getTextContent(void) const;

        XMLNode* createChild(const Ogre::String& name);
        void addChild(XMLNode* child);
        void removeChild(size_t index);
        void removeAllChildren(void);
        size_t getChildCount(void) const;
        XMLNode* getChild(size_t index) const;
        ChildrenIterator getChildrenIterator(void) const;

    protected:
        XMLNode* mParent;
       Ogre::String mName;
       Ogre::String mTextContent;
        XMLAttributes mAttributes;
        XMLNodeVector mChildren;
    };

}

#endif  // 
