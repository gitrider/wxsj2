#pragma once

bool ExtractResToFile(unsigned int nResID, const char* szResType,  const char* szFileName);
bool ExtractFileFromZip(const char* szZipFile, const char* szFileInZip, const char* szFileInDisk);
