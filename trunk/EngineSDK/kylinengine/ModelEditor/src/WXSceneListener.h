#ifndef __FAIRYSCENELISTENER_H__
#define __FAIRYSCENELISTENER_H__

#include "Core/FairyPrerequisites.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <OgreRoot.h>			



namespace Fairy
{
    class Object;

    typedef boost::shared_ptr<Object> ObjectPtr;
    typedef boost::weak_ptr<Object> ObjectWeakPtr;

    struct TerrainInfo
    {
        int x, z;
        Real oldHeight;
        Real nowHeight;
    };

    enum UIChangeType
    {
        UIC_SOUNDEDIT,
        UIC_DATUPOINTEDIT,
    };

    class SceneListener
    {
    public:
        SceneListener(void) {}
        virtual ~SceneListener() {}

        //////////////////////////////////////////////////////////////////////////
		//
		// ���ɾ���� �޸�����Ļص�����
		//
        virtual void onSceneReset(void);

        virtual void onAddObject(const ObjectPtr& object);
        virtual void onRemoveObject(const ObjectPtr& object);

        virtual void onRenameObject(const ObjectPtr& object, const String& oldName);

        virtual void onSelectObject(const ObjectPtr& object);
        virtual void onDeselectObject(const ObjectPtr& object);
        virtual void onDeselectAllObjects(void);

        virtual void onObjectPropertyChanged(const ObjectPtr& object, const String& name);

        virtual void onTerrainHeightChanged(const std::vector<TerrainInfo>& terrainInfo);
        virtual void onCameraPosChanged();

        virtual void onUIChanged(void* data, UIChangeType type);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ������������
		//

		// �༭���� ���һ���µĵ�Ļص�������
		virtual void onRegionAddNewPoint(unsigned long ulRegionId,unsigned long ulPointId, Ogre::Vector3 position);

		// ɾ��һ����Ļص�����
		virtual void onRegionDelPoint(unsigned long ulRegionId,unsigned long ulPointId);


		// ɾ��һ����Ļص�����
		virtual void onRegionSel(unsigned long ulRegionId);

		//
		// ������������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Ѱ·����
		//

		virtual void onAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position);

		virtual void onDelDijkPoint(unsigned long ulPointId);

		virtual void onSelDijkPoint(unsigned long ulPointId);

		//
		// Ѱ·����
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ����༭�� ֪ͨ�ص�������
		//

