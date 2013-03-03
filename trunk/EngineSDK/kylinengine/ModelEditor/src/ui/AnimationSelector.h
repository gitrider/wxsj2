//说明：动画名称显示界面
#ifndef __AnimationSelector_H__
#define __AnimationSelector_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "AnimaitonSelector.h"
#endif

#include <wx/panel.h>
#include "Action.h"
#include "Core/FairyObject.h"
#include <wx/treebase.h>

#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/slider.h>

static const int STATIC_FPS = 30;

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

class AnimationSelector : public wxPanel
{
public:
    AnimationSelector(const wxString& resourceGroup = wxT("General")):m_bPause(false),m_curFuseValue(0.0), mIsMeshAnim(false)
    {
        Init(resourceGroup);
    }

    AnimationSelector(wxWindow *parent, const wxString& resourceGroup = wxT("General"),
                  wxWindowID winid = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxTAB_TRAVERSAL | wxNO_BORDER,
				  const wxString& name = "AnimationSelector"):m_bPause(false),m_curFuseValue(0.0), mIsMeshAnim(false)
    {
        Init(resourceGroup);

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "AnimationSelector");

    ~AnimationSelector();

    void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

    Fairy::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

    void Reload(void);
    void Reload(const wxString& resourceGroup);

	bool hasSelectedItem();

protected:
    Fairy::SceneManipulator* mSceneManipulator;
    wxTreeCtrl* mAnimationList;
    wxString mResourceGroup;
	wxTreeItemId mActivedItem;
    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

    wxTreeItemId AddLeaf(const wxTreeItemId& root, const wxString& name);
    wxTreeItemId AddBranch(const wxTreeItemId& root, const wxString& name);
    wxTreeItemId CreateChildItem(const wxTreeItemId& parent, const wxString& name);
    const Fairy::String DoActiveItem(const wxTreeItemId& item);

	void resetState();
	
	//////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnAnimationListSelChanged(wxTreeEvent& e);

	void OnBtn(wxCommandEvent & e);

	void OnSliderUpdate(wxCommandEvent & e);

	void OnFuseChange(wxCommandEvent& e);

public:
	bool getPaused(){ return m_bPause;}
	void pauseAnimation(bool bPause = true);
public:

	wxButton* m_btnPlay;
	wxButton* m_btnPause;
	wxStaticText* m_staticTextFuse;
	wxTextCtrl* m_fuseValue;
	wxSlider* m_slider;
	wxStaticText* m_staticTextBlank;
	wxStaticText* m_startTimeLabel;
	wxStaticText* m_startFrameLabel;
	wxStaticText* m_startLabel;
	wxTextCtrl* m_startTimeText;
	wxTextCtrl* m_startFrameText;
	wxStaticText* m_curLabel;
	wxTextCtrl* m_curTimeText;
	wxTextCtrl* m_curFrameText;
	wxStaticText* m_endLabel;
	wxTextCtrl* m_endTimeText;
	wxTextCtrl* m_endFrameText;

	Ogre::Real m_curFuseValue;
	bool mIsMeshAnim;
private:
	bool m_bPause;
};

#endif // __AnimationSelector_H__
