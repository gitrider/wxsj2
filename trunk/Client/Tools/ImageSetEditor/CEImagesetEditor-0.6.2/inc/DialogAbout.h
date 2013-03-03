///////////////////////////////////////////////////////////////////////////////
//  For project details and authors, refer to README and AUTHORS files
//
//  This file is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This file is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//  To view the licence online, go to: http://www.gnu.org/copyleft/gpl.html
////////////////////////////////////////////////////////////////////////////////
#ifndef _DIALOG_ABOUT_H_
#define _DIALOG_ABOUT_H_

#include <wx/wx.h>

class DialogAbout : public wxDialog
{
public:
    DialogAbout ( wxWindow* parent );

private:
    // Control ID(s)
    enum
    {
        ID_OK_BUTTON = wxID_HIGHEST
    };
    void OnClickOk( wxCommandEvent& event );

    DECLARE_EVENT_TABLE()
};

#endif // _DIALOG_ABOUT_H_
