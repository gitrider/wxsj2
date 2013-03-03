#ifndef __BulletTree_H__
#define __BulletTree_H__

#include <wx/treectrl.h>
#include "OgreParticle.h"
#include "Core/FairyEffectObject.h"
#include "EffectSystem/FairyEffectElement.h"
#include "EffectSystem/FairyBulletFlowSystemManager.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
#include "EffectSystem/FairyBulletOperator.h"
#include "EffectSystem/FairyBulletTester.h"
class BulletEditor;

class BulletTreeItemData : public wxTreeItemData
{
public:
	BulletTreeItemData(const wxString& desc,const wxString& value = "",void* data = NULL) : m_desc(desc),m_value(value),m_data(data){ }

	void ShowInfo(wxTreeCtrl *tree);
	const wxString& GetDesc() const { return m_desc; }
	const void* GetData() const {return m_data;}
	const wxString& GetValue() const {return m_value;}

private:
	wxString m_desc;
	wxString m_value;
	void* m_data;
};

class BulletTree : public wxTreeCtrl
{
public:

	enum
	{
		TreeCtrlIcon_BulletFlow,
		TreeCtrlIcon_BulletSystem,
		TreeCtrlIcon_BulletEvent,
		TreeCtrlIcon_AnimEffect,
		TreeCtrlIcon_CameraShake,
		TreeCtrlIcon_Ribbon,
		TreeCtrlIcon_Sound,
		TreeCtrlIcon_SceneLight,
		TreeCtrlIcon_Line,
		TreeCtrlIcon_BulletProperty,
		TreeCtrlIcon_Operator,
		TreeCtrlIcon_Tester
	};
  
    BulletTree() { }
	BulletTree(wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style =  wxTAB_TRAVERSAL)
        : wxTreeCtrl(parent, id, pos, size, style)
    {
		m_reverseSort = false;
		mSelectBulletObject = NULL;

		CreateImageList();

    }
    virtual ~BulletTree()
    {

    }
	void AddBulletToTree(Fairy::BulletFlowSystem *pBulletFlow,const Ogre::String &templateName);
	void AddBulletSystemToTree(Fairy::BulletSystem *pBulletSystem,const Ogre::String &templateName);
	void SelectBulletEvent(Fairy::BulletEventSystem* pEvent);
	void SelectBulletOperator(Fairy::BulletOperator* pOperator);
	void SelectBulletSystem(Fairy::BulletSystem* pSystem);
	void SelectBulletSystemElement(Ogre::StringInterface* pElement);
	void SelectElementItem(Fairy::EffectElement* element);
    virtual long MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

	void CreateImageList(int size = 16);

	void OnBeginDrag(wxTreeEvent& event);
	void OnBeginRDrag(wxTreeEvent& event);
	void OnEndDrag(wxTreeEvent& event);
	void OnBeginLabelEdit(wxTreeEvent& event);
	void OnEndLabelEdit(wxTreeEvent& event);
	void OnDeleteItem(wxTreeEvent& event);
	void OnContextMenu(wxContextMenuEvent& event);
	void OnItemMenu(wxTreeEvent& event);
	void OnGetInfo(wxTreeEvent& event);
	void OnSetInfo(wxTreeEvent& event);
	void OnItemExpanded(wxTreeEvent& event);
	void OnItemExpanding(wxTreeEvent& event);
	void OnItemCollapsed(wxTreeEvent& event);
	void OnItemCollapsing(wxTreeEvent& event);
	void OnSelChanged(wxTreeEvent& event);
	void OnSelChanging(wxTreeEvent& event);
	void OnTreeKeyDown(wxTreeEvent& event);
	void OnItemActivated(wxTreeEvent& event);
	void OnItemRClick(wxTreeEvent& event);

	void OnRMouseDown(wxMouseEvent& event);
	void OnRMouseUp(wxMouseEvent& event);
	void OnRMouseDClick(wxMouseEvent& event);

	void OnMenu(wxCommandEvent & event);

	void GetItemsRecursively(const wxTreeItemId& idParent,
		wxTreeItemIdValue cookie = 0);


	void DoSortChildren(const wxTreeItemId& item, bool reverse = false)
	{ m_reverseSort = reverse; wxTreeCtrl::SortChildren(item); }
	void DoEnsureVisible() { if (m_lastItem.IsOk()) EnsureVisible(m_lastItem); }

	void ShowMenu(wxTreeItemId id, const wxPoint& pt);

	int ImageSize(void) const { return m_imageSize; }

	void SetLastItem(wxTreeItemId id) { m_lastItem = id; }

	void refrashSelected();

	//更新制定类型名称的属性
	void refreshItem(const wxString& Desc,const wxString& propertyName,void* data = NULL);

	Ogre::StringInterface* GetSelectBulletObject(){return mSelectBulletObject;}
	void SetSelectEffectObject(Ogre::StringInterface* object){mSelectBulletObject = object;}

protected:
	virtual int OnCompareItems(const wxTreeItemId& i1, const wxTreeItemId& i2);

	// is this the test item which we use in several event handlers?
	bool IsTestItem(const wxTreeItemId& item)
	{
		// the test item is the first child folder
		return GetItemParent(item) == GetRootItem() && !GetPrevSibling(item);
	}
	Ogre::StringInterface* mSelectBulletObject;
private:

	void LogEvent(const wxChar *name, const wxTreeEvent& event);

	int          m_imageSize;               // current size of images
	bool         m_reverseSort;             // flag for OnCompareItems
	wxTreeItemId m_lastItem,                // for OnEnsureVisible()
		m_draggedItem;             // item being dragged right now

	// NB: due to an ugly wxMSW hack you _must_ use DECLARE_DYNAMIC_CLASS()
	//     if you want your overloaded OnCompareItems() to be called.
	//     OTOH, if you don't want it you may omit the next line - this will
	//     make default (alphabetical) sorting much faster under wxMSW.
	DECLARE_DYNAMIC_CLASS(BulletTree)

    DECLARE_EVENT_TABLE()
};

#endif // __BulletTree_H__
