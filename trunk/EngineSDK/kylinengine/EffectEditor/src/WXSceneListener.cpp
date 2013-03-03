#include "WXSceneListener.h"

namespace Fairy
{
    void SceneListener::onSceneReset(void)
    {
    }

    void SceneListener::onAddObject(const ObjectPtr& object)
    {
    }

    void SceneListener::onRemoveObject(const ObjectPtr& object)
    {
    }

    void SceneListener::onRenameObject(const ObjectPtr& object, const String& oldName)
    {
    }

    void SceneListener::onSelectObject(const ObjectPtr& object)
    {
    }

    void SceneListener::onDeselectObject(const ObjectPtr& object)
    {
    }

    void SceneListener::onDeselectAllObjects(void)
    {
    }

    void SceneListener::onObjectPropertyChanged(const ObjectPtr& object, const String& name)
    {
    }






	// �������е�����¼���Ӧ�������鿴̯λ��Ϣ
	void SceneListener::onViewStallInfo(
										unsigned char	bCanStall,	//�Ƿ���԰�̯
										unsigned char	iTradeTax,	//����˰
										unsigned long	dwPosTax,	//̯λ��
										unsigned char	iExtraInfo	//������Ϣ 
										)
	{

	}
	
	//
	// ̯λ�༭�� ֪ͨ�ص�������
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �����༭�� ֪ͨ�ص�������
	//



	// �������е�����¼���Ӧ�������鿴������Ϣ
	void SceneListener::onViewCurSoundItem(
											unsigned long	ulInstance,	//����ʵ��id
											unsigned long	ulSoundType,//����
											unsigned long	dwRadius	//��Ч�뾶 
											)
	{

	}//
	


    void SceneListener::onCameraPosChanged()
    {
    }


    void SceneListener::onUIChanged(void* data, UIChangeType type)
    {

    }






    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// listener ������
	//
    SceneListenerManager::SceneListenerManager(void)
        : mListeners()
    {
    }

    SceneListenerManager::~SceneListenerManager()
    {
    }

    void SceneListenerManager::addSceneListener(SceneListener* listener)
    {
        mListeners.push_back(listener);
    }

    void SceneListenerManager::removeSceneListener(SceneListener* listener)
    {
        mListeners.remove(listener);
    }

    void SceneListenerManager::_fireSceneReset(SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onSceneReset();
        }
    }

    void SceneListenerManager::_fireAddObject(const ObjectPtr& object, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onAddObject(object);
        }
    }

    void SceneListenerManager::_fireRemoveObject(const ObjectPtr& object, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onRemoveObject(object);
        }
    }

    void SceneListenerManager::_fireRenameObject(const ObjectPtr& object, const String& oldName, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onRenameObject(object, oldName);
        }
    }

    void SceneListenerManager::_fireSelectObject(const ObjectPtr& object, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onSelectObject(object);
        }
    }

    void SceneListenerManager::_fireDeselectObject(const ObjectPtr& object, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onDeselectObject(object);
        }
    }

    void SceneListenerManager::_fireDeselectAllObjects(SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onDeselectAllObjects();
        }
    }

    void SceneListenerManager::_fireObjectPropertyChanged(const ObjectPtr& object, const String& name, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onObjectPropertyChanged(object, name);
        }
    }


    void SceneListenerManager::_fireCameraPosChanged(SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onCameraPosChanged();
        }
    }

    void SceneListenerManager::_fireUIChanged(void* data, UIChangeType type, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onUIChanged(data, type);
        }
    }

}
