#ifndef __ADDSTATICENTITYOBJECTPLUGIN_H__
#define __ADDSTATICENTITYOBJECTPLUGIN_H__

#include "WXSceneListener.h"

namespace Fairy {

    class SceneManipulator;
	class StaticEntityObject;

    /** 一个监听者类，负责更新场景中的所有可行走物件的cll（可行走面）在编辑器中的显示（一个模型
        在SceneManipulator中创建
    */
    class AddStaticEntityObjectPlugin : public SceneListener
    {
    public:

        AddStaticEntityObjectPlugin(Fairy::SceneManipulator* sceneManipulator);

        virtual ~AddStaticEntityObjectPlugin();

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
		virtual void onShowHintChanged( bool bShow = true);
    protected:

        class CllIndicator;
		class CllRenderable;
		class Indicator;

        typedef std::map<ObjectPtr, CllIndicator *> CllIndicators;
		typedef std::map<ObjectPtr, Indicator *> Indicators;
        CllIndicators mIndicators;
		Indicators mOriginIndicators;

        Fairy::SceneManipulator* mSceneManipulator;

	protected:

		void clearIndicators(void);
    };
}
#endif // __AddStaticEntityObjectPlugin_H__