

#include "stdafx.h"

CContainerRect g_ContainerRect;
vector<CContainerOffset> g_ContainerOffset;

void CContainerRect::Calculate(ushort gumpID)
{
    WISPFUN_DEBUG("c187_f1");

    CGLTexture *tex = g_Orion.ExecuteGump(gumpID);

    if (tex != NULL)
    {
        if (!g_ConfigManager.OffsetInterfaceWindows)
        {
            X = DefaultX;
            Y = DefaultY;
        }
        else
        {
            int passed = 0;

            IFOR (i, 0, 4 && !passed)
            {
                if (X + tex->Width + CONTAINERS_RECT_STEP > g_OrionWindow.GetSize().Width)
                {
                    X = CONTAINERS_RECT_DEFAULT_POS;

                    if (Y + tex->Height + CONTAINERS_RECT_LINESTEP > g_OrionWindow.GetSize().Height)
                        Y = CONTAINERS_RECT_DEFAULT_POS;
                    else
                        Y += CONTAINERS_RECT_LINESTEP;
                }

                else if (Y + tex->Height + CONTAINERS_RECT_STEP > g_OrionWindow.GetSize().Height)
                {
                    if (X + tex->Width + CONTAINERS_RECT_LINESTEP > g_OrionWindow.GetSize().Width)
                        X = CONTAINERS_RECT_DEFAULT_POS;
                    else
                        X += CONTAINERS_RECT_LINESTEP;

                    Y = CONTAINERS_RECT_DEFAULT_POS;
                }
                else
                    passed = (int)i + 1;
            }

            if (!passed)
                MakeDefault();
            else if (passed == 1)
            {
                X += CONTAINERS_RECT_STEP;
                Y += CONTAINERS_RECT_STEP;
            }
        }
    }
}
