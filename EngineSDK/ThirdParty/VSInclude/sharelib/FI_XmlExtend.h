#ifndef __FI_TINY_XML_EXTEND
#define __FI_TINY_XML_EXTEND


#define TIXML_USE_STL
#include <tinyxml/tinyxml.h>
/*
#ifndef _DEBUG
#pragma comment( lib, "tinyxml_STL.lib" )
#else
#pragma comment( lib, "tinyxmld_STL.lib" )
#endif
*/
static const char *s_pXMLEmptyString = " ";

inline
const char *FI_TiXMLText( TiXmlNode *pNode )
{
	TiXmlNode *pChild = pNode->FirstChild();
	while( pChild  )
	{
		TiXmlText *pText = pChild->ToText();
		if( pText )
			return pText->Value();
		pChild = pChild->NextSibling();
	}
	return s_pXMLEmptyString;
}

inline 
const char *FI_TiXmlAttr( TiXmlNode *pNode, const char *attrName )
{
	TiXmlElement *pEle = pNode->ToElement();
	if( pEle )
	{
		return pEle->Attribute( attrName );
	}
	else
		return s_pXMLEmptyString;
}


inline
const char *FI_TiSubXMLText( TiXmlNode *pNode , const char *sSubNode)
{
	TiXmlNode *pSubNode = pNode->FirstChild(sSubNode);
	if( pSubNode )
	{
		return FI_TiXMLText(pSubNode );
	}
	else
		return s_pXMLEmptyString;
}

inline
const char *FI_TiSubXMLAttr( TiXmlNode *pNode , const char *sSubNode, const char *sAttr)
{
	TiXmlNode *pSubNode = pNode->FirstChild(sSubNode);
	if( pSubNode )
	{
		return FI_TiXmlAttr(pSubNode , sAttr);
	}
	else
		return s_pXMLEmptyString;
}

/*
TiXmlNode * FI_TiSubNodeFind( TiXmlNode *pNode , const char *sSubNode );
void	FI_ReadSubElementNameValueParam( TiXmlNode *pNode, const char *pSpecialNode,
										std::vector<std::string> &sNameArray, std::vector<std::string> &sValueArray );
TiXmlNode*	FI_InsertSubElementNode( TiXmlNode *pNode, const char *sElement, const char *sValue );
bool	FI_SetElementAtribute( TiXmlNode *pNode, const char *sAttrName, const char *sAttrValue );
bool	FI_ReadElementNameValueParam( TiXmlNode *pNode, std::string &sName, std::string &sValue );
void	FI_ReadSubElementNameValueParam( TiXmlNode *pNode, std::vector<std::string> &sNameArray, std::vector<std::string> &sValueArray );
bool	FI_SetElementValue( TiXmlNode *pNode, const char *sValue );
*/
#endif
