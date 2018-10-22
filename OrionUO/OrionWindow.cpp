

#include "stdafx.h"
#include <tchar.h>
#include <iostream>

COrionWindow g_OrionWindow;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    WISPFUN_DEBUG("c_main");
    INITLOGGER("uolog.txt");

    string path = g_App.ExeFilePath("crashlogs");
    CreateDirectoryA(path.c_str(), NULL);

    SYSTEMTIME st;
    GetLocalTime(&st);

    char buf[100] = { 0 };

    sprintf_s(
        buf,
        "\\crash_%i_%i_%i___%i_%i_%i_%i.txt",
        st.wYear,
        st.wMonth,
        st.wDay,
        st.wHour,
        st.wMinute,
        st.wSecond,
        st.wMilliseconds);

    path += buf;

    INITCRASHLOGGER(path.c_str());

    if (!g_OrionWindow.Create(
            hInstance,
            L"Orion UO Client",
            L"Ultima Online",
            true,
            640,
            480,
            LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ORIONUO)),
            LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1))))
        return 0;

    g_OrionWindow.ShowWindow(true);
    g_OrionWindow.NoResize = true;

    g_Orion.LoadPluginConfig();

    return g_App.Run(hInstance);
}

COrionWindow::COrionWindow()
    : WISP_WINDOW::CWindow()
{
    TitleBar = (char *)malloc(1024);
}

COrionWindow::~COrionWindow()
{
    free(TitleBar);
}

void COrionWindow::SetTitle(const string &text)
{
    if (!TitleActive && ::SetWindowTextA(Handle, text.c_str()))
        OldTitlePresent = true;
    else if (OldTitlePresent && ::SetWindowTextA(Handle, ""))
        OldTitlePresent = false;
}

void COrionWindow::SetTitle(const wstring &text)
{
    if (!TitleActive && ::SetWindowTextW(Handle, text.c_str()))
        OldTitlePresent = true;
    else if (OldTitlePresent && ::SetWindowTextW(Handle, L""))
        OldTitlePresent = false;
}

void COrionWindow::SetRenderTimerDelay(int delay)
{
    WISPFUN_DEBUG("c195_f1");
    WISP_THREADED_TIMER::CThreadedTimer *timer = GetThreadedTimer(RENDER_TIMER_ID);

    if (timer == NULL)
    {
        return;
    }

    if (delay == 0)
    {
        timer->Pause();
        return;
    }

    timer->ChangeDelay(delay);
    timer->Resume();
}

bool COrionWindow::OnCreate()
{
    WISPFUN_DEBUG("c195_f2");
    if (!g_GL.Install())
    {
        LOG("Error install OpenGL\n");
        MessageBoxA(Handle, "Error install OpenGL", "Error install OpenGL!", MB_OK);
        return false;
    }

    if (!g_Orion.Install())
        return false;

    g_GL.UpdateRect();

    CreateThreadedTimer(RENDER_TIMER_ID, FRAME_DELAY_ACTIVE_WINDOW, false, true, true);

    CreateTimer(UPDATE_TIMER_ID, 10);

    return true;
}

void COrionWindow::OnDestroy()
{
    WISPFUN_DEBUG("c195_f3");
    g_Orion.Uninstall();

    WISP_LOGGER::g_WispCrashLogger.Close();
    remove(WISP_LOGGER::g_WispCrashLogger.FileName.c_str());
}

void COrionWindow::OnResize(WISP_GEOMETRY::CSize &newSize)
{
    WISPFUN_DEBUG("c195_f4");
    g_GL.UpdateRect();
    if (TitleActive)
        RedrawTitleBar();
}

void COrionWindow::EmulateOnLeftMouseButtonDown()
{
    WISPFUN_DEBUG("c195_f5");
    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
    {
        g_CurrentScreen->Render(false);

        g_PressedObject.InitLeft(g_SelectedObject);

        if (g_SelectedObject.Object != NULL || g_GameState == GS_GAME)
        {
            WISP_MOUSE::g_WispMouse->LeftDropPosition = WISP_MOUSE::g_WispMouse->Position;

            g_CurrentScreen->OnLeftMouseButtonDown();
        }
    }
}

void COrionWindow::OnLeftMouseButtonDown()
{
    WISPFUN_DEBUG("c195_f6");
    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
    {
        g_GeneratedMouseDown = false;

        g_CurrentScreen->Render(false);

        g_PressedObject.InitLeft(g_SelectedObject);

        if (g_SelectedObject.Object != NULL || g_GameState == GS_GAME)
            g_CurrentScreen->OnLeftMouseButtonDown();
    }
}

