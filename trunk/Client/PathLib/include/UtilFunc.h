#ifndef __UtilFunc_H__
#define __UtilFunc_H__

#include "PathLibType.h"

#include "Grid.h"
#include "Triangle.h"

namespace PathLib
{
    /** ֱ���ཻ
    */
    bool intersectWith(
        const Vector2&		A,		// ֱ��1�Ŀ�ʼ�㡣
        const Vector2&		B,			// ֱ��1�Ľ����㡣
        const Vector2&		C,		// ֱ��2�Ŀ�ʼ�㡣
        const Vector2&		D,			// ֱ��2�Ľ����㡣
        float&			fcrossx,		// �ҵ��Ľ���x��
        float&			fcrossy			// �ҵ��Ľ���y
        );

    /// �жϵ��Ƿ���grid��
    bool pointInGrid(const Vector2& p, const Grid& grid);
    /// �ж��������Ƿ�����grid
    bool intersectGridTriangle(const Grid& grid, const Triangle& triangle);
    /// �ж�edge�Ƿ�����grid
    bool edgeInGrid(const Vector2& p1, const Vector2& p2, const Grid& grid);
}

#endif