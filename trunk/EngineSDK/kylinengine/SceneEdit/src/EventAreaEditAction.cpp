#include "EventAreaEditAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "UnreachRegion.h"
#include "CustomShape.h"
#include "WXOperatorManager.h"       // redo undo ������.
#include "ScanLine.h"
#include "OgreCamera.h"
#include "OgreStringConverter.h"
#include "EventAreaEditOperator.h"

#include "Core/TerrainData.h"
#include "Core/FairyResources.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Fairy 
{
	using namespace Ogre;


	/////////////////////////////////////////////////////////////////////////////////
	//
	// ����������
	//


	CEventAreaRectInfo::CEventAreaRectInfo()
		: m_pParentSceneNode(NULL)
		, m_pSceneNode(NULL)
		, m_RectRenderShape(NULL)
		, m_pSceneManipulator(NULL)
	{
		m_iEventAreaId = -1;
		m_iScriptId    = -1;
		
		m_rect.m_LeftTop.x = 0;
		m_rect.m_LeftTop.y = 0;
		m_rect.m_RightBottom.x = 0;
		m_rect.m_RightBottom.y = 0;


	
	}

	CEventAreaRectInfo::~CEventAreaRectInfo()
	{

	}

	// ���ø������ڵ�.
	void CEventAreaRectInfo::SetParentSceneNode(Ogre::SceneNode* pSceneNode)
	{
		m_pParentSceneNode = pSceneNode;
	}

	// ����SceneManipulator���
	void CEventAreaRectInfo::SetSceneManipulator(SceneManipulator*  SceneManipulator)
	{
		m_pSceneManipulator = SceneManipulator;
	}

	// ���������ڵ�ģ��.
	unsigned long CEventAreaRectInfo::CreateSceneNode()
	{
		if(m_pSceneNode)
		{
			return 0;
		}

		if(m_pParentSceneNode)
		{
			 m_pSceneNode = (SceneNode*)m_pParentSceneNode->createChildSceneNode();
			 if(NULL == m_pSceneNode)
			 {
				 // ����ʧ��.
				 return 0xffffffff;
			 }
		}

		// �����ɹ�.
		return 0;
	}

	//// ������ʾ�ľ���
	//void CEventAreaRectInfo::BuildEventAreaRect()
	//{
	//	if(NULL == m_RectRenderShape)
	//	{
	//		return;
	//	}

	//	m_RectRenderShape->ClearRenderShape();

	//	float fStepX = 100.0f;
	//	float fStepZ = 100.0f;
	//	int   iStartX = m_rect.m_LeftTop.x;
	//	int   iStartZ = m_rect.m_LeftTop.y;
	//	int   iEndX   = m_rect.m_RightBottom.x;
	//	int   iEndZ   = m_rect.m_RightBottom.y;

	//	float fx = 0;
	//	float fz = 0;
	//		
	//	Ogre::Vector3 NewPoint;
	//	for(fz = iStartZ; fz < iEndZ; fz += fStepZ)
	//		for(fx = iStartX; fx < iEndX; fx += fStepX)
	//		{
	//			// ������1
	//			NewPoint.x = fx;
	//			NewPoint.z = fz;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);

	//			NewPoint.x = fx;
	//			NewPoint.z = fz + fStepZ;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);


	//			NewPoint.x = fx + fStepX;
	//			NewPoint.z = fz;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);

	//		
	//			// ������2
	//			NewPoint.x = fx + fStepX;
	//			NewPoint.z = fz;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);

	//			NewPoint.x = fx;
	//			NewPoint.z = fz + fStepZ;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);

	//			NewPoint.x = fx + fStepX;
	//			NewPoint.z = fz + fStepZ;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);
	//		}

	//	m_pRectRenderShape->FillPointData();
	//	m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
	//	m_pRectRenderShape->setMaterial("EventAreaEdit/SelectRect");
	//	
	//}//

































///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �¼�����action �༭.
//
//



	 CEventAreaEditAction::CEventAreaEditAction(SceneManipulator* sceneManipulator)
            : Action(sceneManipulator)
			, m_bIsShow(false)
			, m_FuncKeyState(0)
			, m_pSceneNode(NULL)
			, m_pLineShape(NULL)
			, m_pCurOperatorRect(NULL)
			, m_bLocalArea(false)
	 {
		// ��ʼ���¼�����༭
		InitEventArea();
    }

	CEventAreaEditAction::~CEventAreaEditAction(void)
	{
		ReleaseRectInfoMap();
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �������������
	//
		
	// ���ð��µĹ��ܼ�
	void CEventAreaEditAction::SetFuncKey(unsigned long ulFuncKey)
	{

	}

	//  ��չ��ܼ�.
	void CEventAreaEditAction::ClearFuncKey()
	{

	}

		
	////////////////////////////////////////////////////////////////////////////////////
	//
	// ���̺Ͷ��̲���
	//

	// �������ȡ�ļ�
	unsigned long CEventAreaEditAction::ReadEventAreaFromFile(std::string strFileName)
	{

		// ��վ�����Ϣ.
		ReleaseRectInfoMap();

		std::string strEventAreaFileIni = "";
		std::string strEventAreaFile    = "";
		std::string strLocalEventAreaFileIni = "";

		strEventAreaFile    = strFileName;
		int iPos = strEventAreaFile.rfind(".");

		strEventAreaFileIni = strEventAreaFile.substr(0, iPos);
		strEventAreaFileIni += "_area.ini";

		strLocalEventAreaFileIni = strEventAreaFile.substr(0, iPos);
		strLocalEventAreaFileIni +="_local_area.ini";

		_readAreaFromSingleFile(strEventAreaFileIni);
		_readAreaFromSingleFile(strLocalEventAreaFileIni, true);

		
		HideScene(m_bIsShow);
		return 0;
	}

	void CEventAreaEditAction::_readAreaFromSingleFile(const std::string& fileName, bool bLocal)
	{
		char bufInfo[1024];
		memset(bufInfo, 0, sizeof(bufInfo));

		// ��ȡ����ĸ���
		int iRectCount = 0;
		iRectCount = ::GetPrivateProfileInt("area_info", "area_count",  0, fileName.c_str());
		

		///////////////////////////////////////////////////////////////////////////////////////////////
		// ����ѳ�������ת������Ϸ�߼�����.
		float fScale = 0;
		float fTerrainDataPosX = getTerrainData()->mPosition.x;
		float fTerrainDataPosZ = getTerrainData()->mPosition.z;
		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}


		char bufPosInfo[1024];
		int iEventRectIndex = 0;
		CEventAreaInfo* pEventAreaInfo = NULL;

		float fx = 0;
		m_iMaxEventAreaId = 0;
		for(int i = 0; i < iRectCount; i++)
		{
			pEventAreaInfo = new CEventAreaInfo;
			if(pEventAreaInfo)
			{
				pEventAreaInfo->m_iId = i;
				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "area%d", i);
			
				// �¼�����id
				pEventAreaInfo->m_iEventAreaId = GetPrivateProfileInt(bufInfo, "guid", 0, fileName.c_str());
			
				// �ű�id
				pEventAreaInfo->m_iScriptId = GetPrivateProfileInt(bufInfo, "script_id", 0, fileName.c_str());
			

				// ��¼��������λ��
				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				::GetPrivateProfileString(bufInfo, "left",  "none", bufPosInfo, sizeof(bufPosInfo), fileName.c_str());
				fx = atof(bufPosInfo);
				fx = fx * fScale + fTerrainDataPosX;
				pEventAreaInfo->m_rect.m_LeftTop.x = fx;

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				::GetPrivateProfileString(bufInfo, "top",  "none", bufPosInfo, sizeof(bufPosInfo), fileName.c_str());
				fx = atof(bufPosInfo);
				fx = fx * fScale + fTerrainDataPosZ;
				pEventAreaInfo->m_rect.m_LeftTop.y = fx;
			

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				::GetPrivateProfileString(bufInfo, "right",  "none", bufPosInfo, sizeof(bufPosInfo), fileName.c_str());
				fx = atof(bufPosInfo);
				fx = fx * fScale + fTerrainDataPosX;
				pEventAreaInfo->m_rect.m_RightBottom.x = fx;
			

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				::GetPrivateProfileString(bufInfo, "bottom",  "none", bufPosInfo, sizeof(bufPosInfo), fileName.c_str());
				fx = atof(bufPosInfo);
				fx = fx * fScale + fTerrainDataPosZ;
				pEventAreaInfo->m_rect.m_RightBottom.y = fx;

				// Params
				int count = 0;
				for (int index = 0; index<10; ++index)
				{					
					memset(bufPosInfo, 0, sizeof(bufPosInfo));
					char temp[24];
					memset(temp, 0, sizeof(temp));
					sprintf(temp, "param_%d", index);
					::GetPrivateProfileString(bufInfo, temp,  "none", bufPosInfo, sizeof(bufPosInfo), fileName.c_str());

					char* params = bufPosInfo;
					Ogre::String paramVec(params);
					Ogre::StringVector vec = Ogre::StringUtil::split(paramVec);
					if ((int)vec.size()>1)
					{
						pEventAreaInfo->m_paramsName[count] = vec[0];
						pEventAreaInfo->m_paramsValue[count] = vec[1];
						count++;
					}
				}

				// ������Ϣ
			
				bLocal?(m_LocalRectShowMap[i] = pEventAreaInfo):(m_RectShowMap[i] = pEventAreaInfo);						

				// ��ȡid
				//m_iCurEditEventAreaId = pEventAreaInfo->m_iEventAreaId;
				//m_iCurEditScriptid    = pEventAreaInfo->m_iScriptId;

			

				// ���õ�ǰ�����eventid
				if (bLocal)
				{
					m_iLocalEditRectId = i;

					if(m_iMaxLocalEventAreaId < m_iCurEditEventAreaId)
					{
						m_iMaxLocalEventAreaId = m_iCurEditEventAreaId;
					}
				}
				else
				{
					// ���õ�ǰ�༭�ľ���id
					m_iEditRectId = i;

					if(m_iMaxEventAreaId < m_iCurEditEventAreaId)
					{
						m_iMaxEventAreaId = m_iCurEditEventAreaId;
					}
				}
				
				// ������ʾ������
				pEventAreaInfo->SetParentSceneNode(bLocal?m_pLocalSceneNode:m_pSceneNode);
				pEventAreaInfo->SetSceneManipulator(getSceneManipulator());
				pEventAreaInfo->CreateSceneNode();
				pEventAreaInfo->CreateRenderShape();
				pEventAreaInfo->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
				
				m_pCurOperatorRect = pEventAreaInfo;

				m_PrePoint.x  = pEventAreaInfo->m_rect.m_LeftTop.x;
				m_PrePoint.z = pEventAreaInfo->m_rect.m_LeftTop.y;

				m_CurPoint.x =  pEventAreaInfo->m_rect.m_RightBottom.x;
				m_CurPoint.z =  pEventAreaInfo->m_rect.m_RightBottom.y;
				NormalRect();
				BuildEventAreaRect();
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
										
			}
		}
	}
	
	// ������洢���ļ�
	unsigned long CEventAreaEditAction::SaveEventAreaToFile(std::string strFileName)
	{
		std::string strEventAreaFileIni = "";
		std::string strLocalEventAreaFileIni = "";
		std::string strEventAreaFile    = "";

		strEventAreaFile    = strFileName;
		int iPos = strEventAreaFile.rfind(".");

		strEventAreaFileIni = strEventAreaFile.substr(0, iPos);
		strEventAreaFileIni += "_area.ini";
		strLocalEventAreaFileIni = strEventAreaFile.substr(0, iPos);
		strLocalEventAreaFileIni +="_local_area.ini";
		_writeAreaFromSingleFile(strEventAreaFileIni, m_RectShowMap);
		//_writeAreaFromSingleFile(strLocalEventAreaFileIni, m_LocalRectShowMap);		
	
		return 0;
	}

	void CEventAreaEditAction::_writeAreaFromSingleFile(const std::string& fileName, SHOW_RECT_MAP& map)
	{
		SHOW_RECT_MAP::iterator itEventAreaEnd;
		SHOW_RECT_MAP::iterator it;

		// ɾ���ɵ��ļ���
		::DeleteFile(fileName.c_str());

		char bufInfo[1024];
		memset(bufInfo, 0, sizeof(bufInfo));

		// �洢����ĸ���
		int iRectCount = map.size();
		sprintf(bufInfo, "%d", iRectCount);
		WritePrivateProfileString("area_info", "area_count", bufInfo, fileName.c_str());


		// �洢ÿ���������Ϣ
		itEventAreaEnd = map.end();
		///////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ����ѳ�������ת������Ϸ�߼�����.
		//

		float fScale = 0;
		float fTerrainDataPosX = getTerrainData()->mPosition.x;
		float fTerrainDataPosZ = getTerrainData()->mPosition.z;
		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}

		char bufPosInfo[1024];
		int iEventRectIndex = 0;
		CEventAreaInfo* pEventAreaInfo = NULL;

		float x = 0;
		float y = 0;
		for(it = map.begin(); it != itEventAreaEnd; it++)
		{
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "area%d", iEventRectIndex);
				iEventRectIndex++;

				// �¼�����id
				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				sprintf(bufPosInfo, "%d", pEventAreaInfo->m_iEventAreaId);
				WritePrivateProfileString(bufInfo, "guid", bufPosInfo, fileName.c_str());

				// �ű�id
				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				sprintf(bufPosInfo, "%d", pEventAreaInfo->m_iScriptId);
				WritePrivateProfileString(bufInfo, "script_id", bufPosInfo, fileName.c_str());


				// ��¼��������λ��
				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				x = pEventAreaInfo->m_rect.m_LeftTop.x;
				x = (x - fTerrainDataPosX) / fScale;
				sprintf(bufPosInfo, "%.4f", x);
				WritePrivateProfileString(bufInfo, "left", bufPosInfo, fileName.c_str());

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				x = pEventAreaInfo->m_rect.m_LeftTop.y;
				x = (x - fTerrainDataPosZ) / fScale;
				sprintf(bufPosInfo, "%.4f",x);
				WritePrivateProfileString(bufInfo, "top", bufPosInfo, fileName.c_str());

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				x = pEventAreaInfo->m_rect.m_RightBottom.x;
				x = (x - fTerrainDataPosX) / fScale;
				sprintf(bufPosInfo, "%.4f",x);
				WritePrivateProfileString(bufInfo, "right", bufPosInfo, fileName.c_str());

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				x = pEventAreaInfo->m_rect.m_RightBottom.y;
				x = (x - fTerrainDataPosZ) / fScale;
				sprintf(bufPosInfo, "%.4f",x);
				WritePrivateProfileString(bufInfo, "bottom", bufPosInfo, fileName.c_str());

				// Params
				int count = 0;
				for (int i = 0; i<10; ++i)
				{					
					Ogre::String paramName = pEventAreaInfo->m_paramsName[i];
					Ogre::String paramValue = pEventAreaInfo->m_paramsValue[i];
					if (paramName.empty() || paramValue.empty())
					{
						continue;
					}
					memset(bufPosInfo, 0, sizeof(bufPosInfo));
					
					sprintf(bufPosInfo, "%s %s", paramName.c_str(), paramValue.c_str());
					char temp[24];
					memset(temp, 0, sizeof(temp));
					sprintf(temp, "param_%d", count);
					WritePrivateProfileString(bufInfo, temp, bufPosInfo, fileName.c_str());
					count++;
				}
			}
		}

	}		
			
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// action ������
	//
		
	const String& CEventAreaEditAction::getName(void) const
	{
		//����һ���ַ������֣� �����жϵ�ǰ����������.
		static const String name = "EventAreaEdit";
		return name;
	}

	// ���ò�������.
	void CEventAreaEditAction::setParameter(const String& name, const String& value)
	{
		if (name == "isLocal")
		{
			m_bLocalArea = Ogre::StringConverter::parseBool(value);
			HideScene(m_bIsShow);
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// ���������麯��.
	//
    //protected:
        
		
	void CEventAreaEditAction::_onMove(const Point& pt)
	{

	}
	void CEventAreaEditAction::_onBegin(const Point& pt)
	{
		OnMouseLBnDown(pt);
	}
	void CEventAreaEditAction::_onDrag(const Point& pt)
	{
		OnMouseDrag(pt);
	}
	void CEventAreaEditAction::_onEnd(const Point& pt, bool canceled)
	{
		OnMouseLBnUp(pt);
	}

	// ��ǰ����active����.
	void CEventAreaEditAction::_onActive(bool active)
	{

	}


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��Ӧ��Ӧ��������.
	//

	void CEventAreaEditAction::OnMouseMove(const Point& pt)
	{
		switch(m_iOperator)
		{
		case SELECT:
			{
				getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
				ClearFuncKey();
			}
			break;
		case DRAW:
			{
				
			}
			break;
		default:
			{
			}
			break;
		}

		
	}

	void CEventAreaEditAction::OnMouseLBnDown(const Point& pt)
	{
		switch(m_iOperator)
		{
		case SELECT:
			{
				
			}
			break;
		case DRAW:
			{
				DoDrawLBnDown(pt);
			}
			break;
		default:
			{
			}
			break;
		}


		
	}

	void CEventAreaEditAction::OnMouseLBnUp(const Point& pt)
	{
		switch(m_iOperator)
		{
		case SELECT:
			{
				DoSelectLBnUp(pt);
			}
			break;
		case DRAW:
			{
				DoDrawLBnUp(pt);
			}
			break;
		default:
			{
			}
			break;
		}
		
	}

	void CEventAreaEditAction::OnMouseRBnDown(const Point& pt)
	{

	}

	void CEventAreaEditAction::OnMouseRBnUp(const Point& pt)
	{

	}

	void CEventAreaEditAction::OnMouseDrag(const Point& pt)
	{
		switch(m_iOperator)
		{
		case SELECT:
			{
				
			}
			break;
		case DRAW:
			{
				DoDrawLBnDrag(pt);
			}
			break;
		default:
			{
			}
			break;
		}
		
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����Ⱦ����ز���
	//

	// ��������༭�ĳ����ڵ�
	void CEventAreaEditAction::CreateSceneNode()
	{
		m_pSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();		

		m_pLocalSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();
		
		if(m_pSceneNode && m_pLocalSceneNode)
		{
			HideScene(false);
		}
		
	}

	// ���ص�ǰ�༭��action
	void CEventAreaEditAction::HideScene(bool bHide)
	{
		if(bHide)
		{
			if (m_bLocalArea && m_pLocalSceneNode )
			{
				m_pLocalSceneNode->setVisible(bHide);
				if (m_pSceneNode)
				{
					m_pSceneNode->setVisible(false);
				}
			}
			else if (!m_bLocalArea && m_pSceneNode)
			{
				m_pSceneNode->setVisible(bHide);
				if (m_pLocalSceneNode)
				{
					m_pLocalSceneNode->setVisible(false);
				}
			}
		}
		else
		{
			if(m_pSceneNode)
			{
				m_pSceneNode->setVisible(bHide);			
			}
			if (m_pLocalSceneNode)
			{
				m_pLocalSceneNode->setVisible(bHide);
			}			
		}
		
		m_bIsShow = bHide;
	}

	// ������ʾ������
	void CEventAreaEditAction::CreateShowEventArea()
	{
		if(NULL == m_pCurOperatorRect)
		{
			return;
		}

		if(NULL == m_pCurOperatorRect->m_pRectRenderShape)
		{

		}
		else
		{

		}

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �߼����ݲ���
	//

	// ��ʼ���¼�����༭
	void CEventAreaEditAction::InitEventArea()
	{
		// ���������ڵ�
		CreateSceneNode();

		// ���Ի��߼�����.
		InitLogicData();
	}

	void CEventAreaEditAction::NormalRect()
	{
		int iX1 = 0;
		int iY1 = 0;
		int iX2 = 0;
		int iY2 = 0;

		if(m_PrePoint.x < m_CurPoint.x)
		{

			iX1 = m_PrePoint.x;
            iX2 = m_CurPoint.x;

			m_AreaRect.m_LeftTop.x      = iX1;
			m_AreaRect.m_RightBottom.x  = iX2;

			
		}
		else
		{
			iX1 = m_CurPoint.x;
            iX2 = m_PrePoint.x;

			m_AreaRect.m_LeftTop.x      = iX1;
			m_AreaRect.m_RightBottom.x  = iX2;

			
		}

		if(m_PrePoint.z < m_CurPoint.z)
		{
			iY1 = m_PrePoint.z;
            iY2 = m_CurPoint.z;

			m_AreaRect.m_LeftTop.y      = iY1 ;
			m_AreaRect.m_RightBottom.y  = iY2 ;

			

		}
		else
		{
			iY1 = m_CurPoint.z;
            iY2 = m_PrePoint.z;

			m_AreaRect.m_LeftTop.y      = iY1;
			m_AreaRect.m_RightBottom.y  = iY2;

			
		}

		int   iStartX = m_AreaRect.m_LeftTop.x;
		int   iStartZ = m_AreaRect.m_LeftTop.y;
		int   iEndX   = m_AreaRect.m_RightBottom.x;
		int   iEndZ   = m_AreaRect.m_RightBottom.y;


		if(iStartX > 0)
		{
			iStartX = (iStartX / 100) * 100;
		}
		else
		{
			iStartX = ((iStartX - 99 )/ 100) * 100;
		}

		if(iStartZ > 0)
		{
			iStartZ = (iStartZ / 100) * 100;
		}
		else
		{
			iStartZ = ((iStartZ - 99) / 100) * 100;
		}

		if(iEndX > 0)
		{
			iEndX   = ((iEndX + 99) / 100) * 100;
		}
		else
		{
			iEndX   = (iEndX / 100) * 100;
		}

		if(iEndZ > 0)
		{
			iEndZ   = ((iEndZ + 99) / 100) * 100;
		}
		else
		{
			iEndZ   = (iEndZ / 100) * 100;
		}


		m_AreaRect.m_LeftTop.x		= iStartX;
		m_AreaRect.m_LeftTop.y		= iStartZ;
		m_AreaRect.m_RightBottom.x	= iEndX;
		m_AreaRect.m_RightBottom.y	= iEndZ;
	
	}

	// �õ������ϵ������
	bool  CEventAreaEditAction::GetTerrainPos(const Point& pt, Ogre::Vector3& position)
	{
			
		bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);

		return hit;
		
	}

	// �������ε���갴�²���
	void CEventAreaEditAction::DoDrawLBnDown(const Point& pt)
	{
		Ogre::Vector3 TerrainPosition;
		if(GetTerrainPos(pt, TerrainPosition))
		{

			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
			}

			int rect = m_bLocalArea?(++m_iLocalEditRectId):(++m_iEditRectId);
			m_PrePoint = TerrainPosition;
			m_CurPoint = TerrainPosition;
			m_CurPoint.x += 100;
			m_CurPoint.z += 100;
			NormalRect();
			
			CEventAreaInfo* pRectInfo = new CEventAreaInfo;
			pRectInfo->m_iEventAreaId = m_iCurEditEventAreaId;
			pRectInfo->m_iScriptId    = m_iCurEditScriptid;
			pRectInfo->m_iId          = rect;
			m_pCurOperatorRect        = pRectInfo;

			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_rect = m_AreaRect;
				//m_pCurOperatorRect->m_rect.m_LeftTop.x = m_AreaRect.m_LeftTop.x;
				//m_pCurOperatorRect->m_rect.m_LeftTop.y = m_AreaRect.m_LeftTop.y;
				//m_pCurOperatorRect->m_rect.m_RightBottom.x = m_AreaRect.m_RightBottom.x;
				//m_pCurOperatorRect->m_rect.m_RightBottom.y = m_AreaRect.m_RightBottom.y;//
			}

			if (!m_bLocalArea)
			{
				m_RectShowMap[rect] = pRectInfo;
				// ������ʾ������
				pRectInfo->SetParentSceneNode(m_pSceneNode);
			}
			else
			{
				m_LocalRectShowMap[rect] = pRectInfo;
				// ������ʾ������
				pRectInfo->SetParentSceneNode(m_pLocalSceneNode);
			}
			pRectInfo->SetSceneManipulator(getSceneManipulator());
			pRectInfo->CreateSceneNode();

			// Ҫ������Ⱦ��������Ȼ��, release�»س�������.
			
			pRectInfo->CreateRenderShape();
			BuildEventAreaRect();
			pRectInfo->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);

			getSceneManipulator()->_fireAddNewRect(0, rect);

		}
	}
	

	// �������ε������ק����
	void CEventAreaEditAction::DoDrawLBnDrag(const Point& pt)
	{
		Ogre::Vector3 TerrainPosition;
		if(GetTerrainPos(pt, TerrainPosition))
		{
			m_CurPoint = TerrainPosition;
			NormalRect();
			
			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_rect = m_AreaRect;
				//m_pCurOperatorRect->m_rect.m_LeftTop.x = m_AreaRect.m_LeftTop.x;
				//m_pCurOperatorRect->m_rect.m_LeftTop.y = m_AreaRect.m_LeftTop.y;
				//m_pCurOperatorRect->m_rect.m_RightBottom.x = m_AreaRect.m_RightBottom.x;
				//m_pCurOperatorRect->m_rect.m_RightBottom.y = m_AreaRect.m_RightBottom.y;//
			}
			BuildEventAreaRect();

		}//
	}

	
	// ��ʼ���߼�����
	void CEventAreaEditAction::InitLogicData()
	{
		m_iEditRectId    = 0;
		m_iLocalEditRectId = 0;
		m_pCurOperatorRect = NULL;
		m_iOperator = SELECT;

		m_iCurEditEventAreaId = 0;
		m_iCurEditScriptid    = 0;
		m_iMaxEventAreaId     = 0;
		m_iMaxLocalEventAreaId = 0;
	}

	// ���ݾ��δ�����ʾ���¼�����
	void CEventAreaEditAction::BuildEventAreaRect()
	{
		if(NULL == m_pCurOperatorRect)
		{
			return;
		}

		m_pCurOperatorRect->ClearRenderShape();

		float fStepX = 100.0f;
		float fStepZ = 100.0f;
		int   iStartX = m_AreaRect.m_LeftTop.x;
		int   iStartZ = m_AreaRect.m_LeftTop.y;
		int   iEndX   = m_AreaRect.m_RightBottom.x;
		int   iEndZ   = m_AreaRect.m_RightBottom.y;

		float fx = 0;
		float fz = 0;
			
		Ogre::Vector3 NewPoint;
		for(fz = iStartZ; fz < iEndZ; fz += fStepZ)
			for(fx = iStartX; fx < iEndX; fx += fStepX)
			{
				// ������1
				NewPoint.x = fx;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);

				NewPoint.x = fx;
				NewPoint.z = fz + fStepZ;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);


				NewPoint.x = fx + fStepX;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);

			
				// ������2
				NewPoint.x = fx + fStepX;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);

				NewPoint.x = fx;
				NewPoint.z = fz + fStepZ;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);

				NewPoint.x = fx + fStepX;
				NewPoint.z = fz + fStepZ;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);
			}

		m_pCurOperatorRect->m_pRectRenderShape->FillPointData();
		m_pCurOperatorRect->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
		m_pCurOperatorRect->m_pRectRenderShape->setMaterial("EventAreaEdit/SelectRect");
		
	}
		// �������ε����̧�����
	void CEventAreaEditAction::DoDrawLBnUp(const Point& pt)
	{
		if(m_pCurOperatorRect)
		{
			m_pCurOperatorRect->m_pRectRenderShape->setMaterial("EventAreaEdit/PreSelectRect");
			
			// ���redo undo
			CEventAreaEditAddOperator* pAddyOperator = NULL;
			pAddyOperator = new CEventAreaEditAddOperator;

			pAddyOperator->SetAction(this);
			pAddyOperator->SetInstanceId(m_bLocalArea?m_iLocalEditRectId:m_iEditRectId, m_bLocalArea);
			pAddyOperator->SetPosition(m_PrePoint, m_CurPoint);//
			getOperatorManager()->addOperator(pAddyOperator);//
		}
	}


	// �����Ϣ�б�
	void CEventAreaEditAction::ReleaseRectInfoMap()
	{
		
		CEventAreaInfo* pEventAreaInfo = NULL;
		SHOW_RECT_MAP::iterator itEventAreaEnd;
		SHOW_RECT_MAP::iterator it;

		itEventAreaEnd = m_RectShowMap.end();
		for(it = m_RectShowMap.begin(); it != itEventAreaEnd; it++)
		{
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				delete pEventAreaInfo;					
			}
		}

		m_RectShowMap.clear();

		// Clear local event area map
		itEventAreaEnd = m_LocalRectShowMap.end();
		for(it = m_LocalRectShowMap.begin(); it != itEventAreaEnd; it++)
		{
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				delete pEventAreaInfo;					
			}
		}

		m_LocalRectShowMap.clear();

		m_pCurOperatorRect = NULL;

	}

	// ���ݾ���idɾ��һ������
	void CEventAreaEditAction::DeleteRect(int iId)
	{
		SHOW_RECT_MAP::iterator itEventAreaEnd;
		SHOW_RECT_MAP::iterator it;
		SHOW_RECT_MAP::iterator endIt;

		if (!m_bLocalArea)
		{
			it = m_RectShowMap.find(iId);
			endIt = m_RectShowMap.end();
		}
		else
		{
			it = m_LocalRectShowMap.find(iId);
			endIt = m_LocalRectShowMap.end();
		}

		Ogre::Vector3	p1;
		Ogre::Vector3   p2;

		Ogre::StringVector nameVector;
		Ogre::StringVector valueVector;

		int iRectid = 0;
		if(it != endIt)
		{
			CEventAreaInfo* pEventAreaInfo = NULL;
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				iRectid = pEventAreaInfo->m_iId;
				p1.x = pEventAreaInfo->m_rect.m_LeftTop.x;
				p1.z = pEventAreaInfo->m_rect.m_LeftTop.y;

				p2.x = pEventAreaInfo->m_rect.m_RightBottom.x;
				p2.z = pEventAreaInfo->m_rect.m_RightBottom.y;

				if (m_bLocalArea)
				{
					for (int i = 0; i<10; ++i)
					{
						Ogre::String paramName = pEventAreaInfo->m_paramsName[i];
						Ogre::String paramValue = pEventAreaInfo->m_paramsValue[i];
						if (!paramName.empty() && !paramValue.empty())
						{
							nameVector.push_back(paramName);
							valueVector.push_back(paramValue);
						}
					}
				}	

				delete pEventAreaInfo;
			}
			
			m_bLocalArea?(m_LocalRectShowMap.erase(it)):(m_RectShowMap.erase(it));

			getSceneManipulator()->_fireDelRect(0, iRectid);

			// ɾ��redo undo
			CEventAreaEditDelOperator* pDelOperator = NULL;
			pDelOperator = new CEventAreaEditDelOperator;

			pDelOperator->SetAction(this);
			pDelOperator->SetInstanceId(iRectid, m_bLocalArea);
			pDelOperator->SetPosition(p1, p2);//

			for (int index = 0; index<(int)nameVector.size(); ++index)
			{
				pDelOperator->m_paramName.push_back(nameVector[index]);
				pDelOperator->m_paramValue.push_back(valueVector[index]);
			}
			
			getOperatorManager()->addOperator(pDelOperator);//

			m_pCurOperatorRect = NULL;
			
		}

		
	}

	// ����redo undo ������ɾ��
	void CEventAreaEditAction::DeleteRectNoOperator(int iId,  bool bLocal)
	{
		SHOW_RECT_MAP::iterator itEventAreaEnd;
		SHOW_RECT_MAP::iterator it;
		SHOW_RECT_MAP::iterator endIt;

		if (!bLocal)
		{
			it = m_RectShowMap.find(iId);
			endIt = m_RectShowMap.end();
		}
		else
		{
			it = m_LocalRectShowMap.find(iId);
			endIt = m_LocalRectShowMap.end();
		}

		int iRectid = 0;
		if(it != endIt)
		{
			CEventAreaInfo* pEventAreaInfo = NULL;
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				iRectid = pEventAreaInfo->m_iId;
				delete pEventAreaInfo;
			}
			
			if (!m_bLocalArea)
			{
				m_RectShowMap.erase(it);
			}
			else
			{
				m_LocalRectShowMap.erase(it);
			}
			
			getSceneManipulator()->_fireDelRect(0, iRectid);

			m_pCurOperatorRect = NULL;
			
		}

		
	}


	// ���һ������.
	int CEventAreaEditAction::AddRect(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos, Ogre::StringVector nameVector, Ogre::StringVector valueVector, int iId, bool bLocal)
	{
		int iReturnId= 0;
		if(-1 == iId)
		{
			int rect = bLocal?(++m_iLocalEditRectId):(++m_iEditRectId);
			m_PrePoint = vectorUnDoPos;
			m_CurPoint = vectorReDoPos;
			NormalRect();
			
			CEventAreaInfo* pRectInfo = new CEventAreaInfo;
			pRectInfo->m_iEventAreaId = m_iCurEditEventAreaId;
			pRectInfo->m_iScriptId    = m_iCurEditScriptid;
			pRectInfo->m_iId          = rect;

			for (int index = 0; index<(int)nameVector.size(); ++index)
			{
				pRectInfo->m_paramsName[index] = nameVector[index];
				pRectInfo->m_paramsValue[index] = valueVector[index];
			}

			// ������Ϣ
			bLocal?(m_LocalRectShowMap[rect] = pRectInfo):(m_RectShowMap[rect]   = pRectInfo);
			
			m_pCurOperatorRect = pRectInfo;
		
			pRectInfo->m_rect = m_AreaRect;
			// ������ʾ������
			pRectInfo->SetParentSceneNode(bLocal?m_pLocalSceneNode:m_pSceneNode);
			pRectInfo->SetSceneManipulator(getSceneManipulator());
			pRectInfo->CreateSceneNode();
			pRectInfo->CreateRenderShape();
			pRectInfo->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
			BuildEventAreaRect();
			m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
					
			iReturnId = rect;
			if (bLocal == m_bLocalArea)
			{
				getSceneManipulator()->_fireAddNewRect(0, iReturnId);
			}
			/*else
			{
				pRectInfo->m_pSceneNode->setVisible(false);
			}*/
		}
		else
		{
			m_PrePoint = vectorUnDoPos;
			m_CurPoint = vectorReDoPos;
			NormalRect();
			
			CEventAreaInfo* pRectInfo = new CEventAreaInfo;
			pRectInfo->m_iEventAreaId = m_iCurEditEventAreaId;
			pRectInfo->m_iScriptId    = m_iCurEditScriptid;
			pRectInfo->m_iId          = iId;

			for (int index = 0; index<(int)nameVector.size(); ++index)
			{
				pRectInfo->m_paramsName[index] = nameVector[index];
				pRectInfo->m_paramsValue[index] = valueVector[index];
			}

			// ������Ϣ		
			bLocal?(m_LocalRectShowMap[iId] = pRectInfo):(m_RectShowMap[iId] = pRectInfo);

			m_pCurOperatorRect = pRectInfo;
		
			pRectInfo->m_rect = m_AreaRect;
			// ������ʾ������
			pRectInfo->SetParentSceneNode(bLocal?m_pLocalSceneNode:m_pSceneNode);
			pRectInfo->SetSceneManipulator(getSceneManipulator());
			pRectInfo->CreateSceneNode();
			pRectInfo->CreateRenderShape();
			pRectInfo->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
			BuildEventAreaRect();
			m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect

			iReturnId = iId;
			if (bLocal == m_bLocalArea)
			{					
				getSceneManipulator()->_fireAddNewRect(0, iReturnId);
			}	
			/*else
			{
				pRectInfo->m_pSceneNode->setVisible(false);
			}*/
		}
			
		return iReturnId;

	}

	// ѡ��һ������
	void CEventAreaEditAction::SelectEventAreaRect(int iId)
	{
		
		if(!m_bLocalArea && m_RectShowMap.count(iId))
		{
			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
			}

			CEventAreaInfo* pRectInfo = NULL;
			pRectInfo = m_RectShowMap[iId];
			m_pCurOperatorRect = pRectInfo;

			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("EventAreaEdit/SelectRect");//fujia EventAreaEdit/SelectRect
			}

		}
		else if(m_bLocalArea && m_LocalRectShowMap.count(iId))
		{
			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
			}

			CEventAreaInfo* pRectInfo = NULL;
			pRectInfo = m_LocalRectShowMap[iId];
			m_pCurOperatorRect = pRectInfo;

			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("EventAreaEdit/SelectRect");//fujia EventAreaEdit/SelectRect
			}
		}
	}

	// ��ѡ����ε����̧�����
	void CEventAreaEditAction::DoSelectLBnUp(const Point& pt)
	{
		Ogre::Vector3 TerrainPosition;
		if(GetTerrainPos(pt, TerrainPosition))
		{

			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
			}

			CEventAreaInfo* pEventAreaInfo = NULL;
			SHOW_RECT_MAP::iterator itEventAreaEnd;
			SHOW_RECT_MAP::iterator it;
			if (!m_bLocalArea)
			{
				itEventAreaEnd = m_RectShowMap.end();
				it = m_RectShowMap.begin();
			}
			else
			{
				itEventAreaEnd = m_LocalRectShowMap.end();
				it = m_LocalRectShowMap.begin();
			}
			for(; it != itEventAreaEnd; it++)
			{
				pEventAreaInfo = it->second;
				if(pEventAreaInfo)
				{
					if( (TerrainPosition.x > pEventAreaInfo->m_rect.m_LeftTop.x)&&
						(TerrainPosition.x < pEventAreaInfo->m_rect.m_RightBottom.x)&&
						(TerrainPosition.z > pEventAreaInfo->m_rect.m_LeftTop.y)&&
						(TerrainPosition.z < pEventAreaInfo->m_rect.m_RightBottom.y))
					{
						m_pCurOperatorRect = pEventAreaInfo;
						m_pCurOperatorRect->m_pRectRenderShape->setMaterial("EventAreaEdit/SelectRect");
						break;
					}
				}
			}
			
			if(m_pCurOperatorRect)
			{
				getSceneManipulator()->_fireSelRect(0, m_pCurOperatorRect->m_iId);
			}

		}
	}



	// ��������
	void CEventAreaEditAction::ModifyEventArea(int iInstanceId, int iEventId, int iScripId)
	{
		if (m_bLocalArea)
		{
			if(0 == m_LocalRectShowMap.count(iInstanceId))
			{
				return;
			}


			CEventAreaInfo* pEventAreaInfo = m_LocalRectShowMap[iInstanceId];
			if(pEventAreaInfo)
			{
				pEventAreaInfo->m_iEventAreaId  = iEventId;
				pEventAreaInfo->m_iScriptId     = iScripId;

			}
		}
		else
		{
			if(0 == m_RectShowMap.count(iInstanceId))
			{
				return;
			}


			CEventAreaInfo* pEventAreaInfo = m_RectShowMap[iInstanceId];
			if(pEventAreaInfo)
			{
				pEventAreaInfo->m_iEventAreaId  = iEventId;
				pEventAreaInfo->m_iScriptId     = iScripId;

			}
		}
	}


}
