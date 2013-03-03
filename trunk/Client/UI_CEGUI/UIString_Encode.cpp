#include "StdAfx.h"
#include "UIString_Encode.h"

#include <string>
#include <hash_map>

//临时内存
wchar_t*	g_pwMem_mbcs_to_utf8	= 0;
int			g_nwMem_mbcs_to_utf8	= 0;
char*		g_pMem_mbcs_to_utf8		= 0;
int			g_nMem_mbcs_to_utf8		= 0;

char*		g_pMem_utf8_to_mbcs		= 0;
int			g_nMem_utf8_to_mbcs		= 0;
wchar_t*	g_pwMem_utf8_to_mbcs	= 0;
int			g_nwMem_utf8_to_mbcs	= 0;

//缺省的代码页(936)
//1258:Vetnam
int			g_nDefaulat_code_page	=936;

int			g_nSafe_Check			= 1024*512; //512k
const char*		g_szErrorString_mbcs		= "<ERR_MEMORY>";
const wchar_t*	g_szErrorString_ucs			= L"<ERR_MEMORY>";

void vscii_to_ucs16(const char* mbcs_string, int mbcs_string_size, wchar_t* wchar_buf, int wchar_buf_size);
void ucs16_to_vscii(const wchar_t* wchar_string, int wchar_char_counts, char* mbcs_string, int mbcs_string_size);

//临时内存自动释放
struct STRING_ENCODE_MEMORY
{
	~STRING_ENCODE_MEMORY()
	{
		delete[] g_pwMem_mbcs_to_utf8; 
		g_pwMem_mbcs_to_utf8 = 0;

		delete[] g_pMem_mbcs_to_utf8;
		g_pMem_mbcs_to_utf8 = 0;

		delete[] g_pMem_utf8_to_mbcs;
		g_pMem_utf8_to_mbcs = 0;

		delete[] g_pwMem_utf8_to_mbcs;
		g_pwMem_utf8_to_mbcs = 0;
	}
} AUTO_FREE_MEMORY;

void set_code_page(int code_page)
{
	g_nDefaulat_code_page=code_page;
}

int get_code_page(void)
{
	return g_nDefaulat_code_page;
}

// return the number of utf8 code units required to encode the given utf32 code point
int encoded_size(wchar_t code_point)
{
	if (((unsigned int)code_point) < 0x80)
		return 1;
	else if (((unsigned int)code_point) < 0x0800)
		return 2;
	else if (((unsigned int)code_point) < 0x10000)
		return 3;
	else
		return 4;
}

// encoding functions
// for all:
//	dest_len is in code units.
//	returns number of code units put into dest buffer.
int encode_ucs_to_utf8(const wchar_t* src, unsigned char* dest, int dest_len, int src_len)
{
	// count length for null terminated source...
	if(src_len == 0)
	{
		src_len = (int)wcslen(src);
	}

	int destCapacity = dest_len;

	// while there is data in the source buffer,
	for (int idx = 0; idx < src_len; ++idx)
	{
		wchar_t	cp = src[idx];

		// check there is enough destination buffer to receive this encoded unit (exit loop & return if not)
		if (destCapacity < encoded_size(cp))
		{
			break;
		}

		if (cp < 0x80)
		{
			*dest++ = (unsigned char)cp;
			--destCapacity;
		}
		else if (cp < 0x0800)
		{
			*dest++ = (unsigned char)((cp >> 6) | 0xC0);
			*dest++ = (unsigned char)((cp & 0x3F) | 0x80);
			destCapacity -= 2;
		}
		else if (cp < 0x10000)
		{
			*dest++ = (unsigned char)((cp >> 12) | 0xE0);
			*dest++ = (unsigned char)(((cp >> 6) & 0x3F) | 0x80);
			*dest++ = (unsigned char)((cp & 0x3F) | 0x80);
			destCapacity -= 3;
		}
		else
		{
			*dest++ = (unsigned char)((cp >> 18) | 0xF0);
			*dest++ = (unsigned char)(((cp >> 12) & 0x3F) | 0x80);
			*dest++ = (unsigned char)(((cp >> 6) & 0x3F) | 0x80);
			*dest++ = (unsigned char)((cp & 0x3F) | 0x80);
			destCapacity -= 4;
		}

	}

	return dest_len - destCapacity;
}

