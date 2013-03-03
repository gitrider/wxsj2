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
		// 添加删除， 修改物体的回调函数
		//
        virtual void onSceneReset(void);

        virtual void onAddObject(const ObjectPtr& object);
        virtual void onRemoveObject(const ObjectPtr& object);

        virtual void onRenameObject(const ObjectPtr& object, const String& oldName);

        virtual void onSelectObject(const ObjectPtr& object);
        virtual void onDeselectObject(const ObjectPtr& object);
        virtual void onDeselectAllObjects(void);
        virtual void onObjectPropertyChanged(const ObjectPtr& object, const String& name);

        virtual void onCameraPosChanged();

        virtual void onUIChanged(void* data, UIChangeType type);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 不可行走区域
		//

		// 编辑区域， 添加一个新的点的回调函数。
		virtual void onRegionAddNewPoint(unsigned long ulRegionId,unsigned long ulPointId, Ogre::Vector3 position);

		// 删除一个点的回调函数
		virtual void onRegionDelPoint(unsigned long ulRegionId,unsigned long ulPointId);


		// 删除一个点的回调函数
		virtual void onRegionSel(unsigned long ulRegionId);

		//
		// 不可行走区域
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 寻路编译
		//

		virtual void onAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position);

		virtual void onDelDijkPoint(unsigned long ulPointId);

		virtual void onSelDijkPoint(unsigned long ulPointId);

		//
		// 寻路编译
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 怪物编辑， 通知回调函数。
		//

		// 添加一个怪物
		virtual void onAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId);

		// 删除一个怪物
		virtual void onDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId);

		// 选择一个怪物
		virtual void onSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId);
	
		//
		// 怪物编辑， 通知回调函数。
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 生长点编辑编辑， 通知回调函数。
		//

		// 添加一个怪物
		virtual void onAddNewGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId);

		// 删除一个怪物
		virtual void onDelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId);

		// 选择一个怪物
		virtual void onSelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId);
	
		//
		// 怪物编辑， 通知回调函数。
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 事件区域编辑编辑， 通知回调函数。
		//

		virtual void onAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId);
	
		// 删除一个怪物
		virtual void onDelRect( unsigned long ulTypeId, unsigned long ulInstanceId);
		
		// 选择一个怪物
		virtual void onSelRect( unsigned long ulTypeId, unsigned long ulInstanceId);
	

		
	
		//
		// 事件区域编辑， 通知回调函数。
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 摊位编辑， 通知回调函数。
		//



		// 调用所有的添加事件响应函数。查看摊位信息
		virtual void onViewStallInfo(
								unsigned char	bCanStall,	//是否可以摆摊
								unsigned char	iTradeTax,	//交易税
								unsigned long	dwPosTax,		//摊位费
								unsigned char	iExtraInfo	//附加信息 
								);
		
		//
		// 摊位编辑， 通知回调函数。
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 声音编辑， 通知回调函数。
		//


		// 调用所有的添加事件响应函数。查看声音信息
		virtual void onViewCurSoundItem(
										unsigned long	ulInstance,	//声音实例id
										unsigned long	ulSoundType,//类型
										unsigned long	dwRadius	//有效半径 
										);
		
		//
		// 声音编辑， 通知回调函数。
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
        void _fireCameraPosChanged(SceneListener* exclude =0);

		///////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        void _fireUIChanged(void* data, UIChangeType type, SceneListener* exclude = NULL);



	
	



    };
}

#endif
