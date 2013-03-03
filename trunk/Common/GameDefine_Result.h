
#ifndef __GAMEDEFINE_RESULT_H__
#define __GAMEDEFINE_RESULT_H__


// ��������(Ϊ��������WINDOWS��HRESULT��ͬ�����Բ���HRESULT��������ֵ)
#define OR_SUCCEEDED(x)		(((OPERATE_RESULT)(x)) >= 0)
#define OR_FAILED(x)		(((OPERATE_RESULT)(x)) < 0)
enum OPERATE_RESULT
{
	OR_OK								= 0,	// �ɹ�
	OR_ERROR							= -1,	// δ֪����
	OR_DIE								= -2,	// ��������
	OR_INVALID_SKILL					= -3,	// ��Ч����
	OR_TARGET_DIE						= -4,	// Ŀ��������
	OR_LACK_MANA						= -5,	// MANA����
	OR_COOL_DOWNING						= -6,	// ��ȷʱ��δ��
	OR_NO_TARGET						= -7,	// û��Ŀ��
	OR_INVALID_TARGET					= -8,	// ��ЧĿ��
	OR_OUT_RANGE						= -9,	// ������Χ
	OR_IN_RANGE							= -10,	// ����̫��
	OR_NO_PLATFORM						= -11,	// û�в���ƽ̨
	OR_OUT_PLATFORM						= -12,	// �����ƽ̨̫Զ
	OR_NO_TOOL							= -13,	// û�в�������
	OR_STUFF_LACK						= -14,	// ȱ�ٲ���
	OR_BAG_OUT_OF_SPACE					= -15,	// ����ȱ�ٿռ�
	OR_WARNING							= -16,	// ��������Щ���󽫱��ͻ��˹��ˣ�����������֣�����������һЩ���
	OR_BUSY								= -17,	// ��������������
	OR_MISSION_HAVE						= -18,	// �Ѿ�ӵ�и�����
	OR_MISSION_LIST_FULL				= -19,	// �����¼����
	OR_MISSION_NOT_FIND					= -20,	// û�ҵ�������
	OR_EXP_LACK							= -21,	// �����Ȳ���
	OR_CHAR_DO_NOT_KNOW_THIS_SKILL		= -22,	// ��ɫ���������ּ���
	OR_NO_SCRIPT						= -23,	// Ŀ���ɫ�޽ű�
	OR_NOT_ENOUGH_HP					= -24,	// û���㹻������ֵ
	OR_NOT_ENOUGH_RAGE					= -25,	// û���㹻��ŭ��ֵ
	OR_NOT_ENOUGH_STRIKE_POINT			= -26,	// û���㹻��������
	OR_NOT_ENOUGH_ITEM					= -27,	// û���㹻�ĵ���
	OR_NOT_ENOUGH_VIGOR					= -28,	// û���㹻�Ļ���
	OR_NO_LEVEL							= -29, 	// ���ܵȼ�����
	OR_CANNOT_UPGRADE					= -30,	// ��ʯ�޷�������
	OR_FAILURE							= -31,	// ����ʧ��
	OR_GEM_CONFLICT						= -32,	// һ��װ���ϲ�����װ��ͬ���͵ı�ʯ
	OR_NEED_IN_FURY_STANCE				= -33,	// ��Ҫ�ڿ�״̬��ʹ��
	OR_INVALID_TARGET_POS				= -34,	// ��ЧĿ���
	OR_GEM_SLOT_LACK					= -35,	// ȱ����ʯ���
	OR_LIMIT_MOVE						= -36,	// �޷��ƶ�
	OR_LIMIT_USE_SKILL					= -37,	// �޷�ʹ�ü���
	OR_INACTIVE_SKILL					= -38,	// ʹ����δ����ļ���
	OR_TOO_MUCH_HP						= -39,	// HP�����޶���ֵ
	OR_NEED_A_WEAPON					= -40,	// ��Ҫһ������
	OR_NEED_HIGH_LEVEL_XINFA			= -41,	// �ķ��ȼ�����
	OR_NEED_LEARN_SKILL_FIRST			= -42,	// ��δѧ��˼���
	OR_NEED_MENPAI_FOR_LEVELUP			= -43,	// ������ʦ����һ�����ɲ��ܼ�������
	OR_U_CANNT_DO_THIS_RIGHT_NOW		= -44, 	// �������޷�������
	OR_ROBOT_TIMEOUT					= -45,  // �һ�ʱ�䳬ʱ
	OR_NEED_HAPPINESS_60				= -46,  // ��ĳ�����ֶȲ���60�����ܳ�ս����Ҫѱ��
	OR_NEED_HIGH_LEVEL					= -47,	// ��ĵȼ�����
	OR_CANNOT_GETEXP					= -48,	// ��ĳ������޷��õ�����
	OR_NEED_LIFE						= -49,  // ��ĳ�������Ϊ0���޷����ٻ�����Ϊ����������
	OR_EXP_FULL							= -50,  // ���ľ����Ѿ���������
	OR_TOO_MANY_TRAPS					= -51, 	// �޷����ø���Ĵ�������
	OR_PET_PLACARD_ISSUE_FAILED			= -52,	// ����ʧ��
	OR_PET_PLACARD_NOT_APPOINT_PET		= -53,	// δָ������
	OR_PET_PLACARD_ONLY_CAN_ISSUE_ONE	= -54,	// �Բ���ͬʱֻ�ܷ���һֻ��
	OR_PET_PLACARD_NEED_PET_TYPE		= -55,	// �Բ�����ĳ�����Ǳ���Ŷ
	OR_PET_PLACARD_NEED_PET_LEVEL		= -56,	// �Բ�����ĳ輶�𻹲���Ŷ
	OR_PET_PLACARD_NEED_PET_HAPPINESS	= -57,	// �Բ�����ĳ����ֵ������
	OR_PET_PLACARD_NEED_PET_LIFE		= -58,	// �Բ�����ĳ���������
	OR_NEED_IN_STEALTH_MODE				= -59,	// ��Ҫ������״̬��ʹ��
	OR_NOT_ENOUGH_ENERGY				= -60,	// �Բ�����ľ�������
	OR_CAN_NOT_MOVE_STALL_OPEN			= -61,	// �޷��ڰ�̯���ƶ�
	OR_NEED_IN_SHIELD_MODE				= -62,	// ��Ҫ�ڻ��屣����
	OR_PET_DIE							= -63,	// ��ĳ����Ѿ�����
	OR_PET_HADRECALL_NEEDHAPPINESS		= -64,	// ��ĳ�����ֶȲ���60���ѱ��ջ�
	OR_PET_HADRECALL_NEEDLIFE			= -65,	// ��ĳ�������Ϊ0���ѱ��ջ�
	OR_GEM_NOT_FIT_EQUIP				= -66,	// ���ֱ�ʯ������Ƕ������װ����
	OR_CANNOT_ASK_PETDETIAL				= -67,	// ���޷��쿴�ó������Ϣ
	OR_VARIANCEPET_CANNOT_RETURNTOCHILD = -68,  // ����費�ܽ��л�ͯ
	OR_COMBAT_CANNOT_RETURNTOCHILD		= -69,  // ��ս�費�ܽ��л�ͯ
	OR_IMPASSABLE_ZONE					= -70,	// ����������
	OR_NEED_SETMINORPASSWORD			= -71,	// ��Ҫ���� ��������
	OR_NEED_UNLOCKMINORPASSWORD			= -72,	// ��Ҫ���� ��������
	OR_PETINEXCHANGE_CANNOT_GOFIGHT		= -73,	// ���ڽ��׵ĳ����޷���ս
	OR_HASSPOUSE_CANNOT_RETURNTOCHILD	= -74,  // �Ѿ�����ż�ĳ��ﲻ�ܻ�ͯ
	OR_CAN_NOT_FIND_SPECIFIC_ITEM		= -75,	// ȱ��ָ����Ʒ
	OR_CALLUP_HORSE_CAN_NOT_FREE		= -76,	// ��ս�����ܷ���
	OR_HORSE_HAPPINESS_ENOUGH			= -77,	//�����˺ܿ���,����Ҫѱ��,����ʹ��ʧ��
	OR_NOT_ENOUGH_MONEY					= -78,	//�Բ��������ϵĽ�Ǯ����
	OR_HORSE_CALLUP_CAN_NOT_CHECKUP		= -79,	//��ս�����ܼ���
	OR_HORSE_CHECKUP_NEED_10LEVEL		= -80,	//10�����ϲ��ܼ���
	OR_HORSE_NO_ENOUGH_SKILL_SOLT		= -81,	//�Բ���,������û���㹻����ͨ��,���ȶ���ʹ�ü��ܲ۵��߰�
	OR_HORSE_HAVE_TOO_MUCH_SKILL		= -82,	//�Բ���,������û�п���ͨ�鵽����,���ȶ���ʹ�ü����������߰�
	////�������
	OR_ISSUEHORSE_MUSTBE_ZHANMA			= -83,	// ��������˱�����ս��
	OR_ISSUEHORSE_MUSTNOT_HAVEMATE		= -84,  // ��������˲������ѻ��
	OR_ISSUEHORSE_HAVE_ONLYONE			= -85,  // ���ֻ�ܷ���һֻ���,
	OR_ISSUEHORSE_NOTENOUGH_LEVEL		= -86,  // ��������˵ȼ�����,
	OR_ISSUEHORSE_NOTENOUGH_HAPPINESS	= -87,  // ��������˿��ֶȲ���
	OR_ISSUEHORSE_NOTENOUGH_MONEY		= -88,  // �����������Ҫ����Ľ�Ǯ
	OR_ISSUEHORSE_SUCC					= -89,  // ��������˳ɹ�,
	OR_ISSUEHORSE_FAIL					= -90,  // ���������ʧ��,
	OR_CANCELHORSEFINDMATE_SUCC			= -91,  // ȡ��������ѳɹ�
	OR_CANCELHORSEFINDMATE_FAIL			= -92,  // ȡ���������ʧ��
	OR_THIS_PLAYER_IS_OFFLINE			= -93,  // ��������ڲ�����
	OR_DOUBLEMOODACT_REFUSED			= -94,	// ˫����ж������ܾ� [6/5/2008 wuwenbin]
	OR_NOT_ENOUGH_MP					= -95,	// û���㹻������ֵ
	OR_NOT_ENOUGH_PNEUMA				= -96,	// û���㹻��Ԫ��ֵ
	OR_HP_IS_FULL						= -97,	//HP ����
	OR_MP_IS_FULL						= -98,	//MP ����
	OR_PETZHAOHUAN_NO_TIMES				= -99,	//�����ٻ���������
	OR_ZUOJIZHAOHUAN_NO_TIMES			= -100,	//�����ٻ���������

	OR_BUS_HASPASSENGER							= -101,	// �������ڴ��״̬
	OR_BUS_NOTINBUS								= -102,	// �㲢δ���ڴ��״̬
	OR_BUS_RUNNING								= -103,	// Ŀ���Ѿ�Զ��
	OR_BUS_HASMOUNT								= -104,	// �����������״̬
	OR_BUS_HASPET								= -105,	// ������������ڳ�ս״̬
	OR_BUS_CANNOT_TEAM_FOLLOW					= -106,	// ����������Ӹ���״̬
	OR_BUS_CANNOT_DRIDE							= -107,	// ��������˫�����״̬
	OR_BUS_CANNOT_CHANGE_MODEL					= -108,	// �������ڱ���״̬
	OR_BUS_PASSENGERFULL						= -109,	// Ŀ�����޿ռ�
	//////////////////////////////////////////////////////////////
	// �������˸��� GameDefine_Result.cpp ���� s_aResultText ����

};
typedef OPERATE_RESULT ORESULT;


const char *GetOResultText( OPERATE_RESULT code );

#endif
