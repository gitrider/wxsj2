#include "PathFindAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "UnreachRegion.h"
#include "CustomShape.h"
#include "PathFindOperator.h"	
#include "WXOperatorManager.h"       // redo undo ������.
#include "ScanLine.h"
#include "OgreCamera.h"

#include "Core/TerrainData.h"
#include "Core/FairyResources.h"
#include "RegionEditAction.h"

#include <windows.h>

namespace Fairy 
{
	using namespace Ogre;

	CPathFindAction::CPathFindAction(SceneManipulator* sceneManipulator)
		: Action(sceneManipulator)
		,m_pSceneNode(NULL)
		,m_pMaterialPoint(NULL)
		,m_pMaterialPointSel(NULL)
		,m_pMaterialPointTemp(NULL)
		,m_pMaterialLine(NULL)	
		,m_pMaterialLineSel(NULL)
		,m_pMaterialRegion(NULL)	
		,m_pMaterialRegionSel(NULL)
		,m_ulCurMaxId(0)
		,m_ulCurEditRegionId(0)
	{

		// ��ʼ��
		InitPathFind();

		// ����ʹ��.
		//Test_AddRegion();
		m_bIsShow = false;

		//m_iPreOperatorType = NO_OPERATOR;

	}

	CPathFindAction::~CPathFindAction(void)
	{
		ClearRegionData();

		if (m_pSceneNode)
		{
			m_pSceneNode->destroy();
		}
	}

	const String& CPathFindAction::getName(void) const
	{
		//����һ���ַ������֣� �����жϵ�ǰ����������.
		static const String name = "PathFind";
		return name;
	}


	// ����ƶ��Ĳ���
	void CPathFindAction::_onMove(const Point& pt)
	{
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		ClearFuncKey();

	}


	// ��갴�µĲ���.
	void CPathFindAction::_onBegin(const Point& pt)
	{
		// ��¼��ǰ������ �ĵ��λ��.
		RecordPreDoPointPos();
	}


	void CPathFindAction::_onDrag(const Point& pt)
	{
		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
			// ���shift�����������޸Ĳ�������������λ��.
			ChangeCurEditPointPos(pt);
			//m_iPreOperatorType = MODITY_POINT_POSITION;
		}
		else//
		{
			getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		}

