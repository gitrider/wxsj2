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
#ifndef _CEGUI_HELPER_H_
#define _CEGUI_HELPER_H_

#include <CEGUIString.h>
#include <CEGUIcolour.h>

/** Utility class for convertion between CEGUI types and wxWidgets types.
*/
class CEGUIHelper
{
public:
    /** Converts a wxWidgets string into a CEGUI string, Unicode proof.*/
    static CEGUI::String ToCEGUIString( const wxString& str )
    {
        return CEGUI::String(
                   ToCEUTF8( wxConvLibc.cWX2MB( str ) ) );
    }

    /** Converts a ansi string into a CEGUI string.*/
    static const CEGUI::utf8* ToCEUTF8( const char* cstr )
    {
        return reinterpret_cast<const CEGUI::utf8*>( cstr );
    }

    /** Converts a CEGUI string into a wxWidgets string, Unicode proof.*/
    static wxString ToWXString( const CEGUI::String& str )
    {
        return wxString( wxConvLibc.cMB2WX( str.c_str() ) );
    }

    /** Converts a wxWidgets string into a float.*/
    static float WXStringToFloat( const wxString& str )
    {
        double dVal = 0;
        str.ToDouble( &dVal );
        return static_cast<float>( dVal );
    }

    /** Converts a wxWidgets string into an int.*/
    static int WXStringToInt( const wxString& str )
    {
        long val = 0;
        str.ToLong( &val );
        return static_cast<int>( val );
    }

    /** Converts a wxWidgets colour to a cegui colour .*/
    static CEGUI::colour WXColourToCEGUIColour( const wxColour& colour )
    {
        CEGUI::colour col( colour.Red() / 255.0f,
                             colour.Green() / 255.0f,
                             colour.Blue() / 255.0f );
        return col;
    }
};

#endif // _CEGUI_HELPER_H_
