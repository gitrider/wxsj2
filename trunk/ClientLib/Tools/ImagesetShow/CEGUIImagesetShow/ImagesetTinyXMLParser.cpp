#include "StdAfx.h"
#include "ImagesetTinyXMLParser.h"
#include "ImagesetXMLHandler.h"
#include "ImagesetXMLAttributes.h"
#include "tinyxml/tinyxml.h"

class TinyXMLDocument : public TiXmlDocument
{
public:
	TinyXMLDocument(CImagesetXMLHandler& handler, const std::string& filename, const std::string& schemaName);
	~TinyXMLDocument()
	{}
protected:
	void processElement(const TiXmlElement* element);

private:
	CImagesetXMLHandler* d_handler;
};

TinyXMLDocument::TinyXMLDocument(CImagesetXMLHandler& handler, const std::string& filename, const std::string& schemaName)
{
	d_handler = &handler;

	char* pDataBuf = 0;
	try {
		// open file
		FILE* fp = fopen(filename.c_str(), "rb");
		if(!fp)
		{
			char szException[MAX_PATH] = {0};
			_snprintf(szException, MAX_PATH, 
				"TinyXMLDocument:Load file %s error",
				filename.c_str());
			throw std::exception(szException);
		}

		fseek(fp, 0, SEEK_END);
		int nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// load file data
		pDataBuf = new char[nFileSize+1];
		memset(pDataBuf, 0, nFileSize+1);
		fread(pDataBuf, 1, nFileSize, fp);
		pDataBuf[nFileSize] = 0;
		fclose(fp);

		TiXmlDocument doc;
		doc.Parse((const char*)pDataBuf);

		const TiXmlElement* currElement = doc.RootElement();

		if (currElement)
		{
			// function called recursively to parse xml data
			processElement(currElement);
		}
	}catch(...)
	{
		delete[] pDataBuf;	pDataBuf = 0;
		throw;
	}

	delete[] pDataBuf;	pDataBuf = 0;
}

void TinyXMLDocument::processElement(const TiXmlElement* element)
{
	// build attributes block for the element
	CImagesetXMLAttributes attrs;
	const TiXmlAttribute *currAttr = element->FirstAttribute();

	while (currAttr)
	{
		attrs.add(currAttr->Name(), currAttr->Value());
		currAttr = currAttr->Next();
	}

	// start element
	d_handler->elementStart(element->Value(), attrs);

	// do children
	const TiXmlElement* childElement = element->FirstChildElement();

	while (childElement)
	{
		processElement(childElement);
		childElement = childElement->NextSiblingElement();
	}

	// end element
	d_handler->elementEnd(element->Value());
}


CImagesetTinyXMLParser::CImagesetTinyXMLParser(void)
{
	// set ID string
//	d_identifierstd::string = "TinyXMLParser - Official tinyXML based parser module for Imageset System";
}

CImagesetTinyXMLParser::~CImagesetTinyXMLParser(void)
{}

void CImagesetTinyXMLParser::parseXMLFile(CImagesetXMLHandler& handler, const std::string& filename, const std::string& schemaName)
{
	TinyXMLDocument doc(handler, filename, schemaName);
}


bool CImagesetTinyXMLParser::initialiseImpl(void)
{
	return true;
}

void CImagesetTinyXMLParser::cleanupImpl(void)
{}

