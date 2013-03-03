#ifndef __STALLINFOACTION_H__
#define __STALLINFOACTION_H__
#pragma once


#include "Action.h"
#include "HitInfo.h"
#include "ScanLine.h"
#include <OgreMaterial.h>
#include <map>
#include ".\LogicDataStructDef.h"
#include "CustomShape.h"
#include ".\baserendershape.h"

namespace Fairy 
{
	class FairyEditorFrame;
	class CStallInfoData
	{
	public:

		CStallInfoData();
		~CStallInfoData();

	public:

		unsigned char	m_bCanStall;	//�Ƿ���԰�̯
		unsigned char	m_iTradeTax;	//����˰
		unsigned long	m_dwPosTax;		//̯λ��
		unsigned char	m_iExtraInfo;	//������Ϣ

		int     m_iPosX;				// ̯λλ��x
		int     m_iPosY;				// ̯λλ��y

	public:
		
		CBaseRenderShape	m_RectRenderShape;		// ��ʾ��Ϣ.
		
		// ������ʾͼ��.
		void CreateShape();		

		// ��ֵ������غ���
		CStallInfoData& operator=(const CStallInfoData& stallInfo);

	};




	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// action
	//
	class CStallInfoAction : public Action
	{


	public:

		enum OPERATE_TYPE
		{
			SET_STALL_INFO = 0,
			DEL_STALL_INFO,
			VIEW_STALL_INFO,
		};

		int m_iOperateType;

	public:
		CStallInfoAction(SceneManipulator* sceneManipulator);
		~CStallInfoAction(void);


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

		// �汾��
		unsigned long m_dwVersion;

		// �������ȡ�ļ�
		unsigned long ReadStallInfoFromFile(std::string strFileName);
		
		// ������洢���ļ�
		unsigned long SaveStallInfoToFile(std::string strFileName);

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

		// ��������༭�ĳ����ڵ�
		void CreateSceneNode();

		// ��������༭�����ڵ�
		void HideScene(bool bHide);

		// ̯λ��С
		unsigned long	m_dwStallWidth;
		unsigned long 	m_dwStallHeight;
		//��̯λ��ͼ��С----��ͼ����ǰ

		unsigned long	m_dwOldStallHeight;
		unsigned long	m_dwOldStallWidth ;
		Ogre::Vector3   m_vOldFirstGrid;

		// ̯λ��Ϣ.
		CStallInfoData*	m_pStallInfoData;

		// ��ǰ̯λ��ˢ.
		CStallInfoData  m_curStallInfoBrush;

		// ������갴�µĵ�
		Ogre::Vector3	m_PrePoint;
		Ogre::Vector3   m_CurPoint;

		// ��ǰ���ɵľ���.
		AREARECT m_AreaRect;

		// ��ʾģ��
		CBaseRenderShape m_RenderShape;

		// �ѵ�ǰ�ĵ�ת���ɹ�����.
		void NormalRect();

		// �����ķ���
		void CreateShowRect();


		// ����̯λ��С
		void SetStallSize(unsigned long dwWidth, unsigned long dwHeight);

		// ����̯λ
		void CreateStallInfo();

		//�ؽ�̯λ
		void ReCreateStallInfo();

		// ����һ����ʾ����
		void CreateOneStallInfo(int ix, int iy, CStallInfoData& stallInfo);
			


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ����̯λ��Ϣ.
		//

		// �������ε���갴�²���
		void DoDrawLBnDown(const Point& pt);

		// �������ε������ק����
		void DoDrawLBnDrag(const Point& pt);

		// �������ε����̧�����
		void DoDrawLBnUp(const Point& pt);


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ɾ��̯λ��Ϣ.
		//

		// ��ɾ�����ε���갴�²���
		void DoDelLBnDown(const Point& pt);

		// ��ɾ�����ε������ק����
		void DoDelLBnDrag(const Point& pt);

		// ��ɾ�����ε����̧�����
		void DoDelLBnUp(const Point& pt);

		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �鿴̯λ��Ϣ.
		//

		// �鿴̯λ��Ϣ
		void DoViewMouseMove(const Point& pt);


		// �õ������ϵ������
		bool  GetTerrainPos(const Point& pt, Ogre::Vector3& position);

		
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

#endif //__STALLINFOACTION_H__