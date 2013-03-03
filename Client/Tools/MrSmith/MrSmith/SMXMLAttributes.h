/*

Agent template XML Attributes

by fangyuan.
ref from CEGUI
*/
#pragma once

namespace SMITH
{

class XMLAttributes
{
public:
	/*!
	\brief
	XMLAttributes constructor.
	*/
	XMLAttributes(void);

	/*!
	\brief
	XMLAttributes Destructor
	*/
	virtual ~XMLAttributes(void);

	/*!
	\brief
	Adds an attribute to the attribute block.  If the attribute value already exists, it is replaced with
	the new value.

	\param attrName
	std::string object holding the name of the attribute to be added.

	\param attrValue
	std::string object holding a string representation of the attribute value.

	\return
	Nothing.
	*/
	void add(const std::string& attrName, const std::string& attrValue);

	/*!
	\brief
	Removes an attribute from the attribute block.

	\param attrName
	std::string object holding the name of the attribute to be removed.

	\return
	Nothing.
	*/
	void remove(const std::string& attrName);

	/*!
	\brief
	Return whether the named attribute exists within the attribute block.

	\param attrName
	std::string object holding the name of the attribute to be checked.

	\return
	- true if an attribute with the name \a attrName is present in the attribute block.
	- false if no attribute named \a attrName is present in the attribute block.
	*/
	bool exists(const std::string& attrName) const;

	/*!
	\brief
	Return the number of attributes in the attribute block.

	\return
	value specifying the number of attributes in this attribute block.
	*/
	size_t getCount(void) const;

	/*!
	\brief
	Return the name of an attribute based upon its index within the attribute block.

	\note
	Nothing is specified about the order of elements within the attribute block.  Elements
	may not, for example, appear in the order they were specified in the XML file.

	\param index
	zero based index of the attribute whos name is to be returned.

	\return
	std::string object holding the name of the attribute at the requested index.

	\exception IllegalRequestException  thrown if \a index is out of range for this attribute block.
	*/
	const std::string& getName(size_t index) const;

	/*!
	\brief
	Return the value string of an attribute based upon its index within the attribute block.

	\note
	Nothing is specified about the order of elements within the attribute block.  Elements
	may not, for example, appear in the order they were specified in the XML file.

	\param index
	zero based index of the attribute whos value string is to be returned.

	\return
	std::string object holding the string value of the attribute at the requested index.

	\exception IllegalRequestException  thrown if \a index is out of range for this attribute block.
	*/
	const std::string& getValue(size_t index) const;

	/*!
	\brief
	Return the value string for attribute \a attrName.

	\param attrName
	std::string object holding the name of the attribute whos value string is to be returned

	\return
	std::string object hilding the value string for attribute \a attrName.

	\exception UnknownObjectException   thrown if no attribute named \a attrName is present in the attribute block.
	*/
	const std::string& getValue(const std::string& attrName) const;

	/*!
	\brief
	Return the value of attribute \a attrName as a string.

	\param attrName
	std::string object holding the name of the attribute whos value is to be returned.

	\param def
	std::string object holding the default value to be returned if \a attrName does not exist in the attribute block.
	For some parsers, defaults can be gotten from schemas and such like, though for others this may not be desired
	or possible, so this parameter is used to ensure a default is available in the abscence of other mechanisms.

	\return
	std::string object containing the value of attribute \a attrName if present, or \a def if not.
	*/
	const std::string& getValueAsString(const std::string& attrName, const std::string& def = "") const;

	/*!
	\brief
	Return the value of attribute \a attrName as a boolean value.

	\param attrName
	std::string object holding the name of the attribute whos value is to be returned.

	\param def
	bool value specifying the default value to be returned if \a attrName does not exist in the attribute block.
	For some parsers, defaults can be gotten from schemas and such like, though for others this may not be desired
	or possible, so this parameter is used to ensure a default is available in the abscence of other mechanisms.

	\return
	bool value equal to the value of attribute \a attrName if present, or \a def if not.

	\exception IllegalRequestException  thrown if the attribute value string coul dnot be converted to the requested type.
	*/
	bool getValueAsBool(const std::string& attrName, bool def = false) const;

	/*!
	\brief
	Return the value of attribute \a attrName as a integer value.

	\param attrName
	std::string object holding the name of the attribute whos value is to be returned.

	\param def
	integer value specifying the default value to be returned if \a attrName does not exist in the attribute block.
	For some parsers, defaults can be gotten from schemas and such like, though for others this may not be desired
	or possible, so this parameter is used to ensure a default is available in the abscence of other mechanisms.

	\return
	integer value equal to the value of attribute \a attrName if present, or \a def if not.

	\exception IllegalRequestException  thrown if the attribute value string coul dnot be converted to the requested type.
	*/
	int getValueAsInteger(const std::string& attrName, int def = 0) const;

	/*!
	\brief
	Return the value of attribute \a attrName as a floating point value.

	\param attrName
	std::string object holding the name of the attribute whos value is to be returned.

	\param def
	float value specifying the default value to be returned if \a attrName does not exist in the attribute block.
	For some parsers, defaults can be gotten from schemas and such like, though for others this may not be desired
	or possible, so this parameter is used to ensure a default is available in the abscence of other mechanisms.

	\return
	float value equal to the value of attribute \a attrName if present, or \a def if not.

	\exception IllegalRequestException  thrown if the attribute value string coul dnot be converted to the requested type.
	*/
	float getValueAsFloat(const std::string& attrName, float def = 0.0f) const;

protected:
	typedef std::map<std::string, std::string > AttributeMap;
	AttributeMap    d_attrs;
};

}
