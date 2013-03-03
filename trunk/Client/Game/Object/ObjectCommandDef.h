
/** ObjectCommandDef.h
 */

#pragma once



// ��Ч��
#define OC_NONE						(0xFFFF)	// ��ЧID

// ����
// F0	:	��ɫ��ǰX����
// F1	:	��ɫ��ǰZ����
#define OC_IDLE						(0)

// ��Ծ
#define OC_JUMP						(1)
//
//// Ѱ·�ƶ�
//// F0	:	��ɫ��ǰX����
//// F1	:	��ɫ��ǰZ����
//// P2	:	CPathָ��
//// D3	:	·��ID
//#define OC_MOVE_USE_PATH			(2)
//
//// �ƶ�
//// F0	:	��ɫ��ǰX����
//// F1	:	��ɫ��ǰZ����
//// D2	:	·������Ŀ
//// P3	:	·�����б�ָ��
//// D4	:	·��ID
//#define OC_MOVE_USE_NODE_LIST		(3)

// ����
// B0	:	�Ƿ񲥷���������
#define OC_DEATH					(4)

// ˲��
// F0	:	��ɫ��ǰX����
// F1	:	��ɫ��ǰZ����
#define OC_TELEPORT					(5)
//
//// ����(����)
//// D0	:	����ID
//// D1	:	Ŀ��ID
//// F2	:	Ŀ���X����
//// F3	:	Ŀ���Z����
//// F4	:	����
//// F5	:	����ʱ��
//// F6	:	ʹ����X����
//// F7	:	ʹ����Z����
//#define OC_SKILL_GATHER				(11)

// ����(����)
// F0	:	���ٵ�ʱ��
#define OC_SKILL_GATHER_MODIFY		(12)
//
//// ����(����)
//// D0	:	����ID
//// D1	:	Ŀ��ID
//// F2	:	Ŀ���X����
//// F3	:	Ŀ���Z����
//// F4	:	����
//// F5	:	����ʱ��
//// F6	:	ʹ����X����
//// F7	:	ʹ����Z����
//#define OC_SKILL_LEAD				(13)

// ����(����)
// F0	:	���ٵ�ʱ��
#define OC_SKILL_LEAD_MODIFY		(14)
//
//// ����(����)
//// D0	:	����ID
//// D1	:	Ŀ��ID
//// F2	:	Ŀ���X����
//// F3	:	Ŀ���Z����
//// F4	:	����
//// F5	:	����ʱ��
//// F6	:	ʹ����X����
//// F7	:	ʹ����Z����
//// N8	:	�߼�����
//#define OC_SKILL_SEND				(15)

// ����Ԥ��
//#define OC_PREP_SKILL_SEND			(15)


// ����(�ӵ�)
// D0	:	����ID
// D1	:	Ŀ��ID
// F2	:	Ŀ���X����
// F3	:	Ŀ���Z����
// F4	:	����
#define OC_SKILL_CREATE_BULLET		(16)

// �߼�ִ��ʱ�������¼�
// P0	:	_LOGIC_EVENTָ��
#define OC_LOGIC_EVENT				(18)

// ����ˢ��
// D0	:	��������Щ����
// P1	:	���������Ե�ֵ�б�
//#define OC_UPDATE_ATTRIB			(20)

// ����װ��
// D0	:	��������Щװ��
// P1	:	������װ����ID�б�
#define OC_UPDATE_EQUIPMENT			(21)

// ���Ը���Ч��
// D0	:	����Ч��ID
// B1	:	�ӻ��(1=�ӣ�0=��)
// N2	:	BUFF�Ĵ�����ID
#define OC_UPDATE_IMPACT			(22)

// ���Ը���Ч��(������ִ�����Ч)
// D0	:	����Ч��ID
#define OC_UPDATE_IMPACT_EX			(23)
//
//// ����ĳ������ܵĲ�����
//// D0	:   ����ID
//#define OC_LIFE_ABILITE_ACTION		(24)

// ���¶�����
// B0	:	�Ƿ��ж���ı��
// B1	:	�Ƿ��Ƕӳ��ı��
// B2	:	�����Ƿ���Ա�ı��
#define OC_UPDATE_TEAM_FLAG			(25)

