#include "RegionEditAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "UnreachRegion.h"
#include "CustomShape.h"
#include "regioneditoperator.h"		// �������������redo undo ����.
#include "WXOperatorManager.h"       // redo undo ������.
#include "ScanLine.h"
#include "OgreCamera.h"
#include "OgreStringConverter.h"
#include "Core/TerrainData.h"
#include "Core/FairyResources.h"

namespace Fairy 
{
	using namespace Ogre;

	 CRegionEditAction::CRegionEditAction(SceneManipulator* sceneManipulator)
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
			,mRaySceneQuery(NULL)
			,mPlaneBoundedVolumeListSceneQuery(NULL)
			,mSelecting(false)
			,mBoxCenterMgr(sceneManipulator,getSceneManager())
    {
		
		// ��ʼ��
		InitRegionEdit();
		sceneManipulator->addSceneListener(this);
		// ����ʹ��.
		//Test_AddRegion();
		m_bIsShow = false;

		m_iPreOperatorType = NO_OPERATOR;

		m_bShowDeepBuf = true;

		// Create scene queries
		mRaySceneQuery = getSceneManager()->createRayQuery(Ogre::Ray());
		mRaySceneQuery->setSortByDistance(true);

		mPlaneBoundedVolumeListSceneQuery = getSceneManager()->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());
		
    }

	CRegionEditAction::~CRegionEditAction(void)
	{
		ClearRegionData();

        if (m_pSceneNode)
        {
            m_pSceneNode->getCreator()->destroySceneNode(m_pSceneNode);
			m_pSceneNode = NULL;
        }
		if (mRaySceneQuery)
		{
			getSceneManager()->destroyQuery(mRaySceneQuery);
		}
		if (mPlaneBoundedVolumeListSceneQuery)
		{
			getSceneManager()->destroyQuery(mPlaneBoundedVolumeListSceneQuery);
		}
	}

	const String& CRegionEditAction::getName(void) const
	{
		//����һ���ַ������֣� �����жϵ�ǰ����������.
		static const String name = "RegionEdit";
		return name;
	}

    
	// ����ƶ��Ĳ���
    void CRegionEditAction::_onMove(const Point& pt)
	{
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		ClearFuncKey();
		
	}


	// ��갴�µĲ���.
	void CRegionEditAction::_onBegin(const Point& pt)
	{
		if(m_FuncKeyState & FUNC_KEY_ALT )
		{
			mSelecting = true;
			ObjectPtr hitObject ;
			if(getSceneManipulator()->getUseBoxCenterSelection())
				hitObject = findFirstHitObjectByBoxCenter(pt).lock();
			else
				hitObject = findHitObject(pt);

		}
		else
		{
			mSelecting = false;
			// ��¼��ǰ������ �ĵ��λ��.
			RecordPreDoPointPos();

		}
		
	}


    void CRegionEditAction::_onDrag(const Point& pt)
	{
		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
			// ���shift�����������޸Ĳ�������������λ��.

			if(m_iPreOperatorType == ADD_NEW_POINT)//�ж��Ƿ��ڵ�༭״̬
			{
				ChangeCurEditPointPos(pt);
				//m_iPreOperatorType = MODITY_POINT_POSITION;
			}
		}
		else//
		{
			getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		}

		// ÿ��ִ�н�����չ��ܼ�
		ClearFuncKey();
			
	}


	// ���̧��Ĳ���
    void CRegionEditAction::_onEnd(const Point& pt, bool canceled)
	{
		if(!getActive())
		{
			return;
		}

		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
			// ���shift�����������޸Ĳ�������������λ��.

			if(m_iPreOperatorType == ADD_NEW_POINT)//�ж��Ƿ��ڵ�༭״̬
			{
				CRegionEditModifyOperator* pRegionEditModifyOperator = NULL;
				pRegionEditModifyOperator = new CRegionEditModifyOperator;

				pRegionEditModifyOperator->SetRegionAction(this);
				pRegionEditModifyOperator->SetPointId(m_ulCurSelectPointId);
				pRegionEditModifyOperator->SetRegionId(m_ulCurEditRegionId);
				pRegionEditModifyOperator->SetPosition(m_preDoPointPos, m_curPointPos);
				getOperatorManager()->addOperator(pRegionEditModifyOperator);
			}
			
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

						ChangeCurEditRegion(pUnReachRegion->m_ulId,true);
						m_iPreOperatorType = SELECT_REGION;

						break;
						
					}
				}
			}

			// û��ѡ��
		}
		else
		{
			if(!mSelecting)
			{
				// ���̧��ʱ�Ե�Ĳ���.
				if(EditPoint(pt))
				{
					CRegionEditAddPointOperator* pAddNewPointOperator = NULL;
					pAddNewPointOperator = new CRegionEditAddPointOperator;

					pAddNewPointOperator->SetRegionAction(this);
					pAddNewPointOperator->SetPointId(m_ulCurEditPointId);
					pAddNewPointOperator->SetRegionId(m_ulCurEditRegionId);
					pAddNewPointOperator->SetPosition(m_preDoPointPos, m_curPointPos);
					getOperatorManager()->addOperator(pAddNewPointOperator);

				}
				
			}
			mSelecting = false;
		}


		// ��չ��ܼ�
		ClearFuncKey();

	}
	//--------------------------------------------------------------------------------------------
    void CRegionEditAction::_onActive(bool active)
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

   //--------------------------------------------------------------------------------------
   void CRegionEditAction::onSelectObject(const ObjectPtr& object)
   {
 	  
		return;

   }

   ObjectWeakPtr CRegionEditAction::findFirstHitObjectByBoxCenter(const Point& pt)
   {
	   return mBoxCenterMgr.findHitObject(pt);
   }

   ObjectPtr CRegionEditAction::findHitObject(const Point& pt) const
   {
	   Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(pt);
	   mRaySceneQuery->setRay(ray);
	   const Ogre::RaySceneQueryResult& queryResult = mRaySceneQuery->execute();
	   for (Ogre::RaySceneQueryResult::const_iterator it = queryResult.begin(); it != queryResult.end(); ++it)
	   {
		   Ogre::MovableObject* movable = it->movable;
		   ObjectPtr object = getObjectFromMovable(movable);

		   if (object)
		   {
			   if (object->getType() == "REGION")
			   {
				   int pointid = object->getData();
				   Ogre::String regionId = 	object->getName();
				   unsigned long uRegionId = Ogre::StringConverter::parseUnsignedLong(regionId);
				   if(m_ulCurEditRegionId == uRegionId)
					   getSceneManipulator()->_fireRegionSelectPoint(uRegionId,pointid);
			   }
			
		   }
	   }

	   return ObjectPtr();
   }


	////////////////////////////////////////////////////////////////////////////////////
	//
	// �����������������
	//

	// ���һ���µ�����
	unsigned long CRegionEditAction::AddNewRegion()
	{
		m_ulCurMaxId++;
		unsigned long ulId = m_ulCurMaxId;

		CUnreachRegion* pRegion = NULL;
		pRegion = new CUnreachRegion(ulId, m_pSceneNode, getSceneManipulator());
		pRegion->SetRegionEditMaterial();

		if(pRegion)
		{
			m_UnreachRegionMap[ulId] = pRegion;
			m_ulCurEditRegionId = ulId;
			m_iPreOperatorType = ADD_NEW_POINT;
			return m_ulCurEditRegionId;
		}
		
		return 0xfffffff;
	}

	// ͨ��ָ����id�� ɾ��һ������
	unsigned long CRegionEditAction::DelRegion(unsigned long ulId)
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
					
					//pRegion->m_pSceneNode->removeAndDestroyAllChildren();
					pRegion->DestroyAllChildSceneNode();
					m_pSceneNode->removeAndDestroyChild(pRegion->m_pSceneNode->getName());
					//
					
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
	CUnreachRegion* CRegionEditAction::GetRegionById(unsigned long ulId)
	{

		if(m_UnreachRegionMap.count(ulId))
		{
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];

			return pRegion;
		}

        return NULL;
	}


	void CRegionEditAction::InitRegionEdit()
	{
		CreateSceneNode();
	}

	void CRegionEditAction::SetOldHitPointMaterial()
	{
		IntersectPoint* pPoint = (IntersectPoint*)getSceneManipulator()->getHitIndicator("IntersectPoint");
		pPoint->SetMaterial();
	}

	// ��������༭�ĳ����ڵ�
	void CRegionEditAction::CreateSceneNode()
	{
		m_pSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(false);
		}
		
	}

	// �ڵ�ǰ�༭�����������һ���µĵ�
	unsigned long CRegionEditAction::AddNewPoint(Ogre::Vector3 position)
	{
		if(0 == m_UnreachRegionMap.count(m_ulCurEditRegionId))
		{
			return 0 ;
		}

		CUnreachRegion* pRegion = NULL;

		pRegion = m_UnreachRegionMap[m_ulCurEditRegionId];
		if(pRegion)
		{
			
			m_ulCurEditPointId = pRegion->AddNewPoint(position);
			m_ulCurSelectPointId = m_ulCurEditPointId;
			return m_ulCurEditPointId;
		}

		return 0;
		
	}

	bool CRegionEditAction::EditPoint(const Point& pt)
	{
		Ogre::Vector3 position;
		unsigned long ulId = 0;

        bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);
		if(m_iPreOperatorType == ADD_NEW_POINT && hit)
		{
			ulId = AddNewPoint(position);
			if(0 == ulId)
			{
				return false;
			}

			m_preDoPointPos = position;
			m_curPointPos   = position;

			getSceneManipulator()->_fireRegionAddNewPoint(m_ulCurEditRegionId, ulId, position);
			return true;
		}

		return false;
	}

	void CRegionEditAction::Test_AddRegion()
	{
		AddNewRegion();
	}

	void CRegionEditAction::HideScene(bool bHide)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(bHide);
			m_bIsShow = bHide;
		}
	}

	void CRegionEditAction::ClearRegionData()
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
					//pRegion->m_pSceneNode->removeAndDestroyAllChildren();
					
					// ɾ������������ӽڵ�
					pRegion->DestroyAllChildSceneNode();

					// ɾ�������ڵ�������ڵ�.
					m_pSceneNode->removeAndDestroyChild(pRegion->m_pSceneNode->getName());
					//
					
				}

				delete pRegion;
			}
		}

		m_UnreachRegionMap.clear();

		m_ulCurMaxId = 0;

	}

	// ѡ��һ������
	void CRegionEditAction::SelRegion(unsigned long ulId, bool bIsChangeCamera)
	{
		if(m_UnreachRegionMap.count(ulId))
		{
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];
			pRegion->SetRegionSelMaterial();
			m_ulCurEditRegionId = ulId;

			if(bIsChangeCamera)
			{
				if(pRegion->hasBasePoint())
				{
					
					//ȡ����ĵ�һ������
					Ogre::Vector3 vPoint = pRegion->getBasePoint();

					//���������򶥵�Ϊ��ʼ�㣬����ķ�����Ϊ���������
					if(getSceneManipulator()->getCamera()->getProjectionType() == Ogre::PT_ORTHOGRAPHIC)
					{
						Ogre::Vector3 oldPos = getSceneManipulator()->getCamera()->getPosition();
						getSceneManipulator()->getCamera()->setPosition(vPoint.x, oldPos.y, vPoint.z);
					}
					else
					{
						Ogre::Ray rCameraRay = Ogre::Ray(vPoint,Ogre::Vector3(0,1,0.6));
						Ogre::Vector3 vTargetPoint= rCameraRay.getPoint(2000);
						getSceneManipulator()->getCamera()->setPosition(vTargetPoint);
						getSceneManipulator()->getCamera()->lookAt(vPoint);
					}			
				}
			}
			
		}
	}

	void CRegionEditAction::EditSelRegion(unsigned long ulId)
	{
		if(m_UnreachRegionMap.count(ulId))
		{
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];
			pRegion->SetRegionEditMaterial();
			m_iPreOperatorType = ADD_NEW_POINT;

		}

	}

	// ��ѡ��һ������.
	void CRegionEditAction::UnSelRegion(unsigned long ulId)
	{
		if(m_UnreachRegionMap.count(ulId))
		{
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];
			pRegion->SetRegionNorMaterial();


		}
	}

	void CRegionEditAction::FinishNewRegion(unsigned long ulRegionId)
	{

	}

	// �������ȡ�ļ�
	unsigned long CRegionEditAction::ReadRegionFromFileVer1(std::string strFileName)
	{
		FILE* pFile = fopen( strFileName.c_str(), "rb" );

		if(NULL == pFile)
		{
			// ���ܴ��ļ�.
			return 0xfffffffe;
		}

		int			iVerLen = (int)sizeof(REGION_EDIT_VER1);
		char*		pVer = new char[iVerLen];

		fread( pVer, 1, iVerLen-1, pFile );
		pVer[iVerLen-1] = 0;
		if( strcmp( pVer, REGION_EDIT_VER1 ) != 0 )
		{
			delete [] pVer;
			fclose( pFile );
			return 0xfffffffe;
		}

		delete [] pVer;

		// �����ǰ������.
		ClearRegionData();

		int			iRegionCount = 0;
		int			iPointCount  = 0;
		int         iId			 = 0;
		int         iFlag        = 0;
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


		// ��ȡ����ĸ���
		::fread(&iRegionCount, sizeof(iRegionCount), 1, pFile);

		for(int i = 0; i < iRegionCount; i++)
		{
			// ��ȡ�����id
			::fread(&iId, sizeof(iId), 1, pFile);

			// ��ȡ�����Flag
			::fread(&iFlag, sizeof(iFlag), 1, pFile);

			// ���õ�ǰ���ڱ༭��id��
			m_ulCurEditRegionId = AddNewRegion();
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[m_ulCurEditRegionId];
			if( pRegion ) pRegion->SetFlyable( iFlag );

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
			}

			//if( i != 133)
			{
				// ˢ���µ�����
				ReshapeArea(m_ulCurEditRegionId);
			}

		}

		::fclose(pFile);

		// ������ʾ״̬�� ��ʾ����.
		HideScene(m_bIsShow);

		return 0;
	}

	// ������洢���ļ�
	unsigned long CRegionEditAction::SaveRegionToFileVer1(std::string strFileName)
	{
		FILE* pFile = NULL;
		pFile = ::fopen(strFileName.data(), "wb");

		if(NULL == pFile)
		{
			// ���ܴ��ļ�.
			return 0xfffffffe;
		}

		fwrite( REGION_EDIT_VER1, 1, strlen(REGION_EDIT_VER1), pFile );

		int			iRegionCount = 0;
		int			iPointCount  = 0;
		int         iId			 = 0;
		int			iFlag        = 0;
		unsigned long ulId       = 0;

		CUnreachRegion* pRegion  = NULL;
		CPointInfo*		pPoint	 = NULL;
		Ogre::Vector3   position;

		UNREACHREGIONMAP::iterator itRegion;
		UNREACHREGIONMAP::iterator itEndRegion;

		POINT_MAP::iterator itPoint;
		POINT_MAP::iterator itEndPoint;

		// �洢����ĸ���
		iRegionCount = m_UnreachRegionMap.size();
		::fwrite(&iRegionCount, sizeof(iRegionCount), 1, pFile);

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
			pRegion = itRegion->second;
			if(pRegion)
			{
				// �洢�����id
				iId = pRegion->m_ulId;
				::fwrite(&iId, sizeof(iId), 1, pFile);

				// �洢�����iFlag
				iFlag = pRegion->GetFlyable();
				if( pRegion->GetFlyable() ) SET_FLYABLE(iFlag);
				::fwrite(&iFlag, sizeof(iFlag), 1, pFile);

				// �洢��ĸ���.
				iPointCount = pRegion->m_pointMap.size();
				::fwrite(&iPointCount, sizeof(iPointCount), 1, pFile);

				// �洢ÿ�������Ϣ��
				itEndPoint = pRegion->m_pointMap.end();
				for(itPoint = pRegion->m_pointMap.begin(); itPoint != itEndPoint; itPoint++)
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
			}
		}

		::fclose(pFile);

		return 0;
	}

	// �������ȡ�ļ����ɰ汾
	unsigned long CRegionEditAction::ReadRegionFromFileVer0(std::string strFileName)
	{
		FILE* pFile = fopen( strFileName.c_str(), "rb" );

		if(NULL == pFile)
		{
			// ���ܴ��ļ�.
			return 0xfffffffe;
		}

		// �����ǰ������.
		ClearRegionData();

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


		// ��ȡ����ĸ���
		::fread(&iRegionCount, sizeof(iRegionCount), 1, pFile);

		for(int i = 0; i < iRegionCount; i++)
		{
			// ��ȡ�����id
			::fread(&iId, sizeof(iId), 1, pFile);

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
			}

			//if( i != 133)
			{
				// ˢ���µ�����
				ReshapeArea(m_ulCurEditRegionId);
			}

		}

		::fclose(pFile);

		// ������ʾ״̬�� ��ʾ����.
		HideScene(m_bIsShow);

		return 0;
	}

	// ������洢���ļ����ɰ汾
	unsigned long CRegionEditAction::SaveRegionToFileVer0(std::string strFileName)
	{
		FILE* pFile = NULL;
		pFile = ::fopen(strFileName.data(), "wb");

		if(NULL == pFile)
		{
			// ���ܴ��ļ�.
			return 0xfffffffe;
		}

		int			iRegionCount = 0;
		int			iPointCount  = 0;
		int         iId			 = 0;
		unsigned long ulId       = 0;

		CUnreachRegion* pRegion  = NULL;
		CPointInfo*		pPoint	 = NULL;
		Ogre::Vector3   position;

		UNREACHREGIONMAP::iterator itRegion;
		UNREACHREGIONMAP::iterator itEndRegion;

		POINT_MAP::iterator itPoint;
		POINT_MAP::iterator itEndPoint;

		// �洢����ĸ���
		iRegionCount = m_UnreachRegionMap.size();
		::fwrite(&iRegionCount, sizeof(iRegionCount), 1, pFile);

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
			pRegion = itRegion->second;
			if(pRegion)
			{
				// �洢�����id
				iId = pRegion->m_ulId;
				::fwrite(&iId, sizeof(iId), 1, pFile);

				// �洢��ĸ���.
				iPointCount = pRegion->m_pointMap.size();
				::fwrite(&iPointCount, sizeof(iPointCount), 1, pFile);

				// �洢ÿ�������Ϣ��
				itEndPoint = pRegion->m_pointMap.end();
				for(itPoint = pRegion->m_pointMap.begin(); itPoint != itEndPoint; itPoint++)
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
			}
		}

		::fclose(pFile);

		return 0;
	}

	// �������ȡ�ļ�
	unsigned long CRegionEditAction::ReadRegionFromFile(std::string strFileName)
	{
		FILE* fp = fopen( strFileName.c_str(), "rb" );
		if( !fp ) return 0xfffffffe;

		char buffer[20];
		fread( buffer, 1, 20, fp );
		fclose( fp );
		if( strncmp( buffer, REGION_EDIT_VER1, strlen(REGION_EDIT_VER1) ) == 0 )
		{
			return ReadRegionFromFileVer1( strFileName );
		}
		else
		{
			return ReadRegionFromFileVer0( strFileName );
		}
	}
		
	// ������洢���ļ�
	unsigned long CRegionEditAction::SaveRegionToFile(std::string strFileName)
	{
		if( strcmp( REGION_EDIT_VER1, REGION_EDIT_VER1 ) == 0 )
		{
			return SaveRegionToFileVer1( strFileName );
		}
		else
		{
			return SaveRegionToFileVer0( strFileName );
		}
	}

	//����Region����������Ƿ�������Ϣ
	void CRegionEditAction::calculateNavInfo(std::vector<ScanLine>& lines,int PassLevel)
	{
		int minx,maxx,miny,maxy;
		minx = miny = 1024;
		maxx = maxy = 0;

		for(int i=0;i<(int)lines.size();i++)
		{
			Point ptStart = lines[i].getStartPoint(); 

			minx = Ogre::Math::Floor( std::min((float)minx,ptStart.x));
			miny = Ogre::Math::Floor( std::min((float)miny,ptStart.y));
			maxx = Ogre::Math::Ceil( std::max((float)maxx,ptStart.x));
			maxy = Ogre::Math::Ceil( std::max((float)maxy,ptStart.y));
		}

		for(int y = miny;y<maxy;y++)
		{	
			for(int x = minx;x<maxx;x++)
			{
				//�õ������ı���
				int count_left0 = 0;
				int count_right0 = 0;
				int count_left1 = 0;
				int count_right1 = 0;

				//��ǰ��yֵ
				for(int i=0;i<(int)lines.size();i++)
				{
					const Point ptStart =lines[i].getStartPoint(); 		
					const Point ptEnd =lines[i].getEndPoint();

					
					if(Ogre::Math::Abs(ptEnd.y-ptStart.y) <0.01)//��X��ƽ��
						continue;

					float k = (ptEnd.x - ptStart.x) / (ptEnd.y-ptStart.y);
					float curx0 = ptStart.x + (y - ptStart.y) * k;
					float curx1 = ptStart.x + (y+1 - ptStart.y) * k;

					float startx  = std::min(ptStart.x,ptEnd.x);
					float endx = std::max(ptStart.x,ptEnd.x);

					if(curx0> startx && curx0 < endx) //�����Y=yֱ���н���(�������curx0)
					{
						if( curx0 > x )
							count_left0 ++;
						if( curx0 < x + 1 )
							count_right0 ++;
					}
					if(curx1> startx && curx1 < endx) //�����Y=y+1ֱ���н���(�������curx1)
					{
						if( curx1 > x )
							count_left1 ++;
						if( curx1 < x + 1 )
							count_right1 ++;
					}
				}
//fujia	2007.12.30
				if(count_left0 % 2 && count_right0 % 2 && count_left1 % 2 && count_right1 % 2)
				{
					mWorld[ y * mWidth + x] = PassLevel;
				}
			}
		}
	}

	int CRegionEditAction::dumpToNavMapFile(std::string& strFileName)
	{
		FILE* pFile = fopen(strFileName.data(), "wb");

		if( NULL == pFile)
			return 0xfffffffe;

		NavMapHeader header;
		header.height = mHeight;
		header.width = mWidth;

		fwrite((void*)&header,sizeof(struct NavMapHeader),1,pFile);
		fwrite((void*)mWorld,4 * header.height * header.width,1,pFile);	

		fclose(pFile);

		return 0;
	}

	// ��������Ϣת��Ϊ��������Ҫ�ĵ���ͼ�ļ�
	// ע��:����2����Ϊ1m�ĸ��ӱ��з�Ϊ2
	unsigned long CRegionEditAction::SaveRegionToNavMapFile(std::string& strFileName)
	{

		int width = getTerrainData()->getXSize();
		int height = getTerrainData()->getZSize();
		width *= 2;
		height *= 2;

		mHeight = height;
		mWidth = width;

		mWorld = new int[width * height];
		memset((void*)mWorld,0,width * height * 4);

		int			iRegionCount = 0;
		int			iPointCount  = 0;
		int         iId			 = 0;
		unsigned long ulId       = 0;

		CUnreachRegion* pRegion  = NULL;
		CPointInfo*		pStartPoint	 = NULL;
		CPointInfo*		pEndPoint	 = NULL;

		Ogre::Vector3   position;

		UNREACHREGIONMAP::iterator itRegion;
		UNREACHREGIONMAP::iterator itEndRegion;

		POINT_MAP::iterator itPoint;
		POINT_MAP::iterator itBeginPoint;
		POINT_MAP::iterator itEndPoint;
		POINT_MAP::iterator itNextPoint;
		POINT_MAP::iterator itPrevPoint;

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
			fScale *= 0.5;
		}

		itEndRegion = m_UnreachRegionMap.end();
		for(itRegion = m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
		{
			pRegion = itRegion->second;

			//����Ṧ���Է�Խ,�򲻿����������
			int Level = pRegion->GetFlyable();
			if(Level>1)
				Level = 1;
			if(pRegion)
			{
				int j = 0;
				std::vector<ScanLine> scanLines;
				int count;
				
				itBeginPoint = pRegion->m_pointMap.begin();
				itEndPoint = pRegion->m_pointMap.end();
				for( itPoint = itBeginPoint ; itPoint != itEndPoint; itPoint++)
				{
					ScanLine line;
					count = 0;
					
					itPrevPoint = itPoint;
					itNextPoint = ++itPrevPoint;
					
					if(itNextPoint == itEndPoint)
						itNextPoint = itBeginPoint;

					pStartPoint = itPoint->second;
					if(pStartPoint)
					{
						position  = pStartPoint->m_Position;
						x = position.x;
						z = position.z;

						x = (x - fTerrainDataPosX) / fScale;
						z = (z - fTerrainDataPosZ) / fScale;
						line.setStartPoint(Point(x,z));
						count++;
					}

					pEndPoint = itNextPoint->second;
					if(pEndPoint)
					{
						position  = pEndPoint->m_Position;
						x = position.x;
						z = position.z;

						x = (x - fTerrainDataPosX) / fScale;
						z = (z - fTerrainDataPosZ) / fScale;
						line.setEndPoint(Point(x,z));
						count++;
					}

					assert(count == 2);
					if(count == 2)
						scanLines.push_back(line);
				}

				calculateNavInfo(scanLines,Level);
			}
		}

		dumpToNavMapFile(strFileName);

		delete [] mWorld; 

		return 0;
	}

	// ���ݵ��εĸ߶ȸı�༭������
	void CRegionEditAction::SetRegionHeight()
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
				pUnreachRegion->SetPointHeight(getTerrainData());
				pUnreachRegion->SetLineHeight(getTerrainData());
			}
		}
	}


	void CRegionEditAction::SetCamera(float fx, float fy)
	{
		//float fHeight = getSceneManipulator()->getRealCameraHeight();
		//getSceneManipulator()->setRealCamera(fx, fy);
		//getSceneManipulator()->setRealCameraHeight(fHeight);//

		const Ogre::Vector3 OldPos = getSceneManipulator()->getCamera()->getPosition();
		Ogre::Vector3 NewPos = OldPos;
		//Ogre::Vector3 dir(0, 1.0f, 0);
		NewPos.x = fx;
		NewPos.z = fy;
		getSceneManipulator()->getCamera()->setPosition(NewPos);
		//getSceneManipulator()->getCamera()->setDirection(dir);

	}

	void CRegionEditAction::SetCamera()
	{

	}

	// ѡ��һ����.
	void CRegionEditAction::SelPoint(unsigned long ulPointId)
	{
		m_ulCurSelectPointId = ulPointId;

		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// �õ���ǰ���ڱ༭���������Ϣ.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pRegion->SetAllPointMaterial();
			pRegion->m_ulCurSelectPointId = m_ulCurSelectPointId;
			pPoint = pRegion->GetPointInfoById(ulPointId);
			if(pPoint)
			{
				pPoint->SetSelMaterial();
			}
			pPoint = pRegion->GetPointInfoById(m_ulCurEditPointId);
			if(pPoint)
			{
				pPoint->SetCurMaterial();
			}
		}
	}

	// ����һ����Ϊ��ǰ��.
	void CRegionEditAction::SetPoint(unsigned long ulPointId)
	{

		m_ulCurEditPointId = ulPointId;
		m_ulCurSelectPointId = ulPointId;
		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// �õ���ǰ���ڱ༭���������Ϣ.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pRegion->SetAllPointMaterial();
			pRegion->m_ulCurEditPointId = ulPointId;
			pRegion->m_ulCurSelectPointId = ulPointId;
			pPoint = pRegion->GetPointInfoById(m_ulCurEditPointId);
			if(pPoint)
			{
				pPoint->SetCurMaterial();
			}
		}

	}

	// ���ð��µĹ��ܼ�
	void CRegionEditAction::SetFuncKey(unsigned long ulFuncKey)
	{
		m_FuncKeyState |= ulFuncKey;
	}

	//  ��չ��ܼ�.
	void CRegionEditAction::ClearFuncKey()
	{
		m_FuncKeyState = FUNCTION_KEY;
	}


	void CRegionEditAction::setParameter(const String& name, const String& value)
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
		   else if("ATL"  == value)
		   {
			   SetFuncKey(FUNC_KEY_ALT);
		   }
		   else if("ESC" == value)
		   {
				ChangeNoDeepMaterial();
		   }
        }
    
    }

	// �ı䵱ǰ�༭�ĵ��λ��
	void CRegionEditAction::ChangeCurEditPointPos(const Point& pt)
	{
		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// �õ���ǰ���ڱ༭���������Ϣ.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pPoint = pRegion->GetPointInfoById(m_ulCurSelectPointId);
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
					if(m_ulCurSelectPointId == it->first)
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
					if(pRegion->CanChangePointPos(iIndex, position))
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
								pRegion->m_pLineShape->FillPointData();
								//pRegion->ReShapeArea();
							}
						}

					}// if(pRegion->CanChangePointPos(iIndex, position))

				}// if(hit)

			}// if(pPoint)

		}// if(pRegion)
	}



