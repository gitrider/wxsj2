
/** ����ӿ�
 *	
 *	���ܣ�
 *	
 *	�޸ļ�¼��
 *			080304	���� GetContexMenuObjectName
 *			
 */

#include "CGChannelCreate.h"
#include "CGChannelDismiss.h"
#include "CGChannelInvite.h"
#include "CGChannelKick.h"
#include "GCChat.h"
#include "GCChannelResult.h"
#include "GCChannelError.h"
#include "CGModifySetting.h"
#include "GCRetSetting.h"
#include "Type.h"
#include "GITransferItemSystem.h"


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

enum ENUM_CHAT_TYPE;
enum CHAT_NEED_TYPE;
struct EVENT;

enum TALK_MAP_ICON
{	
	//0
	SELF_CMD = 0,
	//1
	SELF_TYPE,
	//2
	SELF_SCENEID,
	SELF_MONSTER_SERVER_ID = SELF_SCENEID,
	SELF_SERVERMSG_TEMPLAT = SELF_SCENEID,
	//3
	SELF_X,
	SELF_MONSTER_PAOPAO_ID = SELF_X,
	SELF_SERVERMSG_PARAM1 = SELF_X,
	//4
	SELF_Z,
	//5
	SELF_TIP,
	SELF_NPC_NAME = SELF_TIP,
};

//����״̬
enum CHAT_SEND_STATE
{
	SEND_CHAT_OK = 1,
	SEND_FAILED_TIME_SLICE = -1,
	SEND_FAILED_LV10 = -2,
	SEND_FAILED_TEMPLATE = -3,
	SEND_FAILED_NEED = -4,
	SEND_FAILED_NOREASON = -5,
	SEND_FAILED_PINGBI = -6,
	SEND_FAILED_TIME_DATA = -7, 
};
//�������ж������
typedef		std::vector<STRING>				TALK_ACT_MSG;
enum
{
	TALK_ACT_NOBODY,
	TALK_ACT_MYSELF,
	TALK_ACT_OTHER,
	
	TALK_ACT_NUMBER,
};

struct TALK_ACT_STRUCT
{
	TALK_ACT_MSG	m_actMsg;				//������ʾ��Ϣ
	STRING			m_actOrder;				//���ﶯ�����

	INT				m_actIdx;				//����������Ϊ�����actbutton��ʾ��

	STRING			m_iconName;				//ͼ��
	STRING			m_tip;					//��ʾ��Ϣ
};

//�ַ����������
enum TALKANALYZE_STR_ENUM
{
	ORGINAL_STR,
	SEND_STR,
	TALK_STR,
	PAOPAO_STR,
	HISTORY_STR,

	PRIVATE_STR,
};

enum TALKANALYZE_STRTYPE_ENUM
{
	STRINGTYPE_INVALID = -1,

	STRINGTYPE_NORMAL = 0,			//��ͨ��Ϣ
	STRINGTYPE_GM,					//GM����
	STRINGTYPE_TXTACT,				//���ֱ���
	STRINGTYPE_ACT,					//�������ж���
// -----------------------------------------------------------------  @*�����������������
	STRINGTYPE_FLAGADD,				//��ͼָʾ�����	[ ]����
	STRINGTYPE_FLASHADD,			//��ͼ��˸�����	[ ]����
	STRINGTYPE_FLAGDEL,				//��ͼָʾ��ɾ��	[ ]����
	STRINGTYPE_FLASHDEL,			//��ͼ��˸��ɾ��	[ ]����

	STRINGTYPE_FLAG_NPC_ADD,		//��NPC���������ָʾ��
	STRINGTYPE_FLAG_POS_ADD,		//��x,z�����ָʾ��
	STRINGTYPE_FLASH_NPC_ADD,		//��NPC��������ӻ�ɫָʾ��
	STRINGTYPE_FLASH_POS_ADD,		//��x,z����ӻ�ɫָʾ��

	STRINGTYPE_MONSTER_PAOPAO,		//��������
	STRINGTYPE_SERVERMSG_TALK,		//Server�˷��͹�������ʾ��Ϣ��Ϣ
// -----------------------------------------------------------------  @*�����������������
	STRINGTYPE_NOUSER,				//˽��Ŀ�겻���ߣ�����������������ʾ��Ϣ

	STRINGTYPE_NUMBERS,
};

//
//�ű�����ɳ�䣬��Щ����Ӧ���ɽű�����
//

