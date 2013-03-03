#include "StdAfx.h"
#include "SMXMLParser.h"
#include "SMXMLAttributes.h"
#include "SMXMLHandler.h"
#include <expat.h>
#include "utf.h"

namespace SMITH
{
    ////////////////////////////////////////////////////////////////////////////////
    //
    // Helper routines
    //
    ////////////////////////////////////////////////////////////////////////////////

	namespace {
		static std::basic_string<char>
			expat_to_local(const std::basic_string<XML_Char>& s)
		{
#if defined(XML_UNICODE)
#if defined(XML_UNICODE_WCHAR_T) || !defined(_NATIVE_WCHAR_T_DEFINED)
			// XML_Char == wchar_t
			return utf::ucs_to_mbcs(s);
#else
			// XML_Char != wchar_t
			typedef std::basic_string<char> cvt(const std::basic_string<XML_Char>& s);
			return (reinterpret_cast<cvt*>(&utf::ucs_to_mbcs))(s);
#endif
#else
			return utf::utf8_to_mbcs(s);
#endif
		}

		class AutoGlobalLocale
		{
			std::string _clocale;
			std::locale _locate;

		public:
			AutoGlobalLocale(const std::locale& loc)
				: _clocale(setlocale(LC_ALL, 0))
				, _locate(std::locale::global(loc))
			{
			}

			~AutoGlobalLocale()
			{
				// Must save/restore C and STL locale separately, since change
				// STL global locale will change C locale too.
				std::locale::global(_locate);
				setlocale(LC_ALL, _clocale.c_str());
			}
		};
	}

XMLParser::XMLParser(void)
{}

XMLParser::~XMLParser(void)
{}

void XMLParser::parseXMLFile(XMLHandler& handler, const std::string& filename)
{
    // All stuff goes here
    XML_Parser parser = XML_ParserCreate(0); // Create a parser

    if (! parser)
    {
		throw std::exception("ExpatParser::parseXMLFile - Unable to create a new Expat Parser");
    }

	AutoGlobalLocale autoGlobalLocale(std::locale(".936"));

    XML_SetUserData(parser, (void*)&handler); // Initialise user data
    XML_SetElementHandler(parser, startElement, endElement); // Register callback for elements

    // Aquire resource using CEGUI ResourceProvider
    FILE* fp = fopen(filename.c_str(), "rb");
	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* pData = new char[nFileSize+1];
	fread(pData, 1, nFileSize, fp);
	fclose(fp); fp = 0;

    // Parse the data (note that the last true parameter tels Expat that this is the last chunk of the document
    if ( ! XML_Parse(parser,pData, nFileSize, true))
    {
        delete[] pData; pData=0;

		char szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH, 
			"ExpatParser::parseXMLFile( \"%s\" ) -XML Parsing error '\r\n%s'", 
			filename.c_str(),
			XML_ErrorString(XML_GetErrorCode(parser)));
    }

    // Release resource
    delete[] pData; pData=0;

	// Free parser
	XML_ParserFree(parser);
}

void XMLParser::startElement(void* data, const char* element, const char** attr)
{
    XMLHandler* handler = static_cast<XMLHandler*>(data);
    XMLAttributes attrs;

    for(size_t i = 0 ; attr[i] ; i += 2)
        attrs.add(expat_to_local(attr[i]), expat_to_local(attr[i+1]));

    handler->elementStart(expat_to_local(element), attrs);
}

void XMLParser::endElement(void* data, const char* element)
{
    XMLHandler* handler = static_cast<XMLHandler*>(data);
    handler->elementEnd(expat_to_local(element));
}

}