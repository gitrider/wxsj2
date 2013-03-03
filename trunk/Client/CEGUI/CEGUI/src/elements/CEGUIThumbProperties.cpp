/************************************************************************
	filename: 	CEGUIThumbProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements the Thumb class properties.
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "elements/CEGUIThumbProperties.h"
#include "elements/CEGUIThumb.h"
#include "CEGUIPropertyHelper.h"
#include <stdio.h>

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ThumbProperties namespace section
namespace ThumbProperties
{
String	HotTracked::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Thumb*>(receiver)->isHotTracked());
}


void	HotTracked::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Thumb*>(receiver)->setHotTracked(PropertyHelper::stringToBool(value));
}


String	VertFree::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Thumb*>(receiver)->isVertFree());
}


void	VertFree::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Thumb*>(receiver)->setVertFree(PropertyHelper::stringToBool(value));
}


String	HorzFree::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Thumb*>(receiver)->isHorzFree());
}


void	HorzFree::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Thumb*>(receiver)->setHorzFree(PropertyHelper::stringToBool(value));
}


String	VertRange::get(const PropertyReceiver* receiver) const
{
	using namespace std;
	char buff[64];

	pair<float, float> range = static_cast<const Thumb*>(receiver)->getVertRange();
	sprintf(buff, "min:%f max:%f", range.first, range.second);

	return String((utf8*)buff);
}


void	VertRange::set(PropertyReceiver* receiver, const String& value)
{
	using namespace std;

	float rangeMin = 0, rangeMax = 0;
	sscanf(value.c_str(), " min:%f max:%f", &rangeMin, &rangeMax);

	static_cast<Thumb*>(receiver)->setVertRange(rangeMin, rangeMax);
}


String	HorzRange::get(const PropertyReceiver* receiver) const
{
	using namespace std;

	char buff[64];
	pair<float, float> range = static_cast<const Thumb*>(receiver)->getHorzRange();
	sprintf(buff, "min:%f max:%f", range.first, range.second);

	return String((utf8*)buff);
}


void	HorzRange::set(PropertyReceiver* receiver, const String& value)
{
	using namespace std;

	float rangeMin = 0, rangeMax = 0;
	sscanf(value.c_str(), " min:%f max:%f", &rangeMin, &rangeMax);

	static_cast<Thumb*>(receiver)->setHorzRange(rangeMin, rangeMax);
}


} // End of  ThumbProperties namespace section

} // End of  CEGUI namespace section