		// ÿ��ִ�н�����չ��ܼ�
		ClearFuncKey();

	}


	// ���̧��Ĳ���
	void CPathFindAction::_onEnd(const Point& pt, bool canceled)
	{
		if(!getActive())
		{
			return;
		}

		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
			// ���shift�����������޸Ĳ�������������λ��.
			CPathFindEditModifyOperator* pPathFindModifyOperator = NULL;
			pPathFindModifyOperator = new CPathFindEditModifyOperator;

			pPathFindModifyOperator->SetRegionAction(this);
			pPathFindModifyOperator->SetPointId(m_ulCurEditPointId);
			pPathFindModifyOperator->SetPosition(m_preDoPointPos, m_curPointPos);//
			getOperatorManager()->addOperator(pPathFindModifyOperator);//

		}
		else if(m_FuncKeyState & FUNC_KEY_CTRL)
		{
			UNREACHREGIONMAP::iterator it;
			UNREACHREGIONMAP::iterator itBegin;
			UNREACHREGIONMAP::iterator itEnd;
			CUnreachRegion* pUnReachRegion = NULL;

			itEnd  = m_UnreachRegionMap.end();
			for(it = m_UnreachRegionMap.begin();  it != itEnd; it++)
			{
				pUnReachRegion = it->second;

				if(pUnReachRegion)
				{
					if(pUnReachRegion->IsMouseSel(pt.x, pt.y))
					{
						// ѡ��
						UnSelRegion(m_ulCurEditRegionId);
						SelRegion(pUnReachRegion->m_ulId, false);

						// ֪ͨ����
						getSceneManipulator()->_fireRegionSel(pUnReachRegion->m_ulId);
						break;

					}
				}
			}

			// û��ѡ��
		}
		else
		{
			// ���̧��ʱ�Ե�Ĳ���.
			if(EditPoint(pt))
			{
				CPathFindEditAddOperator* pAddNewPointOperator = NULL;
				pAddNewPointOperator = new CPathFindEditAddOperator;

				pAddNewPointOperator->SetRegionAction(this);
				pAddNewPointOperator->SetPointId(m_ulCurEditPointId);
				pAddNewPointOperator->SetPosition(m_preDoPointPos, m_curPointPos);//
				getOperatorManager()->addOperator(pAddNewPointOperator);

			}
		}


		// ��չ��ܼ�
		ClearFuncKey();

	}
	void CPathFindAction::_onActive(bool active)
	{

		if (!active)
		{
			IntersectPoint* pPoint = (IntersectPoint*)getSceneManipulator()->getHitIndicator("IntersectPoint");
			pPoint->SetMaterial();
			pPoint->hide();
		}
		else
		{
			IntersectPoint* pPoint = (IntersectPoint*)getSceneManipulator()->getHitIndicator("IntersectPoint");
			String strMaterialName = "Region/PointTemp";
			pPoint->SetMaterial(strMaterialName);//
		}
	}


	////////////////////////////////////////////////////////////////////////////////////
	//
	//npcѲ���������
	//

	// ���һ���µ�����
	unsigned long CPathFindAction::AddNewRegion()
	{
		if( m_UnreachRegionMap.size() > 0 )
		{
			//ShowUnreachRegion();
			return m_ulCurEditRegionId;
		}

		m_ulCurMaxId++;
		unsigned long ulId = m_ulCurMaxId;

		CUnreachRegion* pRegion = NULL;
		pRegion = new CUnreachRegion(ulId, m_pSceneNode, getSceneManipulator());

		if(pRegion)
		{
			CRegionEditAction* pRegionAction = (CRegionEditAction*)getSceneManipulator()->_getAction("RegionEdit");
			pRegion->m_pLineShape1->SetRegion( pRegionAction->GetUnreachRegionMap() );

			m_UnreachRegionMap[ulId] = pRegion;
			m_ulCurEditRegionId = ulId;
			return m_ulCurEditRegionId;
		}

		return 0xfffffff;
	}

	void CPathFindAction::ShowUnreachRegion()
	{
		if( m_UnreachRegionMap.size() == 0 ) return;

		CUnreachRegion* pRegion = m_UnreachRegionMap[m_ulCurEditRegionId];
		if( pRegion )
		{
			pRegion->m_pLineShape1->FillAreaPointData();
		}
	}

	// ͨ��ָ����id�� ɾ��һ������
	unsigned long CPathFindAction::DelRegion(unsigned long ulId)
	{

		CUnreachRegion* pRegion = NULL;
		if(m_UnreachRegionMap.count(ulId))
		{
			pRegion = m_UnreachRegionMap[ulId];
			if(pRegion)
			{
				if(m_pSceneNode)
				{
					pRegion->DetachAllEntity();

					pRegion->DestroyAllChildSceneNode();
					m_pSceneNode->removeAndDestroyChild(pRegion->m_pSceneNode->getName());
				}

				delete pRegion;

				UNREACHREGIONMAP::iterator it;
				it = m_UnreachRegionMap.find(ulId);
				m_UnreachRegionMap.erase(it);

			}

			return 0;
		}

		return 0;
	}


	// ����ָ����id�� �õ�����
	CUnreachRegion* CPathFindAction::GetRegionById(unsigned long ulId)
	{
		if( m_UnreachRegionMap.size() > 0 )
		{
			CUnreachRegion* pRegion = m_UnreachRegionMap[m_ulCurEditRegionId];
			return pRegion;
		}

		return NULL;
	}


	void CPathFindAction::InitPathFind()
	{
		CreateSceneNode();
	}

	void CPathFindAction::SetOldHitPointMaterial()
	{
		IntersectPoint* pPoint = (IntersectPoint*)getSceneManipulator()->getHitIndicator("IntersectPoint");
		pPoint->SetMaterial();
	}

	// ��������༭�ĳ����ڵ�
	void CPathFindAction::CreateSceneNode()
	{
		m_pSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(false);
		}

	}

	// �ڵ�ǰ�༭�����������һ���µĵ�
	unsigned long CPathFindAction::AddNewPoint(Ogre::Vector3 position)
	{
		if(0 == m_UnreachRegionMap.count(m_ulCurEditRegionId))
		{
			return 0 ;
		}

		CUnreachRegion* pRegion = NULL;

		pRegion = m_UnreachRegionMap[m_ulCurEditRegionId];
		if(pRegion)
		{

			m_ulCurEditPointId = pRegion->AddNewPoint1(position);
			return m_ulCurEditPointId;
		}

		return 0;

	}

	bool CPathFindAction::EditPoint(const Point& pt)
	{
		Ogre::Vector3 position;
		unsigned long ulId = 0;

		bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);
		if(hit)
		{
			ulId = AddNewPoint(position);
			if(0 == ulId)
			{
				return false;
			}

			m_preDoPointPos = position;
			m_curPointPos   = position;

			getSceneManipulator()->_fireAddDijkPoint(ulId, position);

			return true;
		}

		return false;
	}

	void CPathFindAction::HideScene(bool bHide)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(bHide);
			m_bIsShow = bHide;
		}
	}

	void CPathFindAction::ClearRegionData()
	{
		UNREACHREGIONMAP::iterator it;
		UNREACHREGIONMAP::iterator itEnd;

		CUnreachRegion* pRegion = NULL;
		itEnd = m_UnreachRegionMap.end();
		for(it = m_UnreachRegionMap.begin(); it != itEnd; it++)
		{
			pRegion = it->second;
			if(pRegion)
			{
				// ��������ڵ����
				if(m_pSceneNode)
				{
					// ɾ��������������ʵ��.
					pRegion->DetachAllEntity();

					// ɾ������������ӽڵ�
					pRegion->DestroyAllChildSceneNode();

					// ɾ�������ڵ�������ڵ�.
					m_pSceneNode->removeAndDestroyChild(pRegion->m_pSceneNode->getName());
					

				}

				delete pRegion;
			}
		}

		m_UnreachRegionMap.clear();

		m_ulCurMaxId = 0;

	}

	// ѡ��һ������
	void CPathFindAction::SelRegion(unsigned long ulId, bool bIsChangeCamera)
	{
		if(m_UnreachRegionMap.count(ulId))
		{
			m_ulCurEditRegionId = ulId;
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];
			pRegion->SetRegionSelMaterial1();

			if(bIsChangeCamera)
			{
				if((pRegion->m_pLineShape)&&(pRegion->m_pLineShape->m_pointArray.size()))
				{

					Ogre::Vector3 point = pRegion->m_pLineShape->m_pointArray[0];
					SetCamera(point.x, point.z);
				}
			}
		}
	}

	// ��ѡ��һ������.
	void CPathFindAction::UnSelRegion(unsigned long ulId)
	{
		if( m_UnreachRegionMap.size() > 0 )
		{
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];
			pRegion->SetRegionNorMaterial1();
		}
	}
	// �������ȡ�ļ�
	unsigned long CPathFindAction::ReadRegionFromFile(std::string& strFileName)
	{
		FILE* fp = fopen( strFileName.c_str(), "rb" );
		if( !fp ) return 0xfffffffe;

		char buffer[20];
		fread( buffer, 1, 20, fp );
		fclose( fp );
		if( strncmp( buffer, DIJKSTRA_EDIT_VER1, strlen(DIJKSTRA_EDIT_VER1) ) == 0 )
		{
			return ReadRegionFromFileVer0( strFileName ); //
		}
		else
		{
			return 0;
		}
	}

	// �������ȡ�ļ�
	unsigned long CPathFindAction::ReadRegionFromFileVer0(std::string& strFileName)
	{
		FILE* pFile = fopen( strFileName.c_str(), "rb" );

		if(NULL == pFile)
		{
			// ���ܴ��ļ�.
			return 0xfffffffe;
		}

		// �����ǰ������.
		ClearRegionData();
		int			iVerLen = (int)sizeof(DIJKSTRA_EDIT_VER1);
		char*		pVer = new char[iVerLen];
		fread( pVer, 1, iVerLen, pFile );
		delete [] pVer;

		int			iRegionCount = 0;
		int			iPointCount  = 0;
		int         iId			 = 0;
		unsigned long ulId       = 0;
		CUnreachRegion* pRegion  = NULL;
		Ogre::Vector3   position;


		////////////////////////////////////////////////////////////////////
		//
		//  ����ת������
		//

		//SaveX
		float x = 0;
		//SaveZ
		float z = 0;

		// ����ϵ��
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;

		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}


		//����ĸ���
		iRegionCount = 1;
		for(int i = 0; i < iRegionCount; i++)
		{
			// �����id
			iId = 1;

			// ���õ�ǰ���ڱ༭��id��
			m_ulCurEditRegionId = AddNewRegion();

			// ��ȡ��ĸ���.
			::fread(&iPointCount, sizeof(iPointCount), 1, pFile);
			for(int k = 0; k < iPointCount; k++)
			{
				// ��ȡx�� z ����
				::fread(&position.x, sizeof(position.x), 1, pFile);
				::fread(&position.z, sizeof(position.z), 1, pFile);
				::fread(&position.y, sizeof(position.y), 1, pFile);

				position.x = position.x * fScale + fTerrainDataPosX;
				position.z = position.z * fScale + fTerrainDataPosZ;

				AddNewPoint(position);
				getSceneManipulator()->_fireAddDijkPoint(k+1, position);
			}

			//if( i != 133)
			//{
			//	// ˢ���µ�����
				ReshapeArea(m_ulCurEditRegionId);
			//}

		}

		::fclose(pFile);

		// ������ʾ״̬�� ��ʾ����.
		HideScene(m_bIsShow);

		return 0;
	}

	// ������洢���ļ�
	unsigned long CPathFindAction::SaveRegionToFile(std::string strFileName)
	{

		FILE* pFile = NULL;
		pFile = ::fopen(strFileName.data(), "wb");
		
		if(NULL == pFile)
		{
			// ���ܴ��ļ�.
			return 0xfffffffe;
		}
		::fwrite(DIJKSTRA_EDIT_VER1, sizeof(DIJKSTRA_EDIT_VER1), 1, pFile);
		int			iRegionCount	 = 0;
		int			iPointCount		 = 0;
		int         iId			 = 0;
		unsigned long ulId			 = 0;

		CUnreachRegion* pPathfind		 = NULL;
		CPointInfo*		pPoint		 = NULL;
		Ogre::Vector3   position;

		UNREACHREGIONMAP::iterator itRegion;
		UNREACHREGIONMAP::iterator itEndRegion;

		POINT_MAP::iterator itPoint;
		POINT_MAP::iterator itEndPoint;

		// �洢����ĸ���
		iRegionCount = m_UnreachRegionMap.size();
		

		// �洢ÿ���������Ϣ
		itEndRegion = m_UnreachRegionMap.end();

		///////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ����ѳ�������ת������Ϸ�߼�����.
		//

		//SaveX
		float x = 0;
		//SaveZ
		float z = 0;

		// ����ϵ��
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;

		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}

		for(itRegion = m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
		{
			pPathfind = itRegion->second;
			if(pPathfind)
			{
				// �洢��ĸ���.
				iPointCount = pPathfind->m_pointMap1.size();
				::fwrite(&iPointCount, sizeof(iPointCount), 1, pFile);

				// �洢ÿ�������Ϣ��
				itEndPoint = pPathfind->m_pointMap1.end();
				for(itPoint = pPathfind->m_pointMap1.begin(); itPoint != itEndPoint; itPoint++)
				{
					pPoint = itPoint->second;
					if(pPoint)
					{
						position  = pPoint->m_Position;
						x = position.x;
						z = position.z;

						x = (x - fTerrainDataPosX) / fScale;
						z = (z - fTerrainDataPosZ) / fScale;

						::fwrite(&x, sizeof(x), 1, pFile);
						::fwrite(&z, sizeof(z), 1, pFile);
						::fwrite(&position.y, sizeof(position.y), 1, pFile);
					}
				}

				float* JuLi = pPathfind->m_pLineShape1->m_Weights;
				::fwrite(JuLi, sizeof(float),iPointCount*iPointCount, pFile);

			}
		}
		
		::fclose(pFile);
		return 0;
	}


	// ���ݵ��εĸ߶ȸı�༭������
	void CPathFindAction::SetRegionHeight()
	{
		UNREACHREGIONMAP::iterator it;
		UNREACHREGIONMAP::iterator itEnd;

		CUnreachRegion* pUnreachRegion = NULL;

		itEnd = m_UnreachRegionMap.end();
		for(it = m_UnreachRegionMap.begin(); it != itEnd; it++)
		{
			pUnreachRegion = it->second;
			if(pUnreachRegion)
			{
				pUnreachRegion->SetPointHeight1(getTerrainData());
				pUnreachRegion->SetLineHeight1(getTerrainData());
			}
		}
	}


	void CPathFindAction::SetCamera(float fx, float fy)
	{
		const Ogre::Vector3 OldPos = getSceneManipulator()->getCamera()->getPosition();
		Ogre::Vector3 NewPos = OldPos;
		NewPos.x = fx;
		NewPos.z = fy;
		getSceneManipulator()->getCamera()->setPosition(NewPos);
	}

	void CPathFindAction::SetCamera()
	{

	}

	// ѡ��һ����.
	void CPathFindAction::SelPoint(unsigned long ulPointId)
	{
		m_ulCurEditPointId = ulPointId;

		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// �õ���ǰ���ڱ༭���������Ϣ.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pRegion->SetPrePointMaterial();
			pRegion->m_ulCurEditPointId = m_ulCurEditPointId;
			pPoint = pRegion->GetPointInfoById1(ulPointId);
			if(pPoint)
			{
				pPoint->SetSelMaterial();
			}
		}
	}


	// ���ð��µĹ��ܼ�
	void CPathFindAction::SetFuncKey(unsigned long ulFuncKey)
	{
		m_FuncKeyState |= ulFuncKey;
	}

	//  ��չ��ܼ�.
	void CPathFindAction::ClearFuncKey()
	{
		m_FuncKeyState = FUNCTION_KEY;
	}


	void CPathFindAction::setParameter(const String& name, const String& value)
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
			else if("ATL")
			{
				SetFuncKey(FUNC_KEY_ALT);
			}
		}

	}

	// �ı䵱ǰ�༭�ĵ��λ��
	void CPathFindAction::ChangeCurEditPointPos(const Point& pt)
	{
		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// �õ���ǰ���ڱ༭���������Ϣ.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pPoint = pRegion->GetPointInfoById1(m_ulCurEditPointId);
			if(pPoint)
			{	

				// �����������±�ֵ.
				int iIndex = -1;
				POINT_MAP::iterator it;
				POINT_MAP::iterator itEnd;
				itEnd = pRegion->m_pointMap.end();
				for(it = pRegion->m_pointMap.begin(); it != itEnd; it++)
				{
					iIndex++;
					if(m_ulCurEditPointId == it->first)
					{
						break;
					}
				}


				// �õ��޸ĺ��λ��.
				Ogre::Vector3 position;
				bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);
				if(hit)
				{

					//��ʱ���ж��ܷ����һ����ȥ��.
					//if(pRegion->CanChangePointPos(iIndex, position))
					{
						// �޸��µĵ��λ��.
						pPoint->SetPos(position);

						// ��ǰ�༭�ĵ��λ��.
						m_curPointPos = position;

						if(it != itEnd)
						{
							// ����±����
							if((-1 != iIndex)&&(iIndex < (int)pRegion->m_pLineShape->m_pointArray.size()))
							{
								pRegion->m_pLineShape->m_pointArray[iIndex] = position;
								pRegion->m_pLineShape->FillPointData1();
								//pRegion->ReShapeArea1();
							}
						}

					}
				}
			}
		}
	}

	void CPathFindAction::ChangeCurEditPointPos(unsigned long ulPointId, Ogre::Vector3 position)
	{
		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// �õ���ǰ���ڱ༭���������Ϣ.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pPoint = pRegion->GetPointInfoById1(ulPointId);
			if(pPoint)
			{	

				// �����������±�ֵ.
				int iIndex = -1;
				POINT_MAP::iterator it;
				POINT_MAP::iterator itEnd;
				itEnd = pRegion->m_pointMap.end();
				for(it = pRegion->m_pointMap.begin(); it != itEnd; it++)
				{
					iIndex++;
					if(ulPointId == it->first)
					{
						break;
					}
				}

				pPoint->SetPos(position);

				// ����±����
				if((-1 != iIndex)&&(iIndex < (int)pRegion->m_pLineShape->m_pointArray.size()))
				{
					pRegion->m_pLineShape->m_pointArray[iIndex] = position;
					pRegion->m_pLineShape->FillPointData1();
				}

			}

		}

	}

	// ��¼��ǰ����ǰһ�ε��λ��.
	void CPathFindAction::RecordPreDoPointPos()
	{

		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// �õ���ǰ���ڱ༭���������Ϣ.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pPoint = pRegion->GetPointInfoById1(m_ulCurEditPointId);
			if(pPoint)
			{	
				m_preDoPointPos = pPoint->m_Position;		
			}
		}
	}

	// ָ������id�� ָ�����id�� ָ��λ�ã� ���һ��С��.
	void CPathFindAction::AddNewPoint(unsigned long ulPointId, Ogre::Vector3 position)
	{
		// �õ���ǰ���ڱ༭���������Ϣ.
		CUnreachRegion* pRegion = NULL;
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pRegion->AddNewPoint1(ulPointId, position);

			getSceneManipulator()->_fireAddDijkPoint(ulPointId, position);
		}
	}


	// ָ������id�� ָ�����id�� ָ��λ�ã� ɾ��һ��С��.
	void CPathFindAction::DelPoint(unsigned long ulPointId)
	{
		// �õ���ǰ���ڱ༭���������Ϣ.
		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pRegion->DelPointFromTail1(ulPointId);

			getSceneManipulator()->_fireDelDijkPoint(ulPointId);
		}
	}


	// ��ǰ�������Ƿ�Ϸ�
	bool CPathFindAction::IsRegionValidate(unsigned long ulRegionId)
	{
		CUnreachRegion* pRegion = NULL;

		// �õ���ǰ���ڱ༭���������Ϣ.
		pRegion = GetRegionById(ulRegionId);
		if(pRegion)
		{
			if(pRegion->IsRegionValidate())
			{
				return true;
			}
		}

		return false;
	}

	// ����һ����������
	void CPathFindAction::HideRegionArea(unsigned long ulRegionId, bool bHide)
	{
		CUnreachRegion* pRegion = NULL;

		// �õ���ǰ���ڱ༭���������Ϣ.
		pRegion = GetRegionById(ulRegionId);
		if(pRegion)
		{

			pRegion->HideAreaNode1(bHide);

		}
	}

	// ���´�����ʾ���������
	void CPathFindAction::ReshapeArea(unsigned long ulRegionId)
	{
		CUnreachRegion* pRegion = NULL;
		// �õ���ǰ���ڱ༭���������Ϣ.
		pRegion = GetRegionById(ulRegionId);
		if(pRegion)
		{

			pRegion->ReShapeArea1();

		}
	}


}