void COrionWindow::OnLeftMouseButtonUp()
{
    WISPFUN_DEBUG("c195_f7");
    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
    {
        g_CurrentScreen->Render(false);

        if ((g_SelectedObject.Object != NULL && g_SelectedObject.Serial) || g_GameState >= GS_GAME)
        {
            g_CurrentScreen->OnLeftMouseButtonUp();

            if (g_MovingFromMouse && g_PressedObject.LeftGump == NULL)
                g_AutoMoving = true;
        }

        if (g_PressedObject.LeftObject != NULL && g_PressedObject.LeftObject->IsGUI() &&
            ((CBaseGUI *)g_PressedObject.LeftObject)->Type == GOT_COMBOBOX &&
            g_PressedObject.LeftGump != NULL)
            g_PressedObject.LeftGump->WantRedraw = true;

        g_PressedObject.ClearLeft();
    }
}

bool COrionWindow::OnLeftMouseButtonDoubleClick()
{
    WISPFUN_DEBUG("c195_f8");
    bool result = false;

    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
    {
        g_CurrentScreen->Render(false);

        g_PressedObject.InitLeft(g_SelectedObject);

        result = (g_SelectedObject.Object != NULL &&
                  g_SelectedObject.Object == g_PressedObject.LeftObject) &&
                 g_CurrentScreen->OnLeftMouseButtonDoubleClick();

        if (result)
        {
            g_PressedObject.ClearLeft();
            g_ClickObject.Clear();
        }
    }

    return result;
}

void COrionWindow::OnRightMouseButtonDown()
{
    WISPFUN_DEBUG("c195_f9");
    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
    {
        g_CurrentScreen->Render(false);

        g_PressedObject.InitRight(g_SelectedObject);

        g_CurrentScreen->OnRightMouseButtonDown();

        if (g_SelectedObject.Gump == NULL &&
            !(g_MouseManager.Position.X < g_ConfigManager.GameWindowX ||
              g_MouseManager.Position.Y < g_ConfigManager.GameWindowY ||
              g_MouseManager.Position.X >
                  (g_ConfigManager.GameWindowX + g_ConfigManager.GameWindowWidth) ||
              g_MouseManager.Position.Y >
                  (g_ConfigManager.GameWindowY + g_ConfigManager.GameWindowHeight)))
        {
            g_MovingFromMouse = true;
            g_AutoMoving = false;
        }
    }
}

void COrionWindow::OnRightMouseButtonUp()
{
    WISPFUN_DEBUG("c195_f10");
    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
    {
        g_CurrentScreen->Render(false);

        if ((g_SelectedObject.Object != NULL &&
             g_SelectedObject.Object == g_PressedObject.RightObject && g_SelectedObject.Serial) ||
            g_GameState >= GS_GAME)
            g_CurrentScreen->OnRightMouseButtonUp();

        g_MovingFromMouse = false;
        g_PressedObject.ClearRight();
    }
}

bool COrionWindow::OnRightMouseButtonDoubleClick()
{
    WISPFUN_DEBUG("c195_f11");
    bool result = false;

    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
    {
        g_CurrentScreen->Render(false);

        g_PressedObject.InitRight(g_SelectedObject);

        result = (g_SelectedObject.Object != NULL &&
                  g_SelectedObject.Object == g_PressedObject.RightObject) &&
                 g_CurrentScreen->OnRightMouseButtonDoubleClick();

        if (result)
            g_PressedObject.ClearRight();
    }

    return result;
}

void COrionWindow::OnMidMouseButtonDown()
{
    WISPFUN_DEBUG("c195_f12");
    if (g_GameState == GS_GAME)
    {
        if (g_PluginManager.MouseButtonDown(UOMSG_MOUSE_BUTTON_MID))
            return;
    }

    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
        g_CurrentScreen->OnMidMouseButtonDown();
}

void COrionWindow::OnMidMouseButtonUp()
{
    WISPFUN_DEBUG("c195_f13");
    if (g_GameState == GS_GAME)
    {
        if (g_PluginManager.MouseButtonUp(UOMSG_MOUSE_BUTTON_MID))
            return;
    }

    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
        g_CurrentScreen->OnMidMouseButtonUp();
}

bool COrionWindow::OnMidMouseButtonDoubleClick()
{
    WISPFUN_DEBUG("c195_f14");
    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
        return g_CurrentScreen->OnMidMouseButtonDoubleClick();

    return false;
}

