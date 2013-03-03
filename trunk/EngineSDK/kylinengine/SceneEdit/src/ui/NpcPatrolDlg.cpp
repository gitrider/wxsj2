/////////////////////////////////////////////////////////////////////////////
// Name:        NpcPatrolDlg.cpp
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "NpcPatrolDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "NpcPatrolDlg.h"

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
#include "res/NpcPatrolEdit_wdr.h"
#include "NpcPatrolAction.h"
#include "UnreachRegion.h"

using namespace Fairy;

BEGIN_EVENT_TABLE(CNpcPatrolEditDlg, wxPanel)
	  EVT_BUTTON(ID_BUTTON_DEL_LINE, CNpcPatrolEditDlg::OnDelRegion)
	  EVT_BUTTON(ID_BN_ADD_NEW_LINE, CNpcPatrolEditDlg::OnAddNewRegion)
	  EVT_BUTTON(ID_BUTTON_DEL_POINT,  CNpcPatrolEditDlg::OnDelPointInRegion)
	  EVT_BUTTON(ID_BUTTON_LINE_OK,  CNpcPatrolEditDlg::OnFinishRegion)

	  EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_LINE, CNpcPatrolEditDlg::OnRegionListCtrlItemSelected)
	  EVT_LISTBOX(ID_LISTBOX_POINT, CNpcPatrolEditDlg::OnPointListBox)
	  EVT_CHECKBOX(ID_IFY, CNpcPatrolEditDlg::OnPointUseY)
	  EVT_TEXT_ENTER(ID_POINT_X, CNpcPatrolEditDlg::OnEditXYZ)
	  EVT_TEXT_ENTER(ID_POINT_Y, CNpcPatrolEditDlg::OnEditXYZ)
	  EVT_TEXT_ENTER(ID_POINT_Z, CNpcPatrolEditDlg::OnEditXYZ)
	 
END_EVENT_TABLE()


CNpcPatrolEditDlg::CNpcPatrolEditDlg(const wxString& resourceGroup)
:m_pListCtrlRegion(NULL)
,m_pListboxPointInRegion(NULL)
,m_pDelPointBn(NULL)
,m_pSceneManipulator(NULL)
,m_pAction(NULL)
,m_pRegionOkBn(NULL)
,m_pDelRegionBn(NULL)
,m_pAddNewRegionBn(NULL)
,m_bIsEditingRegion(false)
,m_iCurSelRegionIndex(-1)
,m_pPointX(NULL)
,m_pPointY(NULL)
,m_pPointZ(NULL)
,m_pUseY(NULL)
,m_pDistance(NULL)

{
    //Init(resourceGroup);
	InitData();
}

CNpcPatrolEditDlg::CNpcPatrolEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup,
							wxWindowID winid,
							const wxPoint& pos,
							const wxSize& size,
							long style,
							const wxString& name
							)
							:m_pListCtrlRegion(NULL)
							,m_pListboxPointInRegion(NULL)
							,m_pDelPointBn(NULL)
							,m_pSceneManipulator(NULL)
							,m_pAction(NULL)
							,m_pRegionOkBn(NULL)
							,m_pDelRegionBn(NULL)
							,m_pAddNewRegionBn(NULL)
							,m_bIsEditingRegion(false)
							,m_iCurSelRegionIndex(-1)
							,m_pPointX(NULL)
							,m_pPointY(NULL)
							,m_pPointZ(NULL)
							,m_pUseY(NULL)
							,m_pDistance(NULL)
{
    //Init(resourceGroup);
    Create(parent, winid, pos, size, style, name);
	InitData();
}

