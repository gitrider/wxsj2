/************************************************************************
	filename: 	CEGUIPropertySet.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements PropertySet class
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
#include "CEGUIPropertySet.h"
#include "CEGUIProperty.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Add a new property to the set
*************************************************************************/
void PropertySet::addProperty(Property* property)
{
	if (property == NULL)
	{
		throw NullObjectException((utf8*)"The given Property object pointer is NULL.");
	}

	if (d_properties.find(property->getName()) != d_properties.end())
	{
		throw AlreadyExistsException((utf8*)"A Property named '" + property->getName() + (utf8*)"' already exists in the PropertySet.");
	}

	d_properties[property->getName()] = property;
}

/*************************************************************************
	Remove a property from the set
*************************************************************************/
void PropertySet::removeProperty(const String& name)
{
	PropertyRegistry::iterator pos = d_properties.find(name);

	if (pos != d_properties.end())
	{
		d_properties.erase(pos);
	}
}

/*************************************************************************
	Remove all properties from the set
*************************************************************************/
void PropertySet::clearProperties(void)
{
	d_properties.clear();
}

/*************************************************************************
	Return true if a property with the given name is in the set
*************************************************************************/
bool PropertySet::isPropertyPresent(const String& name) const
{
	return (d_properties.find(name) != d_properties.end());
}

/*************************************************************************
	Return the help string for a property
*************************************************************************/
const String& PropertySet::getPropertyHelp(const String& name) const
{
	PropertyRegistry::const_iterator pos = d_properties.find(name);

	if (pos == d_properties.end())
	{
		throw UnknownObjectException((utf8*)"There is no Property named '" + name + (utf8*)"' available in the set.");
	}

	return pos->second->getHelp();
}

/*************************************************************************
	Return the current value of a property
*************************************************************************/
String PropertySet::getProperty(const String& name, const PropertyReceiver& receiver) const
{
	PropertyRegistry::const_iterator pos = d_properties.find(name);

	if (pos == d_properties.end())
	{
		throw UnknownObjectException((utf8*)"There is no Property named '" + name + (utf8*)"' available in the set.");
	}

	return pos->second->get(&receiver);
}

/*************************************************************************
	Set the current value of a property
*************************************************************************/
void PropertySet::setProperty(const String& name, const String& value, PropertyReceiver& receiver) const
{
	PropertyRegistry::const_iterator pos = d_properties.find(name);

	if (pos == d_properties.end())
	{
		throw UnknownObjectException((utf8*)"There is no Property named '" + name + (utf8*)"' available in the set.");
	}

	pos->second->set(&receiver, value);
}


/*************************************************************************
	Return a PropertySet::PropertyIterator object to iterate over the
	available Properties.
*************************************************************************/
PropertySet::PropertyIterator PropertySet::getIterator(void) const
{
	return PropertyIterator(d_properties.begin(), d_properties.end());
}


/*************************************************************************
	Returns whether a Property is at it's default value.
*************************************************************************/
bool PropertySet::isPropertyDefault(const String& name, const PropertyReceiver& receiver) const
{
	PropertyRegistry::const_iterator pos = d_properties.find(name);

	if (pos == d_properties.end())
	{
		throw UnknownObjectException((utf8*)"There is no Property named '" + name + (utf8*)"' available in the set.");
	}

	return pos->second->isDefault(&receiver);
}


/*************************************************************************
	Returns the default value of a Property as a String.	
*************************************************************************/
String PropertySet::getPropertyDefault(const String& name, const PropertyReceiver& receiver) const
{
	PropertyRegistry::const_iterator pos = d_properties.find(name);

	if (pos == d_properties.end())
	{
		throw UnknownObjectException((utf8*)"There is no Property named '" + name + (utf8*)"' available in the set.");
	}

	return pos->second->getDefault(&receiver);
}

} // End of  CEGUI namespace section
