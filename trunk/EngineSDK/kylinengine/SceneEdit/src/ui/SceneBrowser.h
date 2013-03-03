#ifndef __SCENEBROWSER_H__
#define __SCENEBROWSER_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "SceneBrowser.h"
#endif

#include <wx/panel.h>

#include "WXSceneListener.h"

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxTreeCtrl;
class wxTreeEvent;
class wxTreeItemId;

class WXEffectEditDialog;

namespace Fairy {
    class SceneManipulator;
}

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class SceneBrowser : public wxPanel, Fairy::SceneListener
{
    typedef Fairy::ObjectPtr ObjectPtr;
    typedef Fairy::String String;

public:
    SceneBrowser(void)
    {
        Init();
    }

	~SceneBrowser();

    SceneBrowser(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "SceneBrowser")
    {
        Init();

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "SceneBrowser");

    void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

    Fairy::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

protected:
    Fairy::SceneManipulator* mSceneManipulator;

    wxTreeCtrl *mObjectsTree;
    wxTreeItemId mActivedItem;
    int mDisableNotifyEvent;

	WXEffectEditDialog *mEffectDialog;

    void Init(void);

    void AddObjectsToTree(void);
    void AddObjectToTree(const ObjectPtr& object);
    void RemoveObjectFromTree(const ObjectPtr& object);
    wxTreeItemId getObjectTreeItem(const ObjectPtr& object);
    wxTreeItemId getCategoryTreeItem(const wxTreeItemId& parent, const String& category);
    wxTreeItemId createCategoryTreeItem(const wxTreeItemId& parent, const String& category);

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnTreeSelChanged(wxTreeEvent& e);
    void OnRightUp(wxTreeEvent& event);
    void OnBeginLabelEdit(wxTreeEvent& event);
    void OnEndLabelEdit(wxTreeEvent& event);
    void OnTreeKeyDown(wxTreeEvent& event);

    void OnCreateModel(wxCommandEvent& event);
    void OnDeleteModel(wxCommandEvent& event);

	void OnContextMenu( ObjectPtr &object, wxTreeEvent& event );

public:
    virtual void onSceneReset(void);
    virtual void onAddObject(const Fairy::ObjectPtr& object);
    virtual void onRemoveObject(const Fairy::ObjectPtr& object);

    virtual void onSelectObject(const Fairy::ObjectPtr& object);
    virtual void onDeselectObject(const Fairy::ObjectPtr& object);
    virtual void onDeselectAllObjects(void);
    virtual void onRenameObject(const Fairy::ObjectPtr& object, const Ogre::String& oldName);
};

#endif // __SceneBrowser_H__
