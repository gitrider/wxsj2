#ifndef __AvatarDialog_H__
#define __AvatarDialog_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "AvatarDialog.h"
#endif

#include <wx/panel.h>
#include "Core/FairyObject.h"
#include "WXSceneListener.h"
#include <OgreFrameListener.h>
#include "BodySelectorDialog.h"
// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

namespace Fairy
{
    class SceneManipulator;  
}

class wxTextCtrl;
class wxButton;
const int a_num = 10;
class AvatarDialog : public wxPanel, Fairy::SceneListener, Ogre::FrameListener
{

public:

    AvatarDialog(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "AvatarDialog")
        : m_pSceneManipulator(NULL)
    {
		for(int i=0;i<a_num;i++)
		{
			m_AvatarDlg[i] = NULL;
		}

        Create(parent, winid, pos, size, style, name);
    }

    ~AvatarDialog(void);

    bool Create(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "AvatarDialog");

	virtual bool frameStarted(const Ogre::FrameEvent& evt);

//ÏûÏ¢º¯Êý
	DECLARE_EVENT_TABLE()
	void OnAvatarDialogButtonChanged(wxCommandEvent &e);
private:
    Fairy::SceneManipulator* m_pSceneManipulator;

	BodySelectorDialog* m_AvatarDlg[a_num];

	wxButton* m_SelBtn[a_num];

};

#endif // __AvatarDialog_H__
