/********************************************************************
filename:   FairyEffectObject.h

purpose:    the object that wrap the effect,so we can put the effect in the scene
*********************************************************************/

#ifndef __EFFECTOBJECT_H__
#define __EFFECTOBJECT_H__

#include "FairyPlacementObject.h"

namespace Fairy {

    class ObjectFactory;
	class Effect;

    class EffectObject : public PlacementObject
    {
    protected:
        class Factory;

    public:
        EffectObject(void);
        virtual ~EffectObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);

        void setEffectName( const Ogre::String &name );

        const Ogre::String &getEffectName(void) const
        {
            return mEffectName;
        }

        //////////////////////////////////////////////////////////////////////////
        Effect * getEffect(void) const
        {
            return mEffect;
        }

    protected:

		/// effect template name
       Ogre::String mEffectName;
        Effect *mEffect;

    protected:

        void createEffect(void);
        void destroyEffect(void);
    };
}

#endif // __EffectObject_H__
