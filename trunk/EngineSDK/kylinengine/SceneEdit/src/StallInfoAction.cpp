#include ".\Stallinfoaction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "Core/TerrainData.h"
#include ".\CustomShape.h"
#include "WXOperatorManager.h"       // redo undo ������.
#include "OgreCamera.h"
#include "Core/FairyResources.h"
#include ".\Stallinfoeditoperator.h"
#include "Windows.h"


namespace Fairy 
{
	using namespace Ogre;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ̯λ��Ϣ
//

	CStallInfoData::CStallInfoData()
		:m_bCanStall(0)				//�Ƿ���԰�̯
		,m_iTradeTax(0)			//����˰
		,m_dwPosTax(0)				//̯λ��
		,m_iExtraInfo(0)			//������Ϣ
	{
	}
	
	CStallInfoData::~CStallInfoData()
	{
	}

	// ������ʾͼ��.
	void CStallInfoData::CreateShape()
	{

	}

	// ��ֵ������غ���
	CStallInfoData& CStallInfoData::operator=(const CStallInfoData& stallInfo)
	{
		m_bCanStall  = stallInfo.m_bCanStall;	//�Ƿ���԰�̯
		m_iTradeTax  = stallInfo.m_iTradeTax;	//����˰
		m_dwPosTax   = stallInfo.m_dwPosTax;	//̯λ��
		m_iExtraInfo = stallInfo.m_iExtraInfo;	//������Ϣ

		return *this;
	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ̯λaction
//

	CStallInfoAction::CStallInfoAction(SceneManipulator* sceneManipulator)
	 : Action(sceneManipulator)
	 , m_pSceneNode(NULL)			// ������ʾ����༭���ܵĽڵ�.
	 , m_dwStallWidth(0)
	 , m_dwStallHeight(0)
	 , m_pStallInfoData(NULL)
	{
		CreateSceneNode();
		
		//��������
		m_iOperateType = VIEW_STALL_INFO;

		// �汾��
		m_dwVersion = 1;
	}
	
	CStallInfoAction::~CStallInfoAction(void)
	{
		if(m_pStallInfoData)
		{
			delete []m_pStallInfoData;
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �������������
	//

	// ���ð��µĹ��ܼ�
	void CStallInfoAction::SetFuncKey(unsigned long ulFuncKey)
	{

	}

	//  ��չ��ܼ�.
	void CStallInfoAction::ClearFuncKey()
	{

	}


	////////////////////////////////////////////////////////////////////////////////////
	//
	// ���̺Ͷ��̲���
	//

	// �������ȡ�ļ�
	unsigned long CStallInfoAction::ReadStallInfoFromFile(std::string strFileName)
	{
		if(m_RenderShape.m_pSceneNode)
		{
			m_RenderShape.m_pSceneNode->setVisible(false);
		}

		if(NULL == m_pStallInfoData)
		{
			CreateStallInfo();
		}

		if(NULL == m_pStallInfoData)
		{
			return 2;
		}

		std::string strStallInfoFile    = "";
		strStallInfoFile    = strFileName;
		int iPos = strStallInfoFile.rfind(".");

		strStallInfoFile = strStallInfoFile.substr(0, iPos);
		strStallInfoFile += "_StallInfo.stall";

		unsigned long dwVersion = 1;
		FILE* pFile = NULL;
		try
		{

			pFile = ::fopen(strStallInfoFile.c_str(), "r");
			if(NULL == pFile)
			{
				return 1;
			}

			// ��ȡ�汾��
			::fread(&m_dwVersion, sizeof(m_dwVersion), 1, pFile);
			
			// д̯λ�Ĵ�С
			::fread(&m_dwStallWidth,  sizeof(m_dwStallWidth),  1, pFile);
			::fread(&m_dwStallHeight, sizeof(m_dwStallHeight), 1, pFile);

			if(1 == m_dwVersion)
			{
				CreateStallInfo();
									
				if(NULL == m_pStallInfoData)
				{
					::fclose(pFile);
					return 1;
				}

				for(unsigned long i = 0; i < m_dwStallHeight; i++)
					for(unsigned long j = 0; j < m_dwStallWidth; j++)
					{
						
						// �õ�̯λ��Ϣ.
						CStallInfoData& stallInfo = m_pStallInfoData[i*m_dwStallWidth + j];
						::fread(&(stallInfo.m_bCanStall),  sizeof(stallInfo.m_bCanStall),  1, pFile);
						::fread(&(stallInfo.m_iTradeTax),  sizeof(stallInfo.m_iTradeTax),  1, pFile);
						::fread(&(stallInfo.m_dwPosTax),   sizeof(stallInfo.m_dwPosTax),   1, pFile);
						::fread(&(stallInfo.m_iExtraInfo), sizeof(stallInfo.m_iExtraInfo), 1, pFile);
						
						if(stallInfo.m_bCanStall)
						{
							// ����һ����ʾģ��.
							// j �� x��  i �� z ����.
							CreateOneStallInfo(j, i, stallInfo);
						}
					
					}
			}

			::fclose(pFile);
		}
		catch(...)
		{

			return 1;
			::fclose(pFile);
		}
	
	
		return 0;
		
	}
			
	// ������洢���ļ�
	unsigned long CStallInfoAction::SaveStallInfoToFile(std::string strFileName)
	{
		std::string strStallInfoFile    = "";

		strStallInfoFile    = strFileName;
		int iPos = strStallInfoFile.rfind(".");

		strStallInfoFile = strStallInfoFile.substr(0, iPos);
		strStallInfoFile += "_StallInfo.stall";

		// ɾ���ɵ��ļ���
		::DeleteFile(strStallInfoFile.c_str());

		unsigned long dwVersion = 1;
		FILE* pFile = NULL;

		try
		{

			pFile = ::fopen(strStallInfoFile.c_str(), "wb");
			if(NULL == pFile)
			{
				return 1;
			}

			// д�汾��
			::fwrite(&m_dwVersion, sizeof(m_dwVersion), 1, pFile);
			if(NULL == m_pStallInfoData)
			{
				unsigned long dwWidth  = 0;
				unsigned long dwHeight = 0;
				::fwrite(&dwWidth,	sizeof(dwWidth),  1, pFile);
				::fwrite(&dwHeight,	sizeof(dwHeight), 1, pFile);
				::fclose(pFile);
				return 0;
			}

			// д�߶ȺͿ��
			// д���
			::fwrite(&m_dwStallWidth,	sizeof(m_dwStallWidth),	 1, pFile);
			// д�߶�
			::fwrite(&m_dwStallHeight,	sizeof(m_dwStallHeight), 1, pFile);
			for(unsigned long i = 0; i < m_dwStallHeight; i++)
				for(unsigned long j = 0; j < m_dwStallWidth; j++)
				{
					
					// �õ�̯λ��Ϣ.
					CStallInfoData& stallInfo = m_pStallInfoData[i*m_dwStallWidth + j];
					::fwrite(&(stallInfo.m_bCanStall),  sizeof(stallInfo.m_bCanStall),  1, pFile);
					::fwrite(&(stallInfo.m_iTradeTax),  sizeof(stallInfo.m_iTradeTax),  1, pFile);
					::fwrite(&(stallInfo.m_dwPosTax),   sizeof(stallInfo.m_dwPosTax),   1, pFile);
					::fwrite(&(stallInfo.m_iExtraInfo), sizeof(stallInfo.m_iExtraInfo), 1, pFile);
				
				}

			::fclose(pFile);
		}
		catch(...)
		{

			::fclose(pFile);
			return 1;
		}
	
	
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// action ������
	//
		
	const String& CStallInfoAction::getName(void) const
	{
		//����һ���ַ������֣� �����жϵ�ǰ����������.
		static const String name = "StallInfoEdit";
		return name;
	}

	// ���ò�������.
	void CStallInfoAction::setParameter(const String& name, const String& value)
	{

	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// ���������麯��.
	//
    //protected:
        
		
	void CStallInfoAction::_onMove(const Point& pt)
	{
		OnMouseMove(pt);
	}
	void CStallInfoAction::_onBegin(const Point& pt)
	{
		OnMouseLBnDown(pt);
	}
	void CStallInfoAction::_onDrag(const Point& pt)
	{
		OnMouseDrag(pt);
	}
	void CStallInfoAction::_onEnd(const Point& pt, bool canceled)
	{
		OnMouseLBnUp(pt);
	}

	// ��ǰ����active����.
	void CStallInfoAction::_onActive(bool active)
	{
        if (!active)
        {
            getSceneManipulator()->getHitIndicator("IntersectPoint")->hide();
        }
	}


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��Ӧ��Ӧ��������.
	//

	void CStallInfoAction::OnMouseMove(const Point& pt)
	{
		switch(m_iOperateType)
		{
		case SET_STALL_INFO:
			{
				
			}
			break;
		case DEL_STALL_INFO:
			{
				
			}
			break;
		case VIEW_STALL_INFO:
			{
				DoViewMouseMove(pt);
			}
			break;
		default:
			{
			}
			break;
		}//

		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		ClearFuncKey();

		
	}

	void CStallInfoAction::OnMouseLBnDown(const Point& pt)
	{
		switch(m_iOperateType)
		{
		case SET_STALL_INFO:
			{
				DoDrawLBnDown(pt);
			}
			break;
		case DEL_STALL_INFO:
			{
				DoDelLBnDown(pt);
			}
			break;
		case VIEW_STALL_INFO:
			{

			}
			break;
		default:
			{
			}
			break;
		}//
		
	}

	void CStallInfoAction::OnMouseLBnUp(const Point& pt)
	{
		switch(m_iOperateType)
		{
		case SET_STALL_INFO:
			{
				DoDrawLBnUp(pt);
			}
			break;
		case DEL_STALL_INFO:
			{
				DoDelLBnUp(pt);
			}
			break;
		case VIEW_STALL_INFO:
			{

			}
			break;
		default:
			{
			}
			break;
		}//
		
	}

	void CStallInfoAction::OnMouseRBnDown(const Point& pt)
	{

	}

	void CStallInfoAction::OnMouseRBnUp(const Point& pt)
	{

	}

	void CStallInfoAction::OnMouseDrag(const Point& pt)
	{
	
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		switch(m_iOperateType)
		{
		case SET_STALL_INFO:
			{
				DoDrawLBnDrag(pt);
			}
			break;
		case DEL_STALL_INFO:
			{
				DoDelLBnDrag(pt);
			}
			break;
		case VIEW_STALL_INFO:
			{

			}
			break;
		default:
			{
			}
			break;
		}//
	
	

	}


	// ��������༭�ĳ����ڵ�
	void CStallInfoAction::CreateSceneNode()
	{
		m_pSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			HideScene(false);
		}

		// ������ʾ������
		m_RenderShape.SetParentSceneNode(m_pSceneNode);
		m_RenderShape.SetSceneManipulator(getSceneManipulator());
		m_RenderShape.CreateSceneNode();
		m_RenderShape.CreateRenderShape();
		m_RenderShape.m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);//
	}

	// ��������༭�����ڵ�
	void CStallInfoAction::HideScene(bool bHide)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(bHide);
			m_bIsShow = bHide;
		}
	}

	// ����̯λ��С
	void CStallInfoAction::SetStallSize(unsigned long dwWidth, unsigned long dwHeight)
	{
		m_dwStallWidth  = dwWidth;
		m_dwStallHeight = dwHeight;
	}

	// �ؽ�̯λ��Ϣ
	void CStallInfoAction::ReCreateStallInfo()
	{
		m_dwOldStallWidth  =	m_dwStallWidth  ;	
		m_dwOldStallHeight =	m_dwStallHeight ;	
		
		//���ݾ�����
		CStallInfoData* oldStallInfoData = new CStallInfoData[m_dwStallWidth*m_dwStallHeight];
		memset(oldStallInfoData, 0, sizeof(CStallInfoData)*m_dwStallWidth*m_dwStallHeight);
		int tmp = sizeof(CStallInfoData);
		for(int i = 0; i < m_dwStallWidth*m_dwStallHeight ; i++)
		{
				// �õ�̯λ��Ϣ.
				oldStallInfoData[i] = m_pStallInfoData[i] ;
		}
		//�ؽ�������
		CreateStallInfo();

		float fTerrainDataPosX = getTerrainData()->mPosition.x;
		float fTerrainDataPosZ = getTerrainData()->mPosition.z;

		//����������
		float fScale = getSceneManipulator()->getBaseScale();
		for(int fz = 0; fz < m_dwOldStallHeight; fz ++)
		{
			for(int fx = 0; fx < m_dwOldStallWidth; fx++)
			{
				int newfZ = fz  + (m_vOldFirstGrid.z -  fTerrainDataPosZ)/fScale;
				int newfX = fx  + (m_vOldFirstGrid.x -  fTerrainDataPosX)/fScale;
				m_pStallInfoData[newfZ*m_dwStallWidth + newfX] = oldStallInfoData[fz*m_dwOldStallWidth + fx] ;

			}
		}

		delete []oldStallInfoData;


	}

	// ����̯λ��Ϣ
	void CStallInfoAction::CreateStallInfo()
	{
		// ̯λ��С.
		m_dwStallWidth	= getTerrainData()->getXSize();
		m_dwStallHeight	= getTerrainData()->getZSize();
		if(m_pStallInfoData)
		{
			delete []m_pStallInfoData;
			m_pStallInfoData = NULL;
		}

		if(m_dwStallWidth*m_dwStallHeight)
		{
			m_pStallInfoData = new CStallInfoData[m_dwStallWidth*m_dwStallHeight];
		}


		if(NULL == m_pStallInfoData)
		{
			return;
		}
		memset(m_pStallInfoData, 0, sizeof(CStallInfoData)*m_dwStallWidth*m_dwStallHeight);

		if(m_RenderShape.m_pSceneNode)
		{
			m_RenderShape.m_pSceneNode->setVisible(false);
		}

	}


	// �������ε���갴�²���
	void CStallInfoAction::DoDrawLBnDown(const Point& pt)
	{
		Ogre::Vector3 TerrainPosition;
		if(GetTerrainPos(pt, TerrainPosition))
		{
			m_PrePoint = TerrainPosition;
			m_CurPoint = TerrainPosition;
			//m_CurPoint.x += 100;
			//m_CurPoint.z += 100;
			NormalRect();
			CreateShowRect();
			
			//// Ҫ������Ⱦ��������Ȼ��, release�»س�������.
			/*pRectInfo->CreateRenderShape();
			BuildEventAreaRect();
			pRectInfo->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
			getSceneManipulator()->_fireAddNewRect(0, m_iEditRectId);*/

			if(m_RenderShape.m_pSceneNode)
			{
				m_RenderShape.m_pSceneNode->setVisible(true);
			}

		}
	}

	// �������ε������ק����
	void CStallInfoAction::DoDrawLBnDrag(const Point& pt)
	{
		Ogre::Vector3 TerrainPosition;
		if(GetTerrainPos(pt, TerrainPosition))
		{
			m_CurPoint = TerrainPosition;
			NormalRect();
			CreateShowRect();
	
		}//
	}

	// �������ε����̧�����
	void CStallInfoAction::DoDrawLBnUp(const Point& pt)
	{
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
		
		
		int   iStartX = m_AreaRect.m_LeftTop.x;
		int   iStartZ = m_AreaRect.m_LeftTop.y;
		int   iEndX   = m_AreaRect.m_RightBottom.x;
		int   iEndZ   = m_AreaRect.m_RightBottom.y;


		if(NULL == m_pStallInfoData)
		{
			// ̯λ��С.
			m_dwStallWidth	= getTerrainData()->getXSize();
			m_dwStallHeight	= getTerrainData()->getZSize();

			// ������λ��Ϣ.
			CreateStallInfo();
		}

		float fx = 0;
		float fz = 0;
		float fCurx = 0;
		float fCurz = 0;
		int iX;
		int iZ;
		Ogre::Vector3 NewPoint;
		for(fz = iStartZ; fz < iEndZ; fz += 100.0f)
			for(fx = iStartX; fx < iEndX; fx += 100.0f)
			{

				fCurx = fx;
				iX = (fCurx - fTerrainDataPosX) / fScale;
			
				fCurz = fz;
				iZ = (fCurz - fTerrainDataPosZ) / fScale;

				// �õ�̯λ��Ϣ.
				CStallInfoData& stallInfo = m_pStallInfoData[iZ*m_dwStallWidth + iX];

				// ������ʾģ��
				if(NULL == stallInfo.m_RectRenderShape.m_pRectRenderShape)
				{
					stallInfo.m_RectRenderShape.SetParentSceneNode(m_pSceneNode);
					stallInfo.m_RectRenderShape.SetSceneManipulator(getSceneManipulator());
					stallInfo.m_RectRenderShape.CreateSceneNode();
					stallInfo.m_RectRenderShape.CreateRenderShape();
					stallInfo.m_RectRenderShape.m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);//
				}

				// ���õ�ǰ�Ļ�ˢֵ
				stallInfo = m_curStallInfoBrush;

				// ���԰�̯.
                stallInfo.m_bCanStall = true;
				// ����̯λ����ֵ
				stallInfo.m_iPosX = iX;
				stallInfo.m_iPosY = iZ;

				stallInfo.m_RectRenderShape.ClearRenderShape();
				// ������1
				NewPoint.x = fx;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);

				NewPoint.x = fx;
				NewPoint.z = fz + 100.0f;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);

