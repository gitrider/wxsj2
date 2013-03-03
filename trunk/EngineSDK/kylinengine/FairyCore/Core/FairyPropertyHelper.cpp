#include "FairyPropertyHelper.h"
#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreStringVector.h>
#include <OgreColourValue.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace Fairy
{
    bool mapEnumLabel(const Constant* constants, int value,Ogre::String& label)
    {
        assert(constants);

        for (const Constant* i = constants; i->label; ++i)
        {
            if (i->value == value)
            {
                label = i->label;
                return true;
            }
        }

        return false;
    }

    bool mapEnumValue(const Constant* constants, const Ogre::String& label, int& value, bool caseSensitive)
    {
        assert(constants);
        typedef int (*CmpFunc)(const Char*, const Char*);
        CmpFunc cmp = caseSensitive ? strcmp : stricmp;

        for (const Constant* i = constants; i->label; ++i)
        {
            if (cmp(i->label, label.c_str()) == 0)
            {
                value = i->value;
                return true;
            }
        }

        return false;
    }

    bool populateFlagsLabel(const Constant* constants, int value,Ogre::String& label)
    {
        assert(constants);

        label.clear();
        for (const Constant* i = constants; i->label; ++i)
        {
            if (i->value & value)
            {
                if (!label.empty())
                    label.append(", ");
                label.append(i->label);
            }
        }

        return true;
    }

    bool populateFlagsValue(const Constant* constants, const Ogre::String& label, int& value, bool caseSensitive)
    {
        assert(constants);
		Ogre::StringVector items = Ogre::StringUtil::split(label, ",|");

        int flags = 0;
		for (Ogre::StringVector::iterator i = items.begin(); i != items.end(); ++i)
        {
           Ogre::String& item = *i;
            Ogre::StringUtil::trim(item);

            int flag;
            if (!mapEnumValue(constants, item, flag, caseSensitive))
                return false;
            flags |= flag;
        }

        value = flags;
        return true;
    }

    static Ogre::String getLabelList(const Constant* constants)
    {
       Ogre::String result;
        for (const Constant* i = constants; i->label; ++i)
        {
            if (!result.empty())
                result.append(", ");
            result.append(i->label);
        }
        return result;
    }

   Ogre::String getEnumTypeString(const Ogre::String& type, const Constant* constants)
    {
        return "Enum " + type + " ( " + getLabelList(constants) + " )";
    }

   Ogre::String getFlagsTypeString(const Ogre::String& type, const Constant* constants)
    {
        return "Flags " + type + " ( " + getLabelList(constants) + " )";
    }

    Ogre::ColourValue String2ColourRGB(const Ogre::String& val)
    {
        // 如果是由color控件得到的颜色字符串，它的第一个字符为(
        if (val[0] == '(')
        {

           Ogre::String tempColorStr = val.substr(1,val.length()-2);

		   Ogre::StringVector vec = Ogre::StringUtil::split(tempColorStr, ",");

            assert(vec.size() == 3);

            int r = Ogre::StringConverter::parseInt(vec[0]);
            int g = Ogre::StringConverter::parseInt(vec[1]);
            int b = Ogre::StringConverter::parseInt(vec[2]);

            return Ogre::ColourValue( (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f );
        }
        else
            return String2ColourARGB(val);
    }

   Ogre::String ColourRGB2String(const Ogre::ColourValue& val)
    {
        Ogre::StringUtil::StrStreamType stream;
        stream << val.r << " " << val.g << " " << val.b;
        return stream.str();
    }

    Ogre::ColourValue String2ColourARGB(const Ogre::String& val)
    {
        return Ogre::StringConverter::parseColourValue(val);
    }

   Ogre::String ColourARGB2String(const Ogre::ColourValue& val)
    {
        return Ogre::StringConverter::toString(val);
    }

    template <>
    extern Ogre::Vector3
    __parseValue<Ogre::Vector3>(const Ogre::String& s)
    {
        return Ogre::StringConverter::parseVector3(s);
    }

    template <>
    extern Ogre::Quaternion
    __parseValue<Ogre::Quaternion>(const Ogre::String& s)
    {
        return Ogre::StringConverter::parseQuaternion(s);
    }

}
