/************************************************************************
    filename:   CEGUIExpatParser.cpp
    created:    Mon Mar 6 2006
    author:     Paul D Turner <paul@cegui.org.uk> (based on Dalfy's code)
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2006 Paul D Turner (paul@cegui.org.uk)

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
#include "CEGUIExpatParser.h"
#include "CEGUIResourceProvider.h"
#include "CEGUISystem.h"
#include "CEGUIXMLHandler.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUIExceptions.h"
#include "CEGUIPropertyHelper.h"
#include <expat.h>

// Start of CEGUI namespace section
namespace CEGUI
{
ExpatParser::ExpatParser(void)
{
    // set ID string
    d_identifierString = "CEGUI::ExpatParser - Official expat based parser module for CEGUI";
}

ExpatParser::~ExpatParser(void)
{
}

void ExpatParser::parseXMLFile(CEGUI::XMLHandler& handler, const CEGUI::String& filename, const CEGUI::String& schemaName, const CEGUI::String& resourceGroup)
{
    // All stuff goes here
    XML_Parser parser = XML_ParserCreate(0); // Create a parser

    if (! parser)
    {
        throw CEGUI::GenericException("ExpatParser::parseXMLFile - Unable to create a new Expat Parser");
    }

    XML_SetUserData(parser, (void*)&handler); // Initialise user data
    XML_SetElementHandler(parser, startElement, endElement); // Register callback for elements

    // Aquire resource using CEGUI ResourceProvider
    CEGUI::RawDataContainer rawXMLData;
    CEGUI::System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, rawXMLData, resourceGroup);

    // Parse the data (note that the last true parameter tels Expat that this is the last chunk of the document
    if ( ! XML_Parse(parser, reinterpret_cast<const char*>(rawXMLData.getDataPtr()), rawXMLData.getSize(), true))
    {
        CEGUI::System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
        throw CEGUI::GenericException(CEGUI::String((CEGUI::utf8*)"ExpatParser::parseXMLFile( ") + 
									CEGUI::String(filename)+
									CEGUI::String((CEGUI::utf8*)" )  - XML Parsing error '\r\n") +
                                    CEGUI::String((CEGUI::utf8*)XML_ErrorString(XML_GetErrorCode(parser))) +
									CEGUI::String((CEGUI::utf8*)"' at line ")  + CEGUI::PropertyHelper::intToString( XML_GetCurrentLineNumber(parser)));
    }

    // Release resource
    CEGUI::System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);

	// Free parser
	XML_ParserFree(parser);
}


void ExpatParser::parseXMLFile2(XMLHandler& handler, const String& filename, const String& schemaName, const String& resourceGroup)
{
	// All stuff goes here
	XML_Parser parser = XML_ParserCreate(0); // Create a parser

	if (! parser)
	{
		throw GenericException("ExpatParser::parseXMLFile - Unable to create a new Expat Parser");
	}

	XML_SetUserData(parser, (void*)&handler); // Initialise user data
	XML_SetElementHandler(parser, startElement, endElement); // Register callback for elements
	XML_SetCharacterDataHandler(parser, characterData); // Register callback for character data

	// Aquire resource using CEGUI ResourceProvider
	CEGUI::RawDataContainer rawXMLData;
	CEGUI::System::getSingleton().getResourceProvider()->loadRawDataContainer2(filename, rawXMLData, resourceGroup);

	// Parse the data (note that the last true parameter tels Expat that this is the last chunk of the document
	if ( ! XML_Parse(parser, reinterpret_cast<const char*>(rawXMLData.getDataPtr()), rawXMLData.getSize(), true))
	{
		System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
		String exception (String((const utf8*)"ExpatParser::parseXMLFile - XML Parsing error '") +
			String((const utf8*)XML_ErrorString(XML_GetErrorCode(parser))) +
			String((const utf8*)"' at line ") +
			PropertyHelper::uintToString(XML_GetCurrentLineNumber(parser)));
		// (We know it is a valid pointer, otherwise an exception would have been thrown above.)
		XML_ParserFree(parser);
		throw GenericException(exception);
	}

	// Release resource
	CEGUI::System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
	// (We know it is a valid pointer, otherwise an exception would have been thrown above.)
	XML_ParserFree(parser);
}


bool ExpatParser::initialiseImpl(void)
{
    return true;
}

void ExpatParser::cleanupImpl(void)
{

}

void ExpatParser::startElement(void* data, const char* element, const char** attr)
{
    CEGUI::XMLHandler* handler = static_cast<XMLHandler*>(data);
    CEGUI::XMLAttributes attrs;

    for(size_t i = 0 ; attr[i] ; i += 2)
        attrs.add((CEGUI::utf8*)attr[i], (CEGUI::utf8*)attr[i+1]);

    handler->elementStart((CEGUI::utf8*)element, attrs);
}

void ExpatParser::endElement(void* data, const char* element)
{
    CEGUI::XMLHandler* handler = static_cast<CEGUI::XMLHandler*>(data);
    handler->elementEnd((CEGUI::utf8*)element);
}

void ExpatParser::characterData(void *data, const char *text, int len)
{
	XMLHandler* handler = static_cast<XMLHandler*>(data);
	String32 str((const utf8*)text, static_cast<String::size_type>(len));
	handler->text(str.c_str());
}

} // End of  CEGUI namespace section