// ���һ������ı༭
void CNpcPatrolEditDlg::FinishRegion()
{

	//if(m_pAction)
	//{
	//	if(m_pListboxRegion)
	//	{
	//		int iIndex = m_pListboxRegion->GetSelection();
	//		if(-1 != iIndex)
	//		{
	//			unsigned long uiId = (unsigned long)m_pListboxRegion->GetClientData(iIndex);
	//			if(!m_pAction->IsRegionValidate(uiId))
	//			{
	//				wxMessageBox(_T("��ǰ�����в��Ϸ��ĵ㣬 �������\n"), _T("���ڱ༭����"), wxOK, this);
	//				return;

	//			}
	//		}
	//	}
	//}//

	if(m_pAction)
	{
		if(m_pListCtrlRegion)
		{
			int iIndex = m_iCurSelRegionIndex;
			if(-1 != iIndex)
			{
				unsigned long uiId = (unsigned long)m_pListCtrlRegion->GetItemData(iIndex);
				//m_pAction->ReshapeArea(uiId);
				//m_pAction->HideRegionArea(uiId, false);//
			}
		}
		
	}
	
	if(m_pAddNewRegionBn)
	{
		m_bIsEditingRegion = false;
		m_pAddNewRegionBn->Enable( true );
	}
}

void CNpcPatrolEditDlg::Init(const wxString& resourceGroup)
{
 //   mSceneManipulator = NULL;
 //   mSplitter = NULL;
 //   mBrushesTree = NULL;
	//mBrushesPreview = NULL;

	//mFlipHorizontal = NULL;
	//mFlipVertical = NULL;
	//mRotateRightAngle = NULL;
	//mRandom = NULL;

	//mEnablePreview = NULL;

	//mCurrentOrientationType = 0;
	//mCurrentFileName = "";

 //   // ����Ԥ��ͼ�Ĵ�С
 //   mPreviewImageWidth = 64;
 //   mPreviewImageHeight = 64;
	//// ��ʼ��image������Ϊ��ɫ
	//mCurrentPreviewImage.Create( mPreviewImageWidth, mPreviewImageHeight, true );

 //   // ��úڰ�ͼ
 //   mBlackWhitePreviewImage = CellGridBitmaps(1).ConvertToImage();

 //   mResourceGroup = resourceGroup;

 //   mPaintInOneGrid = false;//
}

bool CNpcPatrolEditDlg::Create(
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

	EditRegionDlg(this, true, true);

	////////////////////////////////////////////////////////////////////////////////////
	//
	// ��ȡ���ؼ�������
	//

	// �������ؼ���ָ��.
    m_pListCtrlRegion = wxDynamicCast(FindWindow(ID_LISTCTRL_LINE), wxListCtrl);

	// ��õ�ؼ���ָ��.
	m_pListboxPointInRegion = wxDynamicCast(FindWindow(ID_LISTBOX_POINT), wxListBox);

	// �õ�ɾ����Ŀؼ���ָ��
	m_pDelPointBn =  wxDynamicCast(FindWindow(ID_BUTTON_DEL_POINT), wxButton);

	// �õ��������ťָ��
	m_pRegionOkBn = wxDynamicCast(FindWindow(ID_BUTTON_LINE_OK), wxButton);

	// �õ�ɾ������ťָ�롣
	m_pDelRegionBn = wxDynamicCast(FindWindow(ID_BUTTON_DEL_LINE), wxButton);

	// �õ��������ť��ָ�롣
    m_pAddNewRegionBn = wxDynamicCast(FindWindow(ID_BN_ADD_NEW_LINE), wxButton);


	m_pPointX = wxDynamicCast(FindWindow(ID_POINT_X), wxTextCtrl);
	
	m_pPointY = wxDynamicCast(FindWindow(ID_POINT_Y), wxTextCtrl);
	
	m_pPointZ = wxDynamicCast(FindWindow(ID_POINT_Z), wxTextCtrl);

	m_pUseY = wxDynamicCast(FindWindow(ID_IFY), wxCheckBox);

	m_pDistance = wxDynamicCast(FindWindow(ID_DISTANCE), wxTextCtrl);

	//
	//if(m_pDelPointBn)
	//{
	//	wxWindow* pWindow = NULL;
	//	
	//	pWindow = (wxWindow*)m_pDelPointBn;
	//	pWindow->Show(false);
	//		
	//}//

	if(m_pListCtrlRegion)
	{
		m_pListCtrlRegion->DeleteAllItems();
	}

	if(m_pListboxPointInRegion)
	{
		m_pListboxPointInRegion->Clear();
	}
	//mBrushesPreview = wxDynamicCast(this->FindWindow(feID_PIXMAP_PREVIEW), wxStaticBitmap);
	//mFlipHorizontal = wxDynamicCast(this->FindWindow(feID_FLIP_HORIZONTAL), wxCheckBox);
	//mFlipVertical = wxDynamicCast(this->FindWindow(feID_FLIP_VERTICAL), wxCheckBox);
	//mRotateRightAngle = wxDynamicCast(this->FindWindow(feID_ROTATE_RIGHT_ANGLE), wxCheckBox);
	//mRandom = wxDynamicCast(this->FindWindow(feID_RANDOM), wxCheckBox);

	//mEnablePreview = wxDynamicCast(this->FindWindow(feID_ENABLE_PIXMAP_PREVIEW), wxCheckBox);

 //   ReloadTextureList();//
	m_pListCtrlRegion->InsertColumn(0, "���");	
	m_pListCtrlRegion->InsertColumn(1, "����");	

	GetRegionAction();
    return true;
}

