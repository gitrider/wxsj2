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
#ifndef _IMAGESET_HANDLER_H_
#define _IMAGESET_HANDLER_H_

#include "CEGUIXMLHandler.h"

class EditorDocument;
/*************************************************************************
Implementation Classes
*************************************************************************/ 
/*!
\brief
Handler class used to parse the Imageset XML files using SAX2
*/
class ImagesetHandler : public CEGUI::XMLHandler
{
public:
    /*************************************************************************
    Construction & Destruction
    *************************************************************************/
    /*!
    \brief
    Constructor for Imageset::xmlHandler objects

    \param imageset
    Pointer to the Imageset object creating this xmlHandler object
    */
    ImagesetHandler( EditorDocument* document ) : m_document( document )
    {}

    /*!
    \brief
    Destructor for Imageset::xmlHandler objects
    */
    virtual ~ImagesetHandler( void )
    {}

    /*************************************************************************
    SAX2 Handler overrides
    *************************************************************************/
    /*!
    \brief
    document processing (only care about elements, schema validates format)
    */
    virtual void elementStart( const CEGUI::String& element, const CEGUI::XMLAttributes& attributes );
    virtual void elementEnd( const CEGUI::String& element );

    /*************************************************************************
    Functions used by our implementation
    *************************************************************************/

private:
    /*************************************************************************
    Implementation Constants
    *************************************************************************/
    static const CEGUI::String ImagesetElement;             //!< Tag name for Imageset elements.
    static const CEGUI::String ImageElement;                    //!< Tag name for Image elements.
    static const char   ImagesetNameAttribute[];        //!< Attribute name that stores the name of the Imageset
    static const char   ImagesetImageFileAttribute[];   //!< Attribute name that stores the filename for the image file.
    static const char   ImagesetResourceGroupAttribute[];   //!< Attribute name that stores the resource group identifier used when loading image file.
    static const char   ImagesetNativeHorzResAttribute[];   //!< Optional attribute that stores 'native' horizontal resolution for the Imageset.
    static const char   ImagesetNativeVertResAttribute[];   //!< Optional attribute that stores 'native' vertical resolution for the Imageset.
    static const char   ImagesetAutoScaledAttribute[];  //!< Optional attribute that specifies whether the Imageset should be auto-scaled.
    static const char   ImageNameAttribute[];           //!< Attribute name that stores the name of the new Image.
    static const char   ImageXPosAttribute[];           //!< Attribute name that stores the x position of the new Image.
    static const char   ImageYPosAttribute[];           //!< Attribute name that stores the y position of the new Image.
    static const char   ImageWidthAttribute[];          //!< Attribute name that stores the width of the new Image.
    static const char   ImageHeightAttribute[];         //!< Attribute name that stores the height of the new Image.
    static const char   ImageXOffsetAttribute[];        //!< Attribute name that stores the x rendering offset of the new Image.
    static const char   ImageYOffsetAttribute[];        //!< Attribute name that stores the y rendering offset of the new Image.

    /*************************************************************************
    Implementation Data
    *************************************************************************/
    EditorDocument* m_document;         //!< Holds a pointer to the Imageset that created the handler object
};


#endif  // end of guard _IMAGESET_HANDLER_H_