//#define _MSG_POSITION_SHOW_			//������Ϣ����λ�õĶ���

////CHAT_TYPE_SELF���͵��Խ���Ϣ
//#define ADDTALKMSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_PRIVATE, __FILE__, __LINE__);
//
////CHAT_TYPE_TEAM���͵��Խ���Ϣ
//#define ADD_TEAM_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_TEAM, __FILE__, __LINE__);
//
////CHAT_TYPE_GUILD���͵��Խ���Ϣ
//#define ADD_GUILD_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_GUILD, __FILE__, __LINE__);
//
////CHAT_TYPE_MENPAI���͵��Խ���Ϣ
//#define ADD_MENPAI_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_FAMILY, __FILE__, __LINE__);
//
////CHAT_TYPE_SYSTEM���͵��Խ���Ϣ
//#define ADD_SYSTEM_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_SYSTEM, __FILE__, __LINE__);

#define ADDTALKMSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_SELF, __FILE__, __LINE__);

//CHAT_TYPE_TEAM���͵��Խ���Ϣ
#define ADD_TEAM_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_TEAM, __FILE__, __LINE__);

//CHAT_TYPE_GUILD���͵��Խ���Ϣ
#define ADD_GUILD_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_GUILD, __FILE__, __LINE__);

//CHAT_TYPE_MENPAI���͵��Խ���Ϣ
#define ADD_MENPAI_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_MENPAI, __FILE__, __LINE__);

//CHAT_TYPE_SYSTEM���͵��Խ���Ϣ
#define ADD_SYSTEM_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_SYSTEM, __FILE__, __LINE__);

//����������Ϣ�ĳ���
#define CACULATE_MSG_LENGTH(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::caculateMsgLength(szMsg);

namespace SCRIPT_SANDBOX
{
	extern const INT		TALK_TO_USER_MAXNUM;
	extern const INT		CHANNEL_INVITE_USER_MAXNUM;
	extern const INT		CHAT_TALK_SLICE;
	extern const INT		CHAT_TAB_MAX_NUMBER;
	
	typedef std::vector<INT>	ItemCollect;
	//�����Ϣ��װ
	class Global_Msg_Func
	{
	public:
		static VOID addTalkMsg(LPCTSTR szMsg, ENUM_CHAT_TYPE chatType, CHAR* file, INT line);
		static VOID addTalkMsg(STRING szMsg, ENUM_CHAT_TYPE chatType, CHAR* file, INT line);

		static INT	caculateMsgLength(STRING& szMsg);
		static INT	caculateMsgLength(tTransferItemSystem::Element* pElement);
	};

	//�ַ�������
	class TalkAnalyze
	{
	public:
		TalkAnalyze(INT ty, STRING str);
		virtual			~TalkAnalyze(){};

		virtual VOID	doAnalyze(STRING& strTalker);	//����

		INT				getChannelType(){return m_ChannelType;}
		INT				getOrChannelType(){return m_OrChannelTye;}

		STRING			getStr(INT type);
		STRING			getErrStr(){return m_ErrStr;}
		BOOL			IsOk();
		INT				getStrType(){return m_StrType;}

		ItemCollect* GetItemCollect(){ return &m_vInfo; }
		VOID			CheckItemElementInfo();
	protected:
		VOID			JudgeChannelType();
		VOID			MoveHeader();
		VOID			JudgeStringType();
		VOID			GenAllStr();

		virtual VOID	GenSendStr(){}
		virtual	VOID	GenTalkStr(){}
		virtual	VOID	GenPaoPaoStr(){}
		virtual VOID	GenHistoryStr(){}

		VOID			RulerCheck();
		STRING			RemoveInvalidControlChar(const STRING& strIn);
		STRING			RemoveSpaceChar(const STRING& strIn);

	public:
		STRING			ReplaceItemElementInfo1( const STRING& strSrc );
		STRING			CreateItemElementInfo1( const STRING& strSrc );


		STRING			ReplaceItemElementInfo( const STRING& strSrc );
		STRING			CreateItemElementInfo( const STRING& strSrc );
	protected:
		friend class Talk;

		INT m_ChannelType;		//�������ʵ��Ƶ������
		STRING	m_SendStr;		//���紫���õ��ִ�
		STRING	m_TalkStr;		//���������촰����ʾ�õ��ִ�
		STRING	m_PaoPaoStr;	//��������ʾ�õ��ִ�
		STRING	m_HistoryStr;	//��ʷ��¼Ӧʹ�õ��ִ�
		STRING	m_TargetStr;	//˽�Ķ��������
		STRING  m_HeaderStr;	//�����е�������ͷ���������췢����ʷ�ִ�ʹ��

