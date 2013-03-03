#include "StdAfx.h"
#include "SMXMLAttributes.h"
#include <sstream>
#include <iterator>

namespace SMITH
{

XMLAttributes::XMLAttributes(void)
{}

XMLAttributes::~XMLAttributes(void)
{}

void XMLAttributes::add(const std::string& attrName, const std::string& attrValue)
{
	d_attrs[attrName] = attrValue;
}

void XMLAttributes::remove(const std::string& attrName)
{
	AttributeMap::iterator pos = d_attrs.find(attrName);

	if (pos != d_attrs.end())
		d_attrs.erase(pos);
}

bool XMLAttributes::exists(const std::string& attrName) const
{
	return d_attrs.find(attrName) != d_attrs.end();
}

size_t XMLAttributes::getCount(void) const
{
	return d_attrs.size();
}

const std::string& XMLAttributes::getName(size_t index) const
{
	if (index >= d_attrs.size())
	{
		throw std::exception("XMLAttributes::getName - The specified index is out of range for this XMLAttributes block.");
	}

	AttributeMap::const_iterator iter = d_attrs.begin();
	std::advance(iter, index);

	return (*iter).first;
}

const std::string& XMLAttributes::getValue(size_t index) const
{
	if (index >= d_attrs.size())
	{
		throw std::exception("XMLAttributes::getValue - The specified index is out of range for this XMLAttributes block.");
	}

	AttributeMap::const_iterator iter = d_attrs.begin();
	std::advance(iter, index);

	return (*iter).second;
}

const std::string& XMLAttributes::getValue(const std::string& attrName) const
{
	AttributeMap::const_iterator pos = d_attrs.find(attrName);

	if (pos != d_attrs.end())
	{
		return (*pos).second;
	}
	else
	{
		char szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH, "XMLAttributes::getValue - no value exists for an attribute named '%s'.", attrName.c_str());
		throw std::exception(szTemp);
	}
}

const std::string& XMLAttributes::getValueAsString(const std::string& attrName, const std::string& def) const
{
	return (exists(attrName)) ? getValue(attrName) : def;
}


bool XMLAttributes::getValueAsBool(const std::string& attrName, bool def) const
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
		char szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH, "XMLAttributes::getValueAsBool - failed to convert attribute '%s' with value '%s' to bool.", attrName.c_str(), getValue(attrName).c_str());
		throw std::exception(szTemp);
	}
}

int XMLAttributes::getValueAsInteger(const std::string& attrName, int def) const
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
		char szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH, "XMLAttributes::getValueAsInteger - failed to convert attribute '%s' with value '%s' to integer.", attrName.c_str(), getValue(attrName).c_str());
		throw std::exception(szTemp);
	}

	return val;
}

float XMLAttributes::getValueAsFloat(const std::string& attrName, float def) const
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
		char szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH, "XMLAttributes::getValueAsInteger - failed to convert attribute '%s' with value '%s' to float.", attrName.c_str(), getValue(attrName).c_str());
		throw(szTemp);
	}

	return val;
}

}