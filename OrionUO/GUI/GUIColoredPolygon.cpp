

#include "stdafx.h"

CGUIColoredPolygon::CGUIColoredPolygon(
    int serial, ushort color, int x, int y, int width, int height, int polygonColor)
    : CGUIPolygonal(GOT_COLOREDPOLYGON, x, y, width, height)
{
    WISPFUN_DEBUG("c48_f1");
    Serial = serial;
    UpdateColor(color, polygonColor);
}

CGUIColoredPolygon::~CGUIColoredPolygon()
{
}

void CGUIColoredPolygon::UpdateColor(ushort color, int polygonColor)
{
    WISPFUN_DEBUG("c48_f2");
    Color = color;

    ColorR = GetRValue(polygonColor);
    ColorG = GetGValue(polygonColor);
    ColorB = GetBValue(polygonColor);
    ColorA = polygonColor >> 24;

    if (!ColorA)
        ColorA = 0xFF;
}

void CGUIColoredPolygon::Draw(bool checktrans)
{
    WISPFUN_DEBUG("c48_f3");
    glColor4ub(ColorR, ColorG, ColorB, ColorA);

    if (ColorA < 0xFF)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        g_GL.DrawPolygon(m_X, m_Y, Width, Height);

        glDisable(GL_BLEND);
    }
    else
        g_GL.DrawPolygon(m_X, m_Y, Width, Height);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    if (Focused || (DrawDot && g_GumpSelectedElement == this))
        g_GL.DrawPolygon(m_X + (Width / 2) - 1, m_Y + (Height / 2) - 1, 2, 2);
}

void CGUIColoredPolygon::OnMouseEnter()
{
    WISPFUN_DEBUG("c48_f4");
    if (DrawDot && g_SelectedObject.Gump != NULL)
        g_SelectedObject.Gump->WantRedraw = true;
}

void CGUIColoredPolygon::OnMouseExit()
{
    WISPFUN_DEBUG("c48_f5");
    if (DrawDot && g_LastSelectedObject.Gump != NULL)
        g_LastSelectedObject.Gump->WantRedraw = true;
}