CNpcPatrolEditDlg::~CNpcPatrolEditDlg(void)
{
}

void CNpcPatrolEditDlg::OnDelRegion(wxCommandEvent &event)
{
	// test
	if(wxMessageBox(_T("��ȷ��Ҫɾ�����������\n"), _T("ɾ�����򾯸棡"), wxICON_QUESTION | wxYES_NO, this) == wxYES)
	{
		// ������ʱû����.
		DelRegion(0);
	}
}

bool CNpcPatrolEditDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}


void CNpcPatrolEditDlg::ActiveAction(bool bActive)
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

void CNpcPatrolEditDlg::SetActive(bool bShow)
{

	if (bShow)
    {
		if(m_pSceneManipulator)
		{
			Fairy::Action* action = m_pSceneManipulator->_getAction("NpcPatrol");
			m_pSceneManipulator->setActiveAction(action);
			m_pAction = (CNpcPatrolAction *)action;			
		}
       
    }
	else
	{
		if(m_pAction)
		{
			m_pAction->SetOldHitPointMaterial();
			//m_pAction->setActive(false);
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// ��ʾ���س���
	if(m_pAction)
	{
		m_pAction->HideScene(bShow);

		if(bShow)
		{
			// ���ݵ��εĸ߶���ʾ����.
			m_pAction->SetRegionHeight();
		}
	}

}

void CNpcPatrolEditDlg::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		Fairy::Action* action = m_pSceneManipulator->_getAction("NpcPatrol");
		m_pAction = (CNpcPatrolAction *)action;
		SetActive(false);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// �����༭�Ļص�����
//

void CNpcPatrolEditDlg::onAddObject(const Fairy::ObjectPtr& object)
{
	if(m_pAction)
	{
		//m_pAction->get
		;
	}
}

// ѡ��һ������Ļص�������
void CNpcPatrolEditDlg::onRegionSel(unsigned long ulRegionId)
{
	if(NULL == m_pListCtrlRegion)
	{
		return;
	}	

	unsigned long ulFindRegionId = 0;

	int iRegionCount = m_pListCtrlRegion->GetItemCount();

	for(int i = 0; i < iRegionCount; i++)
	{
		// �õ�����id
		ulFindRegionId = (unsigned long)m_pListCtrlRegion->GetItemData(i);

		if(ulFindRegionId == ulRegionId)
		{
			m_bIsChangeCamera = false;
			m_pListCtrlRegion->SetItemState(i, wxLIST_STATE_SELECTED , wxLIST_STATE_SELECTED);
			DoSelRegion();

			m_bIsChangeCamera = true;
			break;
		}
		
	}

}


// ����༭�Ļص������� ֪ͨ�������һ���µĵ�.
void CNpcPatrolEditDlg::onRegionAddNewPoint(unsigned long ulRegionId,unsigned long ulPointId, Ogre::Vector3 position)
{
	// ֻ�е�ǰ�������������ʱ�����.
	if(m_ulCurRegionId == ulRegionId)
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
		if( position.y == -100000.0f )
		{
			m_pPointY->Clear();
			m_pPointY->Enable( false );
			m_pUseY->SetValue( false );
		}
		else
		{
			sprintf( buf, "%.4f", (position.y) / fScale );
			m_pPointY->Clear();
			m_pPointY->Enable( true );
			m_pPointY->WriteText( buf );
			m_pUseY->SetValue( true );
		}
		sprintf( buf, "%.4f", (position.z - fTerrainDataPosZ) / fScale );
		m_pPointZ->Clear();
		m_pPointZ->WriteText( buf );
	}
}

// ɾ��һ����Ļص�����
void CNpcPatrolEditDlg::onRegionDelPoint(unsigned long ulRegionId,unsigned long ulPointId)
{
	// ֻ�е�ǰ�������������ʱ�����.
	if(m_ulCurRegionId == ulRegionId)
	{
		DelRegionPoint(ulPointId);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ui���棬 �¼���Ӧ����
//

// ��ť������ ���һ������.
void CNpcPatrolEditDlg::OnAddNewRegion( wxCommandEvent &event )
{
	AddNewRegion();
}

void CNpcPatrolEditDlg::OnFinishRegion( wxCommandEvent &event )
{
	FinishRegion();
}

void CNpcPatrolEditDlg::OnDelPointInRegion( wxCommandEvent &event )
{
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->getOperatorManager()->undo();
	}
	 
}

void CNpcPatrolEditDlg::OnPointUseY( wxCommandEvent &event )
{
	//if( m_bIsEditingRegion )
	//{
		CPointInfo* pInfo = 0;
		int iIndex = m_pListboxPointInRegion->GetSelection();
		if( iIndex != -1 )
			pInfo = m_pAction->GetRegionById( m_pAction->GetCurEditRegionId() )->GetPointInfoByIndex( iIndex );
		if( m_pUseY->GetValue() )
		{
			m_pUseY->SetValue( true );
			m_pPointY->Enable( true );
			if( pInfo ) pInfo->SetValuedY( true );
		}
		else
		{
			m_pUseY->SetValue( false );
			m_pPointY->Enable( false );
			if( pInfo ) pInfo->SetValuedY( false );
		}
	//}
	//else
	//{
	//	m_pUseY->SetValue( !m_pUseY->GetValue() );
	//}
}

void CNpcPatrolEditDlg::OnEditXYZ( wxCommandEvent &event )
{
	//m_bIsEditingRegion = true;

	Ogre::Vector3 pt;
	pt.x = (float)atof( m_pPointX->GetValue() );
	pt.y = (float)atof( m_pPointY->GetValue() );
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
	if( m_pUseY->GetValue() )
	{
		pt.y = pt.y * fScale;
	}
	else
	{
		pt.y = 0.0f;
	}

	m_pAction->ChangeCurEditPointPos( m_ulCurRegionId, m_ulCurPointId, pt );
}


/*******/
///////////////////////////////////////////////////////////////////////////////////////////
//
// �߼����ݲ���
//

// ɾ��һ������
void CNpcPatrolEditDlg::DelRegion(unsigned long ulId)
{
	if(NULL == m_pListCtrlRegion)
	{
		return;
	}

	// ɾ����ǰ�������Ҫѡ�е������index
	int iNextIndex   = -1;
	int iIndex = m_iCurSelRegionIndex;
	int iRegionCount = m_pListCtrlRegion->GetItemCount();

	unsigned long ulRegionId = 0;
	// ɾ����ǰ�������Ҫѡ�е������ID
	unsigned long ulNextRegionId = 0;

	if(-1 != iIndex)
	{
		if(iIndex > 0)
		{
			// �������ѡ��ɾ����ͷһ������
			// ��һ��ѡ���������ǵ�ǰѡ���ǰһ������
			iNextIndex = iIndex-1;
		}
		else
		{
			if(iRegionCount > 1)
			{				
				iNextIndex = iIndex+1;				
			}
			else
			{
				iNextIndex = -1;
			}
		}

		if(-1 != iNextIndex)
		{
			ulNextRegionId =(unsigned long) m_pListCtrlRegion->GetItemData(iNextIndex);
		}

		// �õ�����id
		ulRegionId = (unsigned long) m_pListCtrlRegion->GetItemData(iIndex);
		if(m_pAction)
		{
			// ɾ����ǰ��region����
			m_pAction->DelRegion(ulRegionId);
									
			//���õ�ǰ���ڱ༭������
			m_ulCurRegionId = ulNextRegionId;
			m_pAction->SelRegion(ulNextRegionId);

			// �����ǰ�����ݣ� ���������
			m_pListboxPointInRegion->Clear();
			
			// �õ���һ��Ҫѡ�������
			CUnreachRegion* pRegion = m_pAction->GetRegionById(ulNextRegionId);
			if(pRegion)
			{
				int iPointCount = pRegion->GetPointCount();
				CPointInfo* pPointInfo = NULL;
				for(int i = 0; i < iPointCount; i++)
				{
					pPointInfo = pRegion->GetPointInfoByIndex(i);
					if(pPointInfo)
					{
						RegionAddNewPoint(pPointInfo->m_ulPointId, true);
					}
				}//
			}
		
		}//if(m_pAction)

		if(-1 != iNextIndex)
		{
			m_iCurSelRegionIndex = iNextIndex;
			m_pListCtrlRegion->SetItemState(iNextIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		}

        m_pListCtrlRegion->DeleteItem(iIndex);
        UpdateListCtrlRegionDataView();

		// ���õ�ǰû�б༭������
		SetEditRegionFlag(false);		
	}// if(-1 != iIndex)
}

// ���һ������.
void CNpcPatrolEditDlg::AddNewRegion()
{
	SetActive(true);
	
	// ���һ���µ�����
	char buf[256];

	if(m_pAction)
	{
		m_pAction->UnSelRegion(m_ulCurRegionId);
		m_ulCurRegionId = m_pAction->AddNewRegion();

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "����%d", m_ulCurRegionId);

		int iCount = m_pListCtrlRegion->GetItemCount();
        m_pListCtrlRegion->InsertItem(iCount,buf);
		m_pListCtrlRegion->SetItemData(iCount,m_ulCurRegionId);

		m_pListCtrlRegion->SetItem(iCount,1,buf);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", m_ulCurRegionId);
		m_pListCtrlRegion->SetItem(iCount,0,buf);

		// ���õ�ǰ���ڱ༭��region����.
		m_iCurSelRegionIndex = iCount;
		m_pListboxPointInRegion->Clear();

		if(m_pAddNewRegionBn)
		{
			m_bIsEditingRegion = true;
			m_pAddNewRegionBn->Enable( false );
		}			
	}
}

// �ڵ�ǰ���������һ���µĵ�
void CNpcPatrolEditDlg::RegionAddNewPoint(unsigned long ulId, bool bRefresh)
{
	if(0 > ulId)
	{
		// ��ǰ���id���Ϸ�
		return;
	}

	if(0 > m_ulCurRegionId)
	{
		// ��ǰ�����id���Ϸ�
		return;
	}

	// ���һ���µ�����
	char buf[256];
	if(m_pAction)
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "����%d->��%d", m_ulCurRegionId, ulId);

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
void CNpcPatrolEditDlg::InitData()
{
	m_ulCurRegionId = 0;	
	m_ulCurPointId  = 0;	
	m_ulCurLineId   = 0;	

	m_bIsChangeCamera = true;
}

// ����ѡ��ListCtrl��Ӧ���¼�����
void CNpcPatrolEditDlg::OnRegionListCtrlItemSelected(wxListEvent& event)
{
	if (event.GetInt() == -1)
	{
		// "ListCtrl has no selections anymore
		return;
	}
	m_iCurSelRegionIndex = event.GetIndex();
	m_ulCurRegionId = m_pListCtrlRegion->GetItemData(m_iCurSelRegionIndex);
	m_pListCtrlRegion->SetItemState(m_iCurSelRegionIndex, wxLIST_STATE_SELECTED , wxLIST_STATE_SELECTED);

	if(m_bIsEditingRegion)
	{
		wxMessageBox(_T("�����ڱ༭һ������ �����������༭��ť\n"), _T("���ڱ༭����"), wxOK, this);

		//if(-1 != m_iCurSelRegionIndex)
		//{
		//	m_pListCtrlRegion->SetItemState(m_iCurSelRegionIndex, wxLIST_STATE_SELECTED , wxLIST_STATE_SELECTED);
		//}

		return;
	}

	if(event.GetId() == ID_LISTCTRL_LINE)
	{
		DoSelRegion();
	}

}

// ��ѡ��listbox��Ӧ���¼�����
void CNpcPatrolEditDlg::OnPointListBox(wxCommandEvent &event)
{
	if (event.GetInt() == -1)
    {
       // "ListBox has no selections anymore
        return;
    }

	//if(m_bIsEditingRegion)
	//{
	//	return;
	//}

    if(event.GetId() == ID_LISTBOX_POINT)
	{
		int iIndex = m_pListboxPointInRegion->GetSelection();
		if( iIndex != -1 )
		{
			CPointInfo* pInfo = m_pAction->GetRegionById( m_pAction->GetCurEditRegionId() )->GetPointInfoByIndex( iIndex );
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
				if( pInfo->GetValuedY() )
				{
					sprintf( buf, "%.4f", (position.y ) / fScale );
					m_pPointY->Clear();
					m_pPointY->Enable( true );
					m_pPointY->WriteText( buf );
					m_pUseY->SetValue( true );
				}
				else
				{
					m_pPointY->Clear();
					m_pPointY->Enable( false );
				}
				sprintf( buf, "%.4f", (position.z - fTerrainDataPosZ) / fScale );
				m_pPointZ->Clear();
				m_pPointZ->WriteText( buf );
			}

		}

		DoSelPoint();
	}

}

// ѡ�е�Ĳ���
void CNpcPatrolEditDlg::DoSelPoint()
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
		// �õ�����id
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
					CPointInfo* pPoint = pRegion->GetPointInfoById(dw);
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
				char buf[128];
				sprintf( buf, "%.4f", fDistance / m_pAction->getSceneManipulator()->getBaseScale() );
				m_pDistance->Clear();
				m_pDistance->WriteText( buf );
			}
		}
	}
}


