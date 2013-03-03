/*

Agent template XML Parser Interface

by fangyuan.
ref from CEGUI
*/

#pragma once

namespace SMITH
{

class XMLHandler;
class XMLParser
{
public:
	/*!
	\brief
	XMLParser base class constructor.
	*/
	XMLParser(void);

	/*!
	\brief
	XMLParser base class destructor.
	*/
	virtual ~XMLParser(void);

	/*!
	\brief
	abstract method which initiates parsing of an XML file.

	\param handler
	XMLHandler based object which will process the XML elements.

	\param filename
	std::string object holding the filename of the XML file to be parsed.

	\return
	Nothing.
	*/
	virtual void parseXMLFile(XMLHandler& handler, const std::string& filename);

private:
    // C++ class methods name are not valide C function pointer. static solve this
    static void startElement(void* data, const char* element, const char**attr); // Expat handlers
    static void endElement(void* data, const char* element); // Expat handlers
};

}