		STRING	m_OrInputStr;	//��������ԭʼ�����ĵ���Ϣ�ִ�
		INT		m_OrChannelTye;	//Ĭ�ϵĵ�ǰƵ������

		BOOL	m_Ok;			//�Ƿ�����ɹ�
		INT		m_StrType;		//�ַ���������
		STRING	m_ErrStr;		//��÷������

		ItemCollect m_vInfo;		//������η����������е�ItemElement�ĸ�����
	protected:
		STRING	m_PrevResult;
	};

	class SendTalkAnalyze : public TalkAnalyze
	{
	public:
		SendTalkAnalyze(INT ty, STRING str):TalkAnalyze(ty,str){}
		BOOL			IsOk();
		STRING		GetSendBakStr(){ return m_SendStrBak; };
	protected:
		virtual VOID	GenSendStr();
		virtual VOID	GenHistoryStr();

		STRING		m_SendStrBak;			//��Ҫ��Ϊ��˽��ʱ������Ʒ����ϵͳ�����ݡ�
	};

	class RecvTalkAnalyze : public TalkAnalyze
	{
	public:
		RecvTalkAnalyze(INT ty, STRING str):TalkAnalyze(ty,str){}
		virtual VOID	doAnalyze(STRING& strTalker);	//����
		STRING			getTalker(){return m_Talker;}
	protected:
		virtual	VOID	GenTalkStr();
		virtual	VOID	GenPaoPaoStr();
		virtual VOID	GenHistoryStr();
	private:
		STRING			m_Talker;	//˵������
	};

	class LuaTalkAnalyze : public TalkAnalyze
	{
	public:
		LuaTalkAnalyze(INT ty, STRING str):TalkAnalyze(ty,str){}
		BOOL			IsOk();
		virtual VOID	doAnalyze(STRING& strTalker);	//����
	protected:
		virtual	VOID	GenTalkStr();
	private:
		STRING			m_Talker;	//˵������
	};

	//������ؽӿ�
	class Talk
	{
	public:
		class HistoryMsg
		{
		public:
			HistoryMsg()
			{
				m_Name = "";
				m_ChannelType = CHAT_TYPE_INVALID;
				m_Data = "";
				m_Time = 0;
			};

			HistoryMsg(ENUM_CHAT_TYPE type);

			VOID					SetMsgName(STRING name){m_Name = name;}
			STRING					GetMsgName(){return m_Name;}

			VOID					SetChannelType(ENUM_CHAT_TYPE ctype){m_ChannelType = ctype;}
			ENUM_CHAT_TYPE			GetChannelType(){return m_ChannelType;}

			VOID					SetMsgData(STRING data, CHAR* file = NULL, INT line = 0);
			STRING					GetMsgData(){return m_Data;}

			VOID					SetMsgTime(UINT time){m_Time = time;}
			UINT					GetMsgTime(){return m_Time;}

			INT						SetByPacket(GCChat *packet);	// GCChat Packet	->	HistoryMsg Msg

			VOID					SetItemCollect(ItemCollect* pCollect);
			ItemCollect*			GetItemCollect() {return &m_vInfo;};
			VOID					DeleteInfoElement();
			VOID					AddInfoElement();

			INT						GetID(VOID){return m_nID;}
			VOID					SetID(VOID){m_nID = _createID(); }
		protected:
			STRING					m_Name;			//talker
			ENUM_CHAT_TYPE			m_ChannelType;	//channel
			STRING					m_Data;			//content

			UINT					m_Time;			//recvtime or sendtime

			ItemCollect			m_vInfo;	//��¼���е�ItemElement.

			INT						m_nID;			//����Ϣid
			int						_createID(void)
			{
				static int s_nID = 0;
				return ++s_nID;
			}
		};
	protected:
		//��ʷ��¼����
		std::deque< HistoryMsg >	m_SendTimeHisQue;		//���һ���ض�ʱ�����ͳ���������Ϣ�ļ�¼
		std::deque< HistoryMsg >	m_SendHisQue;	//�ͳ���������Ϣ
		std::deque< HistoryMsg >	m_SendHisTextQue;		//�ѷ��͵�������Ϣ����ʷ��¼
		std::deque< HistoryMsg >	m_RecvHisQue;	//�յ���������Ϣ�������ͻ����Լ���������Ϣ��
		std::deque< HistoryMsg >	m_GMCmdHisQue;	//GM������ʷ��Ϣ

