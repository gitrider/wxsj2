#ifndef __HeightMapScopeSettingDialog_H__
#define __HeightMapScopeSettingDialog_H__

#include <wx/dialog.h>
#include <list>
#include <map>
#include <OgreString.h>

class HeightMapScopeSettingDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(HeightMapScopeSettingDialog)
 
public:
    HeightMapScopeSettingDialog() {}
 
    // full ctor
    HeightMapScopeSettingDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name), mMinHeight(0), mMaxHeight(0)
    {
    }
public: 
    float getMinHeight();
    float getMaxHeight();
    void setMinHeight(float height);
    void setMaxHeight(float height);

protected:
	wxTextCtrl* mMinHeightTextCtrl;
	wxTextCtrl* mMaxHeightTextCtrl;
    float mMinHeight;
	float mMaxHeight;

    DECLARE_EVENT_TABLE()

    void OnInitDialog(wxInitDialogEvent &e);
};

#endif // __HeightMapScopeSettingDialog_H__
