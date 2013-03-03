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
#include "ElasticBoxProperties.h"

// use PropertyHelper to convert strings
#include <CEGUIPropertyHelper.h>
// class these properties operate on
#include "ElasticBox.h"

// Start of ElasticBoxProperties namespace section
namespace ElasticBoxProperties
{

CEGUI::String Sizable::get( const CEGUI::PropertyReceiver* receiver ) const
{
    return CEGUI::PropertyHelper::boolToString(
        static_cast<const ElasticBox*>( receiver )->isSizable() );
}

void Sizable::set( CEGUI::PropertyReceiver* receiver, const CEGUI::String& value )
{
    static_cast<ElasticBox*>( receiver )->setSizable(
        CEGUI::PropertyHelper::stringToBool( value ) );
}

CEGUI::String Movable::get( const CEGUI::PropertyReceiver* receiver ) const
{
    return CEGUI::PropertyHelper::boolToString(
        static_cast<const ElasticBox*>( receiver )->isMovable() );
}

void Movable::set( CEGUI::PropertyReceiver* receiver, const CEGUI::String& value )
{
    static_cast<ElasticBox*>( receiver )->setMovable(
        CEGUI::PropertyHelper::stringToBool( value ) );
}

CEGUI::String SizingBorderThickness::get( const CEGUI::PropertyReceiver* receiver ) const
{
    return CEGUI::PropertyHelper::floatToString(
        static_cast<const ElasticBox*>( receiver )->getSizingBorderThickness() );
}

void SizingBorderThickness::set( CEGUI::PropertyReceiver* receiver, const CEGUI::String& value )
{
    static_cast<ElasticBox*>( receiver )->setSizingBorderThickness(
        CEGUI::PropertyHelper::stringToFloat( value ) );
}

} // End of ElasticBoxProperties namespace section
