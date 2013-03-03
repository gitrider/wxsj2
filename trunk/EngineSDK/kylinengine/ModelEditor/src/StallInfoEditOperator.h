#ifndef __STALLINFOEDITOPERATOR_H__
#define __STALLINFOEDITOPERATOR_H__
#pragma once

#include "WXOperator.h"
#include <OgreVector3.h>
namespace Fairy 
{
	class CStallInfoAction;

	class CStallInfoEditOperator: public Operator
	{
	public:
		CStallInfoEditOperator(void);
		~CStallInfoEditOperator(void);

	public:

		unsigned long m_dwStartPosX;
		unsigned long m_dwStartPosY;
		unsigned long m_dwEndPosX;
		unsigned long m_dwEndPosY;

		unsigned char	m_bCanStall;	//�Ƿ���԰�̯
		unsigned char	m_iTradeTax;	//����˰
		unsigned long	m_dwPosTax;		//̯λ��
		unsigned char	m_iExtraInfo;	//������Ϣ

		// ̯λaction	
		CStallInfoAction* m_pAction;

		// ��������༭������ָ��.
		void SetAction(CStallInfoAction* pAction);

		// ����λ��
		void SetPos(unsigned long dwposX, unsigned long dwposY, unsigned long m_dwEndPosX, unsigned long m_dwEndPosY);

		// ����λ����Ϣ
		void SetStallInfo(	
							unsigned char	m_bCanStall,	//�Ƿ���԰�̯
							unsigned char	m_iTradeTax,	//����˰
							unsigned long	m_dwPosTax,		//̯λ��
							unsigned char	m_iExtraInfo	//������Ϣ
							);

	public:
			virtual const String& getGroupName(void) const;
			virtual String getDescription(void) const;
			virtual String getHelp(void) const;
			virtual void undo(void);
			virtual void redo(void);


	};


	class CStallInfoEditDelOperator : public CStallInfoEditOperator
	{
	public:
		CStallInfoEditDelOperator(void);
		~CStallInfoEditDelOperator(void);

	public:
		/** Retrieves a group name of this operator
        @remarks
            The OperatorManager will manager grouping operators by group name,
            so undo/redo can be perform independent with each operator group.
        @par
            The operators must use same group name when their may be influence
            by each other.
        @returns
            group name of this operator
        */
        virtual const String& getGroupName(void) const;

        /** Retrieves a description of this operator
        @remarks
            The description should be display as a menu text.
        @returns
            description of this operator
        */
        virtual String getDescription(void) const;

        /** Retrieves a help text of this operator
        @remarks
            The help text should be use as a detail description of this operator,
            when use selecting menu item of this operator, this help text will
            display in status line.
        @returns
            help text of this operator
        */
        virtual String getHelp(void) const;

        /** Perform undo operate
        */
        virtual void undo(void);

        /** Perform redo operate
        */
        virtual void redo(void);

	};



	/////////////////////////////////////////////////////////////////////////////////
	//
	// ���һ�����redo�� undo����.
	//
	//
	class CStallInfoEditAddOperator : public CStallInfoEditOperator
	{
	public:
		CStallInfoEditAddOperator(void);
		~CStallInfoEditAddOperator(void);

	public:
		/** Retrieves a group name of this operator
        @remarks
            The OperatorManager will manager grouping operators by group name,
            so undo/redo can be perform independent with each operator group.
        @par
            The operators must use same group name when their may be influence
            by each other.
        @returns
            group name of this operator
        */
        virtual const String& getGroupName(void) const;

        /** Retrieves a description of this operator
        @remarks
            The description should be display as a menu text.
        @returns
            description of this operator
        */
        virtual String getDescription(void) const;

        /** Retrieves a help text of this operator
        @remarks
            The help text should be use as a detail description of this operator,
            when use selecting menu item of this operator, this help text will
            display in status line.
        @returns
            help text of this operator
        */
        virtual String getHelp(void) const;

        /** Perform undo operate
        */
        virtual void undo(void);

        /** Perform redo operate
        */
        virtual void redo(void);

	};//
}

#endif //__STALLINFOEDITOPERATOR_H__