#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "PathFindDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "PathFindDlg.h"

#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCodec.h>
#include <OgreResourceGroupManager.h>
#include <OgreStringConverter.h>

// ----------------------------------------------------------------------------
#include "SceneManipulator.h"
#include "Action.h"
#include "WXOperatorManager.h"

#include "Core/Terrain.h"
#include "Core/TerrainData.h"

#include "RegionEditDlgSelector.h"
#include "res/PathFindEdit_wdr.h"
#include "PathFindAction.h"
#include "UnreachRegion.h"
#include "RegionEditAction.h"

using namespace Fairy;

BEGIN_EVENT_TABLE(CPathFindEditDlg, wxPanel)
//EVT_BUTTON(ID_BUTTON_DEL_LINE, CPathFindEditDlg::OnDelRegion)
//EVT_BUTTON(ID_BN_ADD_NEW_LINE, CPathFindEditDlg::OnAddNewRegion)
EVT_BUTTON(ID_BUTTON_DEL_POINT,  CPathFindEditDlg::OnDelPointInRegion)
//EVT_BUTTON(ID_BUTTON_LINE_OK,  CPathFindEditDlg::OnFinishRegion)

//EVT_LISTBOX(ID_LISTBOX_LINE, CPathFindEditDlg::OnRegionListBox)
EVT_LISTBOX(ID_LISTBOX_POINT, CPathFindEditDlg::OnPointListBox)
//EVT_CHECKBOX(ID_IFY, CPathFindEditDlg::OnPointUseY)
EVT_TEXT_ENTER(ID_POINT_X, CPathFindEditDlg::OnEditXYZ)
//EVT_TEXT_ENTER(ID_POINT_Y, CPathFindEditDlg::OnEditXYZ)
EVT_TEXT_ENTER(ID_POINT_Z, CPathFindEditDlg::OnEditXYZ)

END_EVENT_TABLE()


CPathFindEditDlg::CPathFindEditDlg(const wxString& resourceGroup)
:m_pListboxPointInRegion(NULL)
,m_pDelPointBn(NULL)
,m_pSceneManipulator(NULL)
,m_pAction(NULL)
,m_bIsEditingRegion(false)
,m_iCurSelRegionIndex(-1)
,m_pPointX(NULL)
,m_pPointZ(NULL)
,m_ulCurRegionId(-1)

{
	InitData();


	// �������������б�
	m_pListboxPointInRegion = NULL;

	// ɾ����İ�ť.
	m_pDelPointBn = NULL;

	// ��������
	m_pSceneManipulator = NULL;

	m_pAction = NULL;
}

CPathFindEditDlg::CPathFindEditDlg(
									 wxWindow *parent, 
									 const wxString& resourceGroup,
									 wxWindowID winid,
									 const wxPoint& pos,
									 const wxSize& size,
									 long style,
									 const wxString& name
									 )
									 :m_pListboxPointInRegion(NULL)
									 ,m_pDelPointBn(NULL)
									 ,m_pSceneManipulator(NULL)
									 ,m_pAction(NULL)
									 ,m_bIsEditingRegion(false)
									 ,m_iCurSelRegionIndex(-1)
									 ,m_pPointX(NULL)
									 ,m_pPointZ(NULL)
									 ,m_ulCurRegionId(-1)
{
	// �������������б�
	m_pListboxPointInRegion = NULL;

	// ɾ����İ�ť.
	m_pDelPointBn = NULL;

	// ��������
	m_pSceneManipulator = NULL;

	m_pAction = NULL;

	Create(parent, winid, pos, size, style, name);
	InitData();

}

bool CPathFindEditDlg::Create(
							   wxWindow *parent, 
							   wxWindowID winid,
							   const wxPoint& pos, 
							   const wxSize& size,
							   long style, 
							   const wxString& name
								  )
{
	if (!wxPanel::Create(parent, winid, pos, size, style, name))
		return false;

	PathFindDlg(this, true, true);

	////////////////////////////////////////////////////////////////////////////////////
	//
	// ��ȡ���ؼ�������
	//

	// ��õ�ؼ���ָ��.
	m_pListboxPointInRegion = wxDynamicCast(FindWindow(ID_LISTBOX_POINT), wxListBox);

	// �õ�ɾ����Ŀؼ���ָ��
	m_pDelPointBn =  wxDynamicCast(FindWindow(ID_BUTTON_DEL_POINT), wxButton);

	m_pPointX = wxDynamicCast(FindWindow(ID_POINT_X), wxTextCtrl);
	m_pPointX->Enable(false);
	m_pPointZ = wxDynamicCast(FindWindow(ID_POINT_Z), wxTextCtrl);
	m_pPointZ->Enable(false);
	if(m_pListboxPointInRegion)
	{
		m_pListboxPointInRegion->Clear();
	}

	GetRegionAction();
	return true;
}

