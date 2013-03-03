#ifndef __EVENTAREAEDITACTION_H__
#define __EVENTAREAEDITACTION_H__
#pragma once


#include "Action.h"
#include "HitInfo.h"
#include "ScanLine.h"
#include <OgreMaterial.h>
#include <map>
#include "LogicDataStructDef.h"
#include "CustomShape.h"
#include "eventareainfo.h"

namespace Fairy 
{
	#define		FUNCTION_KEY        0X00000000
	#define		FUNC_KEY_SHIFT		0X00000001
	#define		FUNC_KEY_CTRL		0X00000002
	#define		FUNC_KEY_ALT		0X00000004

	typedef struct  _EVENT_RECT_INFO
	{
		int			m_iEventAreaId;
		int			m_iScriptId;
		AREARECT	m_rect;
	}EVENT_RECT_INFO, *PEVENT_RECT_INFO;

	
	typedef enum _AREA_EDIT_OPERATRO_TYPE
	{

		SELECT,
		DRAW,

	}AREA_EDIT_OPERATRO_TYPE;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �¼�������Ϣ
	//
	//
	class CEventAreaRectInfo
	{

	public:

		CEventAreaRectInfo();
		~CEventAreaRectInfo();

	public:

		int			m_iEventAreaId;
		int			m_iScriptId;
		AREARECT	m_rect;

	public:

		Ogre::SceneNode*	m_pParentSceneNode;		// ������ʾ����༭���ܵĽڵ�.
		Ogre::SceneNode*	m_pSceneNode;			// ��ĳ����ڵ�.
		
		CCustomShape*		m_RectRenderShape;		// ��ʾ��Ϣ.
		SceneManipulator*   m_pSceneManipulator;	// ������ogre�����ľ����

		
	public:
		
		/////////////////////////////////////////////////////////////////////////////////
		//
		// ����������
		//

		// ���ø������ڵ�.
		void SetParentSceneNode(Ogre::SceneNode* pSceneNode);

		// ����SceneManipulator���
		void SetSceneManipulator(SceneManipulator*  SceneManipulator);

		// ���������ڵ�ģ��.
		unsigned long CreateSceneNode();

		// ������ʾ�ľ���
		//void BuildEventAreaRect();


	};






	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �¼�����action
	// 
	//

	typedef std::map<int, int>					EVENT_AREA_ID_RECT_ID_MAP;
	typedef std::map<int, EVENT_RECT_INFO>		ID_RECT_MAP;
	typedef std::map<int, CEventAreaInfo*>		SHOW_RECT_MAP;

	// �¼�����༭
	class CEventAreaEditAction : public Action
	{
	public:
		 CEventAreaEditAction(SceneManipulator* sceneManipulator);
		~CEventAreaEditAction(void);


	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �������������
		//

		// ��¼���̵İ���״̬.
		unsigned long     m_FuncKeyState;

		// �Ƿ���ʾ����.
		bool  m_bIsShow;

			
		// ���ð��µĹ��ܼ�
		void SetFuncKey(unsigned long ulFuncKey);

		//  ��չ��ܼ�.
		void ClearFuncKey();


	public:
		////////////////////////////////////////////////////////////////////////////////////
		//
		// ���̺Ͷ��̲���
		//

		// �������ȡ�ļ�
		unsigned long ReadEventAreaFromFile(std::string strFileName);
		
		// ������洢���ļ�
		unsigned long SaveEventAreaToFile(std::string strFileName);
		
			
	public:

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// action ������
		//
		
		// �õ����action������.
		const String& getName(void) const;

		// ���ò�������.
		void setParameter(const String& name, const String& value);

	public:

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ��Ӧ��Ӧ��������.
		//

		void OnMouseMove(const Point& pt);

		void OnMouseLBnDown(const Point& pt);

		void OnMouseLBnUp(const Point& pt);

		void OnMouseRBnDown(const Point& pt);

		void OnMouseRBnUp(const Point& pt);

		void OnMouseDrag(const Point& pt);


	public:
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//
		// ����Ⱦ����صĲ���.
		//
		
		Ogre::SceneNode*	m_pSceneNode;			// ������ʾ����༭���ܵĽڵ�.

		CCustomShape*		m_pLineShape;  

		// ���ָʾС��.	
		HitInfo mTerrainHitInfo;

		// ��������༭�ĳ����ڵ�
		void CreateSceneNode();

		// ��������༭�����ڵ�
		void HideScene(bool bHide);

		// ������ʾ������
		void CreateShowEventArea();

		// ���ݾ��δ�����ʾ���¼�����
		void BuildEventAreaRect();

	public:

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �߼����ݲ���
		//


		// ��ǰ���ڱ༭���¼�����id
		int m_iCurEditEventAreaId;

		// ��ǰ���ڱ༭�Ľű�id
		int m_iCurEditScriptid;

		// ��ǰ�༭�����id
		int m_iMaxEventAreaId;

		// ��ǰ�������������.
		CEventAreaInfo*  m_pCurOperatorRect;

		// ��ǰ���ڱ༭�ľ��ε��id
		int  m_iEditRectId;

		// ��ǰ�༭��id
		int m_iCurEditInstanceId;

		// ��ǰ�Ĳ���״̬.
		AREA_EDIT_OPERATRO_TYPE m_iOperator;

		// ������갴�µĵ�
		Ogre::Vector3	m_PrePoint;
		Ogre::Vector3   m_CurPoint;

		// ��ǰ���ɵľ���.
		AREARECT m_AreaRect;

		// �ѵ�ǰ�ĵ�ת���ɹ�����.
		void NormalRect();

		// �¼������б�
		EVENT_AREA_ID_RECT_ID_MAP m_EventIdMap;

		// ���������б�.
		ID_RECT_MAP				  m_RectMap;

		// ��ʾ�����б�.
		SHOW_RECT_MAP			  m_RectShowMap;

		// �����Ϣ�б�
		void ReleaseRectInfoMap();

		// ��ʼ���¼�����༭
		void InitEventArea();

		// �õ������ϵ������
		bool  GetTerrainPos(const Point& pt, Ogre::Vector3& position);

		// �������ε���갴�²���
		void DoDrawLBnDown(const Point& pt);

		// �������ε������ק����
		void DoDrawLBnDrag(const Point& pt);

		// �������ε����̧�����
		void DoDrawLBnUp(const Point& pt);

		// ��ѡ����ε����̧�����
		void DoSelectLBnUp(const Point& pt);

		// ��ʼ���߼�����
		void InitLogicData();

		// ���ݾ���idɾ��һ������
		void DeleteRect(int iId);

		// ����redo undo ������ɾ��
		void DeleteRectNoOperator(int iId);

		// ���һ������.
		int AddRect(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos, int iId = -1);

		// ѡ��һ������
		void SelectEventAreaRect(int iId);

		// ��������
		void ModifyEventArea(int iInstanceId, int iEventId, int iScripId);
	

		

    protected:
        
		// ���������麯��.
        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

		// ��ǰ����active����.
        void _onActive(bool active);


	};


}
#endif //__EVENTAREAEDITACTION_H__