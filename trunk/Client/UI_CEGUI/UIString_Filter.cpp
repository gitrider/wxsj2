#include "StdAfx.h"
#include "UIString_Filter.h"
#include "UIGlobal.h"

#include "GITransferItemSystem.h"

//////////////////////////////////////////////////////////////////////////
//CStringFilter
//////////////////////////////////////////////////////////////////////////
BOOL CStringFilter::IsInclude(const STRING& strIn)
{
	STRING::size_type allsize = m_vIncluce.size();
	for(STRING::size_type i = 0; i < m_vIncluce.size(); ++i)
	{
		if(STRING::npos != strIn.find(m_vIncluce[i]))	return TRUE;	//包含
	}
	return FALSE;
}


BOOL CStringFilter::IsFullCmp(const STRING& strIn)
{
	if(m_sFullCmp.end() != m_sFullCmp.find(strIn)) return TRUE;		//完全匹配
	return FALSE;
}

VOID CStringFilter::ReplaceToSign(const STRING& strIn, STRING& strOut)
{
	const CHAR		KeyStart		= '#';
	const CHAR		ContentsEnd		= '}';

	std::vector<PICE>	vSrcPice;	//来源字串分割成多段
	
	STRING strSrc = strIn;

	PICE			  pc;
	STRING::size_type sB = 0;
	STRING::size_type sC = 0;
	STRING::size_type sE = strSrc.find_first_of(KeyStart);
	STRING::size_type sLen = strSrc.size();

#if	0
	::OutputDebugString("ReplaceToSign Begin=======================\n");
	char dbgmsg[256];
	_snprintf(dbgmsg, 255, "strSrc:%s", strSrc.c_str());
	::OutputDebugString(dbgmsg);
	::OutputDebugString("\n------------------------------------------");
#endif

	do
	{	
		if(sE == STRING::npos)
		{
			//push last replace str
			pc.bReplace = TRUE;
			pc.pice = strSrc.substr(sC);
			vSrcPice.push_back(pc);
			break;
		}

		//get op
		STRING strOp = strSrc.substr(sE+1, 1);

		if(strOp == "{")	//ok, check magic #{} string.
		{
			//item element is valid. ex: #{_INFOID123}
			STRING strItemElement = strSrc.substr(sE+2, 7);
			//info message is valid. ex: #{_INFOMSGxxxxxx}
			STRING strInfoMsg = strSrc.substr(sE+2, 8);

			if(strItemElement == "_INFOID")
			{
				//get itemId
				//todo_yangjun	需要仔细检查剩下的字符是否还是一个完整的INFOID信息
				STRING::size_type sIDEnd = strSrc.find(ContentsEnd, sE+2+7);
				if(sE+2+7 >= sLen)	// fix dead loop if str is "xxx#{_INFOID" [9/25/2006]
				{
					//skip invalid #{
					sE += 2;
					goto LengthOver2;
				}
				STRING strId = strSrc.substr(sE+2+7, sIDEnd-sE-2-7);
				INT itemId = atoi(strId.c_str());

				if(g_pTransferItemSystem->IsElementExist(itemId))
				{//ok, valid item element found.

					//0. push normal replace str
					pc.bReplace = TRUE;
					pc.pice = strSrc.substr(sC, sE-sC);
					vSrcPice.push_back(pc);
					//1. push no replace str
					pc.bReplace = FALSE;
					pc.pice = strSrc.substr(sE, sIDEnd-sE+1);
					vSrcPice.push_back(pc);
				}

				//step to new point.
				sE = sIDEnd + 1;
				sC = sE;
			}
			else if(strInfoMsg == "_INFOMSG")
			{
				//get info message
				INT nContentsLen = atoi(strSrc.substr(sE+2+8,3).c_str());
				STRING::size_type sIDEnd = sE+2+8+3+nContentsLen;
				if(sE+2+8 >= sLen)
				{
					//skip invalid #{
					sE += 2;
					goto LengthOver2;
				}
				
				//ok, valid info message found.
				//0. push normal replace str
				pc.bReplace = TRUE;
				pc.pice = strSrc.substr(sC, sE-sC);
				vSrcPice.push_back(pc);
				//1. push no replace str
				pc.bReplace = FALSE;
				pc.pice = strSrc.substr(sE, sIDEnd-sE+1);
				vSrcPice.push_back(pc);

				//step to new point.
				sE = sIDEnd + 1;
				sC = sE;
			}
			else
			{
				//all other things
				sE += 2;
			}
		}
		else
		{
			//single #
			sE += 1;
		}
LengthOver2:
		if(sE >= sLen) 
		{
			if(sC != sE)
			{
				//push last replace str
				pc.bReplace = TRUE;
				pc.pice = strSrc.substr(sC);
				vSrcPice.push_back(pc);
			}
			break;
		}

		//save new begin point
		sB = sE;

		//find next KeyStart
		sE = strSrc.find(KeyStart, sB);

	}while(TRUE);

	//替换字串中的非法字符
	for(INT i = 0; i < (INT)vSrcPice.size(); ++i)
	{

#if	0
		_snprintf(dbgmsg, 255, "vSrcPice[%d]:%s\n", i, vSrcPice[i].pice.c_str());
		::OutputDebugString(dbgmsg);
#endif

		if(TRUE == vSrcPice[i].bReplace)
		{
			STRING strOld = vSrcPice[i].pice;
			ReplaceToSign_Normal(strOld, vSrcPice[i].pice);
		}
	}

	//生成结果字串
	strOut.clear();
	for(INT i = 0; i < (INT)vSrcPice.size(); ++i)
	{
		strOut += vSrcPice[i].pice;
	}

#if	0
	::OutputDebugString("ReplaceToSign End=========================\n");
#endif

}

