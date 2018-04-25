

#include "stdafx.h"

CCreateCharacterScreen g_CreateCharacterScreen;

CCreateCharacterScreen::CCreateCharacterScreen()
    : CBaseScreen(m_CreateCharacterGump)
{
}

CCreateCharacterScreen::~CCreateCharacterScreen()
{
}

void CCreateCharacterScreen::SetStyleSelection(int val)
{
    m_StyleSelection = val;
    m_Gump.WantUpdateContent = true;
}

void CCreateCharacterScreen::SetColorSelection(int val)
{
    m_ColorSelection = val;
    m_Gump.WantUpdateContent = true;
}

void CCreateCharacterScreen::Init()
{
    WISPFUN_DEBUG("c162_f1");
    g_CreateCharacterManager.Clear();

    Name = "";
    m_StyleSelection = 0;
    m_ColorSelection = 0;

    g_ScreenEffectManager.UseSunrise();
    SmoothScreenAction = 0;

    m_Gump.PrepareTextures();
    m_Gump.WantUpdateContent = true;
}

void CCreateCharacterScreen::ProcessSmoothAction(uchar action)
{
    WISPFUN_DEBUG("c162_f2");
    if (action == 0xFF)
        action = SmoothScreenAction;

    if (action == ID_SMOOTH_CCS_QUIT)
        g_OrionWindow.Destroy();
    else if (action == ID_SMOOTH_CCS_GO_SCREEN_CHARACTER)
        g_Orion.InitScreen(GS_CHARACTER);
    else if (action == ID_SMOOTH_CCS_GO_SCREEN_CONNECT)
    {
        CPacketCreateCharacter(Name).Send();
        g_Orion.InitScreen(GS_GAME_CONNECT);
    }
}

void CCreateCharacterScreen::OnLeftMouseButtonDown()
{
    WISPFUN_DEBUG("c162_f3");
    CBaseScreen::OnLeftMouseButtonDown();

    if (g_SelectedObject.Serial == 0)
    {
        if (m_StyleSelection != 0)
        {
            m_StyleSelection = 0;
            m_Gump.WantUpdateContent = true;
        }
    }
}

void CCreateCharacterScreen::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c162_f4");
    if (wParam >= 0x0100 || !g_FontManager.IsPrintASCII((uchar)wParam))
        return;
    else if (g_EntryPointer == NULL)
        return;

    if (g_EntryPointer->Length() < 20)
        g_EntryPointer->Insert((wchar_t)wParam);

    Name = g_EntryPointer->c_str();
    m_Gump.WantRedraw = true;
}

void CCreateCharacterScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c162_f5");
    if (g_EntryPointer != NULL)
    {
        g_EntryPointer->OnKey(&m_Gump, wParam);

        Name = g_EntryPointer->c_str();
        m_Gump.WantRedraw = true;
    }
}
