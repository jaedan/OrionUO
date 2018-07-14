

#include "stdafx.h"

CGUIBulletinBoardObject::CGUIBulletinBoardObject(int serial, int x, int y, const wstring &text)
    : CBaseGUI(GOT_BB_OBJECT, serial, 0, 0, x, y)
    , Text(text)
{
    WISPFUN_DEBUG("c43_f1");
    MoveOnDrag = true;

    g_FontManager.GenerateW(1, m_Texture, text, 0);
}

CGUIBulletinBoardObject::~CGUIBulletinBoardObject()
{
    WISPFUN_DEBUG("c43_f2");
    m_Texture.Clear();
}

void CGUIBulletinBoardObject::PrepareTextures()
{
    WISPFUN_DEBUG("c43_f5");
    g_Orion.ExecuteGump(0x1523);
}

void CGUIBulletinBoardObject::Draw(bool checktrans)
{
    WISPFUN_DEBUG("c43_f3");
    CGLTexture *th = g_Orion.ExecuteGump(0x1523);

    if (th != NULL)
        th->Draw(m_X, m_Y, checktrans);

    m_Texture.Draw(m_X + 23, m_Y + 1);
}

bool CGUIBulletinBoardObject::Select()
{
    WISPFUN_DEBUG("c43_f4");
    int x = g_MouseManager.Position.X - m_X;
    int y = g_MouseManager.Position.Y - m_Y;

    return (x >= 0 && y >= 0 && x < 230 && y < 18);
}
