#include "StdAfx.h"
#include "DefConfig.h"
#include "tConfig.h"

//---------------
DEFAULT_CONFIG_NODE g_cfgCommend[] =
{
	{ "View_LightmapQuality",	"0"  },		//������Ӱ(��/��/��)
	{ "View_Fanhunjiao",		"0"  },		//�����(1/2/4)
	{ "View_ObjectMove",		"0"  },		//���嶯��(0/1/2)
	{ "View_ColorQuality",		"0"  },		//��ɫ������������(16/32/����)
	{ "View_TextureFiltering",	"0"  },		//����(��/��/��)
	{ "View_HumanLightmap",		"0" },		//��ɫ��Ӱ(0/1/2/3)
	{ "View_FullScreen",		"0"  },		//ȫ��(0/1)
	{ "View_WalkEffect",		"0"  },		//����Ч��(0/1)
	{ "View_Weather",			"0"  },		//����Ч��(0/1)

	{ "System_MaxFPS",			"30"  },	//FPS
};

const int g_cfgCommendSize = sizeof(g_cfgCommend)/sizeof(DEFAULT_CONFIG_NODE);


//---------------
const DEFAULT_CONFIG_NODE g_cfgLow[] =
{
	{ "View_LightmapQuality",	"0"  },		//������Ӱ(��/��/��)
	{ "View_Fanhunjiao",		"0"  },		//�����(1/2/4)
	{ "View_ObjectMove",		"0"  },		//���嶯��(0/1/2)
	{ "View_ColorQuality",		"0"  },		//��ɫ������������(16/32/����)
	{ "View_TextureFiltering",	"0"  },		//����(��/��/��)
	{ "View_HumanLightmap",		"0" },		//��ɫ��Ӱ(0/1/2/3)
	{ "View_FullScreen",		"0"  },		//ȫ��(0/1)
	{ "View_WalkEffect",		"0"  },		//����Ч��(0/1)
	{ "View_Weather",			"0"  },		//����Ч��(0/1)

	{ "System_MaxFPS",			"15"  },	//FPS
};

const int g_cfgLowSize = sizeof(g_cfgLow)/sizeof(DEFAULT_CONFIG_NODE);

//---------------
const DEFAULT_CONFIG_NODE g_cfgMiddle[] =
{
	{ "View_LightmapQuality",	"1"  },		//������Ӱ(��/��/��)
	{ "View_Fanhunjiao",		"0"  },		//�����(1/2/4)
	{ "View_ObjectMove",		"1"  },		//���嶯��(0/1/2)
	{ "View_ColorQuality",		"1"  },		//��ɫ������������(16/32/����)
	{ "View_TextureFiltering",	"1"  },		//����(��/��/��)
	{ "View_HumanLightmap",		"0" },		//��ɫ��Ӱ(0/1/2/3)
	{ "View_FullScreen",		"0"  },		//ȫ��(0/1)
	{ "View_WalkEffect",		"1"  },		//����Ч��(0/1)
	{ "View_Weather",			"1"  },		//����Ч��(0/1)

	{ "System_MaxFPS",			"30"  },	//FPS
};

const int g_cfgMiddleSize = sizeof(g_cfgMiddle)/sizeof(DEFAULT_CONFIG_NODE);

//---------------
const DEFAULT_CONFIG_NODE g_cfgHigh[] =
{
	{ "View_LightmapQuality",	"2"  },		//������Ӱ(��/��/��)
	{ "View_Fanhunjiao",		"4"  },		//�����(1/2/4)
	{ "View_ObjectMove",		"2"  },		//���嶯��(0/1/2)
	{ "View_ColorQuality",		"2"  },		//��ɫ������������(16/32/����)
	{ "View_TextureFiltering",	"2"  },		//����(��/��/��)
	{ "View_HumanLightmap",		"2" },		//��ɫ��Ӱ(0/1/2/3)
	{ "View_FullScreen",		"0"  },		//ȫ��(0/1)
	{ "View_WalkEffect",		"1"  },		//����Ч��(0/1)
	{ "View_Weather",			"1"  },		//����Ч��(0/1)

	{ "System_MaxFPS",			"35"  },	//FPS
};

const int g_cfgHighSize = sizeof(g_cfgHigh)/sizeof(DEFAULT_CONFIG_NODE);

//
void FillCommendCfg(void)
{
    const VideoOptionMap& videoOptionMap = gHardwareInfoSystem.getVideoOptionMap();

    VideoOptionMap::const_iterator it = videoOptionMap.begin();

    //while (it != videoOptionMap.end())
    //{
    //    char debugString[256];
    //    sprintf(debugString, "%s : %d : %d\n", it->first.c_str(), it->second.mMinValue, it->second.mMaxValue);
    //    ::OutputDebugString(debugString);
    //    ++it;
    //}

	//�����ڴ�
	static CStringA  s_staValueBackup[g_cfgCommendSize];

    for(int i=0; i<g_cfgCommendSize; i++)
    {
        std::string variableName(g_cfgCommend[i].szName);

        VideoOptionMap::const_iterator it = videoOptionMap.find(variableName);

        if (it != videoOptionMap.end())
        {
			s_staValueBackup[i].Format("%d", it->second.mMinValue);
            // �����Ƽ�ֵ
			g_cfgCommend[i].szValue = (LPCSTR)(s_staValueBackup[i]);
        }
    }
}