// �������
void CNpcPatrolEditDlg::ClearData()
{
	ClearUiData();

	if(m_pAction)
	{
		m_pAction->ClearRegionData();
	}
}

// ������ѡ��ʱ�Ĳ���
void CNpcPatrolEditDlg::DoSelRegion()
{
	if(NULL == m_pListCtrlRegion)
	{
		return;
	}

	int iIndex = m_iCurSelRegionIndex;
	// ��¼��ǰ��listbox��ѡ�������index
	m_iCurSelRegionIndex = iIndex;
	unsigned long ulRegionId = 0;
	if(-1 != iIndex)
	{
		// �õ�����id
		ulRegionId = (unsigned long)m_pListCtrlRegion->GetItemData(iIndex);
		if(m_pAction)
		{
			CUnreachRegion* pRegion = m_pAction->GetRegionById(ulRegionId);
			if(NULL == pRegion)
			{
				return ;
			}

			m_pAction->UnSelRegion(m_ulCurRegionId);
			// ��ǰ���ڱ༭������
			m_ulCurRegionId = ulRegionId;
			m_pAction->SelRegion(ulRegionId, m_bIsChangeCamera);

			// �����ǰ������.
			m_pListboxPointInRegion->Clear();

			int iPointCount = pRegion->GetPointCount();
			CPointInfo* pPointInfo = NULL;
			for(int i = 0; i < iPointCount; i++)
			{
				pPointInfo = pRegion->GetPointInfoByIndex(i);
				if(pPointInfo)
				{
					RegionAddNewPoint(pPointInfo->m_ulPointId, true);
				}

			}//
		
		}
	}
}

