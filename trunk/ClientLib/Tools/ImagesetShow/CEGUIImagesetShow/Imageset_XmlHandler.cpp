#include "StdAfx.h"
#include "Imageset_XmlHandler.h"
#include "ImagesetXMLAttributes.h"
#include "Imageset.h"

/*************************************************************************
Definition of constant data for Imageset (and sub-classes)
*************************************************************************/
// Declared in Imageset::xmlHandler
const std::string CImagesetItemSet_xmlHandler::StrImagesetElement("Imageset");
const std::string CImagesetItemSet_xmlHandler::StrImageElement("Image");

const char	CImagesetItemSet_xmlHandler::ImageFileAttribute[]	= "Imagefile";
const char	CImagesetItemSet_xmlHandler::ImageNameAttribute[]	= "Name";
const char	CImagesetItemSet_xmlHandler::ImageXPosAttribute[]	= "XPos";
const char	CImagesetItemSet_xmlHandler::ImageYPosAttribute[]	= "YPos";
const char	CImagesetItemSet_xmlHandler::ImageWidthAttribute[]	= "Width";
const char	CImagesetItemSet_xmlHandler::ImageHeightAttribute[]	= "Height";


CImagesetItemSet_xmlHandler::CImagesetItemSet_xmlHandler(CImageset* itemset)
	: m_pItemSet(itemset) 
{
}

/*************************************************************************
SAX2 Handler methods
*************************************************************************/
void CImagesetItemSet_xmlHandler::elementStart(const std::string& element, const CImagesetXMLAttributes& attributes)
{
	// handle an ItemSet element.
	if (element.compare(StrImagesetElement)==0)
	{
		// do nothing...
		m_pItemSet->m_strImageFileName = attributes.getValueAsString(ImageFileAttribute);
	}
	// handle an item element
	else if(element.compare(StrImageElement)==0)
	{
		// get name of item.
		ImagesetImage newImage;

		newImage.strName = attributes.getValueAsString(ImageNameAttribute);
        newImage.xPos = attributes.getValueAsInteger(ImageXPosAttribute);
        newImage.yPos  = attributes.getValueAsInteger(ImageYPosAttribute);
        newImage.width = attributes.getValueAsInteger(ImageWidthAttribute);
        newImage.height = attributes.getValueAsInteger(ImageHeightAttribute);

		m_pItemSet->imageVector.push_back(newImage);
	}
	else
	{
	}
}

void CImagesetItemSet_xmlHandler::elementEnd(const std::string& element)
{
}
