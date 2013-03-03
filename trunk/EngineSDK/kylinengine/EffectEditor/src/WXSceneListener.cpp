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






	// 调用所有的添加事件响应函数。查看摊位信息
	void SceneListener::onViewStallInfo(
										unsigned char	bCanStall,	//是否可以摆摊
										unsigned char	iTradeTax,	//交易税
										unsigned long	dwPosTax,	//摊位费
										unsigned char	iExtraInfo	//附加信息 
										)
	{

	}
	
	//
	// 摊位编辑， 通知回调函数。
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 声音编辑， 通知回调函数。
	//



	// 调用所有的添加事件响应函数。查看声音信息
	void SceneListener::onViewCurSoundItem(
											unsigned long	ulInstance,	//声音实例id
											unsigned long	ulSoundType,//类型
											unsigned long	dwRadius	//有效半径 
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
	// listener 管理器
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
