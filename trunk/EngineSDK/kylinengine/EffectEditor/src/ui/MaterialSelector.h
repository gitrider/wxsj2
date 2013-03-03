#ifndef __MaterialSelector_H__
#define __MaterialSelector_H__


#include <wx/panel.h>
#include <wx/menu.h>
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

class MaterialSelector : public wxPanel
{
public:
    MaterialSelector(const wxString& resourceGroup = wxT("General"))
    {
        Init(resourceGroup);
    }

    MaterialSelector(wxWindow *parent, const wxString& resourceGroup = wxT("General"),
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style =  wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "MaterialSelector")
    {
        Init(resourceGroup);

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "MaterialSelector");

    ~MaterialSelector();

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
    wxListCtrl* mMaterialList;
    wxString mResourceGroup;
	wxToolBar* m_toolBar;
	EffectEditorFrame* m_Frame;
    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnEffectListSelected(wxListEvent& e);
    void OnEffectListLeftDoubleClicked(wxCommandEvent& e);

	void OnMaterialRightClicked(wxListEvent& event);
	void OnReloadEffectList( wxCommandEvent &event );

	void OnSetParticleMaterial(wxCommandEvent& e);
	void OnSetEffectMaterial(wxCommandEvent& e);
	void OnSetSkillMaterial(wxCommandEvent& e);
};

#endif // __ModelSelector_H__