VOID CStringFilter::ReplaceToSign_Normal(const STRING& strIn, STRING& strOut)
{
	static STRING strSign		= "?";
	static BYTE byANSIBegin		= 0X20;
	static BYTE byANSIEnd		= 0X80;
	strOut = strIn;

	STRING::size_type allsize = m_vIncluce.size();
	//包含替换
	for(STRING::size_type i = 0; i < m_vIncluce.size(); ++i)
	{
		STRING::size_type pos = strIn.find(m_vIncluce[i]);

		while(STRING::npos != pos)
		{
			STRING strReplace = "";
			STRING::size_type len = m_vIncluce[i].size();
			//如果包含替换的是1个字节的ANSI字节，替换前，
			//需要确认前一个字节一定不是双字节字符集的前一个字节
			BOOL bSkip = FALSE;
			if(1 == len && pos > 0)
			{
				BYTE byChar = strIn[pos-1];
#if 0
				char dbgmsg[256];
				_snprintf(dbgmsg, 255, "strIn[pos-1]:0x%X(0x%X)\n", strIn[pos-1],byChar);
				::OutputDebugString(dbgmsg);
#endif
				//不是标准ANSI英文字符
				if(!(byChar >= byANSIBegin && byChar <= byANSIEnd || byChar == '\r' || byChar == '\n' || byChar == '\t'))
				{
					bSkip = TRUE;
				}
			}

			if(!bSkip)
			{
				for(STRING::size_type k = 0; k < len; ++k, strReplace += strSign);
				strOut.replace(pos, len, strReplace);
			}

			pos = strIn.find(m_vIncluce[i], pos+len);
		}
	}

	//完全匹配替换
	if(IsFullCmp(strIn))
	{
		STRING::size_type len = strIn.size();
		strOut.clear();
		for(STRING::size_type i = 0; i < len; ++i, strOut += strSign);
	}
}

VOID CStringFilter::ReplaceToSign_New(const STRING& strIn, STRING& strOut)
{
	static STRING strSign = "~$%^&(){}`-_+=?,.<>";
	strOut = strIn;

	STRING::size_type allsize = m_vIncluce.size();
	//包含替换
	for(STRING::size_type i = 0; i < m_vIncluce.size(); ++i)
	{
		STRING::size_type pos = strIn.find(m_vIncluce[i]);
		
		while(STRING::npos != pos)
		{
			STRING strReplace = "";
			STRING::size_type len = m_vIncluce[i].size();
			for(STRING::size_type k = 0; k < len; ++k)
			{
				STRING::size_type ri = rand()%int(strSign.size());
				strReplace += strSign.at(ri);
			}
			strOut.replace(pos, len, strReplace);
			pos = strIn.find(m_vIncluce[i], pos+len);
		}
	}

	//完全匹配替换
	if(IsFullCmp(strIn))
	{
		STRING::size_type len = strIn.size();
		strOut.clear();
		for(STRING::size_type i = 0; i < len; ++i)
		{
			STRING::size_type ri = rand()%int(strSign.size());
			strOut += strSign.at(ri);
		}
	}
}
