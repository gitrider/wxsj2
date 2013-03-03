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
#ifndef _ELASTIC_BOX_PROPERTIES_H_
#define _ELASTIC_BOX_PROPERTIES_H_

// base class for properties
#include <CEGUIProperty.h>

// Start of ElasticBoxProperties namespace section
/*!
\brief
    Namespace containing all classes that make up the properties interface for
    the ElasticBox base class.
*/
namespace ElasticBoxProperties
{
/*!
\brief
    Propery to access the sizable state of the ElasticBox

    \par Usage:
        - Name: Sizable
        - Format: "[text]".

    \par Where [Text] is:
        - "True" to indicate the ElasticBox can be sized by the user.
        - "False" to indicate the ElasticBox can not be sized by the user.
*/
class Sizable : public CEGUI::Property
{
public:
    Sizable() : Property(
        "Sizable",
        "Property to get/set the sizable state of the ElasticBox.  Value is either \"True\" or \"False\".",
        "True")
    {}

    CEGUI::String  get(const CEGUI::PropertyReceiver* receiver) const;
    void set(CEGUI::PropertyReceiver* receiver, const CEGUI::String& value);
};

/*!
\brief
    Propery to access the movable state of the ElasticBox

    \par Usage:
        - Name: Movable
        - Format: "[text]".

    \par Where [Text] is:
        - "True" to indicate the ElasticBox can be moved by the user.
        - "False" to indicate the ElasticBox can not be moved by the user.
*/
class Movable : public CEGUI::Property
{
public:
    Movable() : Property(
        "Movable",
        "Property to get/set the movable state of the ElasticBox.  Value is either \"True\" or \"False\".",
        "True")
    {}

    CEGUI::String get(const CEGUI::PropertyReceiver* receiver) const;
    void set(CEGUI::PropertyReceiver* receiver, const CEGUI::String& value);
};

/*!
\brief
    Propery to access the thickness setting for the sizing border on the
    ElasticBox.

    \par Usage:
        - Name: SizingBorderThickness
        - Format: "[float]".

    \par Where:
        - [float] is the thickness of the sizing border in pixels.
*/
class SizingBorderThickness : public CEGUI::Property
{
public:
    SizingBorderThickness() : Property(
        "SizingBorderThickness",
        "Property to get/set the thickness of the sizing border on the ElasticBox.  Value is a float specifying the border thickness in pixels.",
        "3")
    {}

    CEGUI::String get(const CEGUI::PropertyReceiver* receiver) const;
    void set(CEGUI::PropertyReceiver* receiver, const CEGUI::String& value);
};

} // End of  CheckboxProperties namespace section



#endif // _ELASTIC_BOX_PROPERTIES_H_
 