		struct tabcfg_s
		{
			INT icfg[CHAT_TYPE_NUMBER];

			tabcfg_s()
			{
				memset(icfg, 0,sizeof(icfg));
			}

			VOID	Clean()
			{
				memset(icfg, 0,sizeof(icfg));
			}
		};
		std::vector< tabcfg_s >		m_TabCfg;		//ȫ��Tabҳ���������Ϣ
		INT							m_CurTab;		//��ǰ������Tabҳ��
		
		struct Talk_Need_Struct
		{
			CHAT_NEED_TYPE		type;				//��������
			INT					value;				//��������

			Talk_Need_Struct()
			{
				type = CHAT_NEED_NONE;
				value = 0;
			}
		};
		
		typedef std::vector< Talk_Need_Struct >		TALK_NEED_ARRAY;

		class Channel
		{
		public:
			Channel()
			{
				m_channelType = CHAT_TYPE_INVALID;
				m_channelName = "";
				m_channelHeader = "";
				m_TimeSlice = 0;
				m_close = FALSE;
				m_need.resize(CHAT_NEED_NUMBER);
				m_lastSendTime = 0;
			};
			virtual ~Channel(){};
			
			VOID					SetType(ENUM_CHAT_TYPE ctype){m_channelType = ctype;}
			ENUM_CHAT_TYPE			GetType(){return m_channelType;}

			VOID					SetName(STRING name){m_channelName = name;}
			STRING					GetName(){return m_channelName;}
			STRING					GetHeader(){return m_channelHeader;}

			UINT					GetSliceTime(){return m_TimeSlice;}
			VOID					SetSliceTime(UINT dwTime){m_TimeSlice = dwTime;}
			
			VOID					Close(){m_close = TRUE;}
			VOID					Open(){m_close = FALSE;}
			BOOL					isClose(){return m_close;}
			BOOL					TalkNeedCheck();	//TRUE ���ͨ�� FALSE ��鲻ͨ��
			BOOL					TalkTimeCheck(UINT dwTimeNow);	//TRUE ���ͨ�� FALSE ��鲻ͨ��
			VOID					SetNeed(INT tp, INT value);

			VOID					LoadChannelConfig();
			virtual VOID			SetSendPacket(CGChat *packet, TalkAnalyze* pAna);	//���ñ�Ƶ����Ҫ���õ��������
		protected:
			ENUM_CHAT_TYPE			m_channelType;	//Ƶ������
			STRING					m_channelName;	//Ƶ������
			STRING					m_channelHeader;	//Ƶ����ǰ���ַ�
			UINT					m_TimeSlice;	//Ƶ����Ĭ��ʱ�����
			BOOL					m_close;		//Ƶ���Ƿ�ر�
			TALK_NEED_ARRAY			m_need;			//Ƶ������

			UINT					m_lastSendTime;	//���һ�η�����Ϣ��ʱ��
		};

		class PrivateChannel : public Channel
		{
		public:
			PrivateChannel()
			{
				m_channelType = CHAT_TYPE_CHANNEL;
				//m_channelName = "�Խ�Ƶ��";
				m_GUID = INVALID_GUID;
			};
			virtual ~PrivateChannel(){};

			VOID					SetGUID(GUID_t id){m_GUID = id;}
			GUID_t					GetGUID(){return m_GUID;}
			
			VOID					Clean();	//ɾ���Խ�Ƶ��

			INT						AddToQue(STRING user);
			INT						KickFromQue(STRING user);
			INT						IsInQue(STRING user);
			STRING					GetUserByIndex(UINT idx);
		protected:
			GUID_t					m_GUID;			//�Խ�Ƶ���ڷ������ϵ�Ψһ���
			std::deque< STRING >	m_InviteQue;
		};

		//�Խ�Ƶ��
		PrivateChannel*				m_PrvCh;

		class UserChannel : public Channel
		{
		public:
			UserChannel()
			{
				m_channelType = CHAT_TYPE_TELL;
				//m_channelType = CHAT_TYPE_PRIVATE;
				//m_channelName = "����Ƶ��";

				m_curSelPos = -1;
			};
			virtual ~UserChannel(){};

