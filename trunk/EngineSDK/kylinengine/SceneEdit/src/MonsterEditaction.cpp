#include "MonsterEditAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "CustomShape.h"
#include "WXOperatorManager.h"       // redo undo ������.

#include "OgreTimer.h"
#include "TriFunc.h"
#include "MonsterOperator.h"
#include "Core/TerrainData.h"
#include "Core/FairyResources.h"
#define MODELSYSTEM_STATIC_LINK
#include "Windows.h"
#include "WXSJ_DBC.h"
#include ".\editdobject_nt.h"
#include "ModelSystem\SkeletonMeshUtil.h"
#include "ModelSystem\SkeletonMesh.h"
#include <OgreCamera.h>
#include <windows.h>


#define _2PI		6.28
#define DIR_STEP	36
namespace Fairy 
{
	using namespace Ogre;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CMonsterInfo
//
//
	CMonsterInfo::CMonsterInfo()
		: m_pDobject(NULL)
	{
		
		//m_pDobject	   = NULL;
		m_iRefreshTime = REFRESH_TIME;
		m_ActiveType   = AREA;
		m_fAreaRadius  = AREA_RADIUS;

		m_dwObjectId		= 0xffffffff;			
       	m_iBaseAIId			= -1;					
		m_strAdvanceAIFile	= "";			
		m_strEvent			= "";					
		m_dwMonsterGUID		= 0xffffffff;		
		m_iGroupId			= -1;					
		m_iTeamId			= -1;					
		m_fMonstDir			= 0;
		m_iReputationID		= -1;
		m_iLevel			= -1;

		for(int i = 0; i < SHOP_COUNT; i++)
		{
			m_iShop[i] = -1;
		}

		

	}

	CMonsterInfo::~CMonsterInfo()
	{
		
	}

	void CMonsterInfo::DeleteDObject()
	{
		delete m_pDobject;
		m_pDobject = NULL;

		for(int i = 0; i < SHOP_COUNT; i++)
		{
			m_iShop[i] = -1;
		}

	}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// CMonsterEditaction
//

	CMonsterEditaction::CMonsterEditaction(SceneManipulator* sceneManipulator)
		  : Action(sceneManipulator)
		  , m_tempObject(NULL)
		  , m_dwCurSelMonsterInstanceId(0xffffffff)
		  , m_bIsAddOperator(false)
		  , m_pSceneNode(NULL)
		  , m_bMonsterMoving(false)
	{
		InitEditData();
		CreateSceneNode();
		ClearCounter();
		m_ulAddtime = getRoot()->getTimer()->getMilliseconds();
	}

	CMonsterEditaction::~CMonsterEditaction(void)
	{
		
		
		if(m_tempObject)
		{
			delete m_tempObject;
			m_tempObject = NULL;
		}
		
		ReleaseAllMonster();
	}


