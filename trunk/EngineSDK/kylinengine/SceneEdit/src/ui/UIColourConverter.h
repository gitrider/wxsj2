/********************************************************************
filename:   UIColourConverter.h

purpose:    ���ڱ�ʾ��ɫ�����ַ�����һ����0-255��һ����0-1���������
            �ļ��еĸ�����������ת�������ֱ�ʾ����
*********************************************************************/

#ifndef _UIColourConverter_H_
#define _UIColourConverter_H_

/// �ɸ�������ʾ��ת��������ʾ��
void FColourToIColour(float fr, float fg, float fb, 
                      unsigned char& ir, unsigned char& ig, unsigned char& ib );

/// ��������ʾ��ת�ɸ�������ʾ��
void IColourToFColour(unsigned char ir, unsigned char ig, unsigned char ib,
                      float& fr, float& fg, float& fb);

#endif