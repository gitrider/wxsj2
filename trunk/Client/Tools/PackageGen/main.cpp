// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "MpqPackGen.h"

// test
#define TEST_MPQ	1
char str[]			= "E:\\cjsh-1.4.2.3-1.4.2.4.mpq";


int main( int argc, char* argv[] )
{
#if TEST_MPQ

	CMpqPackGen   PackGen;

	PackGen.GetConfigInfo();
	PackGen.ShowPackInfo();

	int temp = 0;
	printf( "\n����Ҫ����ĺ��룺" );
	scanf( "%d", &temp );

	PackGen.SetPackPotion(temp);
	PackGen.DoPacking();

#else

	//����Ϊ������ʾ����

	// ����MPQ���õ������ļ���
	DWORD R;
	FILE_FIND_DATA	sf;
	BOOL bFound		= TRUE;
	TKPMArchive* pArchive = KPM_OpenArchive( str, R );

	TKPMFile*	pFile2 = KPM_FileOpen(pArchive,"../public/config/xf.tab",R);
	TKPMSearch*		hFind = KPM_FindFirstFile(pArchive,"*",&sf);

	int j = 0;

	while(hFind != NULL && bFound != FALSE)
	{
		if ( strcmp(sf.cFileName, LISTFILE_NAME) != 0 && 
			 strcmp(sf.cFileName, SIGNATURE_NAME) != 0 && 
			 strcmp(sf.cFileName, ATTRIBUTES_NAME) != 0 )
		{
			DWORD Read;
			unsigned char * Buffer = new unsigned char[sf.dwFileSize];
			
			TKPMFile*	pFile = KPM_FileOpen(pArchive,sf.cFileName,R);
			KPM_ReadFile(pFile,Buffer,sf.dwFileSize,&Read);
			KPM_FileEof(pFile);
			KPM_FileClose(pFile);

			printf("%s\n",sf.cFileName);

			j++;
		}
		bFound = KPM_FindNextFile(hFind,&sf);
	}

	KPM_SearchClose(hFind);
	KPM_CloseArchive(pArchive);

#endif


	//DWORD R;
	//
	//TKPMArchive* pArchive = 	KPM_CreateArchive("d:\\a.mpq",R);
	//KPM_AddFile(pArchive,"d:\\b.txt","b.txt",KPM_FILE_ENCRYPTED|KPM_FILE_IMPLODE,0,FILE_TYPE_DATA);
	//KPM_AddFile(pArchive,"d:\\c.txt","c.txt",KPM_FILE_ENCRYPTED|KPM_FILE_IMPLODE,0,FILE_TYPE_DATA);
	//KPM_AddFile(pArchive,"d:\\���������ϲ�.rar","���������ϲ�.tga",KPM_FILE_REPLACEEXISTING,0,FILE_TYPE_DATA);

	//TKPMFile*	pFile = KPM_FileOpen(pArchive,"d:\\c.txt",R);
	//
	//char * Buffer = new char[1024*1024];
	//DWORD Read;
	//KPM_ReadFile(pFile,Buffer,1024*1024,&Read);
	//KPM_FileEof(pFile);
	//
	//KPM_FileClose(pFile);
	//KPM_CloseArchive(pArchive);
	//
	//pArchive =KPM_OpenArchive("d:\\a.mpq",R);
	////KPM_AddFile(pArchive,"d:\\���������ϲ�.rar","���������ϲ�.tga",KPM_FILE_REPLACEEXISTING,0,FILE_TYPE_DATA);

	//KPM_RenameFile(pArchive,"���������ϲ�.tga","a.tga");

	//pFile = KPM_FileOpen(pArchive,"a.tga",R);
	//
	//
	//
	//KPM_ReadFile(pFile,Buffer,1024*1024,&Read);
	//KPM_FileEof(pFile);
	//KPM_FileClose(pFile);
	//
	//FILE* f = fopen("d:\\d.rar","wb");
	//fwrite(Buffer,Read,1,f);
	//fclose(f);

	////KPM_RemoveFile(pArchive,"���������ϲ�.tga");
	//

	//FILE_FIND_DATA	sf;
	//BOOL bFound		= TRUE;
	//TKPMSearch*		hFind = KPM_FindFirstFile(pArchive,"*",&sf);

	//while(hFind != NULL && bFound != FALSE)
	//{
	//	bFound = KPM_FindNextFile(hFind,&sf);
	//}

	//KPM_SearchClose(hFind);
	//
	//KPM_CloseArchive(pArchive);
	//
	

	printf( "\n �������,��������˳���" );
	int val = getchar();
	val = getchar();
	return 0;
}

