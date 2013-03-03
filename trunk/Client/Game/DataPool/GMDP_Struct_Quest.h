
/**  ���ݳ����ݽṹ -����
 */

#pragma once


#include "ScriptCommandStruct.h"
#include "GIObjectManager.h"


struct _MISSION_INFO
{
	bool    m_bFill;
	INT		m_nLevel;			//����ȼ�
	INT		m_nKind;			//��������
	INT		m_nElite;			//�����Ƿ���ѻ���Ҫ���˲ſ������

	//�����Ƿ���Ҫ��ʾĳЩֵ

	INT		m_nOKFail;			// �����Ƿ����
	INT		m_nLimitTime;		// ����ʣ��ʱ��
	INT		m_nRound;			// ������
	INT		m_nBill;			// �������Ʊ��
	
	//------------------------------------------------------
	//��������Ʒ
	struct QuestRewardItem
	{
		SMissionBonus*			pItemData;
		tObject_Item*			pItemImpl;
		BOOL					bSelected;
	};

	//------------------------------------------------------
	//������Ҫ��Ʒ
	struct QuestDemandItem
	{
		SMissionBonusItem*		pDemandItem;
		tObject_Item*			pItemImpl;
	};

	//------------------------------------------------------
	//������Ҫɱ����NPC
	struct QuestDemandKill
	{
		SMissionDemandKill*		pDemandKill;
		std::string				szNPCName;
	};
	//------------------------------------------------------
	//������Ҫ���Զ���
	struct QuestCustom
	{
		std::string				szCustomString;	//�����Զ�����ַ���
		INT						nCount;			//��Ҫ�������������
	};

	std::vector< QuestRewardItem >	m_vecQuestRewardItem;
	std::vector< QuestDemandItem >  m_vecQuestDemandItem;
	std::vector< QuestDemandKill >  m_vecQuestDemandKill;
	std::vector< QuestCustom >		m_vecQuestCustom;

	std::string m_misName;
	std::string m_misDesc;
	std::string m_misState;
	std::string m_misBillName;

};

typedef std::map<INT, _MISSION_INFO>    MisInfo;
typedef void(*parseLine)(CHAR** pDest,CHAR* pSrc,INT j);

static const char *const tokens [] = 
{
	"g_MissionName",
	"g_DemandItem",
	"g_ItemBonus",
	"g_RandomItemBonus",
	"g_RadioItemBonus", 
	"g_MoneyBonus",
	"g_ExpBonus",
	"g_MissionTarget",
	"g_MissionState",
	"g_MissionLevel",
	"g_IfMissionElite",
	"g_MissionKind",
	"g_DemandKill",
	"g_IsMissionOkFail",
	"g_MissionLimitTime",
	"g_MissionRound",
	"g_Custom",
	"g_Bill",
	"g_BillName",
};

//INT  RESERVER = sizeof(tokens) / sizeof(const CHAR*);
extern MisInfo m_misInfo;
extern _MISSION_INFO* misInfo;

extern void parseMissionName(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseDemandItem(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseItemBonus(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseRandItemBonus(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseRadioItemBonus(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMoneyBonus(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseExpBonus(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionBriefTarget(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionState(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionLevel(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionElite(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionKind(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseDemandKill(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionOKFail(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionLimitTime(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionRound(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionCustom(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionBill(CHAR** pDest,CHAR* pSrc,INT j);
extern void parseMissionBillName(CHAR** pDest,CHAR* pSrc,INT j);


extern _MISSION_INFO& GetMissionInfo( UINT scriptId );
