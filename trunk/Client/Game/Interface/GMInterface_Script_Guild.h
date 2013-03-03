
/** 
 *
 *	���ܣ�	�����ؽӿ�
 *	
 *	�޸ļ�¼��
 *			080303	���� ��������GuildInvite
 *			080304	���� AdjustMemberTitle �޸ĳ�Ա�ĳƺ�
 *			080305	DestoryFamily ɾ������
 *					ShowInputDialog ��ʾ�����ص�����Ի���
 *					AgreeInvite ͬ������
 *			080307	CharacterCheck �ַ����
 *					CreateFamily ��������
 *					CreateFamilyConfirm ��������ȷ��
 *					AdjustFamilyMember ��ӻ�ɾ�������Ա
 *					TransferMemberToFamily ����Ա�Ƶ�������
 *					KickMemberFormFamily �Ӽ�����ɾ����Ա
 *					InviteMemberToFamily �����Ա������
 *					FamilyInviteConfirm ���������ȷ��
 *					GetFamilyMembersInfo ��ü���ĳ�Ա��ϸ��Ϣ
 *			080310	SetMemberListSelect ����ÿ�γ�Ա�б��е�ѡ��
 *			080311	GetFamilyMemberIndex 
 *					SetLuaFamilyNum
 *					GetLuaFamilyNum
 *			
 *			
 */

#include "Type.h"


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
};

namespace SCRIPT_SANDBOX
{
	class Guild
	{

	//LuaPlus������ؽӿ�
	public:
		//�������
		INT	CreateGuild(LuaPlus::LuaState* state);

		//�������ȷ��
		INT CreateGuildConfirm(LuaPlus::LuaState* state);

		//��World��������ϸ��Ϣ
		INT AskGuildDetailInfo(LuaPlus::LuaState* state);

		//��World�������Ա��Ϣ
		INT AskGuildMembersInfo(LuaPlus::LuaState* state);

		//��World������ְλ��Ϣ
		INT AskGuildAppointPosInfo(LuaPlus::LuaState* state);

		//������а�������
		INT	GetGuildNum(LuaPlus::LuaState* state);

		//�������������Ϣ
		INT GetGuildInfo(LuaPlus::LuaState* state);

		//������
		INT JoinGuild(LuaPlus::LuaState* state);

		//�������
		INT GuildInvite(LuaPlus::LuaState* state);
		//��ʾ�������Ĵ���
		INT ShowInputDialog(LuaPlus::LuaState* state);

		//�˳����
		INT	QuitGuild(LuaPlus::LuaState* state);

		//�߳����,�ܾ�����
		INT KickGuild(LuaPlus::LuaState* state);

		//���ɻ�Ա
		INT RecruitGuild(LuaPlus::LuaState* state);

		//����Լ��İ�����Ϣ
		INT GetMembersNum(LuaPlus::LuaState* state);

		//Lua��ʾ��list�ؼ���λ��
		INT GetShowMembersIdx(LuaPlus::LuaState* state);
		INT GetShowTraineesIdx(LuaPlus::LuaState* state);

		//����Լ��İ�����ϸ��Ϣ
		INT	GetMembersInfo(LuaPlus::LuaState* state);

		//����Լ��İ�����Ϣ
		INT GetMyGuildInfo(LuaPlus::LuaState* state);

		//����Լ����ɵ���ϸ��Ϣ
		INT GetMyGuildDetailInfo(LuaPlus::LuaState* state);

		//����Լ��İ���Ȩ��
		INT GetMyGuildPower(LuaPlus::LuaState* state);

        //�޸����а���ְλ
		INT	AdjustMemberAuth(LuaPlus::LuaState* state);

		//�����λ������
		INT ChangeGuildLeader(LuaPlus::LuaState* state);

		//���ɾ��
		INT	DestoryGuild(LuaPlus::LuaState* state);

		//�޸İ����Ϣ����ּ��
		INT FixGuildInfo(LuaPlus::LuaState* state);

		//�������
		INT DemisGuild(LuaPlus::LuaState* state);

		//׼������Ա����
		INT PrepareMembersInfomation(LuaPlus::LuaState* state);

		//�޸İ���Ա�ĳƺ�
		INT AdjustMemberTitle(LuaPlus::LuaState* state);

		//ͬ��Ӱ������
		INT AgreeInvite(LuaPlus::LuaState* state);


		//����ÿ�γ�Ա�б��е�ѡ��
		INT SetMemberListSelect(LuaPlus::LuaState* state);

		//�����صĲ˵�
		INT OpenMenu(LuaPlus::LuaState* state);


		//================
		// ����

		//��������
		INT	CreateFamily(LuaPlus::LuaState* state);

		//��������ȷ��
		INT CreateFamilyConfirm(LuaPlus::LuaState* state);

		//����ɾ��
		INT	DestoryFamilyConfirm(LuaPlus::LuaState* state);
		INT	DestoryFamily(LuaPlus::LuaState* state);

		//��ü���ĳ�Ա��ϸ��Ϣ
		INT	GetFamilyMembersInfo(LuaPlus::LuaState* state);

		//�Լ����Ա���в�������ӻ�ɾ����
		INT AdjustFamilyMember(LuaPlus::LuaState* state);