void COrionWindow::OnMidMouseButtonScroll(bool up)
{
    WISPFUN_DEBUG("c195_f15");
    if (g_GameState == GS_GAME)
    {
        if (g_PluginManager.MouseWheel(up))
            return;
    }

    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
    {
        g_CurrentScreen->Render(false);

        g_CurrentScreen->OnMidMouseButtonScroll(up);
    }
}

void COrionWindow::OnXMouseButton(ushort button)
{
    WISPFUN_DEBUG("c195_f15");
    if (g_GameState == GS_GAME)
    {
        g_PluginManager.MouseButtonDown(
            button == 0x1 ? UOMSG_MOUSE_BUTTON_XTRA1 : UOMSG_MOUSE_BUTTON_XTRA2);
    }
}

void COrionWindow::OnDragging()
{
    WISPFUN_DEBUG("c195_f16");
    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
        g_CurrentScreen->OnDragging();
}

void COrionWindow::OnActivate()
{
    WISPFUN_DEBUG("c195_f17");
    WindowActive = true;
    g_Orion.ResumeSound();

    if (TitleActive)
        RedrawTitleBar();
}

void COrionWindow::OnDeactivate()
{
    WISPFUN_DEBUG("c195_f18");
    WindowActive = false;
    if (!g_ConfigManager.BackgroundSound)
        g_Orion.PauseSound();

    if (TitleActive)
        RedrawTitleBar();
}

void COrionWindow::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c195_f19");

    if ((iswprint((wchar_t)wParam) ||
         (g_GameState >= GS_GAME && (wParam == 0x11 || wParam == 0x17))) &&
        g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
        g_CurrentScreen->OnCharPress(wParam, lParam);
    else if (wParam == VK_RETURN)
        g_CurrentScreen->OnKeyDown(wParam, lParam);
    else if (wParam == 0x16 && g_EntryPointer != NULL)
    {
        if (g_GameState == GS_MAIN)
            g_MainScreen.Paste();
        else
            g_EntryPointer->Paste();
    }
}

bool COrionWindow::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c195_f20");
    if (g_GameState == GS_GAME)
    {
        if (g_PluginManager.KeyDown(wParam))
            return true;
    }

    if (wParam != VK_RETURN && g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
        g_CurrentScreen->OnKeyDown(wParam, lParam);

    return false;
}

bool COrionWindow::OnKeyUp(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c195_f21");
    if (g_GameState == GS_GAME)
    {
        if (g_PluginManager.KeyUp(wParam))
            return true;
    }

    if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
        g_CurrentScreen->OnKeyUp(wParam, lParam);

    if (wParam == 0x2C)
        g_ScreenshotBuilder.SaveScreen();

    return false;
}

void COrionWindow::OnRepaint()
{
    WISPFUN_DEBUG("c195_f22");
    if (TitleActive)
        RedrawTitleBar();
}

void COrionWindow::OnShow(bool show)
{
    WISPFUN_DEBUG("c195_f23");

    if (show)
    {
        SetRenderTimerDelay(g_FrameDelay);
        g_Orion.ResumeSound();
    }
    else
    {
        SetRenderTimerDelay(0);
        g_Orion.PauseSound();
    }
    if (TitleActive)
        RedrawTitleBar();
}

void COrionWindow::OnMaximize()
{
    SetRenderTimerDelay(g_FrameDelay);
    g_Orion.ResumeSound();
}

void COrionWindow::OnMinimize()
{
    SetRenderTimerDelay(0);
    g_Orion.PauseSound();
}

void COrionWindow::OnSetText(const LPARAM &lParam)
{
    WISPFUN_DEBUG("c195_f24");
    if (TitleActive)
        RedrawTitleBar();
}

void COrionWindow::OnTimer(uint id)
{
    WISPFUN_DEBUG("c195_f25");
    if (id == UPDATE_TIMER_ID)
    {
        g_Ticks = timeGetTime();
        g_Orion.Process(false);
    }
    if (id == FASTLOGIN_TIMER_ID)
    {
        RemoveTimer(FASTLOGIN_TIMER_ID);
        g_Orion.Connect();
    }
}

void COrionWindow::OnThreadedTimer(uint nowTime, WISP_THREADED_TIMER::CThreadedTimer *timer)
{
    WISPFUN_DEBUG("c195_f26");
    g_Ticks = nowTime;

    if (timer->TimerID == RENDER_TIMER_ID)
    {
        g_Orion.Process(true);
    }
    else if (timer->TimerID == UPDATE_TIMER_ID)
    {
        g_Orion.Process(false);
    }
}

