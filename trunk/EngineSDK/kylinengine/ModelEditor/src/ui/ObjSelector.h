#ifndef __ObjSelector_H__
#define __ObjSelector_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ObjSelector.h"
#endif

#include <wx/panel.h>
#include "Core/FairyObject.h"

#include "WXEditorFrame.h"
// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxTreeCtrl;
class wxTreeEvent;
class wxTreeItemId;

namespace Fairy {
    class SceneManipulator;
}

class ActorEditorFrame;

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class ObjSelector : public wxPanel
{
public:
    ObjSelector(const wxString& resourceGroup = wxT("General"))
    {
        Init(resourceGroup);
    }

    ObjSelector(wxWindow *parent, const wxString& resourceGroup = wxT("General"),
                  wxWindowID winid = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                  const wxString& name = "ObjSelector")
    {
        Init(resourceGroup);

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "ObjSelector");

    ~ObjSelector();

    void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

    Fairy::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

	void selItemByName(wxString& itemName);
	void SetParentFrame(ActorEditorFrame* pParentFrame);

    void Reload(void);
    void Reload(const wxString& resourceGroup);
	void addResources(const wxTreeItemId& root, const wxString& pattern, const wxString& typeName, const wxString& parameterName,const wxString& groupname);
protected:
    Fairy::SceneManipulator* mSceneManipulator;
    wxTreeCtrl* mActorList;
    wxString mResourceGroup;
	wxTreeItemId mActivedItem;
	wxButton* mBtnUpdate;

	ActorEditorFrame* m_pParentFrame;

    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

    wxTreeItemId AddLeaf(const wxTreeItemId& root, const wxString& name);
    wxTreeItemId AddBranch(const wxTreeItemId& root, const wxString& name);
    wxTreeItemId CreateChildItem(const wxTreeItemId& parent, const wxString& name);   
	const Fairy::String DoActiveItem(const wxTreeItemId& item);

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnActorListSelChanged(wxTreeEvent& e);
    void OnChildFocus(wxChildFocusEvent& e);
	void OnBtnUpdate(wxCommandEvent& e);
};

#endif // __ObjSelector_H__
