#include "StdAfx.h"
#include "ImagesetXMLParser.h"
#include "ImagesetXMLHandler.h"

CImagesetXMLParser::CImagesetXMLParser(void) :
d_identifierString("Unknown XML parser (vendor did not set the ID string!)"),
d_initialised(false)
{}

CImagesetXMLParser::~CImagesetXMLParser(void)
{}

bool CImagesetXMLParser::initialise(void)
{
	// do this to ensure only one initialise call is made
	if (!d_initialised)
	{
		d_initialised = initialiseImpl();
	}

	return d_initialised;
}

void CImagesetXMLParser::cleanup(void)
{
	if (d_initialised)
	{
		cleanupImpl();
		d_initialised = false;
	}
}

const std::string& CImagesetXMLParser::getIdentifierString() const
{
	return d_identifierString;
}