	// ��������༭�ĳ����ڵ�
	void CMonsterEditaction::CreateSceneNode()
	{
		m_pSceneNode = getSceneManipulator()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(false);

		}
		
	}



	// ����ƶ��Ĳ���
    void CMonsterEditaction::_onMove(const Point& pt)
	{
		// ����ѡ�е���ʱģ�͵Ĳ���.
		Ogre::Vector3 position;
	    bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);
		if(hit)
		{
			m_curPoint = position;
			if(m_tempObject)
			{
				if(m_tempObject->getSceneNode())
				{
					m_tempObject->getSceneNode()->setPosition(position);
				}
			}
		}

		// ��չ��ܼ�
		ClearFuncKey();
	
	}



	// ��갴�µĲ���.
	void CMonsterEditaction::_onBegin(const Point& pt)
	{
		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
			if(m_dwCurSelMonsterInstanceId != 0xffffffff)
			{
				m_bMonsterMoving = true;
				RecordPreDoMonsterPos();
			}
			
		}
	}



    void CMonsterEditaction::_onDrag(const Point& pt)
	{
		if(m_bMonsterMoving)
		{
			ChangeCurEditMonsterPos(pt);
		}
		else//
		{
			
		}

		// ÿ��ִ�н�����չ��ܼ�
		ClearFuncKey();
			
	}



	// ���̧��Ĳ���
    void CMonsterEditaction::_onEnd(const Point& pt, bool canceled)
	{
		
		if(!getActive())
		{
			return;
		}
		if(m_bMonsterMoving)
		{
			MoveMonster(m_curMonsterPos,m_dwCurSelMonsterInstanceId);
			m_bMonsterMoving = false;
		}

		else if(m_bIsAddOperator)
		{
			
			// ����ÿ����ֻ�����һ����
			if(m_ulAddtime + 1000 < getRoot()->getTimer()->getMilliseconds())
			{
				DoAddNewMonster(pt);
				m_ulAddtime = getRoot()->getTimer()->getMilliseconds();

			}
		}
		else
		{
			DoSelectObject(pt);
		}
	
		// ��չ��ܼ�
		ClearFuncKey();

	}

	// ��¼��ǰ����ǰһ�ε��λ��.
	void CMonsterEditaction::RecordPreDoMonsterPos()
	{
		if(m_dwCurSelMonsterInstanceId != 0xffffffff)
		{
			CMonsterInfo* monsterInfo = GetMonster(m_dwCurSelMonsterInstanceId);
			m_preDoMonsterPos = monsterInfo->m_position;
			m_curMonsterPos = monsterInfo->m_position;
		}

	}

	void CMonsterEditaction::ChangeCurEditMonsterPos(const Point& pt)
	{
		if(m_dwCurEditMonsterId == 0xffffffff)
			return;

		Ogre::Vector3 position;
		bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);

		if(hit)
		{
			CMonsterInfo* monsterInfo = GetMonster(m_dwCurSelMonsterInstanceId);
			CEditDobject_NT* pObject = monsterInfo->m_pDobject;
			Ogre::SceneNode* pNode = pObject->getSceneNode();
			if(pNode)
			 pNode->setPosition(position);
			m_curMonsterPos = position;
			
		}
		
	}


    void CMonsterEditaction::_onActive(bool active)
	{
		
		if (!active)
        {
          	
        }
		else
		{
	
		}
		
		//HideScene(active);
	}



	const String& CMonsterEditaction::getName(void) const
	{
		//����һ���ַ������֣� �����жϵ�ǰ����������.
		static const String name = "MonsterEdit";
		return name;
	}



	// ���ؽڵ㡣
	void CMonsterEditaction::HideScene(bool bHide)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(bHide);
			m_bIsShow = bHide;
		}
	}



	// ����ģ�͡�
	void CMonsterEditaction::ChangeModel(String& name)
	{
		if("" == name)
		{
			return;
		}

		if(m_pSceneNode)
		{
			m_bIsAddOperator = true;

			if(!m_tempObject)
			{
				// ����һ����ʱѡ�е�ͭ�ˡ�		
				DObject_NT::SetSceneMgr(m_pSceneNode->getCreator());
				String resourceName, groupName;
				splitResourceName(name, resourceName, groupName);
				m_tempObject = new CEditDobject_NT(mSceneManipulator);
				SkeletonMeshUtil::Load(m_tempObject, groupName, resourceName, DStringList(), DStringList());
				m_tempObject->SetParentNode(m_pSceneNode);
				//m_tempObject->SetTransparence(0.8f);

				Ogre::ColourValue color(0, 0, 1.0f, 0.5f);
				m_tempObject->SetSelectLook(color);

				// ����λ��
				m_tempObject->getSceneNode()->setPosition(m_curPoint);
			
			}
			else
			{
				delete m_tempObject;
				DObject_NT::SetSceneMgr(m_pSceneNode->getCreator());
				String resourceName, groupName;
				splitResourceName(name, resourceName, groupName);
				m_tempObject = new CEditDobject_NT(mSceneManipulator);
				SkeletonMeshUtil::Load(m_tempObject, groupName, resourceName, DStringList(), DStringList());
				m_tempObject->SetParentNode(m_pSceneNode);
				//m_tempObject->SetTransparence(0.8f);

				Ogre::ColourValue color(0, 0, 1.0f, 0.5f);
				m_tempObject->SetSelectLook(color);

				// ����λ��
				m_tempObject->getSceneNode()->setPosition(m_curPoint);
			}
		}

	}



	// ����ģ�͡�
	void CMonsterEditaction::ChangeModel(const char* pName)
	{
		String strName = pName;
		ChangeModel(strName);
	}


	
	// ���һ������
	bool CMonsterEditaction::AddNewMonster(const char* pName,  Ogre::Vector3& pos, unsigned long dwObjectId)
	{
		
		String strName = pName;
		return AddNewMonster(strName, pos, dwObjectId);
	}



	// ���һ������
	bool CMonsterEditaction::AddNewMonster(String& name,  Ogre::Vector3& pos, unsigned long dwObjectId)
	{

		if("" == name)
		{
			return false;
		}

		CEditDobject_NT* pObject = NULL;
		DObject_NT::SetSceneMgr(m_pSceneNode->getCreator());
		String resourceName, groupName;
		splitResourceName(name, resourceName, groupName);
		pObject = new CEditDobject_NT(mSceneManipulator);
		if(SkeletonMeshUtil::Load(pObject, groupName, resourceName, DStringList(), DStringList()))
		{
			
			pObject->SetParentNode(m_pSceneNode);
			pObject->getSceneNode()->setPosition(pos);

			CMonsterInfo monsterInfo;
			monsterInfo.m_dwInstanceId = m_dwMonsterInstanceId;
			monsterInfo.m_dwObjectId = dwObjectId;
			monsterInfo.m_pDobject = pObject;
			monsterInfo.m_position = pos;
			monsterInfo.m_dwMonsterGUID = m_dwGUID;
			m_dwGUID++;

			m_MonsterInfoMap[m_dwMonsterInstanceId] = monsterInfo;

			POINT_2D _2dPos;

			_2dPos.iX = (int)(pos.x / 100);
			_2dPos.iY = (int)(pos.z / 100);

			if(m_MonsterPosMap.count(_2dPos))
			{
				INSTANCE_ID_LIST& list = m_MonsterPosMap[_2dPos];
				list.push_back(m_dwMonsterInstanceId);
			}
			else
			{
				INSTANCE_ID_LIST list;
				list.push_back(m_dwMonsterInstanceId);
				m_MonsterPosMap[_2dPos] = list;
			}

			getSceneManipulator()->_fireAddNewMonster(dwObjectId, m_dwMonsterInstanceId);
			m_dwMonsterInstanceId++;
			return true;
		}
		else
		{
			delete pObject;
			return false;
		}

	}



	// ��ʼ���༭������
	void CMonsterEditaction::InitEditData()
	{
		m_strCurEditMonsterName = "";
		m_dwCurEditMonsterId = 0xffffffff;
		ReleaseAllMonster();
		m_MonsterInfoMap.clear();

		// ��ȡ�����ļ�
		GetMonsterIdLookTable();
	}



	//�����µı༭ģ�͡�
	void CMonsterEditaction::SetNewEditObject(String& name, unsigned long dwObjectId)
	{
		// ��ǰ�༭�Ĺ�������ơ�
		m_strCurEditMonsterName = name;

		// ��ǰ�༭�Ĺ������ε�id
		m_dwCurEditMonsterId = dwObjectId;

		// ���ӳ���.
		m_IdNameMap[dwObjectId] = name;

		// �ı�����
		ChangeModel(name);

	}



	//�����µı༭ģ�͡�
	void CMonsterEditaction::SetNewEditObject(const char* pName, unsigned long dwObjectId)
	{
		String strName = pName;
		m_IdNameMap[dwObjectId] = strName;
		SetNewEditObject(strName, dwObjectId);
	}



	// �ͷ����еĹ��
	void CMonsterEditaction::ReleaseAllMonster()
	{
		MONSTERINFO_MAP::iterator it;
		MONSTERINFO_MAP::iterator itEnd;

		itEnd = m_MonsterInfoMap.end();

		for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
		{
			CMonsterInfo& monsterInfo = it->second;
			monsterInfo.DeleteDObject();
		}

		m_MonsterInfoMap.clear();

		ClearCounter();
	}



	// �õ�ѡ�еĹ���
	unsigned long  CMonsterEditaction::SelectObject(const Point& pt)
	{

		float fMinDis = 100000.0f;
		float ft      = 0;
		bool  bSel    = false;
		unsigned long dwId = 0xffffffff;

		if(NULL == getSceneManipulator())
		{
			return 0xffffffff;
		}

		Ogre::Vector3 position;
		bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);

		if(false == hit)
		{
			return 0xffffffff;
		}

	


		Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(pt.x, pt.y);
		int iCenterX = (int)(position.x / 100);
		int iCenterY = (int)(position.z / 100);
		
		POINT_2D _2dPos;
		INSTANCE_ID_LIST::iterator it;
		INSTANCE_ID_LIST::iterator itEnd;

		for(int i = iCenterX - 15; i < iCenterX + 15; i++)
		{
			for(int j = iCenterY - 15; j < iCenterY + 15; j++)
			{
				
				_2dPos.iX = i;
				_2dPos.iY = j;
				if(0 == m_MonsterPosMap.count(_2dPos))
				{
					continue;
				}

				INSTANCE_ID_LIST& list = m_MonsterPosMap[_2dPos];

				itEnd = list.end();
				
				for(it = list.begin(); it != itEnd; it++)
				{
					CMonsterInfo& monsterInfo = m_MonsterInfoMap[(*it)];
					if(IsSelectObject(ray, monsterInfo, ft))
					{
						bSel = true;

						if(fMinDis > ft)
						{
							fMinDis = ft;
							dwId = monsterInfo.m_dwInstanceId;
						}

					}
				}
			}
				
		}


		return dwId;
	}


	// �Ƿ�ѡ�й���
	bool CMonsterEditaction::IsSelectObject(Ogre::Ray& ray, CMonsterInfo& monsterInfo, float& fdis)
	{
		
		float fu = 0;
		float fv = 0;
		float fd = 0;
		bool  bRet = false;

		if(NULL == monsterInfo.m_pDobject)
		{
			return false;
		}
		const AxisAlignedBox& BindBox = monsterInfo.m_pDobject->getBoundingBox();
		
		Ogre::Vector3 v1; 
		Ogre::Vector3 v2; 
		Ogre::Vector3 v3;

		const Ogre::Vector3 dir  = ray.getDirection();
		const Ogre::Vector3 orig = ray.getOrigin();

		const Ogre::Vector3 vMax = BindBox.getMaximum(); 
		const Ogre::Vector3 vMin = BindBox.getMinimum() ;
		 
		float fDisTance = 100000.0f;

		////////////////////////////////////////////////////////////////////////////////////////
		// ȡmaxZ
		v1.x = vMin.x + monsterInfo.m_position.x;
		v1.y = vMin.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		// ȡmaxZ
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////
		// ȡminZ
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		// ȡminZ
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////
		// ȡMaxX
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMin.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMax.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  ȡMaxX
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMax.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		// ȡMinX
		v1.x = vMin.x + monsterInfo.m_position.x;
		v1.y = vMin.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMin.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  ȡMinX
		v1.x = vMin.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMin.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		// ȡMaxY
		v1.x = vMin.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMax.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  ȡMaxY
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMax.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}



		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		// ȡMinY
		v1.x = vMin.x + monsterInfo.m_position.x;
		v1.y = vMin.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMin.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  ȡMinY
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMin.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMin.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		fdis = fDisTance;
		return bRet;
	}

	
	void CMonsterEditaction::setParameter(const String& name, const String& value)
    {
        if (name == "FUNC_KEY")
        {
           if("SHIFT" == value)
		   {
			   SetFuncKey(FUNC_KEY_SHIFT);
		   }
		   else if("CTRL" == value)
		   {
			   SetFuncKey(FUNC_KEY_CTRL);
		   }
		   else if("ATL" == value)
		   {
			   SetFuncKey(FUNC_KEY_ALT);
		   }
		   else if("ESC" == value)
		   {
			   m_bIsAddOperator = false;
			   if(m_tempObject)
			   {

				   if(m_tempObject->getSceneNode())
				   {
					   m_tempObject->getSceneNode()->setVisible(false);
				   }
			   }
		   }
        }
		else if("DELETE_MONSTER" == name)
		{
			DelSelectMonster();
		}
    
    }

	// ���ð��µĹ��ܼ�
	void CMonsterEditaction::SetFuncKey(unsigned long ulFuncKey)
	{
		m_FuncKeyState |= ulFuncKey;
	}

	//  ��չ��ܼ�.
	void CMonsterEditaction::ClearFuncKey()
	{
		m_FuncKeyState = FUNCTION_KEY;
	}

	// ����ѡ������Ĳ���
	void CMonsterEditaction::DoSelectObject(const Point& pt)
	{
		// û��ѡ��
		unsigned long  dwId = SelectObject(pt);
		if(0xffffffff != dwId)
		{
			if( 0 == m_MonsterInfoMap.count(dwId))
			{
				return;
			}

			if(m_dwCurSelMonsterInstanceId == dwId)
			{
				return;
			}
				
			
			CMonsterInfo monsterInfo = m_MonsterInfoMap[dwId];
			Ogre::ColourValue color(0, 0, 1.0f, 0.5f);
			monsterInfo.m_pDobject->SetSelectLook(color);
			
			if(m_MonsterInfoMap.count(m_dwCurSelMonsterInstanceId))
			{
				m_MonsterInfoMap[m_dwCurSelMonsterInstanceId].m_pDobject->SetUnSelLook();
			}

			// ֪ͨ����ѡ����һ������.
			getSceneManipulator()->_fireSelMonster(monsterInfo.m_dwObjectId, dwId);
			
			m_dwCurSelMonsterInstanceId = dwId;
		}
	}

	// �����������Ĳ���
	void CMonsterEditaction::DoAddNewMonster(const Point& pt)
	{
		Ogre::Vector3 position;
		bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);

		if(hit)
		{
			if(AddNewMonster(m_strCurEditMonsterName, position, m_dwCurEditMonsterId))
			{
				if(m_MonsterInfoMap.count(m_dwCurSelMonsterInstanceId))
				{
					m_MonsterInfoMap[m_dwCurSelMonsterInstanceId].m_pDobject->SetUnSelLook();
				}

				m_dwCurSelMonsterInstanceId = m_dwMonsterInstanceId - 1;
				if(m_MonsterInfoMap.count(m_dwCurSelMonsterInstanceId))
				{
					Ogre::ColourValue color(0.4f, 0.1f, 0.9f, 0.1f);
					m_MonsterInfoMap[m_dwCurSelMonsterInstanceId].m_pDobject->SetSelectLook(color);
				}


				// ����redo undo ����.
				CMonsterEditAddNewMonsterOperator* pAddNewMonsterOperator = NULL;
				pAddNewMonsterOperator = new CMonsterEditAddNewMonsterOperator;

				pAddNewMonsterOperator->SetMonsterAction(this);
				pAddNewMonsterOperator->SetMonsterName(m_strCurEditMonsterName);
				pAddNewMonsterOperator->SetMonsterInstanceId(m_dwMonsterInstanceId - 1);
				pAddNewMonsterOperator->SetMonsterId(m_dwCurEditMonsterId);
				pAddNewMonsterOperator->SetPosition(position, position);
				getOperatorManager()->addOperator(pAddNewMonsterOperator);
	  			
			}
		}
	}

	// ɾ������
	void CMonsterEditaction::DelSelectMonster()
	{
		DelMonster(m_dwCurSelMonsterInstanceId);
  	}

	
	// ����ɾ������Ĳ���
	void CMonsterEditaction::DoDelMonster(const Point& pt)
	{

	}


	// ɾ������
	void CMonsterEditaction::DelMonster(unsigned long dwInstanceId)
	{
		if(0xffffffff == dwInstanceId)
		{
			return;
		}

		MONSTERINFO_MAP::iterator it;
		it = m_MonsterInfoMap.find(dwInstanceId);
		if(it == m_MonsterInfoMap.end())
		{
			return;
		}

		CMonsterInfo&  MonsterInfo = it->second;
		getSceneManipulator()->_fireDelMonster(MonsterInfo.m_dwObjectId, MonsterInfo.m_dwInstanceId);
		
		delete MonsterInfo.m_pDobject;
		m_MonsterInfoMap.erase(it);

		// ��ǰѡ�еĹ���id
		if(dwInstanceId == m_dwCurSelMonsterInstanceId)
		{
			m_dwCurSelMonsterInstanceId = 0xffffffff;
		}
	}


	
