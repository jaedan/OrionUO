

#include "stdafx.h"

CCharacterListScreen g_CharacterListScreen;

CCharacterListScreen::CCharacterListScreen()
    : CBaseScreen(m_CharacterListGump)
{
}

CCharacterListScreen::~CCharacterListScreen()
{
}

void CCharacterListScreen::Init()
{
    WISPFUN_DEBUG("c160_f1");

    string title = string("Ultima Online - ") + g_MainScreen.m_Account->c_str();

    CServer *server = g_ServerList.GetSelectedServer();

    if (server != NULL)
        title += "(" + server->Name + ")";

    g_OrionWindow.SetTitle(title);

    g_CharacterList.Selected = 0;

    g_ScreenEffectManager.UseSunrise();
    SmoothScreenAction = 0;

    m_Gump.PrepareTextures();
    m_Gump.WantUpdateContent = true;
}

void CCharacterListScreen::ProcessSmoothAction(uchar action)
{
    WISPFUN_DEBUG("c160_f2");
    if (action == 0xFF)
        action = SmoothScreenAction;

    if (action == ID_SMOOTH_CLS_QUIT)
        g_OrionWindow.Destroy();
    else if (action == ID_SMOOTH_CLS_CONNECT)
        g_Orion.Connect();
    else if (action == ID_SMOOTH_CLS_SELECT_CHARACTER)
    {
        if (!g_CharacterList.GetName(g_CharacterList.Selected).length())
            g_Orion.InitScreen(GS_PROFESSION_SELECT);
        else
            g_Orion.CharacterSelection(g_CharacterList.Selected);
    }
    else if (action == ID_SMOOTH_CLS_GO_SCREEN_PROFESSION_SELECT)
        g_Orion.InitScreen(GS_PROFESSION_SELECT);
    else if (action == ID_SMOOTH_CLS_GO_SCREEN_DELETE)
    {
        if (g_CharacterList.GetSelectedName().length())
        {
            g_Orion.InitScreen(GS_DELETE);
            g_ConnectionScreen.SetType(CST_CHARACTER_LIST);
        }
    }
}

void CCharacterListScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c160_f3");
    m_Gump.OnKeyDown(wParam, lParam);

    if (wParam == VK_RETURN)
        CreateSmoothAction(ID_SMOOTH_CLS_SELECT_CHARACTER);
}