int encode_utf8_to_ucs(const unsigned char* src, wchar_t* dest, int dest_len, int src_len)
{
	// count length for null terminated source...
	if (src_len == 0)
	{
		src_len = (int)strlen((const char*)src);
	}

	int destCapacity = dest_len;

	// while there is data in the source buffer, and space in the dest buffer
	for(int idx = 0; ((idx < src_len) && (destCapacity > 0));)
	{
		wchar_t	cp;
		unsigned char cu = src[idx++];

		if (cu < 0x80)
		{
			cp = (wchar_t)(cu);
		}
		else if (cu < 0xE0)
		{
			cp = ((cu & 0x1F) << 6);
			cp |= (src[idx++] & 0x3F);
		}
		else if (cu < 0xF0)
		{
			cp = ((cu & 0x0F) << 12);
			cp |= ((src[idx++] & 0x3F) << 6);
			cp |= (src[idx++] & 0x3F);
		}
		else
		{
			cp = ((cu & 0x07) << 18);
			cp |= ((src[idx++] & 0x3F) << 12);
			cp |= ((src[idx++] & 0x3F) << 6);
			cp |= (src[idx++] & 0x3F);
		}

		*dest++ = cp;
		--destCapacity;
	}

	return dest_len - destCapacity;
}


void mbcs_to_utf8(const std::string& strIn, std::string& strOut)
{
	//MBCS -> Utf16
	if((int)strIn.size()+1 > g_nwMem_mbcs_to_utf8)
	{
		//safe check
		if((int)strIn.size()+1 >= g_nSafe_Check) 
		{
			strOut = g_szErrorString_mbcs;
			return;
		}

		g_nwMem_mbcs_to_utf8 = (int)strIn.size()+1;

		//up bound 4;
		while(g_nwMem_mbcs_to_utf8%4) g_nwMem_mbcs_to_utf8++;

		//realloca memory
		delete[] g_pwMem_mbcs_to_utf8;
		g_pwMem_mbcs_to_utf8 = new wchar_t[g_nwMem_mbcs_to_utf8];
	}
	memset(g_pwMem_mbcs_to_utf8, 0, sizeof(wchar_t)*g_nwMem_mbcs_to_utf8);
	mbcs_to_ucs16(g_nDefaulat_code_page, strIn.c_str(), (int)strIn.size(), g_pwMem_mbcs_to_utf8, (int)strIn.size()+1);

	//Utf16 -> Utf8
	int nUtf8Size = (int)(strIn.size())*3+1;
	if(nUtf8Size > g_nMem_mbcs_to_utf8)
	{
		//safe check
		if(nUtf8Size >= g_nSafe_Check) 
		{
			strOut = g_szErrorString_mbcs;
			return;
		}

		g_nMem_mbcs_to_utf8 = nUtf8Size;

		//up bound 4;
		while(g_nMem_mbcs_to_utf8%4) g_nMem_mbcs_to_utf8++;

		//realloca memory
		delete[] g_pMem_mbcs_to_utf8;
		g_pMem_mbcs_to_utf8 = new char[g_nMem_mbcs_to_utf8];
	}
	memset(g_pMem_mbcs_to_utf8, 0, sizeof(char)*g_nMem_mbcs_to_utf8);
	encode_ucs_to_utf8(g_pwMem_mbcs_to_utf8, (unsigned char*)g_pMem_mbcs_to_utf8, nUtf8Size, 0);

	strOut = g_pMem_mbcs_to_utf8;
}

