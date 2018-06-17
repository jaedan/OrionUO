

#include "stdafx.h"

CGUIHTMLHitBox::CGUIHTMLHitBox(
    CGUIHTMLGump *htmlGump, int serial, int x, int y, int width, int height, bool callOnMouseUp)
    : CGUIHitBox(serial, x, y, width, height, callOnMouseUp)
    , m_HTMLGump(htmlGump)
{
}

CGUIHTMLHitBox::~CGUIHTMLHitBox()
{
}

void CGUIHTMLHitBox::Scroll(bool up, int delay)
{
    WISPFUN_DEBUG("c61_f1");
    if (m_HTMLGump != NULL)
        m_HTMLGump->Scroll(up, delay);
}
