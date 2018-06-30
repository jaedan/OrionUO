#include "stdafx.h"

CPluginManager g_PluginManager;

bool __cdecl plugin_send_to_client(uint8_t *packet, size_t sz)
{
    WISPFUN_DEBUG("c_plgrcvfnc");

    g_PacketManager.SavePluginReceivePacket(packet, sz);

    return true;
}

bool __cdecl plugin_send_to_server(uint8_t *packet, size_t sz)
{
    WISPFUN_DEBUG("c_plgsndfnc");

    uint ticks = g_Ticks;
    g_TotalSendSize += sz;

    CPacketInfo &type = g_PacketManager.GetInfo(*packet);

    LOG("--- ^(%d) s(+%d => %d) Plugin->Server:: %s\n",
        ticks - g_LastPacketTime,
        sz,
        g_TotalSendSize,
        type.Name);

    g_LastPacketTime = ticks;
    g_LastSendTime = ticks;

    if (*packet == 0x80 || *packet == 0x91)
    {
        LOG_DUMP(packet, 1);
        LOG("**** ACCOUNT AND PASSWORD CENSORED ****\n");
    }
    else
        LOG_DUMP(packet, sz);

    g_ConnectionManager.Send(packet, sz);

    return true;
}

void __cdecl plugin_cast_spell(uint32_t index)
{
    if (index >= 0)
    {
        g_LastSpellIndex = index;

        SendNotifyMessage(g_OrionWindow.Handle, ASSISTANTMSG_CAST_SPELL, (WPARAM)index, 0);
    }
}

CPluginManager::CPluginManager()
{
}

void CPluginManager::LoadPlugin(const string &lib_path)
{
    WISPFUN_DEBUG("c194_f16");

    LOG("Loading %s...\n", lib_path);

    HMODULE dll = LoadLibraryA(lib_path.c_str());

    if (dll == nullptr)
    {
        auto errorCode = GetLastError();
        LOG("Failed to load %s\n", lib_path);
        LOG("Error code: %i\n", errorCode);
        return;
    }

    PLUGIN_ENTRY *entry = (PLUGIN_ENTRY *)GetProcAddress(dll, "Initialize");

    if (entry == nullptr)
    {
        auto errorCode = GetLastError();
        LOG("Failed to find function 'Initialize'\n");
        LOG("Error code: %i\n", errorCode);
        FreeLibrary(dll);
        return;
    }

    struct orion_plugin *plugin = new orion_plugin;
    if (plugin == nullptr)
    {
        auto errorCode = GetLastError();
        LOG("Failed to allocate memory for plugin %s\n", lib_path);
        FreeLibrary(dll);
        return;
    }

    CRASHLOG("Plugin['%s'] loaded at: 0x%08X\n", lib_path.c_str(), dll);

    plugin->WindowHandle = g_OrionWindow.Handle;
    plugin->ClientVersionText = g_Orion.ClientVersionText;
    plugin->ClientPath = g_App.UOFilesPathA;

    plugin->client.recv_packet = plugin_send_to_client;
    plugin->client.send_packet = plugin_send_to_server;
    plugin->client.cast_spell = plugin_cast_spell;

    entry(plugin);

    m_Plugins.push_back(plugin);
}

bool CPluginManager::SendPluginMessage(struct UOMSG *msg)
{
    bool block = false;
    for (const auto &plugin : m_Plugins)
    {
        if (plugin->OnMessage(msg))
        {
            block = true;
        }
    }

    return block;
}

bool CPluginManager::RecvNotify(uint8_t *packet, size_t sz)
{
    struct UOMSG msg;

    msg.type = UOMSG_RECV;
    msg.u.recv.packet = packet;
    msg.u.recv.sz = sz;
    return SendPluginMessage(&msg);
}

bool CPluginManager::SendNotify(uint8_t *packet, size_t sz)
{
    struct UOMSG msg;

    msg.type = UOMSG_SEND;
    msg.u.recv.packet = packet;
    msg.u.recv.sz = sz;
    return SendPluginMessage(&msg);
}

void CPluginManager::DrawNotify()
{
    struct UOMSG msg;

    msg.type = UOMSG_DRAW;
    SendPluginMessage(&msg);
}

void CPluginManager::SetServerName(const char *name)
{
    struct UOMSG msg;

    msg.type = UOMSG_SET_SERVER_NAME;
    msg.u.set_server_name.server_name = name;
    SendPluginMessage(&msg);
}

void CPluginManager::SetPlayerName(const char *name)
{
    struct UOMSG msg;

    msg.type = UOMSG_RECV;
    msg.u.set_player_name.player_name = name;
    SendPluginMessage(&msg);
}

void CPluginManager::UpdatePlayerPosition(uint16_t x, uint16_t y, uint8_t z, uint8_t dir)
{
    struct UOMSG msg;

    msg.type = UOMSG_UPDATE_PLAYER_POSITION;
    msg.u.update_position.x = x;
    msg.u.update_position.y = y;
    msg.u.update_position.z = z;
    msg.u.update_position.dir = dir;
    SendPluginMessage(&msg);
}

void CPluginManager::CloseNotify()
{
    struct UOMSG msg;

    msg.type = UOMSG_CLOSE;
    SendPluginMessage(&msg);
}

void CPluginManager::DisconnectNotify()
{
    struct UOMSG msg;

    msg.type = UOMSG_DISCONNECT;
    SendPluginMessage(&msg);
}

bool CPluginManager::MouseButtonDown(enum UOMSG_MOUSE_BUTTON button)
{
    struct UOMSG msg;

    msg.type = UOMSG_MOUSEBUTTONDOWN;
    msg.u.mouse_button_down.button = button;
    return SendPluginMessage(&msg);
}

bool CPluginManager::MouseButtonUp(enum UOMSG_MOUSE_BUTTON button)
{
    struct UOMSG msg;

    msg.type = UOMSG_MOUSEBUTTONUP;
    msg.u.mouse_button_up.button = button;
    return SendPluginMessage(&msg);
}

bool CPluginManager::MouseWheel(bool up)
{
    struct UOMSG msg;

    msg.type = UOMSG_MOUSEWHEEL;
    msg.u.mouse_wheel.up = up;
    return SendPluginMessage(&msg);
}

bool CPluginManager::KeyDown(uint32_t key_code)
{
    struct UOMSG msg;

    msg.type = UOMSG_KEYDOWN;
    msg.u.key_down.key_code = key_code;
    return SendPluginMessage(&msg);
}

bool CPluginManager::KeyUp(uint32_t key_code)
{
    struct UOMSG msg;

    msg.type = UOMSG_KEYUP;
    msg.u.key_up.key_code = key_code;
    return SendPluginMessage(&msg);
}

void CPluginManager::ActivateWindow(bool activated)
{
    struct UOMSG msg;

    msg.type = UOMSG_ACTIVEWINDOW;
    msg.u.active_window.active = activated;
    SendPluginMessage(&msg);
}
