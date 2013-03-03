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

#ifndef _EDITOR_VIEW_H_
#define _EDITOR_VIEW_H_

// Standard wxWidget includes
#include "wx/wx.h" 
// More specific wxWidgets includes
#include "wx/docview.h" 
//#include "wx/cmdproc.h"

class EditorFrame;
class EditorGLCanvas;
class PropertiesPanel;

/** A "view" on a "document". Note that views are not created when the application
 * starts, but only when the user chooses "file->new" or "file->open".
 * Also note that the dialog is not a real view.
 */
class EditorView: public wxView
{
    // Needed for the doc/view managerF
    DECLARE_DYNAMIC_CLASS( EditorView )

public:

    /** Constructor.*/
    EditorView();

    /** Called by the document when the document's name changes, e.g. on Save or New.*/
    void onChangeFilename();

    // possible actions on the document
    void onChangeImageFile( const wxString& newfilename , const wxString& trueFileName );

    /** Called by the document when the imageset name change.*/
    void onChangeImagesetName( const wxString& newname );

    /** Called by the document when the native resolution change.*/
    void onChangeNativeResolution( const wxPoint& newnativeres );

    /** Called by the document when the AutoScaled change.*/
    void onChangeAutoScaled( const bool newAutoScaled );

    /** Called by the document when a region is added.*/
    void onAddRegion( const wxString& name, const wxRect& dim );

    /** Called by the document when a region is deleted.*/
    void onDelRegion( const wxString& name );

    /** Called by the document when all region are deleted.*/
    void onDelAllRegion();

    /** Called by the document when a region is moved and/or scaled.*/
    void onSetRegionArea( const wxString& name, const wxRect& dim, const bool evt_src = false );

    /** Called by the document when all region are deleted.*/
    void onRenameRegion( const wxString& currentname, const wxString& newname );

    // Frame part update section
    /** Updates the mainframe's title bar according to the currently open document.*/
    void updateTitle();

    /** Updates the Mouse part of the status bar.*/
    void updateMouseStatusBar( const float posx, const float posy );

    /** Updates the View part of the status bar.*/
    void updateDocStatusBar( const float zoom, const float width, const float height );


    EditorGLCanvas* getCanvas()
    {
        return m_glcanvasImageset;
    }

    PropertiesPanel* getPropsPanel()
    {
        return m_propsPanel;
    }
private:

    //! EditorFrame we are attached to.
    EditorFrame* m_frame;
    //! PropertiesPanel forming part of this EditorView
    PropertiesPanel* m_propsPanel;
    //! The EditorGLCanvas where we do our OpenGL and CEGUI based rendering.
    EditorGLCanvas* m_glcanvasImageset;

    /** Overwritten because the base class is a Window. We don't use it here.*/
    void OnDraw( wxDC* dc );

    /** Notifies us that a new document has been created, either through
     * File->New or File->Open.*/
    bool OnCreate( wxDocument* doc, long flags );

    /** Notifies that the user closes the current document. Either by an explicit
     * closing, or throuh File->New or File->Open.
    */
    bool OnClose( bool deleteWindow = true );

    /** Notification of the UpdateAllViews call in a document.
    */
    void OnUpdate( wxView* sender, wxObject* hint );

};

#endif // _EDITOR_VIEW_H_
