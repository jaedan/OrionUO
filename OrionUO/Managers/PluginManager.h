#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

/* Message codes for messages sent from the assistant to the client */
enum ASSISTANTMSG
{
    ASSISTANTMSG_RECV = WM_USER + 760,
    ASSISTANTMSG_SEND,
    ASSISTANTMSG_CAST_SPELL,
	ASSISTANTMSG_ATTACK,
	ASSISTANTMSG_TITLEBAR = WM_USER + 776
};

class CPluginManager
{
    std::list<struct orion_plugin *> m_Plugins;

    bool SendPluginMessage(struct UOMSG *msg);

public:
    CPluginManager();
    virtual ~CPluginManager() {}

    void LoadPlugin(const string &lib_path);

    bool RecvNotify(uint8_t *packet, size_t sz);
    bool SendNotify(uint8_t *packet, size_t sz);
    void DrawNotify();
    void SetServerName(const char *name);
    void SetPlayerName(const char *name);
    void UpdatePlayerPosition(uint16_t x, uint16_t y, uint8_t z, uint8_t dir);
    void CloseNotify();
    void DisconnectNotify();
    bool MouseButtonDown(enum UOMSG_MOUSE_BUTTON button);
    bool MouseButtonUp(enum UOMSG_MOUSE_BUTTON button);
    bool MouseWheel(bool up);
    bool KeyDown(uint32_t key_code);
    bool KeyUp(uint32_t key_code);
};

extern CPluginManager g_PluginManager;

#endif
