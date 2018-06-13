/***********************************************************************************
**
** GUIResizeButton.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIResizeButton::CGUIResizeButton(int serial, ushort graphic, ushort graphicSelected, ushort graphicPressed, int x, int y)
: CGUIButton(serial, graphic, graphicSelected, graphicPressed, x, y)
{
	Type = GOT_RESIZEBUTTON;
}
//----------------------------------------------------------------------------------
CGUIResizeButton::~CGUIResizeButton()
{
}
//----------------------------------------------------------------------------------
