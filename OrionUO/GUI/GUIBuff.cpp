

#include "stdafx.h"

CGUIBuff::CGUIBuff(ushort graphic, int timer, const wstring &text)
    : CGUIDrawObject(GOT_BUFF, 0, graphic, 0, 0, 0)
    , Timer(timer)
    , Text(text)
{
}

CGUIBuff::~CGUIBuff()
{
}

void CGUIBuff::Draw(bool checktrans)
{
    WISPFUN_DEBUG("c42_f1");
    CGLTexture *th = g_Orion.ExecuteGump(GetDrawGraphic());

    if (th != NULL)
    {
        glColor4ub(0xFF, 0xFF, 0xFF, Alpha);
        th->Draw(m_X, m_Y, checktrans);
    }
}