// ��������б�
// D0	:	�б�����
// D1..7:	�б��д�ŵ� GUID
#define OC_TEAM_FOLLOW_MEMBER		(26)

// �����Լ��Ķ��������
// B0	:	�Ƿ����ڸ���ӳ�
#define OC_UPDATE_TEAM_FOLLOW_FLAG	(27)
//
//// �ж��ƶ�
//// F0	:	����X
//// F1	:	����Z
//// D3	:	·����ID
//#define OC_ARRIVE_MOVE				(28)

// ����
#define OC_LEVEL_UP					(29)

// ����OBJ����
// N0		:		�߼�����
// N1		:		Ŀ����Ŀ
// P2		:		Ŀ�������б�
#define OC_SPECIAL_OBJ_TRIGGER		(30)

// ����OBJ����
#define OC_SPECIAL_OBJ_DIE			(31)


// ��BUS�ϼ���һ����ɫ
// NO		:		��λ����
// N1		:		�˿�ID
#define OC_BUS_ADD_PASSENGER		(32)

// �Ƴ�BUS��ĳ����ɫ
// N0		:		�˿�ID
#define OC_BUS_REMOVE_PASSENGER		(33)

// �Ƴ�BUS�����н�ɫ
#define OC_BUS_REMOVE_ALL_PASSENGER	(34)

// BUS�ƶ�
// FO		:		Ŀ���x����
// F1		:		Ŀ���y����
// F2		:		Ŀ���z����
#define OC_BUS_MOVE					(35)

// BUS�ƶ�ֹͣ
// FO		:		��ֹ��x����
// F1		:		��ֹ��y����
// F2		:		��ֹ��z����
#define OC_BUS_STOP_MOVE			(36)








// �ж϶���
// N0		:	�߼�����
// U1		:	�жϵ�
#define OC_STOP_ACTION				(100)


// �ж��ƶ�
// N0		:	�߼�����
// N1		:	������·���ڵ�����
// F2		:	������Xλ��
// F3		:	������Zλ��
#define OC_STOP_MOVE				(101)

// �ж�����
// N0		:	�߼�����
#define OC_STOP_SIT					(104)


// ����
// U0		:	��ʼʱ��
// N1		:	�߼�����
// N2		:	����ID
#define OC_ACTION					(200)

// �ƶ�
// U0		:	��ʼʱ��
// N1		:	�߼�����
// N2		:	�ڵ���Ŀ
// P3		:	�ڵ��б�ָ��(WORLD_POS*)
#define OC_MOVE						(201)

// ��������
// U0		:	��ʼʱ��
// N1		:	�߼�����
// N2		:	����ID
// N3		:	Ŀ���ɫID
// F4		:	Ŀ��X����
// F5		:	Ŀ��Z����
// F6		:	Ŀ�귽��
#define OC_MAGIC_SEND				(202)

// ��������
// U0		:	��ʼʱ��
// N1		:	�߼�����
// N2		:	����ID
// N3		:	Ŀ���ɫID
// F4		:	Ŀ��X����
// F5		:	Ŀ��Z����
// F6		:	Ŀ�귽��
// U7		:	�ܽ���ʱ��
#define OC_MAGIC_CHARGE				(203)

// ��������
// U0		:	��ʼʱ��
// N1		:	�߼�����
// N2		:	����ID
// N3		:	Ŀ���ɫID
// F4		:	Ŀ��X����
// F5		:	Ŀ��Z����
// F6		:	Ŀ�귽��
// U7		:	�ܽ���ʱ��
#define OC_MAGIC_CHANNEL			(204)

// �����
// U0		:	��ʼʱ��
// N1		:	�߼�����
// N2		:	�����ID
// N3		:	�䷽ID
// N4		:	Ŀ���ɫID
#define OC_ABILITY					(205)

// ����Ԥ��
#define OC_MAGIC_PREP_SEND			(206)	//test2


// ���Ŷ���
// N0		:	�߼�����
// N1		:	����ʱ��ֵ(���ӻ���ٵ�ʱ��)
#define OC_MODIFY_ACTION				(300)
