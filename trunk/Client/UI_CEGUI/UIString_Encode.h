

#pragma once

#include <string>


//…Ë÷√¥˙¬Î“≥
void	set_code_page(int code_page);
int		get_code_page(void);
//utf<->mbcs
void	mbcs_to_utf8(const std::string& strIn, std::string& strOut);
void	utf8_to_mbcs(const std::string& strIn, std::string& strOut);
int		encode_ucs_to_utf8(const wchar_t* src, unsigned char* dest, int dest_len, int src_len = 0);
int		encode_utf8_to_ucs(const unsigned char* src, wchar_t* dest, int dest_len, int src_len = 0);
void	mbcs_to_ucs16(int code_page, const char* mbcs_string, int mbcs_string_size, wchar_t* wchar_buf, int wchar_buf_size);
void	ucs16_to_mbcs(int code_page, const wchar_t* wchar_string, int wchar_char_counts, char* mbcs_buf, int mbcs_buf_size);

