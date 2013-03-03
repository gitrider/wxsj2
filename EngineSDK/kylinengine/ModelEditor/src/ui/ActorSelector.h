//说明：模型文件(.mesh)显示界面
#ifndef __ActorSelector_H__
#define __ActorSelector_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ActorSelector.h"
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

class ActorSelector : public wxPanel
{
	typedef Fairy::ObjectPtr ObjectPtr;
    typedef Fairy::String String;

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

	void SetParentFrame(ActorEditorFrame* pParentFrame);

    void Reload(void);
    void Reload(const wxString& resourceGroup);

	void addResources(const wxTreeItemId& root, const wxString& pattern, const wxString& typeName, const wxString& parameterName,const wxString& groupname);
protected:
    Fairy::SceneManipulator* mSceneManipulator;
    wxTreeCtrl* mActorList;
    wxString mResourceGroup;
	Ogre::String m_name;
	wxTreeItemId mActivedItem;

	ActorEditorFrame* m_pParentFrame;

    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

    wxTreeItemId AddLeaf(const wxTreeItemId& root, const wxString& name);
    wxTreeItemId AddBranch(const wxTreeItemId& root, const wxString& name);
    wxTreeItemId CreateChildItem(const wxTreeItemId& parent, const wxString& name);
    

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);
    void OnActorListSelChanged(wxTreeEvent& e);
	void OnTreeSelChanged(wxTreeEvent& e);
	//void OnCreateButton(wxCommandEvent& e);
private:
	BOOL bInDel;
};




#endif // __ActorSelector_H__