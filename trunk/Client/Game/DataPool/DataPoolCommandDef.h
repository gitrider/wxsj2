
/** DataPoolCommandDef.h
 */

#pragma once



// ��Ч��
#define DPC_NONE					(0xFFFF)


// ˢ�¼���
// D0	:	����ObjID
// D1	:	���ĵ�����
// P2	:	�������������б�
//#define DPC_UPDATE_SKILL_ATTRIB		(3)

// ˢ���ķ�
// D0	:	����ObjID
// D1	:	���ĵ�����
// P2	:	�����ķ������б�
//#define DPC_UPDATE_XINFA_ATTRIB		(4)

// ˢ��װ��
// D0	:	����ObjID
// D1	:	����װ����mask
// P2	:	����װ�������б�
//#define DPC_UPDATE_EQUIP_ATTRIB		(5)

// �ű�ָ��
// D0	:	script command id
// P1	:	param list
#define DPC_SCRIPT_COMMAND			(6)

// �����б�
// D0	:	����ObjID
// D1	:	����������ı�־
// P2	:	����������BUF
// B3	:	�Ƿ�ˢ����������ɵı�־
// P4	:	������ɵı�־BUF
#define DPC_UPDATE_MISSION_LIST		(7)

// ����һ������
// P0	:	ָ��_OWN_MISSION��ָ��
#define DPC_UPDATE_MISSION_ADD		(8)

// ����һ���������
// P0	:	ָ��_OWN_MISSION��ָ��
#define DPC_UPDATE_MISSION_MODIFY	(9)

// ɾ��һ������
// D0	:	�����ID(MissionID_t)
#define DPC_UPDATE_MISSION_REMOVE	(10)

// ˢ�¿���ʰȡ��������Ʒ�б�
// D0	:	��Ŀ
// P1	:	�б�����
#define DPC_UPDATE_CAN_PICK_MISSION_ITEM_LIST	(11)

// ����һ������ʰȡ��������Ʒ
// D0	:	��ƷID
#define DPC_ADD_CAN_PICK_MISSION_ITEM			(12)

// ɾ��һ������ʰȡ��������Ʒ
// D0	:	��ƷID
#define DPC_REMOVE_CAN_PICK_MISSION_ITEM		(13)

//�����ˢ��
// D0	:	����ObjID
// D1	:	���ĵ�����
// P2	:	�������������б�
//#define DPC_UPDATE_LIFE_ABILITY		(14)

//������䷽ˢ��
// P0	:	�䷽����
//#define DPC_UPDATE_LIFE_PRESCR		(15)

//��ȴʱ��
// D0	:	�����˶��ٸ���Ч��COOL DOWNʱ��
// P1	:	ʱ���б�
//#define DPC_UPDATE_COOL_DOWN_LIST	(16)

//�����Ϣ����
#define DPC_UPDATE_TEAM_OR_GROUP	(17)

// ������ɱ�־
// N0	:	����ID
// B1	:	�Ƿ����
#define DPC_UPDATE_MISSION_HAVEDOWN_FALG	(18)

