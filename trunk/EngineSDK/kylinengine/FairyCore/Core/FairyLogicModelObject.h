/********************************************************************
filename:   FairyLogicModelObject.h

purpose:    ��LogicModelObject��װ��WXObjectϵ����
            ��actor object���ƣ����Ƕ�LogicModel��ԭDObject���ķ�װ��
            ��ͬ�����ڣ�actor object�Ǵ�placement object��������Ҫ
            ���ڳ�������İڷţ���LogicModelObjectֱ�Ӵ�object������
            �Լ�����λ�ã��������Ϣ����Ҫ���ڿͻ��˴������ǣ�npc��
            ��������塣
*********************************************************************/

#ifndef _LOGICMODELOBJECT_H_
#define _LOGICMODELOBJECT_H_

#include "FairyObject.h"

namespace Fairy {

    class ObjectFactory;
    class ObjectProxy;
    class LogicModel;

    class LogicModelObject : public Object
    {
    protected:
        class Factory;

    public:
        LogicModelObject(void);
        virtual ~LogicModelObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);
        void updateRenderInstance(void);

        const Ogre::String& getLogicModelName(void) const
        {
            return mLogicModelName;
        }

        void setLogicModelName(const Ogre::String& modelName);

        LogicModel* getLogicModel(void)
        {
            return mLogicModel;
        }

        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

    protected:

        void _createLogicModel(void);
        void _destroyLogicModel(void);

    protected:

        ObjectProxy* mProxy;

        LogicModel* mLogicModel;

        Ogre::String mLogicModelName;
    };

}

#endif _LogicModelObject_H_