void CRegionEditAction::ChangeCurEditPointPos(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position)
{

	CUnreachRegion* pRegion = NULL;
	CPointInfo*     pPoint  = NULL;

	// �õ���ǰ���ڱ༭���������Ϣ.
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		pPoint = pRegion->GetPointInfoById(ulPointId);
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
				pRegion->m_pLineShape->FillPointData();
				//pRegion->ReShapeArea();
			}
					
		}// if(pPoint)

	}// if(pRegion)

}

void CRegionEditAction::ChangeCurEditRegion(unsigned long ulRegionId,bool bFireUI,bool bIsChangeCamera)
{
	CUnreachRegion* pRegion = NULL;

	// �õ���ǰ���ڱ༭���������Ϣ.
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		// ѡ��
		UnSelRegion(m_ulCurEditRegionId);
		SelRegion(ulRegionId,bIsChangeCamera);

		// ֪ͨ����
		if(bFireUI)
			getSceneManipulator()->_fireRegionSel(ulRegionId);
	}
}

// ��¼��ǰ����ǰһ�ε��λ��.
void CRegionEditAction::RecordPreDoPointPos()
{
	
	CUnreachRegion* pRegion = NULL;
	CPointInfo*     pPoint  = NULL;

	// �õ���ǰ���ڱ༭���������Ϣ.
	pRegion = GetRegionById(m_ulCurEditRegionId);
	if(pRegion)
	{
		pPoint = pRegion->GetPointInfoById(m_ulCurSelectPointId);
		if(pPoint)
		{	
			m_preDoPointPos = pPoint->m_Position;		

		}// if(pPoint)

	}// if(pRegion)

}

