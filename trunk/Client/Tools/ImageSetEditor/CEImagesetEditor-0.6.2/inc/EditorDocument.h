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

#ifndef _EDITOR_DOCUMENT_H_
#define _EDITOR_DOCUMENT_H_

// Standard wxWidget includes
#include "wx/wx.h" 
// More specific wxWidgets includes
#include "wx/docview.h"
#include "wx/hashmap.h" 

#include <CEGUIImageset.h>

class EditorView;

/** The application's Document implementation.
*/
class EditorDocument : public wxDocument
{
    DECLARE_DYNAMIC_CLASS( EditorDocument )

    // Attributes
public:
    /** Constructor.
    */
    EditorDocument();

    /** Opens the given imageset file as editable.
    */
    bool OnOpenDocument( const wxString& filename );

    /** Starts a new imageset.
    */
    bool OnNewDocument() ;

    /** Saves the current imageset to its filepath.
    */
    bool OnSaveDocument( const wxString& filename );

    /** The current imageset has been closed. The system has prompted the user to save changes, if any.
    */
    bool OnCloseDocument();

    /** Sets the document to being 'dirty' and updates the attached views.*/
    void Modify ( bool modify );

    /** Resets the document for editing eg. after a resolution-change which re-starts Ogre.*/
    void reset() ;

	/** Set the Current Path */
	bool setCurrentPath( const wxString& currentPath ,bool force = false );

    // possible actions on the document
    void setImageFilename( const wxString& newfilename );
    void setImagesetName( const wxString& newname );
    void setNativeResolution( const wxPoint& newnativeres );
    void setAutoScaled( const bool newAutoScaled );
    void addRegion( const wxString& name, const wxRect& dim );
    void deleteRegion( const wxString& name );
    void deleteAllRegions();
    void setRegionArea( const wxString& name, const wxRect& dim, const bool evt_src = false );
    void renameRegion( const wxString& currentname, const wxString& newname );

    //! Return current area for a region
    wxRect getRegionArea( const wxString& name ) const;

private:
    //! returns whether we have a valid view assigned
    bool isViewValid() const;
    
    //! returns a pointer to our view (if any)
    EditorView* getView() const;

    //! Helper that generates a real CEGUI imageset based upon the defined regions.*/
    CEGUI::Imageset* generateRealImageset(const wxString& pathName);

    //! Return the given path 'filename' relative to the default resource group directory
    wxString getDefaultResourceGroupRelativePath( const wxString& filename ) const;

    //! default value for the NativeResolution value.
    static const wxPoint c_defaultNativeResolution;

    //! Name of the Imageset
    wxString m_imagesetName;

    //! Filename of the source image.
    wxString m_imageFilename;

	//! File path of the image.
	wxString m_imagePath;

    // Document data
    wxPoint m_nativeResolution;

    // Document data
    bool m_autoScaled;

    // Document data
    WX_DECLARE_STRING_HASH_MAP( wxRect, mapNamedRegion );

    mapNamedRegion m_mapSetRectangles;
};

#endif // _EDITOR_DOCUMENT_H_
