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

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ������������༭�ص�����
	//
	// 

	// ���һ���µ�
	void SceneListener::onRegionAddNewPoint(unsigned long ulRegionId,unsigned long ulPointId, Ogre::Vector3 position)
	{

	}

	// ɾ��һ����Ļص�����
	void SceneListener::onRegionDelPoint(unsigned long ulRegionId,unsigned long ulPointId)
	{

	}

	
	// ѡ��һ������Ļص�������
	void SceneListener::onRegionSel(unsigned long ulRegionId)
	{

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Ѱ·����
	//

	void SceneListener::onAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position)
	{

	}

	void SceneListener::onDelDijkPoint(unsigned long ulPointId)
	{

	}

	void SceneListener::onSelDijkPoint(unsigned long ulPointId)
	{

	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����༭�ص�����
	//
	// 

	// ���һ������
	void SceneListener::onAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId)
	{

	}

	// ɾ��һ������
	void SceneListener::onDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId)
	{

	}

	// ѡ��һ������
	void SceneListener::onSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId)
	{

	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ������༭�ص�����
	//
	void SceneListener::onAddNewGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	// ɾ��һ������
	void SceneListener::onDelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	// ѡ��һ������
	void SceneListener::onSelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �¼�����༭�ص�����
	//
	
	void SceneListener::onAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	// ɾ��
	void SceneListener::onDelRect( unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	// ѡ��
	void SceneListener::onSelRect( unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	//
	// �¼�����༭�ص�����
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ̯λ�༭�� ֪ͨ�ص�������
	//



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
	
	//
	// �����༭�� ֪ͨ�ص�������
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////








    void SceneListener::onTerrainHeightChanged(const std::vector<TerrainInfo>& terrainInfo)
    {
    }

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


	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ִ�в�����������Ļص�����
	//

	// ���һ���µ�
	void SceneListenerManager::_fireRegionAddNewPoint(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
                listener->onRegionAddNewPoint(ulRegionId, ulPointId, position);
			}
        }
	}

	void SceneListenerManager::_fireRegionDelPoint(unsigned long ulRegionId, unsigned long ulPointId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onRegionDelPoint(ulRegionId, ulPointId);
			}
        }
	}

	// ѡ��һ������
	void SceneListenerManager::_fireRegionSel(unsigned long ulRegionId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onRegionSel(ulRegionId);
			}
        }
	}



	//
	// ִ�в�����������Ļص�����
	//
	//////////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ִ������༭��ӵ�Ļص�����
	//

	void SceneListenerManager::_fireAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
		{
			SceneListener* listener = *it;
			if (listener != exclude)
			{
				listener->onAddDijkPoint(ulPointId, position);
			}
		}
	}

	void SceneListenerManager::_fireDelDijkPoint(unsigned long ulPointId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
		{
			SceneListener* listener = *it;
			if (listener != exclude)
			{
				listener->onDelDijkPoint(ulPointId);
			}
		}
	}

	// ѡ��һ������
	void SceneListenerManager::_fireSelDijkRegion(unsigned long ulPointId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
		{
			SceneListener* listener = *it;
			if (listener != exclude)
			{
				listener->onSelDijkPoint(ulPointId);
			}
		}
	}

	//
	// ִ������༭��ӵ�Ļص�����
	//
	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����༭�� ֪ͨ�ص�������
	//

	// �������е���ӹ�����¼���Ӧ������
	void SceneListenerManager::_fireAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onAddNewMonster(ulMonsterId, ulInstanceId);
			}
        }

	}

	// �������е�ɾ��������¼���Ӧ������
	void SceneListenerManager::_fireDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude)
	{

		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onDelMonster(ulMonsterId, ulInstanceId);
			}
        }
	}


	// �������е�ɾ��������¼���Ӧ������
	void SceneListenerManager::_fireSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude)
	{

		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onSelMonster(ulMonsterId, ulInstanceId);
			}
        }
	}


	//
	// ����༭�� ֪ͨ�ص�������
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ������༭�� ֪ͨ�ص�������
	//

		// �������е���ӵ��¼���Ӧ������
	void SceneListenerManager::_fireAddNewGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onAddNewGrowPoint(ulTypeId, ulInstanceId);
			}
        }
	}

		// �������е�ɾ�����¼���Ӧ������
	void SceneListenerManager::_fireDelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onDelGrowPoint(ulTypeId, ulInstanceId);
			}
        }

	}

		// ��������ѡ����¼���Ӧ����
	void SceneListenerManager::_fireSelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onDelMonster(ulTypeId, ulInstanceId);
			}
        }

	}

	//
	// ������༭�� ֪ͨ�ص�������
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �¼�����༭�� ֪ͨ�ص�������
	//



	// �������е�����¼���Ӧ������
	void SceneListenerManager::_fireAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onAddNewRect(ulTypeId, ulInstanceId);
			}
        }
	}

	// �������е�ɾ���¼���Ӧ������
	void SceneListenerManager::_fireDelRect( unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onDelRect(ulTypeId, ulInstanceId);
			}
        }
	}

	// ��������ѡ���¼���Ӧ����
	void SceneListenerManager::_fireSelRect( unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onSelRect(ulTypeId, ulInstanceId);
			}
        }
	}


	//
	// �¼�����༭�� ֪ͨ�ص�������
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ̯λ�༭�� ֪ͨ�ص�������
	//



	// �������е�����¼���Ӧ�������鿴̯λ��Ϣ
	void SceneListenerManager::_fireViewStallInfo(
													unsigned char	m_bCanStall,	//�Ƿ���԰�̯
													unsigned char	m_iTradeTax,	//����˰
													unsigned long	m_dwPosTax,		//̯λ��
													unsigned char	m_iExtraInfo,	//������Ϣ 
													SceneListener*  exclude
													)
	{
		 for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
                listener->onViewStallInfo(m_bCanStall, m_iTradeTax, m_dwPosTax, m_iExtraInfo);
			}
        }
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
		void SceneListenerManager::_fireViewSoundInfo(
														unsigned long	ulInstance,	//����ʵ��id
														unsigned long	ulSoundType,//����
														unsigned long	dwRadius,	//��Ч�뾶 
														SceneListener*  exclude
														)
		{
			for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
			{
				SceneListener* listener = *it;
				if (listener != exclude)
				{
					listener->onViewCurSoundItem(ulInstance, ulSoundType, dwRadius);
				}
			}
		}
		
		//
		// �����༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////









    void SceneListenerManager::_fireTerrainHeightChanged(const std::vector<TerrainInfo>& terrainInfo,SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onTerrainHeightChanged(terrainInfo);
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
