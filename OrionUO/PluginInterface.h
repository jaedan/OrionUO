#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <Windows.h>
#include <string>

#ifdef cplusplus
extern "C"
{
#endif
    enum UOMSG_MOUSE_BUTTON
    {
        UOMSG_MOUSE_BUTTON_MID = 0,
        UOMSG_MOUSE_BUTTON_XTRA1,
        UOMSG_MOUSE_BUTTON_XTRA2,
    };

    /* Message codes for messages sent from the client to the plugin */
    enum UOMSG_TYPE
    {
        UOMSG_RECV = WM_USER + 660,
        UOMSG_SEND,
        UOMSG_DRAW,
        UOMSG_SET_SERVER_NAME,
        UOMSG_SET_PLAYER_NAME,
        UOMSG_UPDATE_PLAYER_POSITION,
        UOMSG_CLOSE,
        UOMSG_DISCONNECT,
        UOMSG_MOUSEBUTTONDOWN,
        UOMSG_MOUSEBUTTONUP,
        UOMSG_MOUSEWHEEL,
        UOMSG_KEYDOWN,
        UOMSG_KEYUP,
    };

    struct UOMSG
    {
        enum UOMSG_TYPE type;

        union {
            struct
            {
                uint8_t *packet;
                size_t sz;
            } recv;

            struct
            {
                uint8_t *packet;
                size_t sz;
            } send;

            struct
            {
                const char *server_name;
            } set_server_name;

            struct
            {
                const char *player_name;
            } set_player_name;

            struct
            {
                uint16_t x;
                uint16_t y;
                uint8_t z;
                uint8_t dir;
            } update_position;

            struct
            {
                enum UOMSG_MOUSE_BUTTON button;
            } mouse_button_down;

            struct
            {
                enum UOMSG_MOUSE_BUTTON button;
            } mouse_button_up;

            struct
            {
                bool up;
            } mouse_wheel;

            struct
            {
                uint32_t key_code;
            } key_down;

            struct
            {
                uint32_t key_code;
            } key_up;
        } u;
    };

    typedef void __cdecl PLUGIN_ENTRY(struct orion_plugin *plugin);
    typedef int __cdecl ON_MESSAGE(struct UOMSG *msg);
    typedef bool __cdecl SEND_PACKET(uint8_t *packet, size_t sz);
    typedef bool __cdecl RECV_PACKET(uint8_t *packet, size_t sz);
    typedef void __cdecl CAST_SPELL(uint32_t index);
    typedef void __cdecl ATTACK(uint32_t serial);

    struct orion_plugin
    {
        size_t size;

        HWND WindowHandle;
        std::string ClientVersionText;
        std::string ClientPath;

        struct
        {
            SEND_PACKET *send_packet;
            RECV_PACKET *recv_packet;
            CAST_SPELL *cast_spell;
            ATTACK *attack;
        } client;

        ON_MESSAGE *OnMessage;
    };

#ifdef cplusplus
}
#endif

#endif
