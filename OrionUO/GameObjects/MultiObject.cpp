

#include "stdafx.h"

CMultiObject::CMultiObject(ushort graphic, short x, short y, char z, int flags)
    : CRenderStaticObject(ROT_MULTI_OBJECT, 0, graphic, 0, x, y, z)
    , OnTarget(flags == 2)
{
    WISPFUN_DEBUG("c25_f1");
    OriginalGraphic = graphic;
    UpdateGraphicBySeason();

#if UO_DEBUG_INFO != 0
    g_MultiObjectsCount++;
#endif
}

CMultiObject::~CMultiObject()
{
#if UO_DEBUG_INFO != 0
    g_MultiObjectsCount--;
#endif
}

void CMultiObject::UpdateGraphicBySeason()
{
    WISPFUN_DEBUG("c25_f2");

    Graphic = g_Orion.GetSeasonGraphic(OriginalGraphic);

    {
        Vegetation = g_Orion.IsVegetation(Graphic);
    }
}

void CMultiObject::Draw(int x, int y)
{
    WISPFUN_DEBUG("c25_f3");

    ushort color = Color;

#if UO_DEBUG_INFO != 0
    g_RenderedObjectsCountInGameWindow++;
#endif

    if (OnTarget)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

        g_Orion.DrawStaticArt(Graphic, color, x, y);

        glDisable(GL_BLEND);
    }
    else
    {
        RenderGraphic = Graphic;

        if (g_DeveloperMode == DM_DEBUGGING && g_SelectedObject.Object == this)
            RenderColor = SELECT_MULTI_COLOR;
        else
            RenderColor = color;

        CRenderStaticObject::Draw(x, y);
    }
}

void CMultiObject::Select(int x, int y)
{
    WISPFUN_DEBUG("c25_f4");
    if (!OnTarget)
    {
        RenderGraphic = Graphic;

        CRenderStaticObject::Select(x, y);
    }
}
