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

#ifndef _EDITOR_FRAME_H_
#define _EDITOR_FRAME_H_

// Standard wxWidget includes
#include "wx/wx.h"
#include "CEGUI.h"
#include "RendererModules/OpenGLGUIRenderer/openglrenderer.h"

// More specific wxWidgets includes
#include "PropertiesPanel.h"
#include "wx/docview.h"
#include "wx/splitter.h"

// We need a tiny addition to the retreived client rect
#define BORDER_SIZE 4

// Forwarding
class EditorDocument;
class EditorGLCanvas;
class EditorView;
class DialogResourceGroups;

class EditorFrame : public wxDocParentFrame
{
    DECLARE_CLASS( EditorFrame )
public:
    /** Constructor. Attaches an OpenGL compatible canvas.*/
    EditorFrame( wxDocManager* manager, wxFrame* frame, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, const long type );

    /** Destructor.*/
    ~EditorFrame();

    /** Receives the document from the attached view for accessing by the menu-events.
    */
    void SetDocument( EditorDocument* document );

    /** Seperate method so we can call it after the entire frame has been initialised,
    * because for the canvas it must be visible, which doesn't look that nice when
    * creating the frame.*/
    EditorGLCanvas* AttachCanvas( EditorView* view );

    EditorGLCanvas* getCanvas()
    {
        return m_glcanvasImageset;
    }

    PropertiesPanel* getPropsPanel()
    {
        return m_propsPanel;
    }

    /** We listen to the Edit events (including the Align part) of the menubar.*/
    DECLARE_EVENT_TABLE()

private:
    EditorDocument* m_document;
    wxSplitterWindow* m_splitter;
    PropertiesPanel* m_propsPanel;
    EditorGLCanvas* m_glcanvasImageset;
    DialogResourceGroups* m_resGrpsEditor;

    wxColour m_backgroundColour;

    /** We store the edit menu, to pass to the CommandProcessor.*/
    wxMenu* m_EditMenu;

    /** Attaches a menubar to the frame.*/
    void AttachMenubar();

    /** Attaches a toolbar to the frame.*/
    void AttachToolbar();

    void createContent();

    /** Overloaded so we can close the dialog as well.
    */
    bool Close ( bool force = FALSE );

    /** Event handler: notifies us that the frame needs a repaint. */
    void OnPaint( wxPaintEvent& event );

    void OnChangeBackgroundColour( wxCommandEvent& event );
    void OnEditResourceGroups( wxCommandEvent& event );
};

#endif // _EDITOR_FRAME_H_
