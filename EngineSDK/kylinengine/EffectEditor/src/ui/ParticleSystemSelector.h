#ifndef __ParticleSystemSelector_H__
#define __ParticleSystemSelector_H__

#include "core/FairyObject.h"

#include <wx/panel.h>
#include <wx/bmpbuttn.h>
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

class ParticleSystemSelector : public wxPanel
{
public:
    ParticleSystemSelector(const wxString& resourceGroup = wxT("General"))
    {
        Init(resourceGroup);
    }

    ParticleSystemSelector(wxWindow *parent, const wxString& resourceGroup = wxT("General"),
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "ParticleSystemSelector")
    {
        Init(resourceGroup);

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "ParticleSystemSelector");

    ~ParticleSystemSelector();

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
    wxListCtrl* mParticleSystemList;
    wxString mResourceGroup;
	wxToolBar* m_toolBar;
	wxBitmapButton* m_ReloadParticleBtn;
	EffectEditorFrame* m_Frame;
    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

	Fairy::ObjectPtr mObject;
	Fairy::PropertyNameStringPairList mParameters;

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnParticleSystemListSelected(wxListEvent& e);
    void OnParticleSystemListLeftDoubleClicked(wxCommandEvent& e);

	void OnReloadParticleList( wxCommandEvent &event );
	void OnStopParticle(wxCommandEvent &event);
};

#endif // __ModelSelector_H__