// �õ�����id�� �����ֵ�ӳ���.
void CMonsterEditaction::LoadMonsterIdNameTable()
{

	FILE* pFile = NULL;
	pFile = ::fopen("..\\LogicEditData\\monster\\CreatureModel.txt", "r");
		
	if(NULL == pFile)
	{
		return;
	}

	char buf[2048];
	::fgets(buf, sizeof(buf), pFile);
	::fgets(buf, sizeof(buf), pFile);

	// ����id
	char bufId[64];

	// ���������ļ�����
	char bufName[128];

	m_IdNameMap.clear();

	int iId = -1;
	while(!feof(pFile))
	{
		memset(buf, 0, sizeof(buf));
		memset(bufId, 0, sizeof(bufId));
		memset(bufName, 0, sizeof(bufName));

		// �õ�һ������
		::fgets(buf, sizeof(buf), pFile);
		sscanf(buf, "%s %s ", bufId, bufName);

		if(0 == strcmp("", bufId))
		{
			continue;
		}
		iId = atoi(bufId);
		m_IdNameMap[iId] = bufName;

	}

	::fclose(pFile);

}

// �õ��������ͱ�
void CMonsterEditaction::LoadMonsterTypeTable()
{

	FILE* pFile = NULL;
	pFile = ::fopen("..\\LogicEditData\\monster\\CreatureInfo.txt", "r");
	
	if(NULL == pFile)
	{
		return;
	}

	char buf[2048];
	::fgets(buf, sizeof(buf), pFile);
	::fgets(buf, sizeof(buf), pFile);

	// ��������id
	char bufId[64];

	// ��������id
	char bufLookId[64];

	int iId     = -1;
	int iLookId = -1;
	
	m_Id_LookId_Map.clear();

	while(!feof(pFile))
	{
		memset(buf, 0, sizeof(buf));
		memset(bufId, 0, sizeof(bufId));
		memset(bufLookId, 0, sizeof(bufLookId));

		// �õ�һ������
		::fgets(buf, sizeof(buf), pFile);
		sscanf(buf, "%s %s ", bufId, bufLookId);
		if(0 == strcmp("", bufId))
		{
			continue;
		}

		iId     = atoi(bufId);
		iLookId = atoi(bufLookId);
		m_Id_LookId_Map[iId] = iLookId;

	}

	::fclose(pFile);

}



