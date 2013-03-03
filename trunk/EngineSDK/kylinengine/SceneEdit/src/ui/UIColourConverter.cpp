/********************************************************************
filename:   UIColourConverter.cpp

purpose:    ���ڱ�ʾ��ɫ�����ַ�����һ����0-255��һ����0-1���������
�ļ��еĸ�����������ת�������ֱ�ʾ����
*********************************************************************/

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "MiniMapMakerDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "UIColourConverter.h"

//----------------------------------------------------------------------------
void FColourToIColour(float fr, float fg, float fb, 
                      unsigned char& ir, unsigned char& ig, unsigned char& ib )
{
    ir = (unsigned char)(fr * 255);
    ig = (unsigned char)(fg * 255);
    ib = (unsigned char)(fb * 255);
}
//----------------------------------------------------------------------------
void IColourToFColour(unsigned char ir, unsigned char ig, unsigned char ib,
                      float& fr, float& fg, float& fb)
{
    fr = ir / 255.0f;
    fg = ig / 255.0f;
    fb = ib / 255.0f;
}