// CollisionTool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "PathLibInterface.h"

#include "..\include\ieee.h"

int _tmain(int argc, _TCHAR* argv[])
{
	printf("-------TLBB Collision util---------\n");
	if(argc<2)
	{
		printf("Usage: %s inputfile\n", argv[0]);
		return 0;
	}

	char* szInputFileName = argv[1];

	PathLib::PathLibRoot root;
	PathLib::BuildingCollision* building = root.getBuildingCollsion();

	FILE* fp = fopen(szInputFileName, "rb");
	if(!fp) return 0;

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* pTempMem = new char[size];
	fread(pTempMem, 1, size, fp);
	fclose(fp); fp=0;

	building->openSceneFile(pTempMem, size);

	delete[] pTempMem; pTempMem=0;

	do
	{
		float x=0,y=0,z=0;
		printf("Input x and z:");
		scanf("%f %f", &x, &z);

		if(x<0 && z<0) break;

		bool ret = building->getHeight(x, y, z);

		if(ret)
		{
			printf("\n*******************x=%f\ty=%f\tz=%f\n", x, y, z);
		}
	}while(true);


	return 0;
}

