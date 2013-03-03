#ifndef __SkillSelector_H__
#define __SkillSelector_H__


#include <wx/panel.h>
#include "Core/FairyObject.h"
#include "ModelSystem/FairyLogicModel.h"
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

class SkillSelector : public wxPanel
{
public:
    SkillSelector(const wxString& resourceGroup = wxT("General"))
    {
        Init(resourceGroup);
    }

    SkillSelector(wxWindow *parent, const wxString& resourceGroup = wxT("General"),
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style =  wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "SkillSelector")
    {
        Init(resourceGroup);
        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "SkillSelector");

    ~SkillSelector();

    void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

    Fairy::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

    void Reload(void);
    void Reload(const wxString& resourceGroup);

	void SetParentFrame(EffectEditorFrame* frame){m_Frame = frame;}
	EffectEditorFrame* GetParentFrame(){ return m_Frame;}

protected:
    Fairy::SceneManipulator* mSceneManipulator;
    wxListCtrl* mSkillList;
    wxString mResourceGroup;
	wxToolBar* m_toolBar;
	EffectEditorFrame* m_Frame;
    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

	
    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnEffectListSelected(wxListEvent& e);
    void OnSkillListLeftDoubleClicked(wxCommandEvent& e);

	void OnReloadEffectList( wxCommandEvent &event );
};

#endif // __ModelSelector_H__
