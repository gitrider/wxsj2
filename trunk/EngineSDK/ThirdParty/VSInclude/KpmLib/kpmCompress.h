#ifndef _KPM_COMPRESS_H_
#define _KPM_COMPRESS_H_
namespace KPM
{
	//-----------------------------------------------------------------------------
	// Compression and decompression functions
	int Compress_pklib  (char * pbOutBuffer, int * pdwOutLength, char * pbInBuffer, int dwInLength, int * pCmpType, int nCmpLevel);
	int Decompress_pklib(char * pbOutBuffer, int * pdwOutLength, char * pbInBuffer, int dwInLength);

	// (De)compression
	int  SCompCompress   (char * pbOutBuffer, int * pdwOutLength, char * pbInBuffer, int dwInLength, int uCompressions, int nCmpType, int nCmpLevel);
	int  SCompDecompress (char * pbOutBuffer, int * pdwOutLength, char * pbInBuffer, int dwInLength);

}

#endif