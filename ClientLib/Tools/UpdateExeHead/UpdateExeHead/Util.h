
#pragma once

void CenterDlg(HWND hDlg);

namespace AXP{ class IUpdater; }
bool ExtractFileFromZip(AXP::IUpdater* pUpdater, const char* szZipFile, const char* szFileInZip, const char* szFileInDisk);

bool checkIfGameRun(HWND hWnd, bool bUserConfirm);
void normalizeVersion(const char* szVersion, char* szNormalizeBuf, int bufSize);
