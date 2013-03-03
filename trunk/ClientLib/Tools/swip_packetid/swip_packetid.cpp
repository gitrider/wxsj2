// swip_packetid.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <set>
#include <string>
#include <windows.h>

struct PacketDefine
{
	int value;
	bool staticPacket;
	std::string enumDefine;
	std::string commend;
};

typedef std::vector< PacketDefine > PacketDefineBuf;

PacketDefineBuf g_thePacket;

const char* STATIC_PACKET[] = 
{
	"PACKET_NONE",
	"PACKET_WM_CONNECT",
	"PACKET_MW_RETCONNECT",
	"PACKET_MW_CMD",
	"PACKET_WM_CMD",
	"PACKET_CL_CONNECT",
	"PACKET_LC_RETCONNECT",
	"PACKET_CL_ASKLOGIN",
	"PACKET_LC_RETLOGIN",
	"PACKET_LC_STATUS",
};

const int STATIC_PACKET_COUNTS = sizeof(STATIC_PACKET)/sizeof(const char*);

typedef std::set< std::string > StaticPacketSet;

std::string trimstring(const std::string& strIn)
{
	std::string strOut = strIn;
	static const std::string EMPTY = "";

	std::string::size_type start = strOut.find_first_not_of("\t ");
	if(start == std::string::npos) return EMPTY;
	strOut = strOut.substr(start, std::string::npos);

	std::string::size_type end = strOut.find_last_not_of("\t \r\n");
	if(end == std::string::npos) return EMPTY;
	strOut = strOut.substr(0, end+1);

	return strOut;
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand(::GetTickCount());
	//
	StaticPacketSet staticPackets;
	for(int i=0; i<STATIC_PACKET_COUNTS; i++)
	{
		staticPackets.insert(std::string(STATIC_PACKET[i]));
	}

	FILE* fpIn = fopen("d:\\in.txt", "r");

	int currentValue = 0;
	while(true)
	{
		if(feof(fpIn)) break;
		char tempBuf[1024] = {0};
		fgets(tempBuf, 1024, fpIn);
		std::string strTemp = trimstring(tempBuf);
		if(strTemp.empty()) continue;

		PacketDefine newDefine;
		
		std::string::size_type dot = strTemp.find(',');
		if(dot == std::string::npos) continue;

		newDefine.enumDefine = trimstring(strTemp.substr(0, dot));

		//check "="
		std::string::size_type equ = newDefine.enumDefine.find('=');
		if(equ != std::string::npos)
		{
			currentValue = newDefine.value = atoi(newDefine.enumDefine.substr(equ+1).c_str());
			newDefine.enumDefine = trimstring(newDefine.enumDefine.substr(0, equ));
		}
		else
		{
			newDefine.value = currentValue;
		}
		currentValue++;

		std::string::size_type comm = strTemp.find("//");
		newDefine.commend = strTemp.substr(comm+2);

		newDefine.staticPacket = staticPackets.find(newDefine.enumDefine) != staticPackets.end();

		g_thePacket.push_back(newDefine);
	};
	fclose(fpIn);


	int* indexBuf = new int[g_thePacket.size()];
	for(int i=0; i<(int)g_thePacket.size(); i++)
	{
		indexBuf[i]=i;
	}

	//swip
	int randCounts = 100000;
	for(int i=0; i<randCounts; i++)
	{
		int first, second;
		do
		{
			first = rand()%(int)g_thePacket.size();
		}while(g_thePacket[first].staticPacket);

		do
		{
			second = rand()%(int)g_thePacket.size();
		}while(g_thePacket[second].staticPacket);

		std::swap(indexBuf[first], indexBuf[second]);
	}


	FILE* fpOut = fopen("d:\\out.txt", "w");
	for(int i=0; i<(int)g_thePacket.size(); i++)
	{
		if((i/20)*20 == i)
		{
			fprintf(fpOut, "\n\t\t//%d\n", i);
		}
		PacketDefine& define = g_thePacket[indexBuf[i]];

		fprintf(fpOut, "\t\t%s%s,\t\t//%s\n", 
			define.enumDefine.c_str(), 
			i==0?" = 0" : "",
			define.commend.c_str());
	}
	fclose(fpOut);
	delete[] indexBuf;
	return 0;
}

