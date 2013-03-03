/****************************************\
*										*
*										*
*										*
* 				TinyXMLParser			*
*										*
*						 by jinchao		*
*						ref from CEGUI	*
\****************************************/

#pragma once

#include "ImagesetXMLParser.h"

class CImagesetTinyXMLParser : public CImagesetXMLParser
{
public:
	CImagesetTinyXMLParser(void);
	~CImagesetTinyXMLParser(void);

	// Implementation of public abstract interface
	void parseXMLFile(CImagesetXMLHandler& handler, const std::string& filename, const std::string& schemaName);

protected:
	// Implementation of abstract interface.
	bool initialiseImpl(void);
	void cleanupImpl(void);
};

