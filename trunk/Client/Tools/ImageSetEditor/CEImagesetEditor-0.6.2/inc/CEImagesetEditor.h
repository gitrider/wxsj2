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

#ifndef _CEIMAGESET_EDITOR_H_
#define _CEIMAGESET_EDITOR_H_

#include "EditorFrame.h" 

class wxDocManager;

/** The application class. The program starts here.
*/
class CEImagesetEditor : public wxApp
{
public:
    /** Constructor - initialises / prepares members.*/
    CEImagesetEditor();

    /** Called by the framework. See this as the Main function
     * of your application.
     * @return succes (true) or failure (false).
     */
    bool OnInit();

    /** Called by the framework when the application exits. Request
     * for saving the current document and such have already been done now.
     */
    int OnExit();

    /** We handle the About box ourself.
    */ 
    void OnAbout(wxCommandEvent& event);

protected:
    wxDocManager* m_docManager;

private:
    DECLARE_EVENT_TABLE()
};

DECLARE_APP( CEImagesetEditor )

extern EditorFrame *GetMainFrame();

#endif // _CEIMAGESET_EDITOR_H_
