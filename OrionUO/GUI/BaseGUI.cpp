

#include "stdafx.h"

CBaseGUI::CBaseGUI(GUMP_OBJECT_TYPE type, int serial, ushort graphic, ushort color, int x, int y)
    : CRenderObject(serial, graphic, color, x, y)
    , Type(type)
{
}

CBaseGUI::~CBaseGUI()
{
}
