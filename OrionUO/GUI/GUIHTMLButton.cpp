

#include "stdafx.h"

CGUIHTMLButton::CGUIHTMLButton(
    CGUIHTMLGump *htmlGump,
    int serial,
    ushort graphic,
    ushort graphicSelected,
    ushort graphicPressed,
    int x,
    int y)
    : CGUIButton(serial, graphic, graphicSelected, graphicPressed, x, y)
    , m_HTMLGump(htmlGump)
{
}

CGUIHTMLButton::~CGUIHTMLButton()
{
}

void CGUIHTMLButton::SetShaderMode()
{
    WISPFUN_DEBUG("c59_f1");
    glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);
}

void CGUIHTMLButton::Scroll(bool up, int delay)
{
    WISPFUN_DEBUG("c59_f2");
    if (m_HTMLGump != NULL)
        m_HTMLGump->Scroll(up, delay);
}
