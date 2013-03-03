#ifndef __ADDPARTICLESYSTEMOBJECTPLUGIN_H__
#define __ADDPARTICLESYSTEMOBJECTPLUGIN_H__

#include "WXSceneListener.h"

namespace Fairy {

    class SceneManipulator;

    /** һ���������࣬������³����е����й�Դ�ڱ༭���е�ָʾ����һ��ģ��
    ��SceneManipulator�д���
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

    protected:

        // ���ǰ����������Դ��ָʾ��
        class Indicator;

        typedef std::map<ObjectPtr, Indicator *> Indicators;
        Indicators mIndicators;

        Fairy::SceneManipulator* mSceneManipulator;
    };
}
#endif // 