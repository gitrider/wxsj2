#ifndef __PACKAGETOOLS_H__
#include "PackageTools.h"
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#include "shlwapi.h"

void Usage( const char* exe )
{
	printf( "\n1:Model" );
	printf( "\n2:Material" );
	printf( "\n3:Effect" );
	printf( "\n4:Interface" );
	printf( "\n5:Sound" );
	printf( "\n6:Scene" );
	printf( "\n7:Brushes" );
	printf( "\n8:Config" );
	printf( "\na:All" );
	printf( "\nexample: %s 123", PathFindFileName(exe) );
	printf( "\nexample: %s a", PathFindFileName(exe) );
}

void Str2StrArr( string strText, vector<string>& vecParts, char div )
{
	while( true )
	{
		size_t ret = strText.find( div );
		if( ret != -1 )
		{
			string strTemp = strText.substr( 0, ret );
			strText = strText.substr( ret + 1 );
			vecParts.push_back( strTemp );
		}
		else
		{
			break;
		}
	}
	if( strText.empty() == false ) vecParts.push_back( strText );
}

int main( int argc, char* argv[] )
{
	char temp[8] = {0};
	if( argc == 1 ) {
		Usage( argv[0] );
		printf( "\n输入要打包的号码：" );
		scanf( "%8s", temp );
	}
	else {
		strncpy( temp, argv[1], 8 );
	}
	char config[8]={0};
	for(int n=0;n<8;n++){if(temp[n]=='a'){memset(config,1,8);}else if(temp[n]){config[temp[n]-49]=1;}}

	string strBasePath = "";

	string strMedia = "../Media";
	string strInterface = "../Interface";

	string strBrushes = "../Brushes";
	string strSound = "../Sound";
	string strScene = "../../public/scene";
	string strSceneSchema = "../../public/scene/schema";
	string strConfig1 = "../config";
	string strConfig2 = "../../public/config";
	string strLua = "../../public/data";

	FILE* fp = fopen( "FairyResources.cfg", "r" );
	if( fp == NULL ) return -1;
	fseek( fp, 0, SEEK_END );
	long lLength = ftell( fp );
	char *pszBuffer = new char[lLength];
	fseek( fp, 0, SEEK_SET );
	fread( pszBuffer, 1, lLength, fp );
	fclose( fp );
	string strFile = pszBuffer;
	delete [] pszBuffer;
	vector<string> vecLines;
	Str2StrArr( strFile, vecLines, '\n' );

	vector<DUMPPATH_DESC> vecPaths;
	string strSection;
	string strKey;
	string strValue;
	vector<string> vecKeyValue;
	size_t Lines = vecLines.size();
	for( size_t line = 0; line < Lines; line ++ )
	{
		string strTemp = vecLines[line];
		if( strTemp.empty() ) continue;
		else if( strTemp.at(0) == '#' ) continue;
		else if( strTemp.at(0) == '[' ) continue;
		else
		{
			vecKeyValue.resize( 0 );
			Str2StrArr( strTemp, vecKeyValue, '=' );
			if( vecKeyValue.size() == 2 )
			{
				strKey = vecKeyValue[0];
				strValue = vecKeyValue[1];
				if( strKey == "FileSystem" )
				{
					bool recursive = false;
					if( strValue.at(0) == '*' )
					{
						recursive = true;
						strValue = strValue.substr( 1 );
					}
					DUMPPATH_DESC desc;
					ZeroMemory( desc.szPath, _MAX_PATH );
					CopyMemory( desc.szPath, strValue.c_str(), strValue.length() );
					desc.bRecursive = recursive;
					vecPaths.push_back( desc );
				}
			}
		}
	}

	CPackageTools pt;

	DUMPPATH_DESC ds;
	vector<DUMPPATH_DESC> vecDesc;
	vector<string> vecBasePaths;
	vecBasePaths.push_back( strMedia );
	vecBasePaths.push_back( strInterface );

	bool bRet;

	// [Model]
	if( config[0] )
	{
		ZeroMemory( ds.szPath, _MAX_PATH ); // 清空ds
		ds.vecExt.resize( 0 );
		vecDesc.resize( 0 );
		ds.vecExt.push_back( ".mesh" ); //要查找的文件扩展名，如果为*则路径下所有文件全部打包
		ds.vecExt.push_back( ".model" );
		ds.vecExt.push_back( ".skeleton" );
		ds.vecExt.push_back( ".obj" );
		ds.vecExt.push_back( ".frame" );
		for(size_t line = 0; line < vecPaths.size(); line ++ )
		{
			DUMPPATH_DESC* pdesc = &vecPaths[line];
			if( strstr( pdesc->szPath, strMedia.c_str() ) != 0 || strstr( pdesc->szPath, strInterface.c_str() ) != 0 )
			{
				strncpy( ds.szPath, ( strBasePath + "/" + pdesc->szPath ).c_str(), _MAX_PATH );
				ds.bRecursive = pdesc->bRecursive;
				vecDesc.push_back( ds );
			}
		}
		bRet = pt.ResourcePackageGen( vecDesc, "Model", vecBasePaths ); //打包，同时输入包文件名

		Sleep( 200 );
	}

	// [Material]
	if( config[1] )
	{
		ZeroMemory( ds.szPath, _MAX_PATH );
		ds.vecExt.resize( 0 );
		vecDesc.resize( 0 );
		ds.vecExt.push_back( ".material" );
		ds.vecExt.push_back( ".cg" );
		ds.vecExt.push_back( ".asm" );
		ds.vecExt.push_back( ".program" );
		ds.vecExt.push_back( ".overlay" );
		ds.vecExt.push_back( ".bmp" );
		ds.vecExt.push_back( ".dib" );
		ds.vecExt.push_back( ".cut" );
		ds.vecExt.push_back( ".dcx" );
		ds.vecExt.push_back( ".dds" );
		ds.vecExt.push_back( ".gif" );
		ds.vecExt.push_back( ".hdr" );
		ds.vecExt.push_back( ".ico" );
		ds.vecExt.push_back( ".cur" );
		ds.vecExt.push_back( ".jpg" );
		ds.vecExt.push_back( ".jpe" );
		ds.vecExt.push_back( ".jpeg" );
		ds.vecExt.push_back( ".lif" );
		ds.vecExt.push_back( ".mdl" );
		ds.vecExt.push_back( ".mng" );
		ds.vecExt.push_back( ".jng" );
		ds.vecExt.push_back( ".pcx" );
		ds.vecExt.push_back( ".pic" );
		ds.vecExt.push_back( ".pix" );
		ds.vecExt.push_back( ".png" );
		ds.vecExt.push_back( ".pbm" );
		ds.vecExt.push_back( ".pgm" );
		ds.vecExt.push_back( ".pnm" );
		ds.vecExt.push_back( ".ppm" );
		ds.vecExt.push_back( ".psd" );
		ds.vecExt.push_back( ".pdd" );
		ds.vecExt.push_back( ".psp" );
		ds.vecExt.push_back( ".pxr" );
		ds.vecExt.push_back( ".sgi" );
		ds.vecExt.push_back( ".bw" );
		ds.vecExt.push_back( ".rgb" );
		ds.vecExt.push_back( ".rgba" );
		ds.vecExt.push_back( ".tga" );
		ds.vecExt.push_back( ".vda" );
		ds.vecExt.push_back( ".icb" );
		ds.vecExt.push_back( ".vst" );
		ds.vecExt.push_back( ".tif" );
		ds.vecExt.push_back( ".tiff" );
		ds.vecExt.push_back( ".wal" );
		ds.vecExt.push_back( ".xpm" );
		ds.vecExt.push_back( ".raw" );
		for( size_t line = 0; line < vecPaths.size(); line ++ )
		{
			DUMPPATH_DESC* pdesc = &vecPaths[line];
			if( strstr( pdesc->szPath, strMedia.c_str() ) != 0 || strstr( pdesc->szPath, strInterface.c_str() ) != 0 )
			{
				strncpy( ds.szPath, ( strBasePath + "/" + pdesc->szPath ).c_str(), _MAX_PATH );
				ds.bRecursive = pdesc->bRecursive;
				vecDesc.push_back( ds );
			}
		}
		bRet = pt.ResourcePackageGen( vecDesc, "Material", vecBasePaths );

		Sleep( 200 );
	}

	//[Effect]
	if( config[2] )
	{
		ZeroMemory( ds.szPath, _MAX_PATH );
		ds.vecExt.resize( 0 );
		vecDesc.resize( 0 );
		ds.vecExt.push_back( ".particle" );
		ds.vecExt.push_back( ".effect" );
		ds.vecExt.push_back( ".skill" );
		for(size_t line = 0; line < vecPaths.size(); line ++ )
		{
			DUMPPATH_DESC* pdesc = &vecPaths[line];
			if( strstr( pdesc->szPath, strMedia.c_str() ) != 0 || strstr( pdesc->szPath, strInterface.c_str() ) != 0 )
			{
				strncpy( ds.szPath, ( strBasePath + "/" + pdesc->szPath ).c_str(), _MAX_PATH );
				ds.bRecursive = pdesc->bRecursive;
				vecDesc.push_back( ds );
			}
		}
		bRet = pt.ResourcePackageGen( vecDesc, "Effect", vecBasePaths );

		Sleep( 200 );
	}

	//[Interface]
	if( config[3] )
	{
		ZeroMemory( ds.szPath, _MAX_PATH );
		ds.vecExt.resize( 0 );
		vecDesc.resize( 0 );
		ds.vecExt.push_back( ".xml" );
		ds.vecExt.push_back( ".lua" );
		ds.vecExt.push_back( ".ttf" );
		ds.vecExt.push_back( ".font" );
		ds.vecExt.push_back( ".shema" );
		ds.vecExt.push_back( ".bin" );
		ds.vecExt.push_back( ".imageset" );
		for(size_t line = 0; line < vecPaths.size(); line ++ )
		{
			DUMPPATH_DESC* pdesc = &vecPaths[line];
			if( strstr( pdesc->szPath, strMedia.c_str() ) != 0 || strstr( pdesc->szPath, strInterface.c_str() ) != 0 )
			{
				strncpy( ds.szPath, ( strBasePath + "/" + pdesc->szPath ).c_str(), _MAX_PATH );
				ds.bRecursive = pdesc->bRecursive;
				vecDesc.push_back( ds );
			}
		}
		bRet = pt.ResourcePackageGen( vecDesc, "Interface", vecBasePaths );

		Sleep( 200 );
	}

	//[Sound]
	if( config[4] )
	{
		vecBasePaths.resize( 0 );
		vecBasePaths.push_back( strSound );
		ZeroMemory( ds.szPath, _MAX_PATH );
		ds.vecExt.resize( 0 );
		vecDesc.resize( 0 );
		ds.bRecursive = true;
		ds.vecExt.push_back( "*" );
		CopyMemory( ds.szPath, ( strBasePath + "/" + strSound ).c_str(), ( strBasePath + "/" + strSound ).length() );
		vecDesc.push_back( ds );
		bRet = pt.ResourcePackageGen( vecDesc, "Sound", vecBasePaths );

		Sleep( 200 );
	}

	//[Scene]
	if( config[5] )
	{
		vecBasePaths.resize( 0 );
		vecBasePaths.push_back( strScene );
		vecBasePaths.push_back( strSceneSchema );
		ZeroMemory( ds.szPath, _MAX_PATH );
		ds.vecExt.resize( 0 );
		vecDesc.resize( 0 );
		ds.bRecursive = true;
		ds.vecExt.push_back( ".jpg" );
		ds.vecExt.push_back( ".dds" );
		ds.vecExt.push_back( ".esd" );
		ds.vecExt.push_back( ".gridinfo" );
		ds.vecExt.push_back( ".heightmap" );
		ds.vecExt.push_back( ".png" );
		ds.vecExt.push_back( ".region" );
		ds.vecExt.push_back( ".scene" );
		ds.vecExt.push_back( ".preload" );
		ds.vecExt.push_back( ".terrain" );
		ds.vecExt.push_back( ".wcollision" );

		ds.vecExt.push_back( ".xsd" );
		ds.vecExt.push_back( ".xml" );
		ds.vecExt.push_back( ".material" );
		ds.vecExt.push_back( ".cg" );
		ds.vecExt.push_back( ".program" );

		CopyMemory( ds.szPath, ( strBasePath + "/" + strScene ).c_str(), ( strBasePath + "/" + strScene ).length() );
		vecDesc.push_back( ds );
		bRet = pt.ResourcePackageGen( vecDesc, "Scene", vecBasePaths );

		Sleep( 200 );
	}

	//[Brushes]
	if( config[6] )
	{
		vecBasePaths.resize( 0 );
		vecBasePaths.push_back( strBrushes );
		ZeroMemory( ds.szPath, _MAX_PATH );
		ds.vecExt.resize( 0 );
		vecDesc.resize( 0 );
		ds.bRecursive = true;
		ds.vecExt.push_back( "*" );
		CopyMemory( ds.szPath, ( strBasePath + "/" + strBrushes ).c_str(), ( strBasePath + "/" + strBrushes ).length() );
		vecDesc.push_back( ds );
		bRet = pt.ResourcePackageGen( vecDesc, "Brushes", vecBasePaths );

		Sleep( 200 );
	}

	//[Config]
	if( config[7] )
	{
		vecBasePaths.resize( 0 );
		vecBasePaths.push_back( strConfig1 );
		vecBasePaths.push_back( strConfig2 );
		vecBasePaths.push_back( strLua );
		ZeroMemory( ds.szPath, _MAX_PATH );
		ds.vecExt.resize( 0 );
		vecDesc.resize( 0 );
		ds.bRecursive = true;
		ds.vecExt.push_back( ".txt" );
		ds.vecExt.push_back( ".lua" );
		ds.vecExt.push_back( "script.dat" );
		CopyMemory( ds.szPath, ( strBasePath + "/" + strLua ).c_str(), ( strBasePath + "/" + strLua ).length() );
		vecDesc.push_back( ds );
		ZeroMemory( ds.szPath, _MAX_PATH );
		CopyMemory( ds.szPath, ( strBasePath + "/" + strConfig1 ).c_str(), ( strBasePath + "/" + strConfig1 ).length() );
		vecDesc.push_back( ds );
		ZeroMemory( ds.szPath, _MAX_PATH );
		CopyMemory( ds.szPath, ( strBasePath + "/" + strConfig2 ).c_str(), ( strBasePath + "/" + strConfig2 ).length() );
		vecDesc.push_back( ds );
		bRet = pt.ResourcePackageGen( vecDesc, "Config", vecBasePaths );
	}

	int input = 0;
	printf( "\n\nFinish and success!! press enter to exit\n" );
	system("pause");

	return 1;
}
