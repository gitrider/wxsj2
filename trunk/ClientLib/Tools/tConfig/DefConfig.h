#pragma once

struct DEFAULT_CONFIG_NODE
{
	const char* szName;
	const char* szValue;
};

//�Ƽ�����
extern DEFAULT_CONFIG_NODE g_cfgCommend[];
const extern int g_cfgCommendSize;

//������
const extern DEFAULT_CONFIG_NODE g_cfgLow[];
const extern int g_cfgLowSize;

//������
const extern DEFAULT_CONFIG_NODE g_cfgMiddle[];
const extern int g_cfgMiddleSize;

//������
const extern DEFAULT_CONFIG_NODE g_cfgHigh[];
const extern int g_cfgHighSize;

//����Ƽ�����
void FillCommendCfg(void);