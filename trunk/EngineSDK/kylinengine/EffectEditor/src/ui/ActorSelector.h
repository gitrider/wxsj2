#ifndef __ActorSelector_H__
#define __ActorSelector_H__

#include <wx/panel.h>
#include <wx/toolbar.h>
#include <wx/listctrl.h>
// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class EffectEditorFrame;
namespace Fairy {
    class SceneManipulator;
}

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class ActorSelector : public wxPanel
{
public:
    ActorSelector(const wxString& resourceGroup = wxT("General"))
    {
        Init(resourceGroup);
    }

    ActorSelector(wxWindow *parent, const wxString& resourceGroup = wxT("General"),
                  wxWindowID winid = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                  const wxString& name = "ActorSelector")
    {
        Init(resourceGroup);

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "ActorSelector");

    ~ActorSelector();

    void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

    Fairy::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

	void SetParentFrame(EffectEditorFrame* frame){m_Frame = frame;}
	EffectEditorFrame* GetParentFrame(){ return m_Frame;}

    void Reload(void);
    void Reload(const wxString& resourceGroup);

protected:
    Fairy::SceneManipulator* mSceneManipulator;
	wxToolBar* m_toolBar;
	wxListCtrl* mActorList;
    wxString mResourceGroup;

	EffectEditorFrame* m_Frame;

    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);
	void OnReload(wxCommandEvent &e);
	void OnObjectListSelected(wxListEvent& e);
	void OnObjectListLeftDoubleClicked(wxCommandEvent& e);
};

#endif // __ActorSelector_H__