LRESULT COrionWindow::OnUserMessages(int message, const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c195_f27");
    switch (message)
    {
        case ASSISTANTMSG_RECV:
            g_PacketManager.SavePluginReceivePacket((PBYTE)wParam, (int)lParam);
            return S_OK;
        case ASSISTANTMSG_SEND:
        {
            uint ticks = g_Ticks;
            puchar buf = (puchar)wParam;
            int size = (int)lParam;
            g_TotalSendSize += size;

            CPacketInfo &type = g_PacketManager.GetInfo(*buf);

            LOG("--- ^(%d) s(+%d => %d) Plugin->Server:: %s\n",
                ticks - g_LastPacketTime,
                size,
                g_TotalSendSize,
                type.Name);

            g_LastPacketTime = ticks;
            g_LastSendTime = ticks;

            if (*buf == 0x80 || *buf == 0x91)
            {
                LOG_DUMP(buf, 1);
                LOG("**** ACCOUNT AND PASSWORD CENSORED ****\n");
            }
            else
                LOG_DUMP(buf, size);

            g_ConnectionManager.Send((PBYTE)wParam, (int)lParam);

            return S_OK;
        }
        case ASSISTANTMSG_CAST_SPELL:
            g_Orion.CastSpell(wParam);
            break;
        case ASSISTANTMSG_ATTACK:
            g_Orion.Attack(wParam);
            break;
        case ASSISTANTMSG_TITLEBAR:
        {
            int len = wParam;
            char *c = (char *)lParam;
            TitleActive = len > 0;
            if (c != NULL)
            {
                memcpy(TitleBar, c, len);
            }
            else
            {
                TitleBar[0] = '\0';
                TitleActive = false;
            }
            if (TitleActive && OldTitlePresent && ::SetWindowTextW(Handle, L""))
                OldTitlePresent = false;
            RedrawTitleBar();
            break;
        }
        case CPingThread::MessageID:
        {
            PING_INFO_DATA *info = (PING_INFO_DATA *)wParam;

            if (info != NULL)
            {
                if (info->ServerID == 0xFFFFFFFF)
                {
                    memcpy(&g_GameServerPingInfo, info, sizeof(g_GameServerPingInfo));
                    char ping[50] = { 0 };
                    sprintf_s(
                        ping,
                        "ping(min:%i max:%i avg:%i lost:%i) ",
                        g_GameServerPingInfo.Min,
                        g_GameServerPingInfo.Max,
                        g_GameServerPingInfo.Average,
                        g_GameServerPingInfo.Lost);
                    g_PingString = ping;
                }
                else
                {
                    CServer *server = g_ServerList.GetServer(info->ServerID);

                    if (server != NULL)
                    {
                        if (info->Min < 9999)
                        {
                            server->Ping = info->Average;

                            server->PacketsLoss = info->Lost;

                            g_ServerScreen.UpdateContent();
                        }
                    }
                }

                LOG("Ping info: id:%i min:%i max:%i average:%i lost:%i\n",
                    info->ServerID,
                    info->Min,
                    info->Max,
                    info->Average,
                    info->Lost);
            }

            break;
        }
        default:
            break;
    }

    return S_OK;
}

void COrionWindow::OnMainScreen()
{
    TitleBar[0] = '\0';
    TitleBar[1] = '\0';
    TitleActive = false;
    OldTitlePresent = false;
}

inline int COrionWindow::GetHex2(LPCSTR hex)
{
    int num = 0;
    if (!isxdigit(hex[0]) || !isxdigit(hex[1]))
        return -1;

    num = isdigit(hex[1]) ? (hex[1] - '0') : (tolower(hex[1]) - 'a' + 10);
    num += (isdigit(hex[0]) ? (hex[0] - '0') : (tolower(hex[0]) - 'a' + 10)) * 16;

    return num;
}

inline int COrionWindow::GetHex4(LPCSTR hex)
{
    return (GetHex2(hex) << 8) | GetHex2(&hex[2]);
}

void COrionWindow::DoStat(HDC hDC, int v, int t, int l, int h, int w)
{
    if (w <= 0)
        return;
    RECT fill;
    fill.top = t;
    fill.left = l;
    fill.bottom = t + h;
    fill.right = l + w;
    HBRUSH hBr = NULL;

    if (v == 0xFF)                                //poisoned
        hBr = CreateSolidBrush(RGB(255, 128, 0)); // orange
    else if (v <= 25)
        hBr = CreateSolidBrush(RGB(255, 0, 0)); // red
    else if (v <= 75)
        hBr = CreateSolidBrush(RGB(255, 255, 0)); // yellow
    else
        hBr = CreateSolidBrush(RGB(0, 255, 0)); // green

    FillRect(hDC, &fill, hBr);
    DeleteObject(hBr);
}

