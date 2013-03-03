
//********************************************
//	Ini ��غ���
//********************************************

#ifndef __INI_H__
#define __INI_H__

#include "Type.h"



#define ERROR_DATA -99999999
#define MAX_INI_VALUE 1024

//�����ļ���
class Ini
{
////////////////////////////////////////////////
// �ڲ�����
////////////////////////////////////////////////
private:
	CHAR			m_strFileName[_MAX_PATH];	//�ļ���
	long			m_lDataLen;					//�ļ�����
	CHAR*			m_strData;					//�ļ�����

	INT				IndexNum;					//������Ŀ��[]����Ŀ��
	INT*			IndexList;					//������λ���б�
	INT				Point;						//��ǰָ��
	INT				Line, Word;					//��ǰ����

	CHAR			m_szValue[MAX_INI_VALUE] ;
	CHAR			m_szRet[MAX_INI_VALUE];

////////////////////////////////////////////////
// ͨ�ýӿ�
////////////////////////////////////////////////
public:
	Ini();
	Ini(const CHAR* filename);								//��ʼ���������ļ�
	virtual ~Ini();									//�ͷ��ڴ�
	CHAR			*GetData();							//�����ļ�����
	INT				GetLines(INT);						//�����ļ�������
	INT				GetLines();						//�����ļ�������

	BOOL			Open(const CHAR* filename);				//�������ļ�
	VOID			Close();							//�ر������ļ�
	BOOL			Save(CHAR *filename=NULL);			//���������ļ�
	INT				FindIndex(CHAR *);					//���ر���λ��

////////////////////////////////////////////////
// �ڲ�����
////////////////////////////////////////////////
private:
	VOID		InitIndex();						//��ʼ������
	INT			FindData(INT, CHAR *);				//��������λ��
	INT			GotoNextLine(INT); 					//����
	CHAR*		ReadDataName(INT &);				//��ָ��λ�ö�һ��������
	CHAR*		ReadText(INT);						//��ָ��λ�ö��ַ���

	BOOL		AddIndex(CHAR *);					//����һ������
	BOOL		AddData(INT, CHAR *, CHAR *);		//�ڵ�ǰλ�ü���һ������
	BOOL		ModityData(INT, CHAR *, CHAR *);	//�ڵ�ǰλ���޸�һ�����ݵ�ֵ
	INT			GotoLastLine(CHAR *section);			//��ָ���ƶ�����INDEX�����һ��

////////////////////////////////////////////////
// ���ýӿ�
////////////////////////////////////////////////
public:
	//��һ������
	INT				ReadInt(CHAR *section, CHAR *key);
	//������ڣ����һ������
	BOOL			ReadIntIfExist(CHAR *section, CHAR *key, INT& nResult);
	//��ָ�����ж�һ����
	INT				ReadInt(CHAR *section, INT lines);	
	//��һ���ַ���
	CHAR*			ReadText(CHAR *section, CHAR *key, CHAR* str, INT size);
	//����������ȡ
	BOOL			ReadTextIfExist(CHAR *section, CHAR *key, CHAR* str, INT size);
	//��ָ�����ж�һ�ַ���
	CHAR*			ReadText(CHAR *section, INT lines, CHAR* str, INT size);	
	//��ָ���ж�һ�ַ�����
	CHAR*			ReadCaption(CHAR *section, INT lines, CHAR* str, INT size);
	//дһ������
	BOOL			Write(CHAR *section, CHAR *key, INT num);			
	//дһ���ַ���
	BOOL			Write(CHAR *section, CHAR *key, CHAR *string);		
	//������������������INDEX����һ�����У�
	INT				GetContinueDataNum(CHAR *section);	
	//��ָ��λ�ö��ַ���
	CHAR*			ReadOneLine(INT);
	INT				FindOneLine(INT);
	//����ָ���ַ����ڵ�����
	INT				ReturnLineNum(CHAR*);
};



#endif



