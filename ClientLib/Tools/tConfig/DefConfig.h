#pragma once

struct DEFAULT_CONFIG_NODE
{
	const char* szName;
	const char* szValue;
};

//Õ∆ºˆ≈‰÷√
extern DEFAULT_CONFIG_NODE g_cfgCommend[];
const extern int g_cfgCommendSize;

//µÕ≈‰÷√
const extern DEFAULT_CONFIG_NODE g_cfgLow[];
const extern int g_cfgLowSize;

//÷–≈‰÷√
const extern DEFAULT_CONFIG_NODE g_cfgMiddle[];
const extern int g_cfgMiddleSize;

//µÕ≈‰÷√
const extern DEFAULT_CONFIG_NODE g_cfgHigh[];
const extern int g_cfgHighSize;

//ÃÓ≥‰Õ∆ºˆ≈‰÷√
void FillCommendCfg(void);