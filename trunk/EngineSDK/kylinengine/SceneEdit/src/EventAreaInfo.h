#ifndef __EVENTAREAINFO_H__
#define __EVENTAREAINFO_H__
#pragma once
#include "baserendershape.h"
#include "LogicDataStructDef.h"

namespace Fairy 
{


	class CEventAreaInfo : public CBaseRenderShape
	{
	public:
		CEventAreaInfo(void);
		~CEventAreaInfo(void);

	public:
		//typedef std::map<Ogre::String,Ogre::String> ParamsMap;
		////////////////////////////////////////////////////////////////////////////////////////
		//
		// Âß¼­Êý¾Ý
		//

		int         m_iId;
		int			m_iEventAreaId;
		int			m_iScriptId;
		AREARECT	m_rect;
		Ogre::String m_paramsName[10];
		Ogre::String m_paramsValue[10];
	};

}

#endif //__EVENTAREAINFO_H__