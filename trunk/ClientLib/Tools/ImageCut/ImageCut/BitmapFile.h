#pragma once

class CBitmapFile
{
public:
	/// ���ļ�
	bool openFromFile(const char* szFileName);

	/// �ָ�
	bool splitToFile(const char* szFileName, int basesize=256, bool sortHorz=false);

private:
	void _fillBmpCrop(int x, int y, int width, int height, char* pBuf);
	bool _saveBmpFile(const char* szFileName, int width, int height, const char* pBuf);
private:
	int m_nWidth;
	int m_nHeight;
	int m_nPitch;
	char* m_pBitBuf;

public:
	CBitmapFile(void);
	virtual ~CBitmapFile(void);
};
