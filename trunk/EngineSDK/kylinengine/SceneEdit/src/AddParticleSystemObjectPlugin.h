#ifndef __ADDPARTICLESYSTEMOBJECTPLUGIN_H__
#define __ADDPARTICLESYSTEMOBJECTPLUGIN_H__

#include "WXSceneListener.h"

namespace Fairy {

    class SceneManipulator;

    /** 一个监听者类，负责更新场景中的所有光源在编辑器中的指示器（一个模型
    在SceneManipulator中创建
    */
    class AddParticleSystemObjectPlugin : public SceneListener
    {
    public:

        AddParticleSystemObjectPlugin(Fairy::SceneManipulator* sceneManipulator);

        virtual ~AddParticleSystemObjectPlugin();

        //////////////////////////////////////////////////////////////////////////
        virtual void onSceneReset(void);
        virtual void onAddObject(const ObjectPtr& object);
        virtual void onRemoveObject(const ObjectPtr& object);
        virtual void onSelectObject(const ObjectPtr& object);
        virtual void onDeselectObject(const ObjectPtr& object);
        virtual void onDeselectAllObjects(void);
        virtual void onObjectPropertyChanged(const ObjectPtr& object, const String& name);
		virtual void onSelectableChanged(const Ogre::String& objType, bool bSelectable = true);
		virtual void onSelectableChanged(const ObjectPtr& object, bool bSelectable = true);

    protected:

        // 类的前向声明，光源的指示器
        class Indicator;

        typedef std::map<ObjectPtr, Indicator *> Indicators;
        Indicators mIndicators;

        Fairy::SceneManipulator* mSceneManipulator;
    };
}
#endif // 