/**
 *	AICommandDef.h
 */

#pragma once



// ��Ч��
#define AIC_NONE					(0xFFFF)	// ��ЧID

// Ѱ·�ƶ�
// F0	:	Ŀ���X����
// F1	:	Ŀ���Z����
#define AIC_MOVE					(1)

// ʹ�ü���
// D0	:	����ID
// D1	:	Ŀ��ID
// F2	:	Ŀ���X����
// F3	:	Ŀ���Z����
// F4	:	����
#define AIC_USE_SKILL				(10)

// ��ĳ��Tripper����в���(���ӣ�����...)
// D0	:	Ŀ��ID
#define AIC_TRIPPER_ACTIVE			(20)

// ȱʡ�¼�
// D0	:	Ŀ��ID
#define AIC_DEFAULT_EVENT			(30)

// ʹ��ĳ��ϳɼ���(���գ�����...)
// D0	:	Ŀ��ID
#define AIC_COMPOSE_ITEM			(40)

// ��Ծ
#define AIC_JUMP					(60)

// ����
#define AIC_FOLLOW					(70)

//test5
// ������
#define AIC_OPERATE_SKILL			(90)
