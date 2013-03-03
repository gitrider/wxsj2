/*
	AXIA|点阵字体支持库

		符合标准The Unicode standard,Version 5.0
		http://www.unicode.org
	
	目前支持12X12,14X14点阵
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
					公用数据结构
	*******************************************************************/
public:
	/**	用于定义UNICODE代码区
	*/
	struct CODEAREA_DEFINE
	{
		wchar_t			wID;			//!< ID(代码区开始)
		const char*		szName;			//!< 代码区名称
		wchar_t			wCodeStart;		//!< 实际代码开始(包括该代码)
		wchar_t			wCodeEnd;		//!< 实际代码结束(包括该代码)
	};
	typedef std::vector< const CODEAREA_DEFINE* > CODEAREA_DEFINE_BUF;

	/** 支持的点阵字符大小
	*/
	enum FONT_SIZE
	{
		FS_12,
		FS_14,
		FS_16,
	};

	/// 标示0X47525841  'AXFT'
	enum { FILE_HEAD_IDENTITY = 0X54465841, };

	/** 文件头
	*/
	struct FILE_HEAD
	{
		unsigned int	identity;			//!< 标示 FILE_HEAD_IDENTITY
		unsigned int	version;			//!< 版本(Major|Minor)
		unsigned int	fontSize;			//!< 点阵尺寸(12x12, 16x16, ...)
		unsigned int	areaCounts;			//!< 支持的代码区数量
		unsigned int	areaDefineOffset;	//!< 代码区定义数据偏移
		unsigned int	codeTableOffset;	//!< 字符数据检索区偏移
		unsigned int	bitDataOffset;		//!< 字体点阵数据
		unsigned int	bitDataSize;		//!< 点阵数据区大小(bytes)
		wchar_t			szFontName[64];		//!< 使用的TrueType字体名称
		unsigned int	nFontHeight;		//!< 使用的TrueType字体高度
	};
	/** 代码区数据结构
	*/
	struct CODE_AREA
	{
		wchar_t		wID;			//!< ID(代码区开始)
		wchar_t		wCodeStart;		//!< 实际代码开始(包括该代码)
		wchar_t		wCodeEnd;		//!< 实际代码结束(包括该代码)
		wchar_t		reserve;		//!< 保留
	};
	typedef std::map< wchar_t, CODE_AREA > CODE_AREA_BUF;
	/** 字符代码定义
	*/
	struct CODE
	{
		const char* pBuf;
	};

	/// 16位UNICODE总共的字符数量
	enum { CODE_COUNTS = 0x10000, };
	/// 所有的字符
	typedef CODE CODE_TABLE[CODE_COUNTS];

	/******************************************************************
					公用接口
	*******************************************************************/
public:
	/** 得到能够支持的所有字符集
	*/
	void getSustainingCodeArea(CODEAREA_DEFINE_BUF& allCodeArea) const;
	/** 得到本字体中实际支持的代码区
	*/
	const CODE_AREA_BUF& getCodeArea(void) const { return m_allCodeArea; }
	/** 从文件中打开
	*/
	bool openFromFile(const char* szFileName);
	/** 从内存中打开
	*/
	bool openFromMemory(const char* pMemory, int nMemoryBytesSize);
	/** 得到字体大小
	*/
	int getFontSize(void) const { return m_fontSize; }
	/** 填充字体点阵数据
	*/
	unsigned short fillFontMemory(wchar_t code, unsigned short* pBitMemory, int pitchOfShort, bool withBorder) const;
	/** 某个代码是否支持
	*/
	bool isCodeValid(wchar_t code) const { return m_codeTable[code].pBuf!=0; }
	/** 得到字符的宽度
	*/
	unsigned short getFontWidth(wchar_t code) const;
	

#ifdef AX_BMPFONT_EDIT
	/** 设置点阵字库大小
	*/
	void setFontSize(FONT_SIZE size);
	/** 设置生成字体数据时使用的TrueType字体
	*/
	void setTrueType(const char* szFontName, int fontHeight);
	/** 得到生成字体数据时使用的TrueType字体
	*/
	std::pair< const char*, int > getTrueType(void) const;
	/** 添加代码区支持
	*/
	void insertCodeArea(wchar_t wAreaID);
	/** 取消代码区支持
	*/
	void removeCodeArea(wchar_t wAreaID);
	/** 重新生成数据
	*/
	void rebuild(void);
	/** 保存到文件中
	*/
	bool saveToFile(const char* szFileName);
	/** Debugoutput
	*/
	void debug(void);

	/******************************************************************
					内部函数
	*******************************************************************/
private:
	/** 生成12点阵数据
	*/
	void rebuild_12pix(const char* pDIBBitBuf, int nDIBPitch, unsigned char* pCodeMemory);
	/** 生成14点阵数据
	*/
	void rebuild_14pix(const char* pDIBBitBuf, int nDIBPitch, unsigned char* pCodeMemory);
	/** 生成16点阵数据
	*/
	void rebuild_16pix(const char* pDIBBitBuf, int nDIBPitch, unsigned char* pCodeMemory);
#endif
	/** 填充12点阵数据
	*/
	unsigned short fillFont_12pix(wchar_t code, unsigned short* pTarBitMemory, int pitchOfShort, bool withBorder) const;
	/** 填充14点阵数据
	*/
	unsigned short fillFont_14pix(wchar_t code, unsigned short* pTarBitMemory, int pitchOfShort, bool withBorder) const;
	/** 填充16点阵数据
	*/
	unsigned short fillFont_16pix(wchar_t code, unsigned short* pTarBitMemory, int pitchOfShort, bool withBorder) const;

	/******************************************************************
					内部变量
	*******************************************************************/
private:
	int m_fontSize;					//!< 字体大小 12,14
	CODE_AREA_BUF m_allCodeArea;	//!< 所支持的代码区
	CODE_TABLE m_codeTable;			//!< 所有支持的字符代码
	char* m_pBitMemory;				//!< 字体点阵数据
	int m_nBitMemoryBytes;			//!< 字体点阵数据区大小(bytes)

#ifdef AX_BMPFONT_EDIT
	bool m_bDirty;					//!< 需要重新生成数据
	std::string m_strFontName;		//!< 生成字体数据时使用的TrueType字体名称
	int m_nFontHeight;				//!< 生成字体数据时使用的TrueType字体高度
	HFONT m_hTrueTypeFont;			//!< 生成字体数据时使用的TrueType
#endif
	/******************************************************************
					构造/析构
	*******************************************************************/
public:
	CAxBitmapFont();
	~CAxBitmapFont();
};