CPathFindEditDlg::~CPathFindEditDlg(void)
{
}

bool CPathFindEditDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}

void CPathFindEditDlg::ActiveAction(bool bActive)
{
	if(m_pAction)
	{
		if(!bActive)
		{
			m_pSceneManipulator->setActiveAction(NULL);
		}
		m_pAction->HideScene(bActive);
	}
}

void CPathFindEditDlg::SetActive(bool bShow)
{

	if (bShow)
	{
		if(m_pSceneManipulator)
		{
			Fairy::Action* action = m_pSceneManipulator->_getAction("PathFind");
			m_pSceneManipulator->setActiveAction(action);
			m_pAction = (CPathFindAction *)action;
			AddNewRegion();
		}
	}
	else
	{
		if(m_pAction)
		{
			m_pAction->SetOldHitPointMaterial();
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// ��ʾ���س���
	if(m_pAction)
	{
		m_pAction->HideScene(bShow);

		Fairy::Action* action = m_pSceneManipulator->_getAction("RegionEdit");
		Fairy::CRegionEditAction* pRegionEditAction = ( Fairy::CRegionEditAction*)action;
		pRegionEditAction->HideScene(bShow);

		if(bShow)
		{
			// ���ݵ��εĸ߶���ʾ����.
			m_pAction->SetRegionHeight();
		}
	}

}

void CPathFindEditDlg::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;

	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		Fairy::Action* action = m_pSceneManipulator->_getAction("PathFind");
		m_pAction = (CPathFindAction *)action;
		SetActive(false);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//
// �����༭�Ļص�����
//

// ѡ��һ������Ļص�������
void CPathFindEditDlg::onSelDijkPoint(unsigned long ulPointId)
{
	//unsigned long ulFindRegionId = 0;

	//int iRegionCount = m_pListboxRegion->GetCount();

	//for(int i = 0; i < iRegionCount; i++)
	//{
	//	// �õ�����id
	//	ulFindRegionId = (unsigned long)m_pListboxRegion->GetClientData(i);
	//	if(ulFindRegionId == ulRegionId)
	//	{
	//		m_bIsChangeCamera = false;
	//		m_pListboxRegion->SetSelection(i);
	//		DoSelRegion();

	//		m_bIsChangeCamera = true;
	//		break;
	//	}
	//}
}
//fujia
// ����༭�Ļص������� ֪ͨ�������һ���µĵ�.
void CPathFindEditDlg::onAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position)
{
	// ֻ�е�ǰ�������������ʱ�����.
	if(m_ulCurRegionId == unsigned long(1) )
	{
		RegionAddNewPoint(ulPointId);

		float fTerrainDataPosX = m_pAction->getTerrainData()->mPosition.x;
		float fTerrainDataPosZ = m_pAction->getTerrainData()->mPosition.z;
		float fScale = 1.0;
		if( m_pAction->getSceneManipulator() )
		{
			fScale = m_pAction->getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001) fScale = 1.0;
		}

		char buf[128];
		sprintf( buf, "%.4f", (position.x - fTerrainDataPosX) / fScale );
		m_pPointX->Clear();
		m_pPointX->WriteText( buf );
		sprintf( buf, "%.4f", (position.z - fTerrainDataPosZ) / fScale );
		m_pPointZ->Clear();
		m_pPointZ->WriteText( buf );
	}
}