// ��ȡ�ļ���ʱ��ʹ��, ���һ������
bool CMonsterEditaction::AddNewMonster(Ogre::Vector3& pos, unsigned long dwObjectId, bool bTransPos)
{
	if(0 == m_IdNameMap.count(dwObjectId))
	{
		return false;
	}

	// �õ�����
	std::string strName = m_IdNameMap[dwObjectId];

	if(bTransPos)
	{
		// ����ϵ��
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;
		float fTerrainDataPosY = 0;

		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;
		fTerrainDataPosY = getTerrainData()->mPosition.y;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}

		// �߼�����ͳ�������ת��.
		pos.x = pos.x * fScale + fTerrainDataPosX;
		pos.z =	pos.z * fScale + fTerrainDataPosZ;
		pos.y = getTerrainData()->getHeightAt(pos.x, pos.z);


	}

	return AddNewMonster(strName, pos, dwObjectId);
}

void CMonsterEditaction::MoveMonster(Ogre::Vector3& pos, unsigned long dwInstanceId)
{
	if(0xffffffff == dwInstanceId)
	{
		return;
	}
	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];
	monsterInfo.m_position = pos;

	CEditDobject_NT* pObject = monsterInfo.m_pDobject;
	Ogre::SceneNode* pNode = pObject->getSceneNode();
	if(pNode)
		pNode->setPosition(pos);

	POINT_2D _old2dPos;

	_old2dPos.iX = (int)(m_preDoMonsterPos.x / 100);
	_old2dPos.iY = (int)(m_preDoMonsterPos.z / 100);
	if(m_MonsterPosMap.count(_old2dPos))
	{
		INSTANCE_ID_LIST& list = m_MonsterPosMap[_old2dPos];
		list.remove(dwInstanceId);
	}

	POINT_2D _2dPos;

	_2dPos.iX = (int)(pos.x / 100);
	_2dPos.iY = (int)(pos.z / 100);

	if(m_MonsterPosMap.count(_2dPos))
	{
		INSTANCE_ID_LIST& list = m_MonsterPosMap[_2dPos];
		list.push_back(dwInstanceId);
	}
	else
	{
		INSTANCE_ID_LIST list;
		list.push_back(dwInstanceId);
		m_MonsterPosMap[_2dPos] = list;
	}

	// ����ϵ��
	float fScale = 1.0;
	float fTerrainDataPosX = 0;
	float fTerrainDataPosZ = 0;
	float fTerrainDataPosY = 0;

	fTerrainDataPosX = getTerrainData()->mPosition.x;
	fTerrainDataPosZ = getTerrainData()->mPosition.z;
	fTerrainDataPosY = getTerrainData()->mPosition.y;


	if(getSceneManipulator())
	{
		fScale = getSceneManipulator()->getBaseScale();
		if(abs((float)fScale) < 0.00001)
		{
			fScale = 1.0;
		}
	}

	// ���г���������߼�����ת��
	Ogre::Vector3 vPostion;
	vPostion.x = (pos.x - fTerrainDataPosX) / fScale;
	vPostion.y = (pos.y - fTerrainDataPosY) / fScale;
	vPostion.z = (pos.z - fTerrainDataPosZ) / fScale;

	getSceneManipulator()->_fireMoveMonster( dwInstanceId,vPostion);

}