int COrionWindow::DrawStatBar(HDC hDC, RECT rect, int width, int status, int hp, int mn, int st)
{
    HGDIOBJ hOld = NULL;
    POINT pt[2];
    rect.right = rect.left + width + 2;

    int o = (rect.bottom - rect.top - 20) / 2;
    if (o > 0)
        rect.top += o;
    rect.bottom = rect.top + 6 + 6 + 6 + 1;

    hOld = SelectObject(hDC, GetStockObject(BLACK_PEN));
    FrameRect(hDC, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

    pt[0].x = rect.left;
    pt[1].x = rect.right;

    pt[0].y = rect.top + 6;
    pt[1].y = rect.top + 6;
    Polyline(hDC, pt, 2);

    pt[0].y = rect.top + 6 + 6;
    pt[1].y = rect.top + 6 + 6;
    Polyline(hDC, pt, 2);

    if (status == 1) // poisoned
        DoStat(hDC, 0xFF, rect.top + 1, rect.left + 1, 5, (int)((double(hp + 1) / 100.0) * width));
    else
        DoStat(hDC, hp, rect.top + 1, rect.left + 1, 5, (int)((double(hp + 1) / 100.0) * width));

    DoStat(hDC, mn, rect.top + 1 + 6, rect.left + 1, 5, (int)((double(mn + 1) / 100) * width));
    DoStat(hDC, st, rect.top + 2 + 6 + 5, rect.left + 1, 5, (int)((double(st + 1) / 100) * width));

    SelectObject(hDC, hOld);
    return width + 2;
}

void COrionWindow::DrawColorTitleBar(
    HTHEME hTheme, HDC hOutDC, bool maximized, LPCSTR str, int len, RECT orig)
{
    const COLORREF def = GetSysColor(WindowActive ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT);
    COLORREF color = def;

    RECT rect;
    RECT window;
    GetWindowRect(g_OrionWindow.Handle, &window);
    rect.left = rect.top = 0;
    rect.right = window.right - window.left;
    rect.bottom = (orig.bottom - orig.top);

    HDC hDC = CreateCompatibleDC(hOutDC);
    HBITMAP hBmp = CreateCompatibleBitmap(hOutDC, rect.right, rect.bottom);
    SelectObject(hDC, hBmp);

    bool needRegFill = true;
    HFONT hFont = NULL;

    if (hTheme)
    {
        LOGFONTW lf;

        if (maximized)
        {
            needRegFill = DrawThemeBackground(
                              hTheme,
                              hDC,
                              WP_MAXCAPTION,
                              WindowActive ? MXCS_ACTIVE : MXCS_INACTIVE,
                              &rect,
                              NULL) != S_OK;
        }
        else
        {
            int modTop = GetSystemMetrics(SM_CYFRAME);
            rect.top -= modTop;
            needRegFill =
                DrawThemeBackground(
                    hTheme, hDC, WP_CAPTION, WindowActive ? CS_ACTIVE : CS_INACTIVE, &rect, NULL) !=
                S_OK;
            rect.top += modTop;
        }

        if (GetThemeSysFont(hTheme, TMT_CAPTIONFONT, &lf) == S_OK)
        {
            hFont = CreateFontIndirectW(&lf);
            SelectObject(hDC, hFont);
        }
    }

    if (needRegFill)
        FillRect(
            hDC,
            &rect,
            GetSysColorBrush(WindowActive ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION));

    if (hFont == NULL)
        SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));

    rect.left = orig.left;

    int start = 0;
    int dlen = 0;
    int t;
    SetBkMode(hDC, TRANSPARENT);
    for (int i = 0; i < len; i++)
    {
        if (rect.left >= rect.right)
            break;

        if (str[i] == '~')
        {
            switch (str[i + 1])
            {
                case '#':
                    if (dlen > 0)
                    {
                        SetTextColor(hDC, color);
                        DrawTextA(
                            hDC, &str[start], dlen, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        SIZE ext;
                        GetTextExtentPoint32A(hDC, &str[start], dlen, &ext);
                        rect.left += ext.cx;
                    }
                    dlen = 0;

                    if (str[i + 2] == '~')
                    {
                        color = def;
                        SetBkMode(hDC, TRANSPARENT);
                        start = i + 3;
                        i += 2;
                    }
                    else
                    {
                        color =
                            RGB(GetHex2(&str[i + 2]), GetHex2(&str[i + 4]), GetHex2(&str[i + 6]));
                        start = i + 8;
                        i += 7;
                    }

                    break;
                case 'S':
                case 's':
                    // ~SSFRRGGBB
                    if (dlen > 0)
                    {
                        SetTextColor(hDC, color);
                        DrawTextA(
                            hDC, &str[start], dlen, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        SIZE ext;
                        GetTextExtentPoint32A(hDC, &str[start], dlen, &ext);
                        rect.left += ext.cx;
                    }
                    dlen = 0;

                    t = 30;
                    switch (toupper(str[i + 2]))
                    {
                        case 'S':
                            t = 15;
                            break;
                            //case 'R': w = 30; break;
                        case 'M':
                            t = 45;
                            break;
                        case 'L':
                            t = 60;
                            break;
                        case 'X':
                            t = 75;
                            break;
                    }

                    rect.left += 1;
                    rect.left += DrawStatBar(
                        hDC,
                        rect,
                        t,
                        str[i + 3] - '0',
                        GetHex2(&str[i + 4]),
                        GetHex2(&str[i + 6]),
                        GetHex2(&str[i + 8]));
                    rect.left += 1;

                    start = i + 10;
                    i += 9;

                    break;
                case 'I':
                case 'i':
                    if (dlen > 0)
                    {
                        SetTextColor(hDC, color);
                        DrawTextA(
                            hDC, &str[start], dlen, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        SIZE ext;
                        GetTextExtentPoint32A(hDC, &str[start], dlen, &ext);
                        rect.left += ext.cx;
                    }
                    dlen = 0;
                    if (str[i + 6] == '~')
                    {
                        rect.left += DrawUOItem(hDC, rect, GetHex4(&str[i + 2]), 0);
                        start = i + 7;
                        i += 6;
                    }
                    else
                    {
                        rect.left +=
                            DrawUOItem(hDC, rect, GetHex4(&str[i + 2]), GetHex4(&str[i + 6]));
                        start = i + 10;
                        i += 9;
                    }
                    break;
                case '^':
                    if (dlen > 0)
                    {
                        SetTextColor(hDC, color);
                        DrawTextA(
                            hDC, &str[start], dlen, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        SIZE ext;
                        GetTextExtentPoint32A(hDC, &str[start], dlen, &ext);
                        rect.left += ext.cx;
                    }
                    dlen = 0;

                    color = RGB(0, 0, 0);
                    SetBkMode(hDC, OPAQUE);
                    SetBkColor(
                        hDC, RGB(GetHex2(&str[i + 2]), GetHex2(&str[i + 4]), GetHex2(&str[i + 6])));

                    start = i + 8;
                    i += 7;
                    break;
                case 'B':
                case 'b': // skip bold identifier
                    i++;
                    break;
            }
        }
        else
        {
            dlen++;
        }
    }

    if (dlen > 0)
    {
        SetTextColor(hDC, color);
        DrawTextA(hDC, &str[start], dlen, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    BitBlt(
        hOutDC,
        orig.left,
        orig.top,
        (orig.right - orig.left),
        (orig.bottom - orig.top) - 1,
        hDC,
        orig.left,
        0,
        SRCCOPY);
    DeleteDC(hDC);
    DeleteObject(hBmp);
    if (hFont)
        DeleteObject(hFont);
}

void COrionWindow::RedrawTitleBar()
{
    bool newNCRP = TitleBar[0] == '\0';

    if (curNCRP != newNCRP)
    {
        DWMNCRENDERINGPOLICY policy = newNCRP ? DWMNCRP_ENABLED : DWMNCRP_DISABLED;
        DwmSetWindowAttribute(
            g_OrionWindow.Handle, DWMWA_NCRENDERING_POLICY, &policy, sizeof(policy));
        curNCRP = newNCRP;
    }

    if (TitleBar[0] == 0)
    {
        TitleActive = false;
        return;
    }

    int len = (int)strlen(TitleBar);
    if (len >= 1024)
        len = 1023;

    WINDOWPLACEMENT place;
    RECT rect;
    HDC hDC = GetWindowDC(g_OrionWindow.Handle);

    GetWindowPlacement(g_OrionWindow.Handle, &place);
    GetWindowRect(g_OrionWindow.Handle, &rect);
    bool maximized = place.showCmd == SW_MAXIMIZE;
    rect.top = GetSystemMetrics(SM_CYFRAME) + (maximized ? 4 : 3);
    rect.bottom = rect.top + GetSystemMetrics(SM_CYCAPTION);

    rect.right =
        (rect.right - rect.left) - (4 * GetSystemMetrics(SM_CXSIZE) + GetSystemMetrics(SM_CXFRAME));
    rect.left = GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXSMICON) + 12;
    HTHEME hTheme = ::OpenThemeData(g_OrionWindow.Handle, L"CompositedWindow::Window");
    if (hTheme)
    {
        DrawColorTitleBar(hTheme, hDC, maximized, TitleBar, len, rect);
        CloseThemeData(hTheme);
    }
    else
    {
        rect.left += GetSystemMetrics(SM_CXFRAME);
        DrawColorTitleBar(NULL, hDC, maximized, TitleBar, len, rect);
    }

    ReleaseDC(g_OrionWindow.Handle, hDC);
}

inline int COrionWindow::Round(float n)
{
    int i = (int)n;
    return i + (n - i >= 0.5 ? 1 : 0);
}

unsigned short *COrionWindow::GetHue(int index)
{
    if (Hues == NULL)
    {
        int length, blockCount, index;
        FILE *huesMul = NULL;

        std::string str = g_App.UOFilesPath("hues.mul");

        huesMul = fopen(str.c_str(), "rb");
        if (!huesMul)
        {
            Hues = new unsigned short *[1];
            Hues[0] = new unsigned short[34];
            memset(Hues[0], 0, 34 * 2);
            NumHues = 1;
            return NULL;
        }

        fseek(huesMul, 0, SEEK_END);
        length = (int)ftell(huesMul);
        fseek(huesMul, 0, SEEK_SET);
        blockCount = length / 708;
        if (blockCount > 375)
            blockCount = 375;
        NumHues = blockCount * 8;

        Hues = new unsigned short *[NumHues];

        index = 0;
        for (int b = 0; b < blockCount; b++)
        {
            fseek(huesMul, 4, SEEK_CUR);

            for (int i = 0; i < 8; i++, index++)
            {
                Hues[index] = new unsigned short[34];
                for (int c = 0; c < 34; c++)
                {
                    unsigned short color;
                    fread(&color, 2, 1, huesMul);
                    Hues[index][c] = color | 0x8000;
                }

                fseek(huesMul, 20, SEEK_CUR); // ignore name
            }
        }
        fclose(huesMul);
    }

    if (index > 0 && index <= NumHues)
        return Hues[index - 1];
    else
        return NULL;
}

unsigned short COrionWindow::ApplyHueToPixel(unsigned short *hue, unsigned short pix)
{
    if (hue)
        return hue[(pix >> 10) & 31];
    else
        return pix;
}

COrionWindow::UOItem *COrionWindow::ReadUOItem(int item, int bh)
{
    if (item == 0 || item >= 0xFFFF)
        return NULL;

    std::string str;
    short *Lookup;
    unsigned short *Run;

    FILE *idxMul, *artMul;
    ArtIdx idx;
    ArtHeader header;
    memset(&header, 0, sizeof(ArtHeader));

    str = g_App.UOFilesPath("Artidx.mul");
    idxMul = fopen(str.c_str(), "rb");
    if (!idxMul)
        return NULL;
    fseek(idxMul, item * sizeof(ArtIdx), SEEK_SET);
    fread(&idx, sizeof(ArtIdx), 1, idxMul);
    fclose(idxMul);
    if (idx.FilePos == -1 || idx.Length == -1)
        return NULL;

    str = g_App.UOFilesPath("Art.mul");
    artMul = fopen(str.c_str(), "rb");
    if (!artMul)
    {
        fclose(idxMul);
        return NULL;
    }
    fseek(artMul, idx.FilePos, SEEK_SET);
    fread(&header, sizeof(ArtHeader), 1, artMul);
    if (header.Height <= 0 || header.Width <= 0 || header.Height >= 1024 || header.Width >= 1024 ||
        header.Unknown > 0xFFFF || header.Unknown == 0)
    {
        fclose(artMul);
        return NULL;
    }

    Run = new unsigned short[header.Width]; // it should never be wider than the whole image!
    Lookup = new short[header.Height];
    fread(Lookup, header.Height * 2, 1, artMul);
    long dataStart = ftell(artMul);

    UOItem *pNew = new UOItem;
    pNew->ItemID = item;
    pNew->pNext = ArtCache;
    ArtCache = pNew;

    unsigned short **Image = new unsigned short *[header.Width];
    for (int i = 0; i < header.Width; i++)
    {
        Image[i] = new unsigned short[header.Height];
        memset(Image[i], 0, header.Height * 2);
    }

    pNew->Left = pNew->Top = 0x7FFFFFFF;
    pNew->Right = pNew->Bottom = 0;
    for (int y = 0; y < header.Height; y++)
    {
        int x = 0;

        fseek(artMul, dataStart + Lookup[y] * 2, SEEK_SET);
        do
        {
            short RunOffset = 0, RunLength = 0;

            fread(&RunOffset, 2, 1, artMul);
            fread(&RunLength, 2, 1, artMul);

            if (RunLength <= 0 || RunOffset < 0 || RunOffset + RunLength >= 2048 ||
                RunLength > header.Width)
                break;

            if (y > pNew->Bottom)
                pNew->Bottom = y;
            if (y < pNew->Top)
                pNew->Top = y;

            x += RunOffset;
            if (x < pNew->Left)
                pNew->Left = x;

            fread(Run, RunLength * 2, 1, artMul);
            for (int o = 0; o < RunLength; o++, x++)
                Image[x][y] = Run[o];

            if (x > pNew->Right)
                pNew->Right = x;
        } while (true);
    }
    fclose(artMul);

    delete[] Run;
    delete[] Lookup;

    float scale = float(bh) / float(pNew->GetHeight());
    if (scale > 1 || scale <= 0)
        scale = 1;

    pNew->RealHeight = (int)(header.Height * scale + 1);
    pNew->RealWidth = (int)(header.Width * scale + 1);
    pNew->Data = new unsigned short *[pNew->RealWidth];
    for (int x = 0; x < pNew->RealWidth; x++)
    {
        pNew->Data[x] = new unsigned short[pNew->RealHeight];
        memset(pNew->Data[x], 0, 2 * pNew->RealHeight);
    }

    for (int x = 0; x < header.Width; x++)
    {
        for (int y = 0; y < header.Height; y++)
            pNew->Data[(int)(x * scale)][(int)(y * scale)] |= Image[x][y];
    }

    pNew->Top = (int)(pNew->Top * scale);
    pNew->Left = (int)(pNew->Left * scale);
    pNew->Bottom = (int)(pNew->Bottom * scale);
    pNew->Right = (int)(pNew->Right * scale);

    for (int x = 0; x < header.Width; x++)
        delete[] Image[x];
    delete[] Image;

    return pNew;
}

COrionWindow::UOItem *COrionWindow::FindItem(int item)
{
    UOItem *node = ArtCache;
    while (node != NULL)
    {
        if (node->ItemID == item)
            return node;
        else
            node = node->pNext;
    }

    return NULL;
}

inline COLORREF COrionWindow::Brightness(int shift, COLORREF c)
{
    return RGB(
        min(255, GetRValue(c) + shift),
        min(255, GetGValue(c) + shift),
        min(255, GetBValue(c) + shift));
}

int COrionWindow::DrawUOItem(HDC hDC, RECT rect, int item, int hueIdx)
{
    item |= 0x4000;

    rect.top++;
    rect.bottom--;
    int maxHeight = rect.bottom - rect.top;

    UOItem *i = FindItem(item);
    if (i == NULL)
        i = ReadUOItem(item, maxHeight);

    if (i == NULL)
        return 0;

    if (i->GetHeight() < maxHeight)
        rect.top += (maxHeight - i->GetHeight()) / 2;

    unsigned short *hue = GetHue(hueIdx);
    for (int x = i->Left; x <= i->Right; x++)
    {
        for (int y = i->Top; y <= i->Bottom; y++)
        {
            if (i->Data[x][y] != 0)
                SetPixel(
                    hDC,
                    rect.left + x - i->Left,
                    rect.top + y - i->Top,
                    Brightness(0x30, Color16to32(ApplyHueToPixel(hue, i->Data[x][y]))));
        }
    }

    return i->GetWidth() + 3;
}

void COrionWindow::FreeItem(UOItem *node)
{
    if (node != NULL)
    {
        FreeItem(node->pNext);
        for (int i = 0; i < node->RealWidth; i++)
            delete[] node->Data[i];
        delete[] node->Data;
        delete node;
    }
}

void COrionWindow::FreeArt()
{
    FreeItem(ArtCache);
    if (Hues && NumHues > 0)
    {
        for (int i = 0; i < NumHues; i++)
            delete[] Hues[i];
        delete[] Hues;
    }
}
