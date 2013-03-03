#ifndef __ParticleSystemSelector_H__
#define __ParticleSystemSelector_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ParticleSystemSelector.h"
#endif

#include <wx/panel.h>
#include <wx/dialog.h>
#include <map>
#include "WXSceneListener.h"
// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxListCtrl;

namespace Fairy {
    class SceneManipulator;
	class StaticEntityObject;
}

class ParticleEditorDialog;
// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class SetToSceneNodeDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SetToSceneNodeDialog)

public:
	SetToSceneNodeDialog() {}

	// full ctor
	SetToSceneNodeDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		Fairy::StaticEntityObject *staticEntity,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize( 289,108 ),
		long style = wxDEFAULT_DIALOG_STYLE)
		: mStaticEntity(staticEntity), mComboBox(NULL),
		wxDialog(parent, id, title, pos, size, style)
	{
	}

public:

	wxComboBox *mComboBox;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

protected:
	Fairy::StaticEntityObject *mStaticEntity;
};

class ParticleSystemSelector : public wxPanel, Fairy:: SceneListener
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

	void SelectItem(const wxString& itemName, bool bParticle = true);
public:
	virtual void onSelectObject(const Fairy::ObjectPtr& object);

protected:
    Fairy::SceneManipulator* mSceneManipulator;
    wxListCtrl* mParticleSystemList;
    wxString mResourceGroup;
	typedef std::map<wxString, long> FileItemMap;
	FileItemMap mFileMap[2];
	bool mFocused;

	ParticleEditorDialog *mParticleEditorDialog; 

    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnParticleSystemListSelected(wxListEvent& e);
	void OnParticleSystemRightClicked(wxListEvent& e);
    void OnParticleSystemListLeftDoubleClicked(wxCommandEvent& e);
	

	void OnReloadParticleList( wxCommandEvent &event );
	void OnSetToSceneObjectNode( wxCommandEvent &event );
};

#endif // __ModelSelector_H__