// ���ݹ���ʵ��id�� �õ��������Ϣָ��
CMonsterInfo* CMonsterEditaction::GetMonster(unsigned long dwInstanceId)
{

	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return NULL;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];

	return &monsterInfo;

}


// �ı����еĹ����ˢ��ʱ�䡣
void CMonsterEditaction::ChangeAllMonsterRefreshTime(int iTime)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		monsterInfo.m_iRefreshTime = iTime;

	}

}

// �ı����й���Ļ�뾶��
void CMonsterEditaction::ChangeAllMonsterRadius(float fRadius)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		monsterInfo.m_fAreaRadius  = fRadius;

	}

}

// �ı�ˢ��ʱ��ͻ�뾶��
void CMonsterEditaction::ChangeAllMonsterRefreshTimeRadius(int iTime, float fRadius)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		monsterInfo.m_iRefreshTime = iTime;
		monsterInfo.m_fAreaRadius  = fRadius;

	}
}

// �ı�ĳһ��ֵ�ˢ��ʱ�䡣
void CMonsterEditaction::ChangeMonsterRefreshTime_Type(unsigned long dwObjectId, int iTime)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		if(monsterInfo.m_dwObjectId == dwObjectId)
		{
			monsterInfo.m_iRefreshTime = iTime;

		}
	}

}

