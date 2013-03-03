#ifndef __BrushSelector_H__
#define __BrushSelector_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "BrushSelector.h"
#endif

#include <wx/panel.h>
#include <wx/image.h>

#include <OgreString.h>
#include "PaintAction.h"
#include "TerrainPaintInfoContainer.h"

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxSplitterWindow;
class wxTreeCtrl;

namespace Fairy {
    class SceneManipulator;
    class Action;
}

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class BrushSelector : public wxPanel
{
	/// ����ͼƬ���ڵ����·����wxTreectrl�еĽڵ�֮��Ķ�Ӧ��ϵ��������·����һ����ͼƬ���������·��
	/// ����·�����£�a/b/c.jpg����ô���map�оͻᱣ��������Ԫ�أ��ֱ�Ϊa<-->id1��b<-->id2��c.jpg<-->id3
	/// ���ԣ����û�ѡȡ����ͼʱ�����ô����������·�������a/b/c.jpg
	typedef std::map< Ogre::String, wxTreeItemId> CatalogMap;

public:
    BrushSelector(const wxString& resourceGroup = wxT("Brushes"))
    {
        Init(resourceGroup);
    }

	~BrushSelector();

    BrushSelector(wxWindow *parent, const wxString& resourceGroup = wxT("Brushes"),
                  wxWindowID winid = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                  const wxString& name = "BrushSelector")
    {
        Init(resourceGroup);

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "BrushSelector");

    void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

    Fairy::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

	/// ���¼�����ԴĿ¼�е����������ؼ���
    void ReloadTextureList(void);
    void ReloadTextureList(const wxString& resourceGroup);

	typedef std::map<Ogre::String, Ogre::String> OwnerTexs;

	void setBrushByTexName(const Ogre::String& textureName);

protected:
    Fairy::SceneManipulator* mSceneManipulator;
    wxSplitterWindow* mSplitter;

	CatalogMap mCatalogMap;

	/// �����������ƵĿؼ�
    wxTreeCtrl *mBrushesTree;
	/// ��ǰ������ʾ��wxImage
	wxImage mCurrentPreviewImage;
    /// �ڰ�ͼ
    wxImage mBlackWhitePreviewImage;

    int mPreviewImageWidth;
    int mPreviewImageHeight;

    /// ��ʾ��ѡ��������ͼ�Ŀؼ�
	wxStaticBitmap *mBrushesPreview;

	/// ����Ի����е��ĸ�checkbox������
	wxCheckBox *mFlipHorizontal;
	wxCheckBox *mFlipVertical;
	wxCheckBox *mRotateRightAngle;
	wxCheckBox *mRandom;
    wxCheckBox *mMirrorDiagonal;

	wxCheckBox *mEnablePreview;

	/// ��ǰ�����ת��
	int mCurrentOrientationType;
    /// �Ƿ�ʹ������ķ���
    bool mIsRandomOrientation;

	/// ��ǰ���������
	wxString mCurrentFileName;
   
    wxString mResourceGroup;

    typedef std::vector< Fairy::PaintPixmap > FullTerrainPaintPixmapArray;

    FullTerrainPaintPixmapArray mPixmapArray;

    bool mPaintInOneGrid;

protected:

    void Init(const wxString& resourceGroup);

	/** ���ݴ����·��������ȡ��wxTreeCtrl�ؼ��ĸ��ڵ��id��������Ӧ
	    �Ķ�Ӧ��ϵ���浽map��
	*/
	wxTreeItemId  GetParentId( Ogre::String &catalog, CatalogMap &map );

    void buildPreviewBitmap( const Fairy::TextureInfo texInfo );
    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnPixmapListSelected(wxTreeEvent& e);

	void OnCheck( wxCommandEvent &event );

	void OnReloadTextureList( wxCommandEvent &event );

    const Fairy::Action * OnStartAutoTexAction( const wxTreeItemId &itemId, const wxString &brushName );

    // �ڻ�ˢ�ϵ������Ҽ�
    void OnRightButtonUp(wxTreeEvent& event);
    // ��ʾ����Ҽ���brush�˵��ϵ��ʱ�Ĳ˵���������һЩ��⣩
    void ShowBrushRightButtonMenu(const wxTreeItemId &itemId);
    // ����ȫ���ε�����ƽ�̲���
    void OnFullTerrainPaint(wxCommandEvent& event);
    void OnPaintInOneGrid(wxCommandEvent& event);
};

#endif // __BrushSelector_H__