		//�ƶ���Ա������
		INT TransferMemberToFamily(LuaPlus::LuaState* state);

		//�Ӽ������߳�
		INT KickMemberFormFamily(LuaPlus::LuaState* state);

		//��������
		INT InviteMemberToFamily(LuaPlus::LuaState* state);

		//��������ȷ��
		INT FamilyInviteConfirm(LuaPlus::LuaState* state);

		//��ȡĳ�����Ա����
		INT GetFamilyMemberNum(LuaPlus::LuaState* state);
		//��ȡ�����Ա�ڳ�Ա�б��е�����
		INT GetFamilyMemberIndex(LuaPlus::LuaState* state);

		//����ű��е�ǰ��ʾ�ļ�������
		INT SetLuaFamilyNum(LuaPlus::LuaState* state);
		INT GetLuaFamilyNum(LuaPlus::LuaState* state);

		// �������ݣ����������ƣ�λ�ã�
		INT SetFamilyWindowData(LuaPlus::LuaState* state);
		INT GetFamilyWindowData(LuaPlus::LuaState* state);
		// ɾ�������ָ������
		INT DelFamilyWindowData(LuaPlus::LuaState* state);


	//���ú���
	public:
		//��ʾ��ʾ��Ϣ
		// msgType		��Ϣ�ţ��������ֵ����ȡ��Ӧ������
		// bTalk		��Ҫ��ʾ�����촰��
		// bTip			��Ҫ��ʾ����Ļ�м����ʾ
		VOID ShowMsg(INT msgType, BOOL bTalk = FALSE, BOOL bTip = TRUE);

		struct Name_Idx
		{
			STRING	m_MemberName;		//��ӦDataPool��GuildMemberInfo_t�ṹ���m_szName
			INT		m_MemberIdx;		//��ӦDataPool��GuildMemberInfo_t������ֵ
			INT		m_Position;			//�ڰ��е�ְλ

			Name_Idx()
			{
				m_MemberName.erase();
				m_MemberIdx = -1;
				m_Position = GUILD_POSITION_INVALID;
			}

			VOID	CleanUp()
			{
				m_MemberName.erase();
				m_MemberIdx = -1;
				m_Position = GUILD_POSITION_INVALID;
			}
		};

		struct ShowColor
		{
			STRING	m_OnlineLeaderColor;		//�����쵼����ʾ��ɫ
			STRING	m_LeaveLeaderColor;			//�����쵼����ʾ��ɫ
			STRING	m_OnlineMemberColor;		//���߰�����ʾ��ɫ
			STRING	m_LeaveMemberColor;			//���߰�����ʾ��ɫ
			STRING	m_OnlineTraineeColor;		//������������ʾ��ɫ
			STRING	m_LeaveTraineeColor;		//������������ʾ��ɫ

			ShowColor()
			{
				m_OnlineLeaderColor = "#B";			//��ɫ
				m_OnlineMemberColor = "#W";			//��ɫ
				m_OnlineTraineeColor = "#W";

				m_LeaveLeaderColor = "#c808080";	//��ɫ
				m_LeaveMemberColor = "#c808080";
				m_LeaveTraineeColor = "#c808080";
			}
		};


		// ����ַ��Ϸ���
		BOOL CharacterCheck( const STRING& str );

	private:
		//������ʱ��ת��
		VOID ConvertServerTime(INT dTime, STRING& strTime);

		//���°����Ӧ����ʾ��Ϣ
		VOID PerpareMembersBeforeShow();

		//ѡ����ʾ��ɫ
		STRING GetShowColor_For_Lua(INT idx);

		std::vector<STRING>		m_MsgArray;			//���ShowMsg����ʹ��
		INT						m_iMemberBak;		//�ı�ְλʱ�ı�����Ҫ�޸��ĸ���ҵ�ְλ��Ϣ

		// �����Ա�б��е�ѡ��
		INT						m_iMemberSelect;


		//��Ա�б�
		typedef		std::vector<Name_Idx>	MEMBER_TABLE;
		typedef		std::vector<INT>		MEMBER_INDEX_TABLE;

		MEMBER_INDEX_TABLE						m_AllMembers;			//���г�Ա
		MEMBER_INDEX_TABLE						m_AllTrainees;			//����Ԥ����Ա
		ShowColor								m_LuaShowColors;		//Lua����ʾ����ɫ

		typedef		std::map< INT, MEMBER_INDEX_TABLE >		FAMILY_MEMBER_INDEX_TABLE;
		FAMILY_MEMBER_INDEX_TABLE				m_FamilyMembers;		// ����м����Ա
		INT										m_iFamilyNum;			// ���������
		INT										m_iCurFamilyNum;		// ����ű��е�ǰ���еļ�������

	public:

		Guild();
		virtual ~Guild();

		static Guild s_Guild;
		static LuaPlus::LuaObject* s_pMetaTable;
	};


	class SimpleCmpFunc
	{
	public:
		static BOOL less_strcmp(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2);
		static BOOL less_strcoll(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2);

		static BOOL less_position(const Guild::Name_Idx&s1, const Guild::Name_Idx& s2);
	};
};