// �ı�ĳһ��ֵĻ�뾶��
void CMonsterEditaction::ChangeMonsterRefreshRadius_Type(unsigned long dwObjectId, float fRadius)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		if(monsterInfo.m_dwObjectId == dwObjectId)
		{
			monsterInfo.m_fAreaRadius  = fRadius;

		}
	}
}

// �ı�ĳһ��ֵ�ˢ��ʱ��ͻ�뾶��
void CMonsterEditaction::ChangeMonsterRefreshTimeRadius_Type(unsigned long dwObjectId, int iTime, float fRadius)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		if(monsterInfo.m_dwObjectId == dwObjectId)
		{
			monsterInfo.m_iRefreshTime = iTime;
			monsterInfo.m_fAreaRadius  = fRadius;

		}
	}
	
}

	
// �ı�����ˢ��ʱ�䡣
void CMonsterEditaction::ChangeMonsterRefreshTime(unsigned long dwInstanceId, int iTime)
{
	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];

	monsterInfo.m_iRefreshTime = iTime;
}

// �ı����Ļ�뾶��
void CMonsterEditaction::ChangeMonsterRefreshRadius(unsigned long dwInstanceId, float fRadius)
{
	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];

	monsterInfo.m_fAreaRadius = fRadius;
}

// �ı�����ˢ��ʱ��Ͱ뾶��
void CMonsterEditaction::ChangeMonsterRefreshTimeRadius(unsigned long dwInstanceId, int iTime, float fRadius)
{

	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];

	monsterInfo.m_iRefreshTime = iTime;
	monsterInfo.m_fAreaRadius  = fRadius;
}

// �ı����ķ���
void CMonsterEditaction::ChangeMonsterDir(unsigned long dwInstanceId, float radian)
{

	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];

	if(	monsterInfo.m_pDobject )
	{
		if(monsterInfo.m_pDobject->getSceneNode())
		{
			float fw = 0;
			float fx = 0;
			float fy = 0;
			float fz = 0;

			monsterInfo.m_fMonstDir = radian;

			if(monsterInfo.m_fMonstDir >= _2PI)
			{
				monsterInfo.m_fMonstDir -= _2PI;
			}

			if(monsterInfo.m_fMonstDir <= -_2PI)
			{
				monsterInfo.m_fMonstDir += _2PI;
			}

			fw = cos(radian/2);
			fx = 0;
			fy = sin(radian/2);
			fz = 0;
			monsterInfo.m_pDobject->getSceneNode()->setOrientation(fw, fx, fy, fz);
		}
	}

	SetModifyed();


}