void utf8_to_mbcs(const std::string& strIn, std::string& strOut)
{
	//Utf8->Utf16
	int nUtf16Size = (int)strIn.size()+1;
	if(nUtf16Size > g_nwMem_utf8_to_mbcs)
	{
		//safe check
		if(nUtf16Size >= g_nSafe_Check) 
		{
			strOut = g_szErrorString_mbcs;
			return;
		}

		//realloc memory
		g_nwMem_utf8_to_mbcs = nUtf16Size;

		//up bound 4;
		while(g_nwMem_utf8_to_mbcs%4) g_nwMem_utf8_to_mbcs++;

		delete[] g_pwMem_utf8_to_mbcs;
		g_pwMem_utf8_to_mbcs = new wchar_t[g_nwMem_utf8_to_mbcs];
	}
	memset(g_pwMem_utf8_to_mbcs, 0, sizeof(wchar_t)*g_nwMem_utf8_to_mbcs);
	encode_utf8_to_ucs((const unsigned char*)strIn.c_str(), g_pwMem_utf8_to_mbcs, g_nwMem_utf8_to_mbcs, 0);

	//Utf16->mbcs
	int nMbcsSize = g_nwMem_utf8_to_mbcs;
	if(nMbcsSize > g_nMem_utf8_to_mbcs)
	{
		//safe check
		if(nMbcsSize >= g_nSafe_Check) 
		{
			strOut = g_szErrorString_mbcs;
			return;
		}

		//realloc memory
		g_nMem_utf8_to_mbcs = nMbcsSize;

		//up bound 4;
		while(g_nMem_utf8_to_mbcs%4) g_nMem_utf8_to_mbcs++;

		delete[] g_pMem_utf8_to_mbcs;
		g_pMem_utf8_to_mbcs = new char[g_nMem_utf8_to_mbcs];
	}
	memset(g_pMem_utf8_to_mbcs, 0, sizeof(char)*g_nMem_utf8_to_mbcs);
	ucs16_to_mbcs(g_nDefaulat_code_page, g_pwMem_utf8_to_mbcs, (int)wcslen(g_pwMem_utf8_to_mbcs), 
		g_pMem_utf8_to_mbcs, g_nMem_utf8_to_mbcs);

	strOut = g_pMem_utf8_to_mbcs;
}

//mbcs->ucs16
void mbcs_to_ucs16(int code_page, const char* mbcs_string, int mbcs_char_counts, wchar_t* wchar_buf, int wchar_buf_size)
{
	switch(code_page)
	{
	//convert vscii to unicode16
	case 1258:
		vscii_to_ucs16(mbcs_string, mbcs_char_counts, wchar_buf, wchar_buf_size);
		break;
	case 950:
		::MultiByteToWideChar(936, 0, mbcs_string, mbcs_char_counts, wchar_buf, wchar_buf_size);
		break ;
	case 936:
	default:
		::MultiByteToWideChar(code_page, 0, mbcs_string, mbcs_char_counts, wchar_buf, wchar_buf_size);
		break;
	}
}

//ucs16->mbcs
void ucs16_to_mbcs(int code_page, const wchar_t* wchar_string, int wchar_char_counts, char* mbcs_buf, int mbcs_buf_size)
{
	switch(code_page)
	{
	//convert vscii to unicode16
	case 1258:
		ucs16_to_vscii(wchar_string, wchar_char_counts, mbcs_buf, mbcs_buf_size);
		break;
	case 950:
		{
			::WideCharToMultiByte( 936, 0, wchar_string, wchar_char_counts, mbcs_buf, mbcs_buf_size, 0, 0);
		}
		break ;
	case 936:
	default:
		::WideCharToMultiByte( code_page, 0, wchar_string, wchar_char_counts, mbcs_buf, mbcs_buf_size, 0, 0);
		break;
	}
}

