

#include "stdafx.h"

CCustomHouseMultiObject::CCustomHouseMultiObject(
    ushort graphic, ushort color, short x, short y, char z, int flags)
    : CMultiObject(graphic, x, y, z, flags)
{
    WISPFUN_DEBUG("");
    Color = color;
}

CCustomHouseMultiObject::~CCustomHouseMultiObject()
{
}