			INT						AddUser(STRING user);
			INT						IsInQue(STRING user);
			VOID					Clean();
			STRING					GetUserByIndex(UINT idx);
			UINT					GetUserNum(){return (UINT)m_UserQue.size();};

			STRING					SelectFirstUser();
			STRING					SelectLastUser();
			STRING					SelectNextUser(BOOL dir); //TRUE 0->end, FALSE end->0
			BOOL					IsFirstUserSelect(){return (0 == m_curSelPos)?TRUE:FALSE;}
			BOOL					IsLastUserSelect(){return (m_curSelPos == (INT)m_UserQue.size()-1)?TRUE:FALSE;}
			BOOL					SetSelectByUser(STRING name);

			virtual VOID			SetSendPacket(CGChat *packet, TalkAnalyze* pAna);
		protected:
			std::deque< STRING >	m_UserQue;

			INT						m_curSelPos;	//��ǰѡ��λ��
		};

		//����Ƶ��
		UserChannel*				m_UsrCh;

		class TeamChannel : public Channel
		{
		public:
			TeamChannel()
			{
				m_channelType = CHAT_TYPE_TEAM;
				//m_channelName = "���Ƶ��";
			};
			
			virtual ~TeamChannel(){};

			VOID					SetTeamID(TeamID_t id){m_teamID = id;};
			TeamID_t				GetTeamID(){return m_teamID;};
			virtual VOID			SetSendPacket(CGChat *packet, TalkAnalyze* pAna);
		protected:
			TeamID_t	m_teamID;
		};

		//���Ƶ��
		TeamChannel*				m_TeamCh;

		typedef Channel SystemChannel;
		typedef Channel	SelfChannel;
		
		//ϵͳƵ��
		SystemChannel*				m_SysCh;
		//�ͻ�������Ƶ��
		SelfChannel*				m_SelfCh;

		class MenPaiChannel : public Channel
		{
		public:
			MenPaiChannel()
			{
				m_channelType = CHAT_TYPE_MENPAI;
				//m_channelType = CHAT_TYPE_FAMILY;
				//m_channelName = "����Ƶ��";
			};
			virtual ~MenPaiChannel(){};

			VOID					SetMenPaiID(MenPaiID_t id){m_menpaiID = id;};
			MenPaiID_t				GetMenPaiID(){return m_menpaiID;};
			virtual VOID			SetSendPacket(CGChat *packet, TalkAnalyze* pAna);
		protected:
			MenPaiID_t	m_menpaiID;
		};

		//����Ƶ��
		MenPaiChannel*				m_MenPaiCh;

		class GuildChannel : public Channel
		{
		public:
			GuildChannel()
			{
				m_channelType = CHAT_TYPE_GUILD;
				//m_channelName = "����Ƶ��";
			};

			virtual ~GuildChannel(){};

			VOID					SetGuildID(GuildID_t id){m_guildID = id;};
			GuildID_t				GetGuildID(){return m_guildID;};
			virtual VOID			SetSendPacket(CGChat *packet, TalkAnalyze* pAna);
		protected:
			GuildID_t	m_guildID;
		};

		//����Ƶ��
		GuildChannel*				m_GuildCh;

		//����Ƶ��(������ѡ��Ĭ��Ƶ����ʾʹ��)
		std::list<Talk::Channel *>	m_listChannel;

		//����������Ϣ�����ֵ
		INT							m_MaxSaveNum;

		//�ϴδ�����ʷ������Ϣ��λ��
		INT							m_PrevSendPos;

		//�ϴδ���GM�������Ϣ��λ��
		INT							m_PrevGMCmdPos;

		//�ϴδ���˵���link����
		STRING						m_PrevMenuLink;

		//����������Ϣ�Ƿ�����
		BOOL						m_bPaoPaoRule;

		//���춯����ѯ��
		typedef		std::map<STRING, TALK_ACT_STRUCT>		TALK_ACTMAP;
		TALK_ACTMAP					m_TalkActMap;
	
		//���������б�
		std::deque< STRING >		m_PingBiQue;	

	//�¼��ص���������ʱ���������Ϣ
	public:
		static VOID WINAPI	_SendTalkHelpMsg(const EVENT* pEvent, UINT dwOwnerData);
		static VOID WINAPI	_HandleWindowSizeChange(const EVENT* pEvent, UINT dwOwnerData);
		VOID				Initial();