// ָ������id�� ָ�����id�� ָ��λ�ã� ���һ��С��.
void CRegionEditAction::AddNewPoint(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position)
{
	// �õ���ǰ���ڱ༭���������Ϣ.
	CUnreachRegion* pRegion = NULL;
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		pRegion->AddNewPoint(ulPointId, position);

		getSceneManipulator()->_fireRegionAddNewPoint(ulRegionId, ulPointId, position);
	}
}

	
// ָ������id�� ָ�����id�� ָ��λ�ã� ɾ��һ��С��.
void CRegionEditAction::DelPoint(unsigned long ulRegionId, unsigned long ulPointId)
{
	// �õ���ǰ���ڱ༭���������Ϣ.
	CUnreachRegion* pRegion = NULL;
	CPointInfo*     pPoint  = NULL;

	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		pRegion->DelPointFromTail(ulPointId);

		getSceneManipulator()->_fireRegionDelPoint(ulRegionId, ulPointId);
	}
}


// ��ǰ�������Ƿ�Ϸ�
bool CRegionEditAction::IsRegionValidate(unsigned long ulRegionId)
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
void CRegionEditAction::HideRegionArea(unsigned long ulRegionId, bool bHide)
{
	CUnreachRegion* pRegion = NULL;

	// �õ���ǰ���ڱ༭���������Ϣ.
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		
		pRegion->HideAreaNode(bHide);

	}
}

// ���´�����ʾ���������
void CRegionEditAction::ReshapeArea(unsigned long ulRegionId)
{
	CUnreachRegion* pRegion = NULL;
	// �õ���ǰ���ڱ༭���������Ϣ.
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{	
		
		pRegion->ReShapeArea();
		pRegion->SetRegionNorMaterial();

	}
}

// �ı�û����Ȼ������Ĳ���
void CRegionEditAction::ChangeNoDeepMaterial()
{
	if(m_UnreachRegionMap.size())
	{
		CUnreachRegion* pRegion = NULL;
		pRegion = m_UnreachRegionMap.begin()->second;

		if(pRegion)
		{
			m_bShowDeepBuf = !m_bShowDeepBuf;
			pRegion->ChangeNoDeepMaterial(m_bShowDeepBuf);
		}
		
	}
}


} 
