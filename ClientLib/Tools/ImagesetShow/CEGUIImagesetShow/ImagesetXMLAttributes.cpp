#include "StdAfx.h"
#include "ImagesetXMLAttributes.h"
#include <sstream>
#include <iterator>
#include "utf/utf.h"

struct AUTO_LOCALE_CONVERT
{
	AUTO_LOCALE_CONVERT()
	{
		std::locale langLocale("");
		old = std::locale::global(langLocale);
	}
	~AUTO_LOCALE_CONVERT()
	{
		std::locale::global(old);
	}

	std::locale old;
};


CImagesetXMLAttributes::CImagesetXMLAttributes(void)
{}

CImagesetXMLAttributes::~CImagesetXMLAttributes(void)
{}

void CImagesetXMLAttributes::add(const std::string& attrName, const std::string& attrValue)
{
	d_attrs[attrName] = attrValue;
}

void CImagesetXMLAttributes::remove(const std::string& attrName)
{
	AttributeMap::iterator pos = d_attrs.find(attrName);

	if (pos != d_attrs.end())
		d_attrs.erase(pos);
}

bool CImagesetXMLAttributes::exists(const std::string& attrName) const
{
	return d_attrs.find(attrName) != d_attrs.end();
}

size_t CImagesetXMLAttributes::getCount(void) const
{
	return d_attrs.size();
}

const std::string& CImagesetXMLAttributes::getName(size_t index) const
{
	if (index >= d_attrs.size())
	{
		throw std::exception("CImagesetXMLAttributes::getName - The specified index is out of range for this CImagesetXMLAttributes block.");
	}

	AttributeMap::const_iterator iter = d_attrs.begin();
	std::advance(iter, index);

	return (*iter).first;
}

const std::string& CImagesetXMLAttributes::getValue(size_t index) const
{
	if (index >= d_attrs.size())
	{
		throw std::exception("CImagesetXMLAttributes::getValue - The specified index is out of range for this CImagesetXMLAttributes block.");
	}

	AttributeMap::const_iterator iter = d_attrs.begin();
	std::advance(iter, index);

	return (*iter).second;
}

const std::string& CImagesetXMLAttributes::getValue(const std::string& attrName) const
{
	AttributeMap::const_iterator pos = d_attrs.find(attrName);

	if (pos != d_attrs.end())
	{
		return (*pos).second;
	}
	else
	{
		char szException[MAX_PATH] = {0};
		_snprintf(szException, MAX_PATH, "CImagesetXMLAttributes::getValue - no value exists for an attribute named '%s'.", 
			attrName.c_str());

		throw std::exception(szException);
	}
}

const std::string CImagesetXMLAttributes::getValueAsString(const std::string& attrName, const std::string& def) const
{
	AUTO_LOCALE_CONVERT theConverter;

	if(!exists(attrName)) return def;
	// utf8 -> mbcs
	return utf::utf8_to_mbcs(getValue(attrName));
}


bool CImagesetXMLAttributes::getValueAsBool(const std::string& attrName, bool def) const
{
	if (!exists(attrName))
	{
		return def;
	}

	const std::string& val = getValue(attrName);

	if (val == "false" || val == "0")
	{
		return false;
	}
	else if (val == "true" || val == "1")
	{
		return true;
	}
	else
	{
		char szException[MAX_PATH] = {0};
		_snprintf(szException, MAX_PATH, 
			"CImagesetXMLAttributes::getValueAsInteger - failed to convert attribute '%s' with value '%s' to bool.", 
			attrName.c_str(), 
			getValue(attrName).c_str());

		throw std::exception(szException);
	}
}

int CImagesetXMLAttributes::getValueAsInteger(const std::string& attrName, int def) const
{
	if (!exists(attrName))
	{
		return def;
	}

	int val;
	std::istringstream strm(getValue(attrName).c_str());

	strm >> val;

	// success?
	if (strm.fail())
	{
		char szException[MAX_PATH] = {0};
		_snprintf(szException, MAX_PATH, 
			"CImagesetXMLAttributes::getValueAsInteger - failed to convert attribute '%s' with value '%s' to integer.",
			attrName.c_str(), 
			getValue(attrName).c_str());

		throw std::exception(szException);
	}

	return val;
}

float CImagesetXMLAttributes::getValueAsFloat(const std::string& attrName, float def) const
{
	if (!exists(attrName))
	{
		return def;
	}

	float val;
	std::istringstream strm(getValue(attrName).c_str());

	strm >> val;

	// success?
	if (strm.fail())
	{
		char szException[MAX_PATH] = {0};
		_snprintf(szException, MAX_PATH, 
			"CImagesetXMLAttributes::getValueAsInteger - failed to convert attribute '%s' with value '%s' to float.",
			attrName.c_str(), 
			getValue(attrName).c_str());

		throw std::exception(szException);
	}

	return val;
}
