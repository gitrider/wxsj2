#ifndef _DBTHREAD_MANAGER_H_
#define _DBTHREAD_MANAGER_H_

#include "DBThread.h"
#define MAX_DB_THREAD_POOL	90


class DBThreadManager
{


public:
		DBThreadManager();
		~DBThreadManager();

		BOOL		Init();
		BOOL		Start();
		BOOL		Stop();

		BOOL		IsPoolTID(TID tid);

		//����һ��DB������Ϣ
		BOOL		SendPacket(Packet* pPacket,PlayerID_t pID);

		ODBCInterface*	GetInterface(TID	tid);
private:
		DBThread*	mpDBThread[MAX_DB_THREAD_POOL];	
		UINT		mDBThreadCount;
};

extern DBThreadManager*	g_pDBThreadManager;

#endif