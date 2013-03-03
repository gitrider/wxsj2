#ifndef __FAIRYPROPERTYHELPER_H__
#define __FAIRYPROPERTYHELPER_H__

#include "FairyPrerequisites.h"
#include "FairyPropertyInterface.h"

#include <OgreStringConverter.h>

#include <boost/lexical_cast.hpp>

namespace Fairy
{
    typedef Ogre::String::value_type Char;

    struct Constant
    {
        const Char* label;
        int value;
    };

    bool mapEnumLabel(const Constant* constants, int value,Ogre::String& label);
    bool mapEnumValue(const Constant* constants, const Ogre::String& label, int& value, bool caseSensitive = true);

    bool populateFlagsLabel(const Constant* constants, int value,Ogre::String& label);
    bool populateFlagsValue(const Constant* constants, const Ogre::String& label, int& value, bool caseSensitive = true);

   Ogre::String getEnumTypeString(const Ogre::String& type, const Constant* constants);
   Ogre::String getFlagsTypeString(const Ogre::String& type, const Constant* constants);

    //////////////////////////////////////////////////////////////////////////

    Ogre::ColourValue String2ColourRGB(const Ogre::String& val);
   Ogre::String ColourRGB2String(const Ogre::ColourValue& val);

    Ogre::ColourValue String2ColourARGB(const Ogre::String& val);
   Ogre::String ColourARGB2String(const Ogre::ColourValue& val);

    //////////////////////////////////////////////////////////////////////////

    template <class T>
    inline Ogre::String
    __toString(const T& v)
    {
        return Ogre::StringConverter::toString(v);
    }

    template <>
    inline Ogre::String
    __toString<String>(const Ogre::String& v)
    {
        return v;
    }

    //////////////////////////////////////////////////////////////////////////

    template <class T>
    inline T
    __parseValue(const Ogre::String& s)
    {
        return boost::lexical_cast<T>(s);
    }

    template <>
    inline Ogre::String
    __parseValue<String>(const Ogre::String& s)
    {
        return s;
    }

    template <>
    inline bool
    __parseValue<bool>(const Ogre::String& s)
    {
        return Ogre::StringConverter::parseBool(s);
    }

    template <>
    extern Ogre::Vector3
    __parseValue<Ogre::Vector3>(const Ogre::String& s);

    template <>
    extern Ogre::Quaternion
    __parseValue<Ogre::Quaternion>(const Ogre::String& s);

    //////////////////////////////////////////////////////////////////////////

    template <class Class, typename Type, Type Class::*PtrToMember>
    struct ObjectMemberPropertyCommand : PropertyCommand
    {
        Variant doGet(const void* source) const
        {
            const Class* object = static_cast<const Class*>(source);
            return object->*PtrToMember;
        }

       Ogre::String doGetAsString(const void* source) const
        {
            const Class* object = static_cast<const Class*>(source);
            return __toString(object->*PtrToMember);
        }

        void doSet(void* target, const Variant& value)
        {
            Class* object = static_cast<Class*>(target);
            object->*PtrToMember = VariantCast<Type>(value);
            object->updateRenderInstance();
        }

        void doSetAsString(void* target, const Ogre::String& value)
        {
            Class* object = static_cast<Class*>(target);
            object->*PtrToMember = __parseValue<Type>(value);
            object->updateRenderInstance();
        }
    };

}

#endif 
