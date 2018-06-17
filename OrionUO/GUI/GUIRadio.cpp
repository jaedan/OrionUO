

#include "stdafx.h"

CGUIRadio::CGUIRadio(
    int serial, ushort graphic, ushort graphicChecked, ushort graphicDisabled, int x, int y)
    : CGUICheckbox(serial, graphic, graphicChecked, graphicDisabled, x, y)
{
    Type = GOT_RADIO;
}

CGUIRadio::~CGUIRadio()
{
}
