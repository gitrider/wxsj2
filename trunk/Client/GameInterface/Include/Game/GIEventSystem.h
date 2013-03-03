
/** ��Ϸ�߼��¼�������
 */

#pragma once


#include "GINode.h"



/// �¼�����ص�����
enum GAME_EVENT_ID;
struct EVENT;

typedef VOID (__stdcall* FUNC_EVENT_HANDLE)(const EVENT* pEvent, UINT dwOwnerData);

struct EVENT_DEFINE
{
	typedef std::list< std::pair< FUNC_EVENT_HANDLE, UINT > > REGISTER_STRUCT;

	GAME_EVENT_ID		idEvent;
	LPCTSTR				szEvent;
	BOOL				delayProcess;
	REGISTER_STRUCT		listFuncNotify;
};

struct EVENT
{
	EVENT_DEFINE*		pEventDef;
	std::vector<STRING>	vArg;

	bool operator == (const EVENT& other)
	{
		if(other.pEventDef != pEventDef) 
			return false;
		if(other.vArg.size() != vArg.size())
			return false;

		for(register size_t i=0; i<vArg.size(); i++)
		{
			if(vArg[i] != other.vArg[i]) 
				return false;
		}

		return true;
	}
};


//=====================================================
//��Ϸ�¼�ϵͳ
class KYLIN_API tEventSystem : public tNode
{
public:

	//�����¼�
	/*
		!!!! �˺���������Ϊ��ӿ�  !!!
		���ܻ������ڴ��ڲ�ͬģ��֮������ͷ�
	
		virtual VOID	PushEvent(GAME_EVENT_ID id, const std::vector< STRING >& vParam) = 0;
	*/

	virtual VOID	PushEvent(GAME_EVENT_ID id)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, INT iArg0)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2 )	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3)	= 0;

	virtual VOID	PushEvent(GAME_EVENT_ID, LPCTSTR szArg0, LPCTSTR szArg1, LPCTSTR szArg2 ) = 0; // 20100517 AddCode

	//ע���¼�������
	virtual VOID	RegisterEventHandle(const STRING& nameEvent, FUNC_EVENT_HANDLE funHandle, UINT dwOwnerData=NULL) = 0;
	//�����¼�
	virtual VOID	ProcessAllEvent(VOID) = 0;

	virtual VOID	UNRegisterEventHandle(const STRING& nameEvent, FUNC_EVENT_HANDLE funHandle, UINT uOwnerData) =0;

protected:

	KL_DECLARE_DYNAMIC(tEventSystem);

};