// �������ȡ�ļ�
unsigned long CMonsterEditaction::ReadEditMonsterFromFile(std::string strFileName)
{
	char bufMonsterIndex[64];

	int iMonsterCount = ::GetPrivateProfileInt("info", "monstercount", 0, strFileName.c_str());
	int iMonsterType  = 0;
	unsigned long   dwGUID = 0xffffffff;
	
	int  iMonsterDir     = 0;
	int  iMonsterGroupId = 0;
	int  iMonsterTeamId  = 0;
	int  iBaseAIId       = 0;

	char bufX[64];
	char bufY[64];
	char bufZ[64];

	char bufAI[1024];
	char bufEven[1024];


	// ����ϵ��
	float fScale = 1.0;
	float fTerrainDataPosX = 0;
	float fTerrainDataPosZ = 0;
	float fTerrainDataPosY = 0;

	fTerrainDataPosX = getTerrainData()->mPosition.x;
	fTerrainDataPosZ = getTerrainData()->mPosition.z;
	fTerrainDataPosY = getTerrainData()->mPosition.y;

	if(getSceneManipulator())
	{
		fScale = getSceneManipulator()->getBaseScale();
		if(abs((float)fScale) < 0.00001)
		{
			fScale = 1.0;
		}
	}


	String  MonsterTypename;
	Ogre::Vector3 pos;

	float fDir = 0;

	CMonsterInfo* pMonsterInfo = NULL;

	int iLookId = -1;
	for(int i = 0; i < iMonsterCount; i++)
	{

		memset(bufMonsterIndex, 0, sizeof(bufMonsterIndex));
		memset(bufX, 0, sizeof(bufX));
		memset(bufY, 0, sizeof(bufY));
		memset(bufZ, 0, sizeof(bufZ));
		sprintf(bufMonsterIndex, "monster%d", i);

		// ��ȡ��������
		iMonsterType = ::GetPrivateProfileInt(bufMonsterIndex, "monster_type", -1, strFileName.c_str());
		if(m_Id_LookId_Map.count(iMonsterType))
		{
			continue;
		}

		iLookId = m_Id_LookId_Map[iMonsterType];
		if(m_IdNameMap.count(iLookId))
		{
			
			MonsterTypename = m_IdNameMap[iLookId];
		
			// ��ȡ����guid
			dwGUID = ::GetPrivateProfileInt(bufMonsterIndex, "monster_guid", 0xffffffff, strFileName.c_str());

			// ��ȡλ��
			::GetPrivateProfileString(bufMonsterIndex, "position_X",  "none", bufX, sizeof(bufX), strFileName.c_str());
			::GetPrivateProfileString(bufMonsterIndex, "position_Y",  "none", bufY, sizeof(bufY),strFileName.c_str());
			::GetPrivateProfileString(bufMonsterIndex, "position_Z",  "none", bufZ, sizeof(bufZ),strFileName.c_str());

			// ��ȡ����
			iMonsterDir = ::GetPrivateProfileInt(bufMonsterIndex, "monster_dir",  -1, strFileName.c_str());

			// ��ȡgroupid
			iMonsterGroupId = ::GetPrivateProfileInt(bufMonsterIndex, "monster_groupid", -1, strFileName.c_str());

			// ��ȡteamid
			iMonsterTeamId = ::GetPrivateProfileInt(bufMonsterIndex, "monster_teamid", -1, strFileName.c_str());

			// �������basseAiId
			iBaseAIId = ::GetPrivateProfileInt(bufMonsterIndex, "monster_BaseAI",  -1, strFileName.c_str());

			// ������ĸ߼�ai�ļ�����
			memset(bufAI, 0, sizeof(bufAI));
			::GetPrivateProfileString(bufMonsterIndex, "monster_AIFile",  "none", bufAI, sizeof(bufAI), strFileName.c_str());

			// ��������¼��ļ�����
			memset(bufEven, 0, sizeof(bufEven));
			::GetPrivateProfileString(bufMonsterIndex, "monster_EvenFile", "none", bufEven, sizeof(bufEven), strFileName.c_str());

			// ��֤��ǰ��ͼ��guid������.
			if(0xffffffff != dwGUID)
			{
				if(m_dwGUID < dwGUID)
				{
					m_dwGUID = dwGUID;
				}
			}

			iMonsterDir += 9;
			iMonsterDir = iMonsterDir % DIR_STEP;

			fDir = (iMonsterDir*1.0 / DIR_STEP) * _2PI;

			pos.x = atof(bufX);
			pos.y = atof(bufY);
			pos.z = atof(bufZ);

			// �߼�����ͳ�������ת��.
			pos.x = pos.x * fScale + fTerrainDataPosX;
			pos.y = pos.y * fScale + fTerrainDataPosY;
			pos.z =	pos.z * fScale + fTerrainDataPosZ;

			if(AddNewMonster(MonsterTypename,  pos, iMonsterType))
			{
				ChangeMonsterDir(m_dwMonsterInstanceId - 1, fDir);

				CMonsterInfo& monsterInfo   = m_MonsterInfoMap[m_dwMonsterInstanceId - 1];
				monsterInfo.m_dwMonsterGUID = m_dwGUID;
				monsterInfo.m_iGroupId  = iMonsterGroupId;
				monsterInfo.m_iTeamId   = iMonsterTeamId;
				monsterInfo.m_iBaseAIId = iBaseAIId;
				monsterInfo.m_strAdvanceAIFile = bufAI;
				monsterInfo.m_strEvent = bufEven;
			}

		}

	}

	return 0;
}
	
