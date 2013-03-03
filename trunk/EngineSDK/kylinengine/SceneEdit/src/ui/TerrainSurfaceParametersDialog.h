/********************************************************************
filename:   TerrainSurfaceParametersDialog.h

purpose:    �������õ��β��ʵĶԻ���
*********************************************************************/

#ifndef __TerrainSurfaceParametersDialog_H__
#define __TerrainSurfaceParametersDialog_H__

#include <wx/dialog.h>

#include "SceneManipulator.h"

#include "Core/TerrainData.h"

namespace Fairy {
    class SceneManipulator;
    class Terrain;
}

class wxTextCtrl;

class TerrainSurfaceParametersDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(TerrainSurfaceParametersDialog)

public:
    TerrainSurfaceParametersDialog() {}

    // full ctor
    TerrainSurfaceParametersDialog(wxWindow *parent, wxWindowID id,
        const wxString& title,
        Fairy::SceneManipulator* manipulator,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE & (~wxCLOSE_BOX),
        const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name),
        mManipulator(manipulator),
        mTerrainData(NULL),
        mTerrain(NULL),
        mAmbientText(NULL),
        mDiffuseText(NULL),
        mSpecularText(NULL),
        mEmissiveText(NULL),
        mShininessText(NULL),
        mAmbientButton(NULL),
        mDiffuseButton(NULL),
        mSpecularButton(NULL),
        mEmissiveButton(NULL)
    {
    }

public:


protected:

    /// �����ı�����������
    void setColourText(void);
    /// ���ð�ť��ɫ
    void setButtonColour(void);
    /// ����ɫ�Ի��򲢻�ȡ��ѡ����ɫ
    bool getColourFromDialog(wxColour& col);

    DECLARE_EVENT_TABLE()
    void OnInitDialog(wxInitDialogEvent &e);

    void OnAmbientTextChanged(wxCommandEvent &e);
    void OnDiffuseTextChanged(wxCommandEvent &e);
    void OnSpecularTextChanged(wxCommandEvent &e);
    void OnEmissiveTextChanged(wxCommandEvent &e);
    void OnShininessTextChanged(wxCommandEvent &e);

    void OnAmbientColourButtonDown(wxCommandEvent &e);
    void OnDiffuseColourButtonDown(wxCommandEvent &e);
    void OnSpecularColourButtonDown(wxCommandEvent &e);
    void OnEmissiveColourButtonDown(wxCommandEvent &e);

    void OnOK(wxCommandEvent& event);    
    void OnCancel(wxCommandEvent& event);

protected:

    Fairy::SceneManipulator* mManipulator;
    Fairy::TerrainData* mTerrainData;
    Fairy::Terrain* mTerrain;

    /// �����޸�ǰ�ĵ��β�������
    Fairy::TerrainData::SurfaceParams mOriginSurfaceParams;

    wxTextCtrl* mAmbientText;
    wxTextCtrl* mDiffuseText;
    wxTextCtrl* mSpecularText;
    wxTextCtrl* mEmissiveText;
    wxTextCtrl* mShininessText;

    wxButton* mAmbientButton;
    wxButton* mDiffuseButton;
    wxButton* mSpecularButton;
    wxButton* mEmissiveButton;
};

#endif // __TerrainSurfaceParametersDialog_H__