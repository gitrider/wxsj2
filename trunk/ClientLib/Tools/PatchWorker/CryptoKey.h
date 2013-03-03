
#pragma once

extern unsigned char	g_byPairKey[];
extern unsigned int		g_nPairKeySize;

extern unsigned char	g_byPubKey[];
extern unsigned int		g_nPubKeySize;

bool TryLoadKeyFromFile(const char* szFileName);
