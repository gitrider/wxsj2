/****************************************\
*										*
*										*
*										*
* 			ImagesetXMLHandler			*
*										*
*						 by jinchao		*
*						ref from CEGUI	*
\****************************************/
#pragma once

class CImagesetXMLAttributes;
class CImagesetXMLHandler
{
public:
	/*!
	\brief
	XMLHandler base class constructor.
	*/
	CImagesetXMLHandler(void);

	/*!
	\brief
	XMLHandler base class destructor.
	*/
	virtual ~CImagesetXMLHandler(void);

	/*!
	\brief
	Method called to notify the handler at the start of each XML element encountered.

	\param element
	std::string object holding the name of the element that is starting.

	\param attributes
	An XMLAttributes object holding the collection of attributes specified for the element.

	\return
	Nothing.
	*/
	virtual void elementStart(const std::string& element, const CImagesetXMLAttributes& attributes);

	/*!
	\brief
	Method called to notify the handler at the end of each XML element encountered.

	\param element
	std::string object holding the name of the element that is ending.

	\return
	Nothing.
	*/
	virtual void elementEnd(const std::string& element);
};