		//���������GM�����ַ���
		BOOL	ParGMCharacter( STRING strTalkMsg );
	//LuaPlus������ؽӿ�
	public:
		//�õ�Ƶ������
		/*
		|	���ز���2 [String]  - Ƶ������
		*/
		INT	GetChannelNumber(LuaPlus::LuaState* state);

		//�õ�ĳ��Ƶ������
		/*
		|	���ز���1 [String]  - Ƶ������ 
		|		"near"		- �������
		|		"scene"		- ����
		|		"system"	- ϵͳ
		|		"team"		- ����
		|		"guild"		- ���
		|		"user"		- �Խ�
		|		"private"	- ˽��
		|		"self"		- �ͻ��˼𵽵��ߵȲ��ɷ��������͹�������Ϣ�� �Ǹ����ص�Ƶ��
		|
		|	���ز���2 [String]  - Ƶ������
		|
		|
		*/
		INT	GetChannel(LuaPlus::LuaState* state);

		//�������ǰ���ִ�
		INT	GetChannelHeader(LuaPlus::LuaState* state);

		//��ҷ�������ָ��
		/*
		|	�������1 [String] - ��ǰ����Ƶ��
		|	�������2 [String] - ��Ϣ��������
		|
		*/
		INT	SendChatMessage(LuaPlus::LuaState* state);

		//���ʹ���Ƶ������Ϣ C->G
		INT	CreateChannel(LuaPlus::LuaState* state);

		//����������Ҽ����Խ�Ƶ�� C->G
		/*
		|	�������1 [String] - Ҫ������������
		*/
		INT	JoinUserToChannel(LuaPlus::LuaState* state);

		//�ߵ��Խ�Ƶ���е�������� C->G
		/*
		|	�������1 [String] - Ҫ�߳����������
		*/
		INT KickUserFromChannel(LuaPlus::LuaState* state);

		//����ɾ��Ƶ������Ϣ C->G
		INT	DestoryChannel(LuaPlus::LuaState* state);

		//�����ʷ��Ϣ��Ϣ������
		/*
		|	�������1 [int]		- tabҳ��ı�ţ���0��ʼ
		|	�������2 [String]	- ��ҳ���������Ϣ�ַ���
		*/
		INT	InsertHistory(LuaPlus::LuaState* state);
		
		//����������Ϣ�������������
		/*
		|	�������1 [int] - ������ʷ�����ֵ
		*/
		INT SetMaxSaveNumber(LuaPlus::LuaState* state);

		//��������ҳ��Tab
		INT	CreateTab(LuaPlus::LuaState* state);

		//��������ҳ��Tab
		INT	ConfigTab(LuaPlus::LuaState* state);

		//��������ҳ�����
		INT	CreateTabFinish(LuaPlus::LuaState* state);

		//��������ҳ�������
		INT SaveTab(LuaPlus::LuaState* state);

		//�������ҳ�������
		INT ClearTab(LuaPlus::LuaState* state);

		//��������ҳ�����
		INT	ConfigTabFinish(LuaPlus::LuaState* state);

		//��Ƶ�����ƻ�ȡ��Ӧ��Ƶ����
		INT	GetChannelType(LuaPlus::LuaState* state);

		//��ȡ���鴰��
		INT	SelectFaceMotion(LuaPlus::LuaState* state);

		//��ȡ��ɫ����
		INT	SelectTextColor(LuaPlus::LuaState* state);

		//���浱ǰ����EditBox���Ѿ��������Ϣ
		INT SaveOldTalkMsg(LuaPlus::LuaState* state);

		//�ü��̿�ݼ�����Ĭ��Ƶ��
		INT	ChangeCurrentChannel(LuaPlus::LuaState* state);

		//���̿�ݼ����µ�����EditBox�����ֱ��
		INT ModifyChatTxt(LuaPlus::LuaState* state);

		//��ʾ����˵�
		INT	ShowContexMenu(LuaPlus::LuaState* state);

		//����˵�����
		INT	ContexMenuTalk(LuaPlus::LuaState* state);
		// �õ���ǰѡ�������
		INT GetCurrentSelectName( LuaPlus::LuaState* state );

		//����˵��ĸ�������
		INT	ContexMenuCopy( LuaPlus::LuaState* state );

		//�˵�����ʱ���ַ���ת��
		INT	ChangeClipboardTxt( LuaPlus::LuaState* state );

		//����˵���������
		INT HandleMenuAction(LuaPlus::LuaState* state);

		//�����������ִ�
		INT	GetTalkTemplateString_Lua(LuaPlus::LuaState* state);