// ��RegionAction��ˢ������, ���̵�ʱ��ʹ��
void CNpcPatrolEditDlg::RefreshData()
{
	if((NULL == m_pAction)||(NULL == m_pListCtrlRegion))
	{
		return;
	}
	// ���ui��������.
	ClearUiData();

	// 
	UNREACHREGIONMAP::iterator itRegion;
	UNREACHREGIONMAP::iterator itEndRegion;
	unsigned long   ulId    = 0;
	int				iCount  = 0;

	CUnreachRegion* pRegion = NULL;

	char buf[256];
	// �洢ÿ���������Ϣ
	itEndRegion  = m_pAction->m_UnreachRegionMap.end();
	for(itRegion = m_pAction->m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
	{	
		pRegion = itRegion->second;
		if(pRegion)
		{
			memset(buf, 0, sizeof(buf));
	
			// �洢�����id
			ulId = pRegion->m_ulId;
			sprintf(buf, "����%d", ulId);
			iCount = m_pListCtrlRegion->GetItemCount();
			m_pListCtrlRegion->InsertItem(iCount,buf);
			m_pListCtrlRegion->SetItemData(iCount,ulId);

			m_pListCtrlRegion->SetItem(iCount,1,buf);

			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%d", ulId);
			m_pListCtrlRegion->SetItem(iCount,0,buf);			
		}
	}

	if(m_pListCtrlRegion->GetItemCount())
	{
		m_pListCtrlRegion->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
}


// ˢ�������еĵ������
void CNpcPatrolEditDlg::RefreshRegionPointData(unsigned long ulRegionId)
{
	if(m_pAction)
	{

		CUnreachRegion* pRegion = m_pAction->GetRegionById(ulRegionId);
		if(NULL == pRegion)
		{
			return ;
		}
		
		// �����ǰ������.
		m_pListboxPointInRegion->Clear();

		int iPointCount = pRegion->GetPointCount();
		CPointInfo* pPointInfo = NULL;
		for(int i = 0; i < iPointCount; i++)
		{
			pPointInfo = pRegion->GetPointInfoByIndex(i);
			if(pPointInfo)
			{
				RegionAddNewPoint(pPointInfo->m_ulPointId, true);
			}
		}//
	}
}

// ���ui���������
void CNpcPatrolEditDlg::ClearUiData()
{
	if(m_pListCtrlRegion)
	{
		m_pListCtrlRegion->DeleteAllItems();
	}

	if(m_pListboxPointInRegion)
	{
		m_pListboxPointInRegion->Clear();
	}
}


// ��ȡ����
void CNpcPatrolEditDlg::LoadData(std::string& strFile)
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
void CNpcPatrolEditDlg::SaveData(std::string strFile)
{
	if(m_pAction)
	{
		m_pAction->SaveRegionToFile(strFile);
			
		//yangfei_begin
		std::string strNavMapFile = strFile;
		const char* dotChar = ".";
		std::basic_string <char>::size_type chBegin, chEnd;
		chBegin = strNavMapFile.find_last_of(dotChar);
		chEnd = strNavMapFile.npos;
		
		strNavMapFile.replace(chBegin,chEnd,".nav");
	
		m_pAction->SaveRegionToNavMapFile(strNavMapFile);
	}
}

void CNpcPatrolEditDlg::GetRegionAction()
{

	// �õ�����༭�ĳ�������
	if(m_pSceneManipulator)
	{
		Fairy::Action* action = m_pSceneManipulator->_getAction("NpcPatrolEdit");
		m_pAction = (CNpcPatrolAction *)action;
	}

}

// ɾ�������е�һ����
void CNpcPatrolEditDlg::DelRegionPoint(unsigned long ulPointId)
{
	if(m_pListboxPointInRegion)
	{
		int iCount = m_pListboxPointInRegion->GetCount();
		m_pListboxPointInRegion->Delete(iCount - 1);

		if(iCount > 1)
		{
			// �õ�ѡ�е�����
			int iIndex = iCount - 2;

			m_pListboxPointInRegion->Select(iIndex);
			unsigned long ulPointId = 0;
			if(-1 != iIndex)
			{
				// �õ�����id
				ulPointId = (unsigned long)m_pListboxPointInRegion->GetClientData(iIndex);
				if(m_pAction)
				{
					m_pAction->SelPoint(ulPointId);
				}
			}
		}
	}
}

// �������ڱ༭�����ǡ�
void CNpcPatrolEditDlg::SetEditRegionFlag(bool bIsEdit)
{
	m_bIsEditingRegion = bIsEdit;

	if(m_pAddNewRegionBn)
	{
		m_pAddNewRegionBn->Enable( !bIsEdit );
	}
}

// ��������༭��ʾ
void CNpcPatrolEditDlg::UpdateListCtrlRegionDataView()
{
	if(NULL == m_pAction)
	{	
		return;
	}

    char buf[256];
    for (int i=0; i < m_pListCtrlRegion->GetItemCount(); ++i)
    {
		m_pListCtrlRegion->SetItemData(i,i);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", i);
        m_pListCtrlRegion->SetItem(i,0,buf);
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "����%d", i);
		m_pListCtrlRegion->SetItem(i,1,buf);
    }
}