// ɾ��һ����Ļص�����
void CPathFindEditDlg::onDelDijkPoint(unsigned long ulPointId)
{
	// ֻ�е�ǰ�������������ʱ�����.
	if(m_ulCurRegionId == unsigned long(1))
	{
		DelRegionPoint(ulPointId);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ui���棬 �¼���Ӧ����
//
void CPathFindEditDlg::OnDelPointInRegion( wxCommandEvent &event )
{
	int iIndex = m_pListboxPointInRegion->GetSelection();
	if (iIndex == -1)
	{
		return;
	}
	CPointInfo* pInfo = m_pAction->GetRegionById( m_pAction->GetCurEditRegionId() )->GetPointInfoByIndex1( iIndex );
	m_pAction->DelPoint(pInfo->m_ulPointId);
}

void CPathFindEditDlg::OnEditXYZ( wxCommandEvent &event )
{
	Ogre::Vector3 pt;
	pt.x = (float)atof( m_pPointX->GetValue() );
	pt.z = (float)atof( m_pPointZ->GetValue() );

	float fTerrainDataPosX = m_pAction->getTerrainData()->mPosition.x;
	float fTerrainDataPosZ = m_pAction->getTerrainData()->mPosition.z;
	float fScale = 1.0;
	if( m_pAction->getSceneManipulator() )
	{
		fScale = m_pAction->getSceneManipulator()->getBaseScale();
		if(abs((float)fScale) < 0.00001) fScale = 1.0;
	}

	pt.x = pt.x * fScale + fTerrainDataPosX;
	pt.z = pt.z * fScale + fTerrainDataPosZ;

	m_pAction->ChangeCurEditPointPos(m_ulCurPointId, pt );
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// �߼����ݲ���
//

// ���һ������.
void CPathFindEditDlg::AddNewRegion()
{
	//SetActive(true);

	if(m_pAction)
	{
		unsigned long ul = m_pAction->AddNewRegion();
		if( m_ulCurRegionId != ul )
		{
			if( m_ulCurRegionId != unsigned long(-1) )
				m_pAction->UnSelRegion(m_ulCurRegionId);
			m_ulCurRegionId = ul;

			m_iCurSelRegionIndex = 0;
			m_pListboxPointInRegion->Clear();
		}
	}
}

// �ڵ�ǰ���������һ���µĵ�
void CPathFindEditDlg::RegionAddNewPoint(unsigned long ulId, bool bRefresh)
{
	if(0 == ulId)
	{
		// ��ǰ���id���Ϸ�
		return;
	}

	// ���һ���µ�����
	char buf[256];
	if(m_pAction)
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "��%d��", ulId);

		int iCount = m_pListboxPointInRegion->GetCount();
		m_pListboxPointInRegion->Insert(buf, iCount, (void*)ulId);

		// ��ǰ�༭�ĵ�
		m_ulCurPointId = ulId;

		m_pListboxPointInRegion->Select(iCount);

		if(!bRefresh)
		{
			// ���õ�ǰ���ڱ༭����
			SetEditRegionFlag(true);
		}

	}
}

// ��ʼ���߼�����
void CPathFindEditDlg::InitData()
{
	m_ulCurPointId  = 0;	
	m_ulCurLineId   = 0;	
	m_bIsChangeCamera = true;
	
}


// ��ѡ��listbox��Ӧ���¼�����
void CPathFindEditDlg::OnPointListBox(wxCommandEvent &event)
{
	if (event.GetInt() == -1)
	{
		return;
	}

	if(event.GetId() == ID_LISTBOX_POINT)
	{
		int iIndex = m_pListboxPointInRegion->GetSelection();
		if( iIndex != -1 )
		{
			CPointInfo* pInfo = m_pAction->GetRegionById( m_pAction->GetCurEditRegionId() )->GetPointInfoByIndex1( iIndex );
			if( pInfo )
			{
				float fTerrainDataPosX = m_pAction->getTerrainData()->mPosition.x;
				float fTerrainDataPosZ = m_pAction->getTerrainData()->mPosition.z;
				float fScale = 1.0;
				if( m_pAction->getSceneManipulator() )
				{
					fScale = m_pAction->getSceneManipulator()->getBaseScale();
					if(abs((float)fScale) < 0.00001) fScale = 1.0;
				}

				Ogre::Vector3 position = pInfo->GetPosition();
				char buf[128];
				sprintf( buf, "%.4f", (position.x - fTerrainDataPosX) / fScale );
				m_pPointX->Clear();
				m_pPointX->WriteText( buf );
				sprintf( buf, "%.4f", (position.z - fTerrainDataPosZ) / fScale );
				m_pPointZ->Clear();
				m_pPointZ->WriteText( buf );
			}

		}
		DoSelPoint();
	}
}

// ѡ�е�Ĳ���
void CPathFindEditDlg::DoSelPoint()
{
	if(NULL == m_pListboxPointInRegion)
	{
		return;
	}

	// �õ�ѡ�е�����
	int iIndex = m_pListboxPointInRegion->GetSelection();

	unsigned long ulPointId = 0;
	if(-1 != iIndex)
	{
		// �õ���id
		ulPointId = (unsigned long)m_pListboxPointInRegion->GetClientData(iIndex);
		if(m_pAction)
		{
			DWORD dwOldID = m_ulCurPointId;
			DWORD dwMax, dwMin;
			FLOAT fDistance = 0.0f;
			Ogre::Vector3 p1, p2;

			//����ѡ�е�
			m_ulCurPointId = ulPointId;
			m_pAction->SelPoint(ulPointId);

			//��ԭ�ȵ�ѡ�е���µ�ѡ�е�����߾���
			CUnreachRegion* pRegion = m_pAction->GetRegionById( m_ulCurRegionId );
			if( dwOldID != m_ulCurPointId && pRegion )
			{
				//��ͼ�õ�ԭ�ȵ��ѡ�е���ÿһ����
				dwMax = dwOldID > m_ulCurPointId ? dwOldID : m_ulCurPointId;
				dwMin = dwOldID > m_ulCurPointId ? m_ulCurPointId : dwOldID;
				for( DWORD dw = dwMin; dw <= dwMax; dw ++ )
				{
					CPointInfo* pPoint = pRegion->GetPointInfoById1(dw);
					if( !pPoint ) continue;
					if( dw == dwMin )
					{
						p1 = pPoint->GetPosition();
					}
					else
					{
						p2 = pPoint->GetPosition();
						fDistance += sqrtf( ( p1.x - p2.x ) * ( p1.x - p2.x ) + ( p1.y - p2.y ) * ( p1.y - p2.y ) + ( p1.z - p2.z ) * ( p1.z - p2.z ) );
						p1 = p2;
					}
				}
			}
		}
	}
}


// �������
void CPathFindEditDlg::ClearData()
{
	ClearUiData();

	if(m_pAction)
	{
		m_pAction->ClearRegionData();
		m_pAction->m_ulCurEditRegionId = m_pAction->AddNewRegion();
	}
}

// ��RegionAction��ˢ������, ���̵�ʱ��ʹ��
void CPathFindEditDlg::RefreshData()
{
	//if((NULL == m_pAction)||(NULL == m_pListboxRegion))
	//{
	//	return;
	//}
	//// ���ui��������.
	//ClearUiData();

	//UNREACHREGIONMAP::iterator itRegion;
	//UNREACHREGIONMAP::iterator itEndRegion;
	//unsigned long   ulId    = 0;
	//int				iCount  = 0;

	//CUnreachRegion* pRegion = NULL;


	//char buf[256];
	//// �洢ÿ���������Ϣ
	//itEndRegion  = m_pAction->m_UnreachRegionMap.end();
	//for(itRegion = m_pAction->m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
	//{	
	//	pRegion = itRegion->second;
	//	if(pRegion)
	//	{
	//		memset(buf, 0, sizeof(buf));

	//		// �洢�����id
	//		ulId = pRegion->m_ulId;
	//		sprintf(buf, "����%d", ulId);
	//		iCount = m_pListboxRegion->GetCount();
	//		m_pListboxRegion->Insert(buf, iCount, (void*)ulId);
	//	}
	//}

	//if(m_pListboxRegion->GetCount())
	//{
	//	m_pListboxRegion->SetSelection(0);
	//}
}


// ˢ�������еĵ������
void CPathFindEditDlg::RefreshRegionPointData(unsigned long ulRegionId)
{
	//if(m_pAction)
	//{
	//	CUnreachRegion* pRegion = m_pAction->GetRegionById(ulRegionId);
	//	if(NULL == pRegion)
	//	{
	//		return ;
	//	}

	//	// �����ǰ������.
	//	m_pListboxPointInRegion->Clear();

	//	int iPointCount = pRegion->GetPointCount1();
	//	CPointInfo* pPointInfo = NULL;
	//	for(int i = 0; i < iPointCount; i++)
	//	{
	//		pPointInfo = pRegion->GetPointInfoByIndex1(i);
	//		if(pPointInfo)
	//		{
	//			RegionAddNewPoint(pPointInfo->m_ulPointId, true);
	//		}
	//	}
	//}
}

// ���ui���������
void CPathFindEditDlg::ClearUiData()
{
	if(m_pListboxPointInRegion)
	{
		m_pListboxPointInRegion->Clear();
	}
}


// ��ȡ����
void CPathFindEditDlg::LoadData(std::string& strFile)
{
	if(m_pAction)
	{
		// �����ǰ������.
		ClearData();

		if(0 == m_pAction->ReadRegionFromFile(strFile))
		{
			RefreshData();
		}
	}
}

// �洢����
void CPathFindEditDlg::SaveData(std::string strFile)
{
	if(m_pAction)
	{
		m_pAction->SaveRegionToFile(strFile);
	}
}

void CPathFindEditDlg::GetRegionAction()
{
	// �õ�����༭�ĳ�������
	if(m_pSceneManipulator)
	{
		Fairy::Action* action = m_pSceneManipulator->_getAction("PathFindEdit");
		m_pAction = (CPathFindAction *)action;
	}
}


// ɾ�������е�һ����
void CPathFindEditDlg::DelRegionPoint(unsigned long ulPointId)
{
	if(m_pListboxPointInRegion)
	{
		int iCount = m_pListboxPointInRegion->GetCount();
		int iIndex = m_pListboxPointInRegion->GetSelection();;
		m_pListboxPointInRegion->Delete(iIndex);
		if (iCount > 1)
		{
			if(iIndex > 0)
			{
				iIndex --;
			}
			m_pListboxPointInRegion->Select(iIndex);
			unsigned long ulPointId = 0;
			ulPointId = (unsigned long)m_pListboxPointInRegion->GetClientData(iIndex);
			if(m_pAction)
			{
				m_pAction->SelPoint(ulPointId);
			}
		}
	}
}


// �������ڱ༭�����ǡ�
void CPathFindEditDlg::SetEditRegionFlag(bool bIsEdit)
{
	m_bIsEditingRegion = bIsEdit;
}