		//��þ���������������ж���
		INT	EnumChatMood(LuaPlus::LuaState* state);

		//��ʾ�������ж�������
		INT	ShowChatMood(LuaPlus::LuaState* state);

		//�����������ж�������
		INT HideChatMood(LuaPlus::LuaState* state);

		//��ʾ���������б���
		INT ShowPingBi(LuaPlus::LuaState* state);

		//��������б������
		INT GetPingBiNum(LuaPlus::LuaState* state);

		//���������������Ϣ
		INT GetPingBiName(LuaPlus::LuaState* state);

        //ȡ���������
		INT DelPingBi(LuaPlus::LuaState* state);

		//��ȡ��ǰ�����뷨����
		INT GetCurInputLanguage(LuaPlus::LuaState* state);

		//��ȡ��ǰ�Ҽ������Ķ�������
		INT GetContexMenuObjectName(LuaPlus::LuaState* state);

		INT ClearAllHistoryFrame(LuaPlus::LuaState* state );

		////��ȡ�ѷ�����Ϣ�����е�ĳһ����Ϣ
		//INT GetSendHistoryText( LuaPlus::LuaState* state );

		////��ȡ�ѷ�����Ϣ���еĴ�С
		//INT GetSendHistorySize( LuaPlus::LuaState* state );


	//���紦����ؽӿ�
	public:
		//������Ϣ�� G->C
		INT		HandleRecvTalkPacket(GCChat* pPacket);

		//������ؽ���� G->C
		INT		HandleChannelResultPacket(GCChannelResult* pPacket);

		//������ش���� G->C
		INT		HandleChannelErrorPacket(GCChannelError* pPacket);

	//���ú����ӿ�
	public:
		//Ƶ������ת��
		static STRING				ChannelType2String(ENUM_CHAT_TYPE type);
		static ENUM_CHAT_TYPE		String2ChannelType(STRING strType);	
		VOID ChangeDefaultChannel(INT nType, const STRING& strName);

		//��ӿͻ���������Ϣ only used with CHANNEL_SELF
		// SCRIPT_SANDBOX::Global_Msg_Func

		//˽��Ƶ����ؽӿ�
		VOID						UserCreate();
		//���Ƶ����ؽӿ�
		VOID						TeamCreate(TeamID_t id);
		VOID						TeamDestory();

		//����Ƶ����ؽӿ�
		VOID						MenPaiJoin(MenPaiID_t id);
		VOID						MenPaiLeave();

		//����Ƶ����ؽӿ�
		VOID						GuildJoin(GuildID_t id);
		VOID						GuildLeave();

		//������ؿ�ݼ�����
		VOID						HandleKey_Shift_ArrowUp(INT type);		// �ϼ�ͷ���ع���һ�������������Ϣ type:0������Ϣ 1GMָ��
		VOID						HandleKey_Shift_ArrowDown(INT type);	// �¼�ͷ���ع���һ�������������Ϣ type:0������Ϣ 1GMָ��

		VOID						HandleKey_Ctrl_ArrowUp();		//Alt��+�ϼ�ͷ���ҵ�˽�ļ�������һ����ҵ�����
		VOID						HandleKey_Ctrl_ArrowDown();	//Alt��+�¼�ͷ���ҵ�˽�ļ����б�����һ����ҵ�����

		VOID						HandleKey_Alt_S();				//����
		VOID						HandleKey_Ctrl_Enter();			//����

		VOID						HandleKey_Alt_FaceMotion();		//Alt + '-'	�򿪱���ѡ�񴰿�
		VOID						HandleKey_PageUp();				//�л���һ��Ƶ��
		VOID						HandleKey_PageDown();			//�л���һ��Ƶ��

		VOID						SendGMCommand(LPCTSTR msg);		//����GM����

		BOOL						ExecuteGMCommand(LPCTSTR msg);		//ִ�пͻ���GM����

		VOID						DoAct(LPCTSTR act, LPCTSTR user);	//���ݷ��ص���Ϣ�������ﶯ��
		VOID						GenTalkActMap();					//�������ﶯ����

		STRING						GetTalkActMsg(LPCTSTR act, INT idx);	//���ﶯ�������Ӧ��������Ϣ
		STRING						GetTalkActOrder(LPCTSTR act);			//���ﶯ�������Ӧ������
		STRING						FindTalkActKey(INT idx);				//����id���������ﶯ����key
		TALK_ACT_STRUCT*			GetTalkActStructByID(INT idx);			//����id�����Ҷ�Ӧ��TALK_ACT_STRUCT��Ϊ�˽���ActionItem��׼����

