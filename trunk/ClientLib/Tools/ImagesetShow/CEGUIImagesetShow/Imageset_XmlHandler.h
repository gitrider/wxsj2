/****************************************\
*										*
*										*
*										*
* 		ImagesetItemSet_xmlHandler		*
*										*
*						 by jinchao		*
*						ref from CEGUI	*
\****************************************/

#pragma once

#include "ImagesetXMLHandler.h"

class CImageset;
class CImagesetItemSet_xmlHandler : public CImagesetXMLHandler
{
public:
	/*************************************************************************
	Construction & Destruction
	*************************************************************************/
	/*!
	\brief
	Constructor for HelpItemSet:xmlHandler objects

	\param imageset
	Pointer to the HelpItemSet object creating this xmlHandler object
	*/
	CImagesetItemSet_xmlHandler(CImageset* itemset);

	/*!
	\brief
	Destructor for HelpItemSet:xmlHandler objects
	*/
	virtual ~CImagesetItemSet_xmlHandler(void) {}

	/*************************************************************************
	SAX2 Handler overrides
	*************************************************************************/ 
	/*!
	\brief
	document processing (only care about elements, schema validates format)
	*/
	virtual void elementStart(const std::string& element, const CImagesetXMLAttributes& attributes);
	virtual void elementEnd(const std::string& element);

	/*************************************************************************
	Functions used by our implementation
	*************************************************************************/
//	CImagesetItemSet*	getHelpItemset(void) const				{return m_pItemSet;}

private:
	/*************************************************************************
	Implementation Constants
	*************************************************************************/
	static const std::string StrImagesetElement;	//!< Tag name for ItemSet elements.
	static const std::string StrImageElement;		//!< Tag name for Item elements.
	static const char	ImageFileAttribute[];		//!< Attribute name that stores the name of the item
	static const char	ImageNameAttribute[];		//!< Attribute name that stores the name of the item
	static const char	ImageXPosAttribute[];		//!< Attribute name that stores the anchor for the item.
	static const char	ImageYPosAttribute[];		//!< Attribute name that stores the url for the item.
	static const char	ImageWidthAttribute[];		//!< Attribute name that stores the url for the item.
	static const char	ImageHeightAttribute[];		//!< Attribute name that stores the url for the item.

	/*************************************************************************
	Implementation Data
	*************************************************************************/
	CImageset*	m_pItemSet;			//!< Holds a pointer to the ImagesetItemSet that created the handler object

//	typedef std::vector< CImagesetItem* >	ItemStack;	
//	CImagesetItem*	m_root;				//!< Will point to first item(created by itemset!)

//	ItemStack	m_stack;				//!< Stack used to keep track of what we're doing to which item.
};