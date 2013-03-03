#ifndef __BulletSelector_H__
#define __BulletSelector_H__


#include <wx/panel.h>
#include "WXGraphDialog.h"
#include "Core/FairyObject.h"

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxListCtrl;
class EffectEditorFrame;

namespace Fairy {
    class SceneManipulator;
}

class EffectObjectPropertyEditor;
// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class BulletSelector : public wxPanel
{
public:
    BulletSelector(const wxString& resourceGroup = wxT("General"))
    {
        Init(resourceGroup);
    }

    BulletSelector(wxWindow *parent, const wxString& resourceGroup = wxT("General"),
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style =  wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "BulletSelector")
    {
        Init(resourceGroup);

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "BulletSelector");

    ~BulletSelector();

    void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

    Fairy::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

    void Reload(void);
    void Reload(const wxString& resourceGroup);

	void SetParentFrame(EffectEditorFrame* frame){m_Frame = frame;}
	EffectEditorFrame* GetParentFrame(){ return m_Frame;}

	void DestroySelectObject();

protected:
    Fairy::SceneManipulator* mSceneManipulator;
    wxListCtrl* mBulletFlowList;
    wxString mResourceGroup;
	wxToolBar* m_toolBar;
	EffectEditorFrame* m_Frame;
    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

	Fairy::ObjectPtr mObject;
	Ogre::NameValuePairList mParameters;


    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnBulletListSelected(wxListEvent& e);
    void OnBulletListLeftDoubleClicked(wxCommandEvent& e);

	void OnReloadBulletList( wxCommandEvent &event );
};

#endif // __BulletSelector_H__