		//�����ַ�������
		BOOL						HandleSpecialSelfString(TalkAnalyze* pAna);
		
		VOID						HandleFlagAdd(LPCTSTR msg);		//[ ]����
		VOID						HandleFlagDel(LPCTSTR msg);		//[ ]����
		VOID						HandleFlashAdd(LPCTSTR msg);	//[ ]����
		VOID						HandleFlashDel(LPCTSTR msg);	//[ ]����

		VOID						HandleFlagNPCAdd(LPCTSTR msg);
		VOID						HandleFlagPOSAdd(LPCTSTR msg);
		VOID						HandleFlashNPCAdd(LPCTSTR msg);
		VOID						HandleFlashPOSAdd(LPCTSTR msg);

		VOID						HandleMonsterPaoPao(LPCTSTR msg);
		VOID						HandleServerMsgTalk(LPCTSTR msg, INT channelType);

		//��������
		VOID						SetMonsterPaoPaoText(INT idServer, INT idMsg);

		//�����б�
		VOID						AddPingBi(STRING& strUser);
		VOID						DelPingBi(STRING& strUser);
		BOOL						IsInPingBi(STRING& strUser);
		VOID						ClearPingBi();

		BOOL						IsTabReceiveChannelMsg(INT tabIdx,INT channelType);
		VOID						AddChatMessageToUI(const std::vector< STRING >& vParam,INT nOp = -1);	//�����Ϣ��������ʷ��Ϣ����

		CHAR*						GetColorByChannelType( INT nType, BOOL bHeader );

	public:
		//������Ϣͷ���Ƶ������
		static ENUM_CHAT_TYPE		GetChatTypeFromMessage(STRING &strMsg);
		
		//��������ģ�����ִ�
		static STRING				GetTalkTemplateString(STRING& strMsg, STRING& strTalker);

		//�������춯������ִ�
		static STRING				GetTalkActString(STRING& strMsg, STRING& strTalker);

		//���������ִ���ʾ
		VOID						SetPaoPaoTxt(STRING& strUser, STRING& strTxt, INT strType = STRINGTYPE_NORMAL, INT extLength = 0);

		//���������Ϣ�����
		VOID						ShowTalkHelpMsg();

		//��Ļ��С�仯
		VOID						TalkDisplaySizeChanged(INT width, INT height);

		//������Ϣ����β����
		STRING						TalkFixTail(STRING& strTalk);
	public:
		//��Ϣ��ʷ��¼
		INT							AddToSendHistoryQue(HistoryMsg& msg);
		INT							AddToRecvHistoryQue(HistoryMsg& msg);
		INT							AddToGMCmdHistoryQue(HistoryMsg& msg);
		INT							AddToSendHistoryText(HistoryMsg& msg);

		bool							IsSendSucce(STRING data);		//����ʱ�����жϴ�����Ϣ�Ƿ���Է���

		//����Ƶ����ؽӿ�
		VOID						AddUserToUserChannel(STRING &strUser);

		//�������ﵱǰ�����������������������Ϣ
		VOID						SetTalkRule();
		//��ȡ����ҳ���������Ϣ
		VOID						LoadTabSetting(GCRetSetting* pPacket);

		//����������ʷ��¼
		bool							GetMsgFromCurTabHistoryQue(HistoryMsg& msg,int id);


	public:
		Talk();
		virtual ~Talk();

		static Talk s_Talk;
		static LuaPlus::LuaObject* s_pMetaTable;

		friend class TalkAnalyze;
	};

	extern const char*		NAMETYPE_TALK_NEAR;
	extern const char*		NAMETYPE_TALK_PRIVATE;
	extern const char*		NAMETYPE_TALK_FAMILY;	
	extern const char*		NAMETYPE_TALK_TEAM;	
	extern const char*		NAMETYPE_TALK_GUILD;		
	extern const char*		NAMETYPE_TALK_NATION;	
	extern const char*		NAMETYPE_TALK_SYSTEM;	
	extern const char*		NAMETYPE_TALK_WORLD;	
	extern const char*		NAMETYPE_TALK_USER;
	extern const char*		NAMETYPE_TALK_COUNTRY;

	extern const char*		GM_TALK_HEADER;
	extern const char*		CHAT_ACT_HEADER;
}