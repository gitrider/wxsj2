#include <wx/wxprec.h>
#include "AvatarDialog.h"
#include "BodySelectorDialog.h"
#include "res/AvatarDialog_wdr.h"
#include "DataManipulator.h"

#include <OgreStringConverter.h>
BEGIN_EVENT_TABLE(AvatarDialog, wxPanel)

// EVT_BUTTON(ID_BUTTON_HEAD, 		AvatarDialog::OnAvatarDialogButtonChanged)
// EVT_BUTTON(ID_BUTTON_FACE, 		AvatarDialog::OnAvatarDialogButtonChanged)
// EVT_BUTTON(ID_BUTTON_SHOULDER,  AvatarDialog::OnAvatarDialogButtonChanged)
// EVT_BUTTON(ID_BUTTON_BODY,		AvatarDialog::OnAvatarDialogButtonChanged)
// EVT_BUTTON(ID_BUTTON_HAND,		AvatarDialog::OnAvatarDialogButtonChanged)
// EVT_BUTTON(ID_BUTTON_FOOT,		AvatarDialog::OnAvatarDialogButtonChanged)
// EVT_BUTTON(ID_BUTTON_LWEAPON,	AvatarDialog::OnAvatarDialogButtonChanged)
// EVT_BUTTON(ID_BUTTON_RWEAPON,	AvatarDialog::OnAvatarDialogButtonChanged)
// EVT_BUTTON(ID_BUTTON_SHIELD,	AvatarDialog::OnAvatarDialogButtonChanged)
// EVT_BUTTON(ID_BUTTON_RIDER,		AvatarDialog::OnAvatarDialogButtonChanged)

END_EVENT_TABLE()

AvatarDialog::~AvatarDialog(void)
{
	for(int i=0;i<a_num;i++)
	{
		if(m_AvatarDlg[i])
		{
			delete m_AvatarDlg[i];
			m_AvatarDlg[i] = NULL;
		}
	}
}

bool AvatarDialog::Create(wxWindow *parent, wxWindowID winid,
						  const wxPoint& pos, const wxSize& size,
						  long style, const wxString& name)
{
	if (!wxPanel::Create(parent, winid, pos, size, style, name))
		return false;
	AvatarDlg(this, true, true);

	for(int i=0;i<10;i++)
	{
		int nId = i + ID_BUTTON_HEAD;
		m_SelBtn[i] = wxDynamicCast(this->FindWindow(nId), wxButton); 
	}

	return true;
}

bool AvatarDialog::frameStarted(const Ogre::FrameEvent &evt)
{
	return true;
}

void AvatarDialog::OnAvatarDialogButtonChanged(wxCommandEvent &e)
{
	int nId = e.GetId();
	int nIndex = nId - ID_BUTTON_HEAD;

	if(NULL == m_AvatarDlg[nIndex])
	{
		m_AvatarDlg[nIndex] = new BodySelectorDialog(this,wxID_ANY,(E_MODEL_PART)nIndex,"选择部件");
	}

	if(GetDataManipulatr())
	{
		Fairy::CGod* pModel = GetDataManipulatr()->FindModel(GetDataManipulatr()->m_CurModelName);

		if(pModel != NULL)
		{
			m_AvatarDlg[nIndex]->SetUpdateName(GetDataManipulatr()->m_CurModelName);
			m_AvatarDlg[nIndex]->OnInitDialog();
		}
		else		//当前场景没有创建好的模型
		{
			if(!m_AvatarDlg[nIndex]->IsShown())
			{
				m_AvatarDlg[nIndex]->SetUpdateName("");
				m_AvatarDlg[nIndex]->OnInitDialog();
			}
		}
		GetDataManipulatr()->SetActiveButton(m_SelBtn[nIndex],nIndex);
	}
}