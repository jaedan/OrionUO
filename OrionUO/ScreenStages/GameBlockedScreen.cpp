

#include "stdafx.h"

CGameBlockedScreen g_GameBlockedScreen;

CGameBlockedScreen::CGameBlockedScreen()
    : CBaseScreen(m_GameBlockedScreenGump)
{
}

CGameBlockedScreen::~CGameBlockedScreen()
{
}

void CGameBlockedScreen::Init()
{
    Code = 0;
}

void CGameBlockedScreen::Render(bool mode)
{
    WISPFUN_DEBUG("c163_f1");
    if (mode)
    {
        g_GumpManager.Draw(true);

        InitToolTip();

        g_MouseManager.Draw(0x2073);
    }
    else
    {
        g_SelectedObject.Clear();

        g_GumpManager.Select(true);

        if (g_SelectedObject.Object != g_LastSelectedObject.Object)
        {
            if (g_SelectedObject.Object != NULL)
                g_SelectedObject.Object->OnMouseEnter();

            if (g_LastSelectedObject.Object != NULL)
                g_LastSelectedObject.Object->OnMouseExit();
        }

        g_LastSelectedObject.Init(g_SelectedObject);
    }
}

void CGameBlockedScreen::OnLeftMouseButtonDown()
{
    WISPFUN_DEBUG("c163_f2");
    if (g_SelectedObject.Gump != NULL)
        g_GumpManager.OnLeftMouseButtonDown(true);
}

void CGameBlockedScreen::OnLeftMouseButtonUp()
{
    WISPFUN_DEBUG("c163_f3");
    if (g_PressedObject.LeftGump != NULL)
        g_GumpManager.OnLeftMouseButtonUp(true);
}

void CGameBlockedScreen::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c163_f4");
    if (g_EntryPointer == NULL || g_EntryPointer == &g_GameConsole)
        return;

    g_GumpManager.OnCharPress(wParam, lParam, true);
}

void CGameBlockedScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c163_f5");
    CGumpNotify *notify = (CGumpNotify *)g_GumpManager.GetGump(0, 0, GT_NOTIFY);

    if (g_EntryPointer == NULL || g_EntryPointer == &g_GameConsole)
    {
        if (wParam == VK_RETURN && notify != NULL)
            notify->OnKeyDown(wParam, lParam);
    }
    else
    {
        CGump *gump = g_GumpManager.GetTextEntryOwner();

        if (gump != NULL && gump->GumpType == GT_TEXT_ENTRY_DIALOG)
            gump->OnKeyDown(wParam, lParam);
        else if (notify != NULL)
            notify->OnKeyDown(wParam, lParam);
    }
}