				NewPoint.x = fx + 100.0f;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);



				// ������2
				NewPoint.x = fx + 100.0f;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);

				NewPoint.x = fx;
				NewPoint.z = fz + 100.0f;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);

				NewPoint.x = fx + 100.0f;
				NewPoint.z = fz + 100.0f;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);//
				
				if(stallInfo.m_RectRenderShape.m_pRectRenderShape)
				{
					stallInfo.m_RectRenderShape.m_pRectRenderShape->FillPointData();
					stallInfo.m_RectRenderShape.m_pSceneNode->setVisible(true);
					stallInfo.m_RectRenderShape.m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
					stallInfo.m_RectRenderShape.m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
				}
			}

			CStallInfoEditOperator* pAddOperator = new CStallInfoEditOperator;
			getOperatorManager()->addOperator(pAddOperator);//

	}

	// �õ������ϵ������
	bool  CStallInfoAction::GetTerrainPos(const Point& pt, Ogre::Vector3& position)
	{
			
		bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);

		return hit;
		
	}

	// �ѵ�ǰ�ĵ�ת���ɹ�����.
	void CStallInfoAction::NormalRect()
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


	// �����ķ���
	void CStallInfoAction::CreateShowRect()
	{
		m_RenderShape.ClearRenderShape();

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
				m_RenderShape.AddNewPoint(NewPoint);

				NewPoint.x = fx;
				NewPoint.z = fz + fStepZ;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_RenderShape.AddNewPoint(NewPoint);


				NewPoint.x = fx + fStepX;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_RenderShape.AddNewPoint(NewPoint);

			
				// ������2
				NewPoint.x = fx + fStepX;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_RenderShape.AddNewPoint(NewPoint);

				NewPoint.x = fx;
				NewPoint.z = fz + fStepZ;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_RenderShape.AddNewPoint(NewPoint);

				NewPoint.x = fx + fStepX;
				NewPoint.z = fz + fStepZ;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_RenderShape.AddNewPoint(NewPoint);
			}

		m_RenderShape.m_pRectRenderShape->FillPointData();
		m_RenderShape.m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
		m_RenderShape.m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia
		
	}

	// ��ɾ�����ε���갴�²���
	void CStallInfoAction::DoDelLBnDown(const Point& pt)
	{
		DoDrawLBnDown(pt);
	}

	// ��ɾ�����ε������ק����
	void CStallInfoAction::DoDelLBnDrag(const Point& pt)
	{
		DoDrawLBnDrag(pt);
	}


	// ��ɾ�����ε����̧�����
	void CStallInfoAction::DoDelLBnUp(const Point& pt)
	{
		if(m_RenderShape.m_pSceneNode)
		{
			m_RenderShape.m_pSceneNode->setVisible(false);
		}

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
		
		
		int   iStartX = m_AreaRect.m_LeftTop.x;
		int   iStartZ = m_AreaRect.m_LeftTop.y;
		int   iEndX   = m_AreaRect.m_RightBottom.x;
		int   iEndZ   = m_AreaRect.m_RightBottom.y;


		if(NULL == m_pStallInfoData)
		{
			// ̯λ��С.
			m_dwStallWidth	= getTerrainData()->getXSize();
			m_dwStallHeight	= getTerrainData()->getZSize();

			// ������λ��Ϣ.
			CreateStallInfo();
		}

		float fx = 0;
		float fz = 0;
		float fCurx = 0;
		float fCurz = 0;
		int iX;
		int iZ;
		Ogre::Vector3 NewPoint;
		for(fz = iStartZ; fz < iEndZ; fz += 100.0f)
			for(fx = iStartX; fx < iEndX; fx += 100.0f)
			{

				fCurx = fx;
				iX = (fCurx - fTerrainDataPosX) / fScale;
			
				fCurz = fz;
				iZ = (fCurz - fTerrainDataPosZ) / fScale;

				// �õ�̯λ��Ϣ.
				CStallInfoData& stallInfo = m_pStallInfoData[iZ*m_dwStallWidth + iX];
				
				// ���ܰ�̯
				stallInfo.m_bCanStall = false;

				if(stallInfo.m_RectRenderShape.m_pRectRenderShape)
				{
					
					if(stallInfo.m_RectRenderShape.m_pSceneNode)
					{
						stallInfo.m_RectRenderShape.m_pSceneNode->setVisible(false);
					}
				}//

			}

		CStallInfoEditOperator* pAddOperator = new CStallInfoEditOperator;
		getOperatorManager()->addOperator(pAddOperator);//
	}

	// �鿴̯λ��Ϣ
	void CStallInfoAction::DoViewMouseMove(const Point& pt)
	{

		Ogre::Vector3 TerrainPosition;
		if(!GetTerrainPos(pt, TerrainPosition))
		{
			return;
		}

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
		
		
		float fx = 0;
		float fz = 0;
		float fCurx = 0;
		float fCurz = 0;
		int iX;
		int iZ;
		
		fCurx = TerrainPosition.x;
		iX = (fCurx - fTerrainDataPosX) / fScale;
			
		fCurz = TerrainPosition.z;
		iZ = (fCurz - fTerrainDataPosZ) / fScale;

		if(m_pStallInfoData)
		{
			// �õ�̯λ��Ϣ.
			CStallInfoData& stallInfo = m_pStallInfoData[iZ*m_dwStallWidth + iX];
			getSceneManipulator()->_fireViewStallInfo(stallInfo.m_bCanStall, stallInfo.m_iTradeTax, stallInfo.m_dwPosTax, stallInfo.m_iExtraInfo);

		}

	}

	// ����һ����ʾ����
	void CStallInfoAction::CreateOneStallInfo(int ix, int iy, CStallInfoData& stallInfo)
	{
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


		// ������ʾģ��
		if(NULL == stallInfo.m_RectRenderShape.m_pRectRenderShape)
		{
			stallInfo.m_RectRenderShape.SetParentSceneNode(m_pSceneNode);
			stallInfo.m_RectRenderShape.SetSceneManipulator(getSceneManipulator());
			stallInfo.m_RectRenderShape.CreateSceneNode();
			stallInfo.m_RectRenderShape.CreateRenderShape();
			stallInfo.m_RectRenderShape.m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);//
		}

		Ogre::Vector3 NewPoint;
		// ����̯λ����ֵ
		stallInfo.m_iPosX = ix;
		stallInfo.m_iPosY = iy;

		float fx;
		float fy;

		fx = ix * fScale + fTerrainDataPosX;
		fy = iy * fScale + fTerrainDataPosZ;

		stallInfo.m_RectRenderShape.ClearRenderShape();
		
		// ������1
		NewPoint.x = fx;
		NewPoint.z = fy;
		NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
		stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);

		NewPoint.x = fx;
		NewPoint.z = fy + 100.0f;
		NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
		stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);

		NewPoint.x = fx + 100.0f;
		NewPoint.z = fy;
		NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
		stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);


		// ������2
		NewPoint.x = fx + 100.0f;
		NewPoint.z = fy;
		NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
		stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);

		NewPoint.x = fx;
		NewPoint.z = fy + 100.0f;
		NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
		stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);

		NewPoint.x = fx + 100.0f;
		NewPoint.z = fy + 100.0f;
		NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
		stallInfo.m_RectRenderShape.AddNewPoint(NewPoint);//
				
		if(stallInfo.m_RectRenderShape.m_pRectRenderShape)
		{
			stallInfo.m_RectRenderShape.m_pRectRenderShape->FillPointData();
			stallInfo.m_RectRenderShape.m_pSceneNode->setVisible(true);
			stallInfo.m_RectRenderShape.m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
			stallInfo.m_RectRenderShape.m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//FUJIA
		}
			
	}


}