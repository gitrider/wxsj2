// ImageCut.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BitmapFile.h"

std::string g_strSourceFile;
std::string g_strTargetFile;
int g_nFileSize;

void _printCopyright(void)
{
	printf("-------------------------------\n");
	printf("TLBB Image cut util\n");
}

void _printUsage(void)
{
	printf("Usage:imagecut sourcefile targetfile fileSize horz?\n");
}


int _tmain(int argc, _TCHAR* argv[])
{
	_printCopyright();
	if(argc<5)
	{
		_printUsage();
		return 0;
	}

	g_strSourceFile = argv[1];
	g_strTargetFile = argv[2];
	g_nFileSize = atoi(argv[3]);
	bool horz = atoi(argv[4])!=0;

	CBitmapFile bmpFile;
	if(!bmpFile.openFromFile(g_strSourceFile.c_str()))
	{
		return 0;
	}

	printf("Open source file: %s OK!\n", g_strSourceFile.c_str());

	if(!bmpFile.splitToFile(g_strTargetFile.c_str(), g_nFileSize, horz))
	{
		return 0;
	}

	printf("Cut OK!\n");

	return 0;
}

