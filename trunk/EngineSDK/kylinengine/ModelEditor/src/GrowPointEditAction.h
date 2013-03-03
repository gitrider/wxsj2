#pragma once

#include "Action.h"
#include "HitInfo.h"
#include <OgreMaterial.h>
#include <map>
#include <vector>
#include "LogicDataStructDef.h"

namespace Fairy 
{

	class LogicModel;

	#define		FUNCTION_KEY        0X00000000
	#define		FUNC_KEY_SHIFT		0X00000001
	#define		FUNC_KEY_CTRL		0X00000002
	#define		FUNC_KEY_ALT		0X00000004
	
	class CGrowPointInfo
	{
	public:
		CGrowPointInfo();
		~CGrowPointInfo();

	public:

		bool			m_bIsRecreateMaterial;	// �Ƿ����´�������
		unsigned long	m_dwInstanceId;			// ÿһ��ʵ����id
		unsigned long	m_dwTypeId;				// ÿһ��ʵ����id
	

		// ���ڵ�scene Node
		Ogre::SceneNode*	m_pParentSceneNode;	

		// ������ʾ����༭���ܵĽڵ�.
		Ogre::SceneNode*	m_pSceneNode;	
		Ogre::Entity*		m_pEntity;
		Ogre::Vector3		m_position;			// �����λ�á�
		Fairy::LogicModel*	m_pModel;

	public:

		// ����һ�������㡣
		bool CreateGrowPoint(const std::string& strMeshName);

		// ����λ��
		void SetGrowPointPos(Ogre::Vector3 pos);

		// ���ø��ڵ�
		void SetParentSceneNode(Ogre::SceneNode* pParentSceneNode);

		// �ͷ����������Դ
		void ReleaseGrowPoint();

		// �Ƿ�ѡ��.
		bool IsSelected(Ogre::Ray& ray, float& fdis);



		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	���ʲ�����
		//

		// object ѡ��ʱʹ�õĲ��ʡ�
		Ogre::MaterialPtr m_pSelectedMaterial;

		// object ԭ���Ĳ���.
		Ogre::MaterialPtr m_pOldMaterial;

		// ѡ��
		void SetSelMaterial();

		// δѡ��.
		void SetUnSelMaterial();

	

	};


	typedef std::multimap<unsigned long, CGrowPointInfo>	SORT_GROWPOINT_MAP;
	typedef std::map<unsigned long, CGrowPointInfo>			GROWPOINT_MAP;
	typedef std::map<unsigned long, std::string>			ID_MESH_MAP;
	typedef std::pair<unsigned long, CGrowPointInfo >		GROWPOINT_PAIR;


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	action
	//
	class CGrowPointEditAction: public Action
	{
	public:
		CGrowPointEditAction(SceneManipulator* sceneManipulator);
		~CGrowPointEditAction(void);

	public:

		///////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ��������.
		//
		//

		// �Ƿ���ʾ��ʱѡ�е�ģ�͡�
		bool  m_bIsShowTempObject;


		// ������ʾ����༭���ܵĽڵ�.
		CGrowPointInfo m_tempGrowPoint;


		// ��Ļ�������
		Point m_screenPt;

		// �Ƿ���ʾ����.
		bool  m_bIsShow;

		// ��ǰ������
		Ogre::Vector3	m_curPoint;

		// ������ʾ����༭���ܵĽڵ�.
		Ogre::SceneNode*	m_pSceneNode;	

		// ��������༭�ĳ����ڵ�
		void CreateSceneNode();

		// ��������༭�����ڵ�
		void HideScene(bool bHide);

		// ����ӵ�������ı�š�
		unsigned long m_dwGrowPointInstanceId;

		// ��ǰѡ�е�������
		unsigned long m_dwCurSelGrowPointInstanceId;

		// ��ǰ�༭�������������id
		unsigned long m_dwGrowPointTypeId;
		
		// ��������Ϣ.
		GROWPOINT_MAP	m_GrowPointMap;

		// ���������ͺ�ģ��ӳ���ļ���
		ID_MESH_MAP		m_typeMeshMap;

		// ������͡�
		void AddGrowPointMeshType(unsigned long dwId, std::string strName);

		// �ı����͡�
		void ChangeGrowPointMeshType(unsigned long dwId, std::string strName);

		// ���ü�����
		void ResetCounter();


		// ��������㡣
		// ����ֵ0xffffffff ��ʾ����ʧ��
		unsigned long AddNewGrowPoint(unsigned long dwGrowPointTypeId, Ogre::Vector3 pos);


		// ��������㡣
		// ����ֵ0xffffffff ��ʾ����ʧ��
		unsigned long AddNewGrowPoint(const std::string& strMeshName, Ogre::Vector3 pos);

		// �����������λ��
		void SetGrowPointPos(unsigned long dwGrowPointInstanceId, Ogre::Vector3 pos);

		// �õ��������λ��
		Ogre::Vector3 GetGrowPos(unsigned long dwGrowPointInstanceId);

		// �õ��������߼�λ��
		Ogre::Vector3 GetGrowLogicPos(unsigned long dwGrowPointInstanceId);

		// ��һ��������.
		void ViewGrowPos(unsigned long dwGrowPointInstanceId);

		// �Ƿ�ѡ��������.
		bool IsSelectObject(Ogre::Ray& ray, CGrowPointInfo& GrowPointInfo, float& fdis);

		// �Ƿ�ѡ��������.
		bool IsSelectObject(Ogre::Ray& ray, unsigned long dwGrowPointInstanceId, float& fdis);

		// �õ�ѡ�е�������
		bool GetSelectedObject(unsigned long& dwGrowPointInstanceId);

		// �Ƿ�ѡ��������.
		void SetSelectObjectLook(unsigned long dwGrowPointInstanceId);

		// �Ƿ�ѡ��������.
		void SetUnSelectObjectLook(unsigned long dwGrowPointInstanceId);

		// ɾ�������㡣
		void DelGrowPoint(unsigned long dwGrowPointInstanceId);

		// �������������
		void ClearGrowPointData();
	
		/////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ���ܼ����ݲ�����
		//
		//
	public:

		// ��¼���̵İ���״̬.
		unsigned long     m_FuncKeyState;
		
		// ���ò�������.
		void setParameter(const String& name, const String& value);

		// ���ð��µĹ��ܼ�
		void SetFuncKey(unsigned long ulFuncKey);

		//  ��չ��ܼ�.
		void ClearFuncKey();


		//
		//
		// ���ܼ����ݲ�����
		//
		///////////////////////////////////////////////////////////////////////////////////////////////




		

		////////////////////////////////////////////////////////////////////////////////////
		//
		// ���̺Ͷ��̲���
		//

		// �������ȡ�ļ�
		unsigned long ReadGrowPointFromFile(std::string strFileName);
		
		// ������洢���ļ�
		unsigned long SaveGrowPointToFile(std::string strFileName);

			
		//
		// ���̺Ͷ��̲���
		//
		////////////////////////////////////////////////////////////////////////////////////


			
	protected:
  
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �ⲿ���õ��麯����
		//

		// ���������麯��.
		void _onMove(const Point& pt);
		void _onBegin(const Point& pt);
		void _onDrag(const Point& pt);
		void _onEnd(const Point& pt, bool canceled);

		// ��ǰ����active����.
		void _onActive(bool active);

		//
		// �ⲿ���õ��麯����
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////


			
	public:

		// �õ�������������.
		const String& getName(void) const;

	};

}
