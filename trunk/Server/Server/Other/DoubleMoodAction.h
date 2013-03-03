/********************************************************************
	created:	2008/05/30
	created:	30:5:2008   9:29
	filename: 	d:\prj\src\MapServer\Server\Other\DoubleMoodAction.h
	file path:	d:\prj\src\MapServer\Server\Other
	file base:	DoubleMoodAction
	file ext:	h
	author:		wuwenbin
	
	purpose:	双人情感动作
*********************************************************************/

#ifndef DoubleMoodAction_h__
#define DoubleMoodAction_h__

#include "Type.h"

class Obj_Human;
class DoubleMoodAction
{
public:
	static DoubleMoodAction& GetInstance(){
		static DoubleMoodAction instance;
		return instance;
	}

	~DoubleMoodAction() {}

	//BOOL IsDoubleMoodAction(const CHAR* szMoodAct) const;
	VOID ProcessDoubleMoodAction(INT iChatType, const CHAR* szMoodAct, 
								Obj_Human* pSource, Obj_Human* pDest);

protected:

	//INT GetDoubleMoodActionOptType(const CHAR* szMoodAct) const;
	VOID DoDoubleMoodAction(const CHAR* szMoodAct, Obj_Human* pSource, Obj_Human* pDest);
	VOID RequestDoubleMoodAction(const CHAR* szMoodAct, Obj_Human* pSource, Obj_Human* pDest);
	VOID CancelDoubleMoodAction(const CHAR* szMoodAct, Obj_Human* pSource, Obj_Human* pDest);
	VOID RefuseDoubleMoodAction(const CHAR* szMoodAct, Obj_Human* pSource, Obj_Human* pDest);
	
private:
	DoubleMoodAction() {}
};

#endif // DoubleMoodAction_h__
