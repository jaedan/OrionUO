

#include "stdafx.h"

CGUIGlobalColor::CGUIGlobalColor(bool enabled, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    : CBaseGUI(GOT_GLOBAL_COLOR, 0, 0, 0, 0, 0)
    , R(r)
    , G(g)
    , B(b)
    , A(a)
{
    Enabled = enabled;
}

CGUIGlobalColor::~CGUIGlobalColor()
{
}

void CGUIGlobalColor::Draw(bool checktrans)
{
    WISPFUN_DEBUG("c55_f1");
    if (Enabled)
        glColor4f(R, G, B, A);
}
