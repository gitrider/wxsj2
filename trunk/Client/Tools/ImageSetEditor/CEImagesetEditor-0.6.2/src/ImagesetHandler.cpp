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
#include "ImagesetHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUILogger.h"
#include "CEGUIXMLAttributes.h"


#include "EditorDocument.h"
#include "CEGUIHelper.h"

// Start of CEGUI namespace section
using namespace CEGUI;


/*************************************************************************
Definition of constant data for Imageset (and sub-classes)
*************************************************************************/ 
// Declared in Imageset::xmlHandler
const CEGUI::String ImagesetHandler::ImagesetElement( "Imageset" );
const CEGUI::String ImagesetHandler::ImageElement( "Image" );
const char  ImagesetHandler::ImagesetImageFileAttribute[] = "Imagefile";
const char  ImagesetHandler::ImagesetResourceGroupAttribute[] = "ResourceGroup";
const char  ImagesetHandler::ImagesetNameAttribute[] = "Name";
const char  ImagesetHandler::ImagesetNativeHorzResAttribute[] = "NativeHorzRes";
const char  ImagesetHandler::ImagesetNativeVertResAttribute[] = "NativeVertRes";
const char  ImagesetHandler::ImagesetAutoScaledAttribute[] = "AutoScaled";
const char  ImagesetHandler::ImageNameAttribute[] = "Name";
const char  ImagesetHandler::ImageXPosAttribute[] = "XPos";
const char  ImagesetHandler::ImageYPosAttribute[] = "YPos";
const char  ImagesetHandler::ImageWidthAttribute[] = "Width";
const char  ImagesetHandler::ImageHeightAttribute[] = "Height";
const char  ImagesetHandler::ImageXOffsetAttribute[] = "XOffset";
const char  ImagesetHandler::ImageYOffsetAttribute[] = "YOffset";

/*************************************************************************
SAX2 Handler methods
*************************************************************************/
void ImagesetHandler::elementStart( const String& element, const XMLAttributes& attributes )
{
    // handle an Image element (extract all element attributes and use data to define an Image for the Imageset)
    if ( element == ImageElement )
    {
        wxString    name;

        if ( attributes.exists ( ImageNameAttribute ) )
            name = CEGUIHelper::ToWXString( attributes.getValueAsString( ImageNameAttribute ) );

        wxRect  rect;

        if ( attributes.exists ( ImageXPosAttribute ) )
            rect.SetX( attributes.getValueAsInteger( ImageXPosAttribute ) );
        if ( attributes.exists ( ImageYPosAttribute ) )
            rect.SetY( attributes.getValueAsInteger( ImageYPosAttribute ) );
        if ( attributes.exists ( ImageWidthAttribute ) )
            rect.SetWidth( attributes.getValueAsInteger( ImageWidthAttribute ) );
        if ( attributes.exists ( ImageHeightAttribute ) )
            rect.SetHeight( attributes.getValueAsInteger( ImageHeightAttribute ) );

        wxPoint offset;

        if ( attributes.exists ( ImageXOffsetAttribute ) )
            offset.x    = attributes.getValueAsInteger( ImageXOffsetAttribute, 0 );
        if ( attributes.exists ( ImageYOffsetAttribute ) )
            offset.y    = attributes.getValueAsInteger( ImageYOffsetAttribute, 0 );

        m_document->addRegion( name, rect );
    }
    // handle root Imageset element
    else if ( element == ImagesetElement )
    {
        if ( attributes.exists ( ImagesetNameAttribute ) )
        {
            wxString    name( CEGUIHelper::ToWXString( attributes.getValueAsString( ImagesetNameAttribute ) ) );
            m_document->setImagesetName( name );
        }

        if ( attributes.exists ( ImagesetImageFileAttribute ) )
        {
            wxString    filename( CEGUIHelper::ToWXString( attributes.getValueAsString( ImagesetImageFileAttribute ) ) );
            m_document->setImageFilename( filename );
        }

        //
        // load auto-scaling configuration
        //
        int hres = 640, vres = 480;

        if ( attributes.exists ( ImagesetNativeHorzResAttribute ) )
        {
            // get native horizontal resolution
            hres = attributes.getValueAsInteger( ImagesetNativeHorzResAttribute, 640 );
        }
        if ( attributes.exists ( ImagesetNativeVertResAttribute ) )
        {
            // get native vertical resolution
            vres = attributes.getValueAsInteger( ImagesetNativeVertResAttribute, 480 );
        }
        m_document->setNativeResolution( wxPoint( hres, vres ) );


        bool autoscal = false;
        if ( attributes.exists ( ImagesetAutoScaledAttribute ) )
        {
            // enable / disable auto-scaling for this Imageset according to the setting
            autoscal = attributes.getValueAsBool( ImagesetAutoScaledAttribute, false );
        }
        m_document->setAutoScaled( autoscal );
    }
}

void ImagesetHandler::elementEnd( const String& element )
{
    if ( element == ImagesetElement )
    {
        //Logger::getSingleton().logEvent("Finished creation of Imageset '" + d_imageset->d_name + "' via XML file.", Informative);
        m_document->Modify( true );
    }
}
