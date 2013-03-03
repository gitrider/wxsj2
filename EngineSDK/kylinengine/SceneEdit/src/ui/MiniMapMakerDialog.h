/********************************************************************
filename:   MiniMapMakerDialog.h

purpose:    用于设置小地图制作的参数
*********************************************************************/

#ifndef __MINIMAPMAKERDIALOG_H__
#define __MINIMAPMAKERDIALOG_H__

#include <wx/dialog.h>

#include "WXMiniMapMaker.h"

class wxTextCtrl;
class wxComboBox;
class wxCheckBox;

namespace Fairy {
    class SceneManipulator;
}

class MiniMapMakerDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(MiniMapMakerDialog)

public:
    MiniMapMakerDialog() {}

    // full ctor
    MiniMapMakerDialog(wxWindow *parent, wxWindowID id,
        const wxString& title,
        Fairy::SceneManipulator* manipulator,
        const wxString& sceneName,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name),
        mFileNameText(NULL),
        mTileSizeText(NULL),
        mTileTexWidthText(NULL),
        mTileTexHeightText(NULL),
        mTexExtensionCombo(NULL),
        mDelTempTexturesCheckBox(NULL),
        mCameraDirType(NULL),
        mMiniMapMaker(NULL),
        mManipulator(manipulator),
        mCurrentSceneName(sceneName)
    {
    }

    ~MiniMapMakerDialog();

public:


protected:

    Fairy::MiniMapMaker::CameraDirectionType getCameraDirType(void);

    DECLARE_EVENT_TABLE()
    void OnInitDialog(wxInitDialogEvent &e);
  
    void OnCreateMiniMap(wxCommandEvent& event);    

protected:

    wxTextCtrl* mFileNameText;
    wxTextCtrl* mTileSizeText;
    wxTextCtrl* mTileTexWidthText;
    wxTextCtrl* mTileTexHeightText;

    wxComboBox* mTexExtensionCombo;

    wxCheckBox* mDelTempTexturesCheckBox;

    wxComboBox* mCameraDirType;

    Fairy::MiniMapMaker* mMiniMapMaker;
    Fairy::SceneManipulator* mManipulator;

    wxString mCurrentSceneName;
};

#endif // __MiniMapMakerDialog_H__