		// ���һ������
		virtual void onAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId);

		// ɾ��һ������
		virtual void onDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId);

		// ѡ��һ������
		virtual void onSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId);
	
		//
		// ����༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ������༭�༭�� ֪ͨ�ص�������
		//

		// ���һ������
		virtual void onAddNewGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId);

		// ɾ��һ������
		virtual void onDelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId);

		// ѡ��һ������
		virtual void onSelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId);
	
		//
		// ����༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �¼�����༭�༭�� ֪ͨ�ص�������
		//

		virtual void onAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId);
	
		// ɾ��һ������
		virtual void onDelRect( unsigned long ulTypeId, unsigned long ulInstanceId);
		
		// ѡ��һ������
		virtual void onSelRect( unsigned long ulTypeId, unsigned long ulInstanceId);
	

		
	
		//
		// �¼�����༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ̯λ�༭�� ֪ͨ�ص�������
		//



		// �������е�����¼���Ӧ�������鿴̯λ��Ϣ
		virtual void onViewStallInfo(
								unsigned char	bCanStall,	//�Ƿ���԰�̯
								unsigned char	iTradeTax,	//����˰
								unsigned long	dwPosTax,		//̯λ��
								unsigned char	iExtraInfo	//������Ϣ 
								);
		
		//
		// ̯λ�༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �����༭�� ֪ͨ�ص�������
		//


		// �������е�����¼���Ӧ�������鿴������Ϣ
		virtual void onViewCurSoundItem(
										unsigned long	ulInstance,	//����ʵ��id
										unsigned long	ulSoundType,//����
										unsigned long	dwRadius	//��Ч�뾶 
										);
		
		//
		// �����༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




		
	};

    class SceneListenerManager
    {
    protected:
        typedef std::list<SceneListener*> Listeners;
        Listeners mListeners;

    public:
        SceneListenerManager(void);
        ~SceneListenerManager();

        void addSceneListener(SceneListener* listener);
        void removeSceneListener(SceneListener* listener);

        //////////////////////////////////////////////////////////////////////////

        void _fireSceneReset(SceneListener* exclude = 0);

        void _fireAddObject(const ObjectPtr& object, SceneListener* exclude = 0);
        void _fireRemoveObject(const ObjectPtr& object, SceneListener* exclude = 0);

        void _fireRenameObject(const ObjectPtr& object, const String& oldName, SceneListener* exclude = 0);

        void _fireSelectObject(const ObjectPtr& object, SceneListener* exclude = 0);
        void _fireDeselectObject(const ObjectPtr& object, SceneListener* exclude = 0);
        void _fireDeselectAllObjects(SceneListener* exclude = 0);

        void _fireObjectPropertyChanged(const ObjectPtr& object, const String& name, SceneListener* exclude = 0);

        void _fireTerrainHeightChanged(const std::vector<TerrainInfo>& terrainInfo,SceneListener* exclude =0);
        void _fireCameraPosChanged(SceneListener* exclude =0);

		///////////////////////////////////////////////////////
		//
		// ��������༭��ӵ�Ļص�����
		//

		// ���һ����
		void _fireRegionAddNewPoint(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position, SceneListener* exclude = 0);

		// ɾ��һ����
		void _fireRegionDelPoint(unsigned long ulRegionId, unsigned long ulPointId, SceneListener* exclude = 0);

		// ѡ��һ������
		void _fireRegionSel(unsigned long ulRegionId, SceneListener* exclude = 0);


		//
		// ��������༭��ӵ�Ļص�����
		//
		///////////////////////////////////////////////////////


		///////////////////////////////////////////////////////
		//
		// ����Ѱ·�༭�ص�����
		//

		// ���һ����
		void _fireAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position, SceneListener* exclude = 0);

		// ɾ��һ����
		void _fireDelDijkPoint(unsigned long ulPointId, SceneListener* exclude = 0);

		// ѡ��һ������
		void _fireSelDijkRegion(unsigned long ulPointId, SceneListener* exclude = 0);


		//
		// ����Ѱ·�༭�ص�����
		//
		///////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ����༭�� ֪ͨ�ص�������
		//

		// �������е���ӹ�����¼���Ӧ������
		void _fireAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude = 0);

		// �������е�ɾ��������¼���Ӧ������
		void _fireDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude = 0);

		// ��������ѡ�������¼���Ӧ����
		void _fireSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude = 0);



		//
		// ����༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ������༭�� ֪ͨ�ص�������
		//


		// �������е���ӹ�����¼���Ӧ������
		void _fireAddNewGrowPoint(unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude = 0);

		// �������е�ɾ��������¼���Ӧ������
		void _fireDelGrowPoint( unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude = 0);

		// ��������ѡ�������¼���Ӧ����
		void _fireSelGrowPoint( unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude = 0);


		//
		// ������༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �¼�����༭�� ֪ͨ�ص�������
		//


		// �������е���ӹ�����¼���Ӧ������
		void _fireAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude = 0);

		// �������е�ɾ��������¼���Ӧ������
		void _fireDelRect( unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude = 0);

		// ��������ѡ�������¼���Ӧ����
		void _fireSelRect( unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude = 0);


		//
		// �¼�����༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ̯λ�༭�� ֪ͨ�ص�������
		//



		// �������е�����¼���Ӧ�������鿴̯λ��Ϣ
		void _fireViewStallInfo(
								unsigned char	m_bCanStall,	//�Ƿ���԰�̯
								unsigned char	m_iTradeTax,	//����˰
								unsigned long	m_dwPosTax,		//̯λ��
								unsigned char	m_iExtraInfo,	//������Ϣ 
								SceneListener*  exclude = 0
								);
		
		//
		// ̯λ�༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �����༭�� ֪ͨ�ص�������
		//


		// �������е�����¼���Ӧ�������鿴������Ϣ
		void _fireViewSoundInfo(
								unsigned long	ulInstance,	//����ʵ��id
								unsigned long	ulSoundType,//����
								unsigned long	dwRadius,	//��Ч�뾶 
								SceneListener*  exclude = 0
								);
		
		//
		// �����༭�� ֪ͨ�ص�������
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        void _fireUIChanged(void* data, UIChangeType type, SceneListener* exclude = NULL);



	
	



    };
}

#endif
