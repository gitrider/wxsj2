
/** ���ݳ����ݽṹ -�ƺ�
 */

#pragma once


// ����ĳƺ�����
struct _TITLE_
{
	// ����ƺŵ�����
	enum
	{
		INVALID_TITLE = 0,	// ��Ч
		
		ID_TITLE,			// ����id
		STRING_TITLE,		// �����ַ���
	};


	BOOL		bFlag;		// ������title�Ƿ���id
	BYTE		bType;		// ������title������
	BYTE		bArrId;		// ��¼ĳ��ƺ����������������
	INT			nTime;		// titleʱ�޵���ʱ�䣬�����ڵ���0

//	union
//	{
		INT		ID;									// �����id,������Ǳ���id
		CHAR	szTitleData[ MAX_CHARACTER_TITLE ];	// �������id���������title����
//	};
	
	
	_TITLE_()
	{
		bFlag	= INVALID_TITLE;
		bType	= _TITLE::NO_TITLE;
		bArrId	= -1;
		nTime	= 0;
		ID		= -1;

		memset( szTitleData, 0, MAX_CHARACTER_TITLE );
	}

};