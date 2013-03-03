/*
	AXIA|��������֧�ֿ�

		���ϱ�׼The Unicode standard,Version 5.0
		http://www.unicode.org
	
	Ŀǰ֧��12X12,14X14����
*/

#pragma once

#include <vector>
#include <map>

#ifdef AX_BMPFONT_EDIT
#include <windows.h>
#endif

class CAxBitmapFont
{
	/******************************************************************
					�������ݽṹ
	*******************************************************************/
public:
	/**	���ڶ���UNICODE������
	*/
	struct CODEAREA_DEFINE
	{
		wchar_t			wID;			//!< ID(��������ʼ)
		const char*		szName;			//!< ����������
		wchar_t			wCodeStart;		//!< ʵ�ʴ��뿪ʼ(�����ô���)
		wchar_t			wCodeEnd;		//!< ʵ�ʴ������(�����ô���)
	};
	typedef std::vector< const CODEAREA_DEFINE* > CODEAREA_DEFINE_BUF;

	/** ֧�ֵĵ����ַ���С
	*/
	enum FONT_SIZE
	{
		FS_12,
		FS_14,
		FS_16,
	};

	/// ��ʾ0X47525841  'AXFT'
	enum { FILE_HEAD_IDENTITY = 0X54465841, };

	/** �ļ�ͷ
	*/
	struct FILE_HEAD
	{
		unsigned int	identity;			//!< ��ʾ FILE_HEAD_IDENTITY
		unsigned int	version;			//!< �汾(Major|Minor)
		unsigned int	fontSize;			//!< ����ߴ�(12x12, 16x16, ...)
		unsigned int	areaCounts;			//!< ֧�ֵĴ���������
		unsigned int	areaDefineOffset;	//!< ��������������ƫ��
		unsigned int	codeTableOffset;	//!< �ַ����ݼ�����ƫ��
		unsigned int	bitDataOffset;		//!< �����������
		unsigned int	bitDataSize;		//!< ������������С(bytes)
		wchar_t			szFontName[64];		//!< ʹ�õ�TrueType��������
		unsigned int	nFontHeight;		//!< ʹ�õ�TrueType����߶�
	};
	/** ���������ݽṹ
	*/
	struct CODE_AREA
	{
		wchar_t		wID;			//!< ID(��������ʼ)
		wchar_t		wCodeStart;		//!< ʵ�ʴ��뿪ʼ(�����ô���)
		wchar_t		wCodeEnd;		//!< ʵ�ʴ������(�����ô���)
		wchar_t		reserve;		//!< ����
	};
	typedef std::map< wchar_t, CODE_AREA > CODE_AREA_BUF;
	/** �ַ����붨��
	*/
	struct CODE
	{
		const char* pBuf;
	};

	/// 16λUNICODE�ܹ����ַ�����
	enum { CODE_COUNTS = 0x10000, };
	/// ���е��ַ�
	typedef CODE CODE_TABLE[CODE_COUNTS];

	/******************************************************************
					���ýӿ�
	*******************************************************************/
public:
	/** �õ��ܹ�֧�ֵ������ַ���
	*/
	void getSustainingCodeArea(CODEAREA_DEFINE_BUF& allCodeArea) const;
	/** �õ���������ʵ��֧�ֵĴ�����
	*/
	const CODE_AREA_BUF& getCodeArea(void) const { return m_allCodeArea; }
	/** ���ļ��д�
	*/
	bool openFromFile(const char* szFileName);
	/** ���ڴ��д�
	*/
	bool openFromMemory(const char* pMemory, int nMemoryBytesSize);
	/** �õ������С
	*/
	int getFontSize(void) const { return m_fontSize; }
	/** ��������������
	*/
	unsigned short fillFontMemory(wchar_t code, unsigned short* pBitMemory, int pitchOfShort, bool withBorder) const;
	/** ĳ�������Ƿ�֧��
	*/
	bool isCodeValid(wchar_t code) const { return m_codeTable[code].pBuf!=0; }
	/** �õ��ַ��Ŀ��
	*/
	unsigned short getFontWidth(wchar_t code) const;
	

#ifdef AX_BMPFONT_EDIT
	/** ���õ����ֿ��С
	*/
	void setFontSize(FONT_SIZE size);
	/** ����������������ʱʹ�õ�TrueType����
	*/
	void setTrueType(const char* szFontName, int fontHeight);
	/** �õ�������������ʱʹ�õ�TrueType����
	*/
	std::pair< const char*, int > getTrueType(void) const;
	/** ��Ӵ�����֧��
	*/
	void insertCodeArea(wchar_t wAreaID);
	/** ȡ��������֧��
	*/
	void removeCodeArea(wchar_t wAreaID);
	/** ������������
	*/
	void rebuild(void);
	/** ���浽�ļ���
	*/
	bool saveToFile(const char* szFileName);
	/** Debugoutput
	*/
	void debug(void);

	/******************************************************************
					�ڲ�����
	*******************************************************************/
private:
	/** ����12��������
	*/
	void rebuild_12pix(const char* pDIBBitBuf, int nDIBPitch, unsigned char* pCodeMemory);
	/** ����14��������
	*/
	void rebuild_14pix(const char* pDIBBitBuf, int nDIBPitch, unsigned char* pCodeMemory);
	/** ����16��������
	*/
	void rebuild_16pix(const char* pDIBBitBuf, int nDIBPitch, unsigned char* pCodeMemory);
#endif
	/** ���12��������
	*/
	unsigned short fillFont_12pix(wchar_t code, unsigned short* pTarBitMemory, int pitchOfShort, bool withBorder) const;
	/** ���14��������
	*/
	unsigned short fillFont_14pix(wchar_t code, unsigned short* pTarBitMemory, int pitchOfShort, bool withBorder) const;
	/** ���16��������
	*/
	unsigned short fillFont_16pix(wchar_t code, unsigned short* pTarBitMemory, int pitchOfShort, bool withBorder) const;

	/******************************************************************
					�ڲ�����
	*******************************************************************/
private:
	int m_fontSize;					//!< �����С 12,14
	CODE_AREA_BUF m_allCodeArea;	//!< ��֧�ֵĴ�����
	CODE_TABLE m_codeTable;			//!< ����֧�ֵ��ַ�����
	char* m_pBitMemory;				//!< �����������
	int m_nBitMemoryBytes;			//!< ���������������С(bytes)

#ifdef AX_BMPFONT_EDIT
	bool m_bDirty;					//!< ��Ҫ������������
	std::string m_strFontName;		//!< ������������ʱʹ�õ�TrueType��������
	int m_nFontHeight;				//!< ������������ʱʹ�õ�TrueType����߶�
	HFONT m_hTrueTypeFont;			//!< ������������ʱʹ�õ�TrueType
#endif
	/******************************************************************
					����/����
	*******************************************************************/
public:
	CAxBitmapFont();
	~CAxBitmapFont();
};