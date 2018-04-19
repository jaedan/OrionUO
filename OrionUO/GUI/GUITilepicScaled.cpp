

#include "stdafx.h"

CGUITilepicScaled::CGUITilepicScaled(
    ushort graphic, ushort color, int x, int y, int width, int height)
    : CGUITilepic(graphic, color, x, y)
    , Width(width)
    , Height(height)
{
}

CGUITilepicScaled::~CGUITilepicScaled()
{
}

void CGUITilepicScaled::Draw(bool checktrans)
{
    WISPFUN_DEBUG("c82_f1");
    CGLTexture *th = g_Orion.ExecuteStaticArt(Graphic);

    if (th != NULL)
    {
        SetShaderMode();

        g_GL.Draw(*th, m_X, m_Y);
    }
}
