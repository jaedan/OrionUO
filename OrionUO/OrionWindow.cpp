

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
}

COrionWindow::~COrionWindow()
{
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
    g_PluginManager.CloseNotify();

    g_Orion.Uninstall();

    WISP_LOGGER::g_WispCrashLogger.Close();
    remove(WISP_LOGGER::g_WispCrashLogger.FileName.c_str());
}

void COrionWindow::OnResize(WISP_GEOMETRY::CSize &newSize)
{
    WISPFUN_DEBUG("c195_f4");
    g_GL.UpdateRect();
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
    g_Orion.ResumeSound();

    g_PluginManager.ActivateWindow(true);
}

void COrionWindow::OnDeactivate()
{
    WISPFUN_DEBUG("c195_f18");
    if (!g_ConfigManager.BackgroundSound)
        g_Orion.PauseSound();

    g_PluginManager.ActivateWindow(false);
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

HRESULT COrionWindow::OnRepaint(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c195_f22");

    return (HRESULT)DefWindowProc(Handle, WM_NCPAINT, wParam, lParam);
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
