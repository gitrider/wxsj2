
/** GICursorSystem.h
 */

#pragma once


#include "GINode.h"



/// ָ������
enum ENUM_CURSOR_TYPE
{
	CURSOR_WINBASE = 0,

	CURSOR_NORMAL,			/// ��ͨ
	CURSOR_ATTACK,			/// ����
	CURSOR_AUTORUN,			/// �Զ�������
	CURSOR_PICKUP,			/// ʰȡ��Ʒ
	CURSOR_UNREACHABLE,		/// �޷����������
	CURSOR_MINE,			/// �ɿ�
	CURSOR_HERBS,			/// ��ҩ
	CURSOR_SPEAK,			/// ̸��
	CURSOR_INTERACT,		/// ����
	CURSOR_REPAIR,			/// ����
	CURSOR_HOVER,			/// ��꼤��(�ҽ���Ʒ...)
	CURSOR_IDENTIFY,		/// ������
	CURSOR_RING,			/// ���ܻ�
	CURSOR_DIRECTION,		///	������

	CURSOR_NUMBER,
};


class KYLIN_API tCursorSystem : public tNode
{
public:

	/** ��������� */
	virtual VOID SetCursor(ENUM_CURSOR_TYPE nType) = 0;
	/** �õ���ǰ��� */
	virtual HCURSOR GetCursor(VOID) = 0;

	/** ����UI����ģʽ */
	virtual VOID EnterUICursor(HCURSOR hCursor) = 0;
	virtual VOID LeaveUICursor(VOID) = 0;

	/** ��ӦWM_SETCURSOR */
	virtual VOID OnSetCursor(VOID) = 0;

	/** ��ʾ/������� */
	virtual VOID ShowCursor(BOOL bShow) = 0;

protected:

	KL_DECLARE_DYNAMIC(tCursorSystem);

};	// class