// ������洢���ļ�
unsigned long CMonsterEditaction::SaveEditMonsterToFile(std::string strFileName)
{
	char bufWrite[128];
	char bufMonsterIndex[128];
	int iMonsterType  = 0;
	int  iMonsterDir;
	char bufX[64];
	char bufY[64];
	char bufZ[64];

	float fx = 0;
	float fy = 0;
	float fz = 0;


	// ����ϵ��
	float fScale = 1.0;
	float fTerrainDataPosX = 0;
	float fTerrainDataPosZ = 0;
	float fTerrainDataPosY = 0;

	fTerrainDataPosX = getTerrainData()->mPosition.x;
	fTerrainDataPosZ = getTerrainData()->mPosition.z;
	fTerrainDataPosY = getTerrainData()->mPosition.y;

	if(getSceneManipulator())
	{
		fScale = getSceneManipulator()->getBaseScale();
		if(abs((float)fScale) < 0.00001)
		{
			fScale = 1.0;
		}
	}


	// ɾ���ɵ��ļ���
	::DeleteFile(strFileName.c_str());

    String  MonsterTypename;
	Ogre::Vector3 pos;

	int iMonsterCount = 0;
	iMonsterCount = m_MonsterInfoMap.size();

	memset(bufWrite, 0, sizeof(bufWrite));
	sprintf(bufWrite, "%d", iMonsterCount);

	// ��д����ĸ���
	WritePrivateProfileString("info", "monstercount", bufWrite, strFileName.c_str());
	
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;
	
	itEnd = m_MonsterInfoMap.end();
	int i = 0;
	
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& MonsterInfo = it->second;

		memset(bufMonsterIndex, 0, sizeof(bufMonsterIndex));
		sprintf(bufMonsterIndex, "monster%d", i);

		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_dwObjectId);
		
		
		// д���������.
		::WritePrivateProfileString(bufMonsterIndex, "monster_type", bufWrite, strFileName.c_str());

		// д�����λ��
		memset(bufX, 0, sizeof(bufX));

		// ���г���������߼�����ת��
        fx = (MonsterInfo.m_position.x - fTerrainDataPosX) / fScale;
		fy = (MonsterInfo.m_position.y - fTerrainDataPosY) / fScale;
		fz = (MonsterInfo.m_position.z - fTerrainDataPosZ) / fScale;

		sprintf(bufX, "%.4f", fx);
		::WritePrivateProfileString(bufMonsterIndex, "position_X",  bufX, strFileName.c_str());

		memset(bufY, 0, sizeof(bufY));
		sprintf(bufY, "%.4f", fy);
		::WritePrivateProfileString(bufMonsterIndex, "position_Y",  bufY, strFileName.c_str());

		memset(bufZ, 0, sizeof(bufZ));
		sprintf(bufZ, "%.4f", fz);
		::WritePrivateProfileString(bufMonsterIndex, "position_Z",  bufZ, strFileName.c_str());

		// д����ķ���.
		iMonsterDir = (MonsterInfo.m_fMonstDir / _2PI) * DIR_STEP;
		iMonsterDir = iMonsterDir + 27;
		iMonsterDir = iMonsterDir % DIR_STEP;

		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", iMonsterDir);
		::WritePrivateProfileString(bufMonsterIndex, "monster_dir",  bufWrite, strFileName.c_str());

		// д�����guid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_dwMonsterGUID);
		::WritePrivateProfileString(bufMonsterIndex, "monster_guid",  bufWrite, strFileName.c_str());

		// д�����groupid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iGroupId);
		::WritePrivateProfileString(bufMonsterIndex, "monster_groupid",  bufWrite, strFileName.c_str());

		// д�����teamid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iTeamId);
		::WritePrivateProfileString(bufMonsterIndex, "monster_teamid",  bufWrite, strFileName.c_str());

		// д�����basseAiId
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iBaseAIId);
		::WritePrivateProfileString(bufMonsterIndex, "monster_BaseAI",  bufWrite, strFileName.c_str());

		// д����ĸ߼�ai�ļ�����
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%s", MonsterInfo.m_strAdvanceAIFile.c_str());
		::WritePrivateProfileString(bufMonsterIndex, "monster_AIFile",  bufWrite, strFileName.c_str());

		// д������¼��ļ�����
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%s", MonsterInfo.m_strEvent.c_str());
		::WritePrivateProfileString(bufMonsterIndex, "monster_EvenId",  bufWrite, strFileName.c_str());


        // ������
		i++;
	
	}

	return 0;
}

void CMonsterEditaction::ClearCounter()
{
	m_dwGUID = 0;
	m_dwMonsterInstanceId = 0;
	m_dwCurEditMonsterId  = 0xffffffff;
	m_dwCurSelMonsterInstanceId = 0xffffffff;
}



	// �����Ѿ��޸�
void CMonsterEditaction::SetModifyed(bool bModifyed)
{

	CMonsterOperator* pOperator = new CMonsterOperator;
	getOperatorManager()->addOperator(pOperator);

}

void CMonsterEditaction::GetMonsterIdLookTable()
{
	DBC::DBCFile excelDBFile(1);
	DBC::DBCFile excelDBFileModelName(1);

	std::map<int, std::string> Id_Model_Map;
	const DBC::DBCFile::FIELD* pInfo = NULL;	
	
	if(!excelDBFile.OpenFromTXT("..\\..\\public\\config\\mon_attr_ex.tab"))
	{
		return;
	}
	if(!excelDBFileModelName.OpenFromTXT("..\\resource\\config\\char_model.tab"))
	{
		return;
	}

	int iRecordCount = excelDBFile.GetRecordsNum(); 
	int iModelCount = excelDBFileModelName.GetRecordsNum();

	int iId = -1;
	for(int iIndex = 0; iIndex < iModelCount; iIndex++)
	{
		pInfo = excelDBFileModelName.Search_Posistion(iIndex, 0);
		iId = pInfo->iValue;
		
		//��������(��:ɽ��.obj)
		pInfo = excelDBFileModelName.Search_Posistion(iIndex, 2);
		Id_Model_Map[iId] = pInfo->pString;
	}
		
	iId = -1;
	for(int i = 0; i < iRecordCount; i++)
	{
		// ��ȡ��������
		pInfo = excelDBFile.Search_Posistion(i, 0);
		if(pInfo)
			iId = pInfo->iValue;
		
		// ��������
		pInfo = excelDBFile.Search_Posistion(i, 33);//fixbug:��39��Ϊ29//��29��Ϊ33
		m_IdNameMap[iId] = Id_Model_Map[pInfo->iValue];//
	}

}


// �õ������λ��
Ogre::Vector3   CMonsterEditaction::GetMonsterPos(unsigned long ulInstance)
{
	Ogre::Vector3 pos(0 ,0, 0);

	float fx = 0;
	float fz = 0;

	if(m_MonsterInfoMap.count(ulInstance))
	{
		CMonsterInfo& monsterInfo = m_MonsterInfoMap[ulInstance];
		// ����ϵ��
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;
		float fTerrainDataPosY = 0;

		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;
		fTerrainDataPosY = getTerrainData()->mPosition.y;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}

		// ���г���������߼�����ת��
        fx = (monsterInfo.m_position.x - fTerrainDataPosX) / fScale;
		fz = (monsterInfo.m_position.z - fTerrainDataPosZ) / fScale;

		pos.x = fx;
		pos.z = fz;
	}


	return pos;
}



} 
