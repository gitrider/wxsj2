#ifndef __ActorSelector_H__
#define __ActorSelector_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ActorSelector.h"
#endif

#include <wx/panel.h>
#include <map>
#include "WXSceneListener.h"

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxTreeCtrl;
class wxTreeEvent;
class wxTreeItemId;

namespace Fairy {
    class SceneManipulator;
}

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class ActorSelector : public wxPanel, Fairy:: SceneListener
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

    void Reload(void);
    void Reload(const wxString& resourceGroup);

	void SelectItem(const wxString& itemName);
public:
	  virtual void onSelectObject(const Fairy::ObjectPtr& object);

protected:
    Fairy::SceneManipulator* mSceneManipulator;
    wxTreeCtrl* mActorList;
    wxString mResourceGroup;
	typedef std::map<wxString, wxTreeItemId> FileIdMap;
	FileIdMap mFileMap;
	bool mFocused;

    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

    wxTreeItemId AddLeaf(const wxTreeItemId& root, const wxString& name);
    wxTreeItemId AddBranch(const wxTreeItemId& root, const wxString& name);
    wxTreeItemId CreateChildItem(const wxTreeItemId& parent, const wxString& name);
    void DoActiveItem(const wxTreeItemId& item);

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnActorListSelChanged(wxTreeEvent& e);
    void OnChildFocus(wxChildFocusEvent& e);
};

#endif // __ActorSelector_H__
