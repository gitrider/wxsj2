/********************************************************************************
 *	�ļ�����	GameDefine_Guild.h
 *	ȫ·����	d:\Prj\Common\GameDefine_Guild.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 12 �� 5 ��	11:06
 *
 *	����˵����	����һЩ״̬�����Լ���������ֵ
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __GAMEDEFINE_GUILD_H__
#define __GAMEDEFINE_GUILD_H__

#define MAX_NUM_PER_POSITION		    20
#define GUILD_CREATE_LEVEL_LIMIT        20			//�����������ȼ�
#define GUILD_CREATE_MONEY_LIMIT        500000		//������������Ǯ
#define GUILD_CREATE_ITEM_INDEX         11990001    //ItemIndex
#define GUILD_REJOIN_TIME_LIMIT         24*3600     //24Сʱ
#define GUILD_FAMILY_USER_MAX           20          //�ƽ������������
#define GUILD_NORMAL_FAMILY_SIZE        12          //��ͨ������������
#define MAX_GUILD_USER_POINT            99999       //��������߹��׶�
#define MAX_GUILD_TOTAL_POINT           9999999     //����ܹ��׶�


enum GUILD_STATUS
{
	GUILD_STATUS_INVALID			= 0,
	GUILD_STATUS_NORMAL,
};
//���ְλ����
enum GUILD_POSITION
{
    GUILD_POSITION_INVALID			= 0,

    GUILD_POSITION_TRAINEE,					// Ԥ�����ڣ�����׼
    GUILD_POSITION_MEMBER,					// ��ͨ���ڣ�δ����

    GUILD_POSITION_ELITE_MEMBER,			// ��Ӣ���ڣ��ѷ������
    GUILD_POSITION_FAMILY_BOSS,             // ���峤

    GUILD_POSITION_ASS_CHIEFTAIN,			// ������
    GUILD_POSITION_CHIEFTAIN,				// ����

    GUILD_POSITION_SIZE,					// ְλ������
};
//���Ȩ��
enum GUILD_AUTHORITY
{
	GUILD_AUTHORITY_INVALID			= 0,

	GUILD_AUTHORITY_ASSIGN			= 0x1,		// ְ�����Ȩ
	GUILD_AUTHORITY_AUTHORIZE		= 0x2,		// Ȩ�޵���Ȩ
	GUILD_AUTHORITY_RECRUIT			= 0x4,		// ���հ���Ȩ
	GUILD_AUTHORITY_EXPEL			= 0x8,		// ��������Ȩ
	GUILD_AUTHORITY_DEMISE			= 0x10,		// ����Ȩ
	GUILD_AUTHORITY_WITHDRAW		= 0x20,		// ֧ȡ����Ȩ
	GUILD_AUTHORITY_DEPOSIT			= 0x40,		// ������Ȩ
	GUILD_AUTHORITY_LEAVE			= 0x80,		// �뿪���Ȩ�����Ȩ���е���ࣩ
	GUILD_AUTHORITY_DIMISS			= 0x100,	// ��ɢ���Ȩ
	GUILD_AUTHORITY_ADDFAMILYUSER   = 0x200,	// ��Ӽ����ԱȨ��
    GUILD_AUTHORITY_DELFAMILYUSER   = 0x400,    // ɾ�������ԱȨ��


	GUILD_AUTHORITY_NUMBER			= 8,	// Ȩ������

	// ���ڵ�ͨ��Ȩ��
	GUILD_AUTHORITY_MEMBER			= GUILD_AUTHORITY_DEPOSIT | GUILD_AUTHORITY_LEAVE,
	GUILD_AUTHORITY_ASSCHIEFTAIN	= GUILD_AUTHORITY_ASSIGN | GUILD_AUTHORITY_RECRUIT | GUILD_AUTHORITY_EXPEL | GUILD_AUTHORITY_LEAVE,
	GUILD_AUTHORITY_CHIEFTAIN		= 0x17F,	// û���뿪����Ȩ��
};
//��᷵����Ϣ
enum GUILD_RETURN_TYPE
{
	GUILD_RETURN_INVALID			= -1,

	// ��������
	GUILD_RETURN_CREATE,					// �����ɹ�
	GUILD_RETURN_RESPONSE,					// ��Ӧ�ɹ�

	// ���벿��
	GUILD_RETURN_JOIN,						// ���������б�ȴ���׼
    GUILD_RETURN_INVIT,                     // ���������ظ�

	// Ȩ�޲���
	GUILD_RETURN_PROMOTE,					// 0001������ְ����ְ
	GUILD_RETURN_DEMOTE,					// 0001������ְ�񣬽�ְ
	GUILD_RETURN_AUTHORIZE,					// 0002������Ȩ�ޣ���Ȩ
	GUILD_RETURN_DEPRIVE_AUTHORITY,			// 0002������Ȩ�ޣ����Ȩ��
	GUILD_RETURN_RECRUIT,					// 0003�����հ���
	GUILD_RETURN_EXPEL,						// 0004����������
	GUILD_RETURN_DEMISE,					// 0005������
	GUILD_RETURN_WITHDRAW,					// 0006��֧ȡ���
	GUILD_RETURN_DEPOSIT,					// 0007��������
	GUILD_RETURN_LEAVE,						// 0008���뿪���
	GUILD_RETURN_REJECT,					// 0009���ܾ�����
	
	GUILD_RETURN_FOUND,						// ��ʽ�����������ﵽ��
	GUILD_RETURN_DISMISS,					// ��ɢ���
	GUILD_RETURN_CHANGEDESC,				// �޸İ����ּ

	GUILD_RETURN_NAME,						// ˢ�°����

	//�������Ժ�������
	GUILD_RETURN_CREATE_FAILED,				// �������ʧ��
    GUILD_RETURN_CREATE_FAMILY,             // ��������
    GUILD_RETURN_ADD_FAMILY_USER,           // ��Ӽ����Ա
    GUILD_RETURN_REMOVE_FAMILY_USER,        // ɾ�������Ա
    GUILD_RETURN_MODIFY_DESC,               // �޸İ����ּ
    GUILD_RETURN_FAMILY_DISMISS,            // ��ɢ����
    GUILD_RETURN_BEADD_FAMILY,              // ֪ͨ�����Ϊ�����Ա    
    GUILD_RETURN_LEVEL_UP,                  // �������    
    GUILD_RETURN_ALREADY_IN,                // �Ѿ����������
    GUILD_RETURN_ERASE_ITEM_FAIL,           // �۳�������ʧ��

	GUILD_RETURN_SIZE,
};
//��������Ϣ
enum GUILD_ERROR_TYPE
{
	// ��������
	GUILD_ERROR_NOTHING				= 0,	// ûʲô���󡭡�
	GUILD_ERROR_WANTING				= -1,	// �����ϴ���������ֻ���������������Ĵ��������ɽű�����
	GUILD_ERROR_INVALID_NAME		= -2,	// ���ַǷ�
	GUILD_ERROR_DUPLICATED_NAME		= -3,	// ����
	GUILD_ERROR_GUILD_FULL			= -4,	// ���ܴ���������

	// ���벿��
	GUILD_ERROR_MEMBER_FULL			= -5,	// �����Ա��
	GUILD_ERROR_PROPOSER_FULL		= -6,	// ����������
	GUILD_ERROR_IN_GUILD			= -7,	// ����Ѽ�����
	GUILD_ERROR_NOT_EXIST			= -8,	// ��᲻����

	// Ȩ�޲���
	GUILD_ERROR_UNAUTHORIZED		= -9,	// û��Ȩ��
	GUILD_ERROR_NO_VACANCY			= -10,	// 0001������ְ��û��ְλ��ȱ
	GUILD_ERROR_NO_QUALIFICATION	= -11,	// 0002������Ȩ�ޣ���������Ա��ְλ�������ʸ�
	GUILD_ERROR_CASHBOX_FULL		= -12,	// 0007����������������

	GUILD_ERROR_ALREADY_MEMBER		= -13,	// �����Ѿ��ǰ���Ա
	GUILD_ERROR_MEMBER_NOT_EXIST	= -14,	// ���˲��ڰ����

	//����
	GUILD_ERROR_NO_ASS_CHIEF		= -15,	// û�и�����
	GUILD_ERROR_GUILD_ALREADY_EXIST = -16,	//	����Ѵ���

	//���ְλ������������������
	GUILD_ERROR_POS_FULL			= -17,	//���ְλ������������������

	//�������֣�ID�Ժ�������
	GUILD_ERROR_LEVEL_TOO_LOW		= -18,	// �ȼ�̫�Ͳ��ܴ������
	GUILD_ERROR_LEVEL_TO_JOIN		= -19,	// �ȼ�̫�Ͳ��ܼ�����


    GUILD_ERROR_PASSWORD		    = -20,	// �������
    GUILD_ERROR_WAITTING		    = -21,	// �����ȷ��
    GUILD_ERROR_FAMILY_DIMISS       = -22,	// ��ɢ����ʧ��
	// ��ɢ����

	GUILD_ERROR						= -64,	// ��������
};
//��������
enum GUILD_PACKET_TYPE
{
	GUILD_PACKET_INVALID			= -1,
	GUILD_PACKET_CG_ASKLIST,				// ѯ�ʰ���б�(UCHAR)
	GUILD_PACKET_CG_CREATE,					// �������
 	GUILD_PACKET_CG_JOIN,					// ������
	GUILD_PACKET_CG_ASKINFO,				// ѯ�ʰ����Ϣ
	GUILD_PACKET_CG_APPOINT,				// �������
	GUILD_PACKET_CG_ADJUSTAUTHORITY,		// �������Ȩ��
	GUILD_PACKET_CG_RECRUIT,				// �������
	GUILD_PACKET_CG_EXPEL,					// �������
	GUILD_PACKET_CG_WITHDRAW,				// ��ȡ����
	GUILD_PACKET_CG_DEPOSIT,				// �������
	GUILD_PACKET_CG_LEAVE,					// �뿪���
	GUILD_PACKET_CG_DISMISS,				// ��ɢ���
	GUILD_PACKET_CG_DEMISE,					// ��ɢ���
	GUILD_PACKET_CG_CHANGEDESC,				// ���İ����ּ
    GUILD_PACKET_CG_INVITE,                 // ���������
    GUILD_PACKET_CG_INVITE_CONFIRM,         // ����������ȷ��
    GUILD_PACKET_CG_CREATE_FAMILY,          // ���󴴽�����
    GUILD_PACKET_CG_ADD_FAMILY_USER,        // ��Ӽ����Ա
    GUILD_PACKET_CG_REMOVE_FAMILY_USER,     // ɾ�������Ա
    GUILD_PACKET_CG_MODIFY_DESC,            // �޸İ����ּ
    GUILD_PACKET_CG_MODIFY_TITLE_NAME,      // �޸İ��ƺ�
    GUILD_PACKET_CG_FAMILY_DISMISS,         // ��ɢ����
    GUILD_PACKET_CG_LEVELUP,                // �������

	GUILD_PACKET_CG_GW_SEPARATOR,			// �ָ���

	GUILD_PACKET_GW_ASKLIST,				// ѯ�ʰ���б�(GUID_t, UCHAR)
	GUILD_PACKET_GW_CREATE,					// �������
	GUILD_PACKET_GW_JOIN,					// ������
	GUILD_PACKET_GW_ASKINFO,				// ѯ�ʰ����Ϣ
	GUILD_PACKET_GW_APPOINT,				// �������
	GUILD_PACKET_GW_ADJUSTAUTHORITY,		// �������Ȩ��
	GUILD_PACKET_GW_RECRUIT,				// �������
	GUILD_PACKET_GW_EXPEL,					// �������
	GUILD_PACKET_GW_WITHDRAW,				// ��ȡ����
	GUILD_PACKET_GW_DEPOSIT,				// �������
	GUILD_PACKET_GW_LEAVE,					// �뿪���
	GUILD_PACKET_GW_DISMISS,				// ��ɢ���
	GUILD_PACKET_GW_DEMISE,					// ����
	GUILD_PACKET_GW_CHANGEDESC,				// ���İ����ּ
    GUILD_PACKET_GW_INVITE,                 // ���������
    GUILD_PACKET_GW_INVITE_CONFIRM,         // ����������ȷ��
    GUILD_PACKET_GW_CREATE_FAMILY,          // ���󴴽�����
    GUILD_PACKET_GW_ADD_FAMILY_USER,        // ��Ӽ����Ա
    GUILD_PACKET_GW_REMOVE_FAMILY_USER,     // ɾ�������Ա
    GUILD_PACKET_GW_MODIFY_DESC,            // �޸İ����ּ
    GUILD_PACKET_GW_MODIFY_TITLE_NAME,      // �޸İ��ƺ�
    GUILD_PACKET_GW_FAMILY_DISMISS,         // ��ɢ����
    GUILD_PACKET_GW_LEVELUP,                // �������
    GUILD_PACKET_GW_ASKMEMBERLIST,			// ����Ա��Ϣ�б�
    GUILD_PACKET_GW_ADD_GP,                 // ���Ӱ���ܰﹱ
    GUILD_PACKET_GW_SUB_GP,                 // ���ٰ���ܰ��
    GUILD_PACKET_GW_LEVEL_INFO,             // ���ȼ���Ϣ
	
	GUILD_PACKET_GW_WG_SEPARATOR,			// �ָ���

	GUILD_PACKET_WG_LIST,					// ����б�
	GUILD_PACKET_WG_MEMBER_LIST,			// �����б�
	GUILD_PACKET_WG_GUILD_INFO,				// �����Ϣ
	GUILD_PACKET_WG_APPOINT_INFO,			// ����������Ϣ
	GUILD_PACKET_WG_SELF_GUILD_INFO,		// ���˰�����Ϣ
	GUILD_PACKET_WG_CONFIRM_GUILD_INVITE,	// ת�����������Ϣ
    GUILD_PACKET_WG_MODIFY_TITLE_NAME,		// ֪ͨ�ͻ����޸İ��ƺ�
    GUILD_PACKET_WG_ADD_GP,                 // ���Ӱ���ܰﹱ
    GUILD_PACKET_WG_SUB_GP,                 // ���ٰ���ܰ��


	GUILD_PACKET_WG_GC_SEPARATOR,			// �ָ���

	GUILD_PACKET_GC_LIST,					// ����б�
	GUILD_PACKET_GC_MEMBER_LIST,			// �����б�
	GUILD_PACKET_GC_GUILD_INFO,				// �����Ϣ
	GUILD_PACKET_GC_APPOINT_INFO,			// ����������Ϣ
	GUILD_PACKET_GC_SELF_GUILD_INFO,		// ���˰�����Ϣ
	GUILD_PACKET_GC_CONFIRM_GUILD_INVITE,	// ȷ�ϰ������
    GUILD_PACKET_GC_MODIFY_TITLE_NAME,      // ֪ͨ�ͻ����޸İ��ƺ�
    GUILD_PACKET_GC_FAMILY_DISMISS,         // ��ɢ����
};


#endif // __GAMEDEFINE_GUILD_H__
