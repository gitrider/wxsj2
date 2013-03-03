// FileSplit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const unsigned char XOR_CODE = 0xFE;

void printUsage(void)
{
	printf(
		"=========================\n"
		"File Split Util\n"
		"=========================\n"
		"Usage: filesplit.exe inputFilename a|u ...\n"
	);
}

size_t getFileSize(const char* szFileName)
{
	FILE* fp = fopen(szFileName, "rb");
	if(!fp) return 0;

	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	fclose(fp);
	return fileSize;
}

size_t size2size(const char* szSize)
{
	assert(szSize);
	size_t nMultiple = 1;
	size_t len = strlen(szSize);

	char last = szSize[len-1];
	if(last=='k' || last=='K') nMultiple = 1024;
	else if(last=='M' || last=='m') nMultiple = 1024*1024;
	else if(last=='G' || last=='g') nMultiple = 1024*1024*1024;

	return atoi(szSize)*nMultiple;
}

void writeFilePart(const char* szInputFileName, const char* szOutputfileName, size_t start, size_t end, bool encrypt)
{
	size_t fileSize = getFileSize(szInputFileName);
	assert(fileSize>0);
	assert(start >= 0  && end <= fileSize);

	FILE* fpOut = fopen(szOutputfileName, "wb");
	if(!fpOut)
	{
		printf("\nCan't write output file: %s\n", szOutputfileName);
		return;
	}

	FILE* fpIn = fopen(szInputFileName, "rb");
	fseek(fpIn, (int)start, SEEK_SET);
	
	char* pTempBuf = new char[end-start];

	fseek(fpIn, (long)start, SEEK_SET);
	size_t readSize = fread(pTempBuf, 1, end-start, fpIn);
	if(encrypt)
	{
		for(size_t i=0; i<readSize; i++)
		{
			pTempBuf[i] ^= XOR_CODE;
		}
	}
	fwrite(pTempBuf, 1, readSize, fpOut);

	delete[] pTempBuf;

	fclose(fpIn); fpIn=0;
	fclose(fpOut); fpOut=0;
}

void splitFileAverage(const char* szInputFileName, int counts, bool encrypt)
{
	size_t fileSize = getFileSize(szInputFileName);
	assert(fileSize>0);

	size_t nodeSize = fileSize/counts;
	for(int i=0; i<counts; i++)
	{
		char szFileName[260]={0};
		_snprintf(szFileName, 260, "%d_%s", i, szInputFileName);

		size_t start = nodeSize*i;
		size_t end = start + nodeSize;
		if(i==counts-1) end = fileSize;
		printf("Write %s...", szFileName);
		writeFilePart(szInputFileName, szFileName, start, end, encrypt);
		printf("\tOK\n");
	}
}

void splitFileUser(const char* szInputFileName, std::vector< size_t >& sizeBuf, bool encrypt)
{
	size_t totalSize = 0;
	for(int i=0; i<(int)sizeBuf.size(); i++) totalSize += sizeBuf[i];
	size_t fileSize = getFileSize(szInputFileName);

	if(totalSize > fileSize)
	{
		printf("Total file size should littler than %d\n", fileSize);
		return;
	}
	if(totalSize < fileSize)
	{
		sizeBuf.push_back(fileSize-totalSize);
	}

	size_t sizeNow = 0;
	for(int i=0; i<(int)sizeBuf.size(); i++)
	{
		size_t end = sizeNow + sizeBuf[i];
		char szFileName[260]={0};
		_snprintf(szFileName, 260, "%d_%s", i, szInputFileName);

		printf("Write %s...", szFileName);
		writeFilePart(szInputFileName, szFileName, sizeNow, end, encrypt);
		printf("\tOK\n");

		sizeNow = end;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{

	if(argc < 3) 
	{
		printUsage();
		return 0;
	}

	//-------------------------------------------
	std::string strInputFileName = argv[1];
	if(_access(strInputFileName.c_str(), 4) != 0)
	{
		printf("Can't read file: %s\n", strInputFileName.c_str());
		return 0;
	}
	size_t fileSize = getFileSize(strInputFileName.c_str());
	if(fileSize == 0)
	{
		printf("Can't split empty file: %s\n", strInputFileName.c_str());
		return 0;
	}

	bool encrypt = false;

	//-------------------------------------------
	char* splitType = argv[2];
	int nSplitType = 0;		//0-average, 1-user
	if(splitType[0] == 'a' || splitType[0]=='A')
	{
		nSplitType = 0;
		if(splitType[1]=='e' || splitType[1] == 'E') encrypt = true;
	}
	else if(splitType[0] == 'u' || splitType[0]=='U')
	{
		nSplitType = 1;
		if(splitType[1]=='e' || splitType[1] == 'E') encrypt = true;
	}
	else
	{
		printf("Not valid split type(must 'a' or 'u'): %s\n", nSplitType);
		return 0;
	}

	//-------------------------------------------
	if(nSplitType==0)	//average
	{
		if(argc < 4)
		{
			printf("Need split counts\n");
			return 0;
		}

		int splitCounts = atoi(argv[3]);
		if(splitCounts < 0 || splitCounts > (int)fileSize)
		{
			printf("Not a valid split counts:%s", splitCounts);
			return 0;
		}

		splitFileAverage(strInputFileName.c_str(), splitCounts, encrypt);
	}
	else if(nSplitType==1)	//user
	{
		if(argc < 4)
		{
			printf("Need split sizes\n");
			return 0;
		}

		std::vector< size_t > sizeBuf;
		for(int i=3; i<argc; i++)
		{
			sizeBuf.push_back(size2size(argv[i]));
		}

		splitFileUser(strInputFileName.c_str(), sizeBuf, encrypt);
	}

	return 0;
}