static const wchar_t g_vscii_code_page[0x100] = 
{   0x0000, 0x0001, 0x1EB2, 0x0003, 0x0004, 0x1EB4, 0x1EAA, 0x0007,
    0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
    0x0010, 0x0011, 0x0012, 0x0013, 0x1EF6, 0x0015, 0x0016, 0x0017,
    0x0018, 0x1EF8, 0x001a, 0x001b, 0x001c, 0x001d, 0x1EF4, 0x001f,
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
    0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
    0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
    0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
    0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
    0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
    0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
    0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
    0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x007f,
    0x1EA0, 0x1EAE, 0x1EB0, 0x1EB6, 0x1EA4, 0x1EA6, 0x1EA8, 0x1EAC,
    0x1EBC, 0x1EB8, 0x1EBE, 0x1EC0, 0x1EC2, 0x1EC4, 0x1EC6, 0x1ED0,
    0x1ED2, 0x1ED4, 0x1ED6, 0x1ED8, 0x1EE2, 0x1EDA, 0x1EDC, 0x1EDE,
    0x1ECA, 0x1ECE, 0x1ECC, 0x1EC8, 0x1EE6, 0x0168, 0x1EE4, 0x1EF2,
    0x00D5, 0x1EAF, 0x1EB1, 0x1EB7, 0x1EA5, 0x1EA7, 0x1EA9, 0x1EAD,
    0x1EBD, 0x1EB9, 0x1EBF, 0x1EC1, 0x1EC3, 0x1EC5, 0x1EC7, 0x1ED1,
    0x1ED3, 0x1ED5, 0x1ED7, 0x1EE0, 0x01A0, 0x1ED9, 0x1EDD, 0x1EDF,
    0x1ECB, 0x1EF0, 0x1EE8, 0x1EEA, 0x1EEC, 0x01A1, 0x1EDB, 0x01AF,
    0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x1EA2, 0x0102, 0x1EB3, 0x1EB5,
    0x00C8, 0x00C9, 0x00CA, 0x1EBA, 0x00CC, 0x00CD, 0x0128, 0x1EF3,
    0x0110, 0x1EE9, 0x00D2, 0x00D3, 0x00D4, 0x1EA1, 0x1EF7, 0x1EEB,
    0x1EED, 0x00D9, 0x00DA, 0x1EF9, 0x1EF5, 0x00DD, 0x1EE1, 0x01B0,
    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x1EA3, 0x0103, 0x1EEF, 0x1EAB,
    0x00E8, 0x00E9, 0x00EA, 0x1EBB, 0x00EC, 0x00ED, 0x0129, 0x1EC9,
    0x0111, 0x1EF1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x1ECF, 0x1ECD,
    0x1EE5, 0x00F9, 0x00FA, 0x0169, 0x1EE7, 0x00FD, 0x1EE3, 0x1EEE
};

#ifdef __SGI_STL_PORT
	typedef std::hash_map< wchar_t, unsigned char>	Ucs16_to_vscii_map
#else
	typedef stdext::hash_map< wchar_t, unsigned char> Ucs16_to_vscii_map;
#endif

//vscii->unicode16
void vscii_to_ucs16(const char* mbcs_string, int mbcs_char_counts, wchar_t* wchar_buf, int wchar_buf_size)
{
	register int i;
	for(i=0; i<mbcs_char_counts; i++)
	{
		if(i>=wchar_buf_size-1) break;
		wchar_buf[i] = g_vscii_code_page[(unsigned char)mbcs_string[i]];
	}
	wchar_buf[i]=0;
}

//ucs16->vscii
void ucs16_to_vscii(const wchar_t* wchar_string, int wchar_char_counts, char* mbcs_string, int mbcs_string_size)
{
	static bool s_vscii_map_build=false;
	static Ucs16_to_vscii_map g_ucs16_2_vscii_map;
	const static char defaultChar = '?';
	
	unsigned int i=0;
	if(!s_vscii_map_build)
	{
		for(i=0; i<0x100; i++)
		{
			g_ucs16_2_vscii_map.insert(std::make_pair(g_vscii_code_page[i], (unsigned char)i));
		}
		s_vscii_map_build = true;
	}

	for(register int i=0; i<wchar_char_counts; i++)
	{
		if(i>=mbcs_string_size) break;
		Ucs16_to_vscii_map::iterator it = g_ucs16_2_vscii_map.find(wchar_string[i]);
		char ch = (it==g_ucs16_2_vscii_map.end()) ? defaultChar : (char)(it->second);
		mbcs_string[i] = ch;
	}

	mbcs_string[i]=0;
}
