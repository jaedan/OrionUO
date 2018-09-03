#include "Crypt.h"

#include <cstdlib>
#include <cstdint>

#include <msclr/marshal_cppstd.h>

using namespace Assistant;
using namespace System::Threading;

struct orion_plugin *g_plugin = nullptr;
HWND g_razor_hwnd = nullptr;

#pragma unmanaged
int APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls((HMODULE)hModule);
            break;

        case DLL_PROCESS_DETACH:
            std::free(g_plugin);
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }

    return 1;
}

static int OnMessage(struct UOMSG *msg)
{
    /* This is executing on the Orion thread, but needs to be processed
	 * by the Razor thread. The UOMSG structure is only valid
	 * during this call, so we can't post messages. Send them synchronously.
	 */

    ULONG block = 0;

    switch (msg->type)
    {
        case UOMSG_SET_SERVER_NAME:
            return SendMessage(
                g_razor_hwnd, msg->type, (WPARAM)msg->u.set_server_name.server_name, 0);
        case UOMSG_SET_PLAYER_NAME:
            return SendMessage(
                g_razor_hwnd, msg->type, (WPARAM)msg->u.set_player_name.player_name, 0);
        case UOMSG_UPDATE_PLAYER_POSITION:
            SendMessageTimeout(
                g_razor_hwnd,
                msg->type,
                (WPARAM)&msg->u.update_position,
                0,
                SMTO_BLOCK,
                500,
                &block);
            return block;
        case UOMSG_RECV:
            SendMessageTimeout(
                g_razor_hwnd,
                msg->type,
                (WPARAM)msg->u.recv.packet,
                (LPARAM)msg->u.recv.sz,
                SMTO_BLOCK,
                500,
                &block);
            return block;
        case UOMSG_SEND:
            SendMessageTimeout(
                g_razor_hwnd,
                msg->type,
                (WPARAM)msg->u.send.packet,
                (LPARAM)msg->u.send.sz,
                SMTO_BLOCK,
                500,
                &block);
            return block;
        case UOMSG_CLOSE:
            return SendMessage(g_razor_hwnd, msg->type, 0, 0);
        case UOMSG_DISCONNECT:
            return SendMessage(g_razor_hwnd, msg->type, 0, 0);
        case UOMSG_MOUSEBUTTONDOWN:
            SendMessageTimeout(
                g_razor_hwnd,
                msg->type,
                (WPARAM)msg->u.mouse_button_down.button,
                0,
                SMTO_BLOCK,
                500,
                &block);
            return block;
        case UOMSG_MOUSEBUTTONUP:
            SendMessageTimeout(
                g_razor_hwnd,
                msg->type,
                (WPARAM)msg->u.mouse_button_up.button,
                0,
                SMTO_BLOCK,
                500,
                &block);
            return block;
        case UOMSG_MOUSEWHEEL:
            SendMessageTimeout(
                g_razor_hwnd, msg->type, (WPARAM)msg->u.mouse_wheel.up, 0, SMTO_BLOCK, 500, &block);
            return block;
        case UOMSG_KEYDOWN:
            SendMessageTimeout(
                g_razor_hwnd,
                msg->type,
                (WPARAM)msg->u.key_down.key_code,
                0,
                SMTO_BLOCK,
                500,
                &block);
            return block;
        case UOMSG_KEYUP:
            SendMessageTimeout(
                g_razor_hwnd,
                msg->type,
                (WPARAM)msg->u.key_up.key_code,
                0,
                SMTO_BLOCK,
                500,
                &block);
            return block;
        case UOMSG_ACTIVEWINDOW:
            return SendMessage(g_razor_hwnd, msg->type, (WPARAM)msg->u.active_window.active, 0);
    }

    return 0;
}

#pragma managed

/* Called by Razor once it's main window initializes */
int InstallLibrary(IntPtr razorWindow);

IntPtr FindUOWindow()
{
    return (IntPtr)g_plugin->WindowHandle;
}

#define PACKET_VARIABLE_SIZE 0

uint16_t g_PacketSizes[] = { 0x68,
                             0x05,
                             0x07,
                             PACKET_VARIABLE_SIZE,
                             0x02,
                             0x05,
                             0x05,
                             0x07,
                             0x0e,
                             0x05,
                             0x0b,
                             0x07,
                             PACKET_VARIABLE_SIZE,
                             0x03,
                             0x01,
                             0x3d,
                             0xd7,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x0a,
                             0x06,
                             0x09,
                             0x01,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x25,
                             PACKET_VARIABLE_SIZE,
                             0x05,
                             0x04,
                             0x08,
                             0x13,
                             0x08,
                             0x03,
                             0x1a,
                             0x07,
                             0x14,
                             0x05,
                             0x02,
                             0x05,
                             0x01,
                             0x05,
                             0x02,
                             0x02,
                             0x11,
                             0x0f,
                             0x0a,
                             0x05,
                             0x01,
                             0x02,
                             0x02,
                             0x0a,
                             0x28d,
                             PACKET_VARIABLE_SIZE,
                             0x08,
                             0x07,
                             0x09,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x02,
                             0x25,
                             PACKET_VARIABLE_SIZE,
                             0xc9,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x229,
                             0x2c9,
                             0x05,
                             PACKET_VARIABLE_SIZE,
                             0x0b,
                             0x49,
                             0x5d,
                             0x05,
                             0x09,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x06,
                             0x02,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x02,
                             0x0c,
                             0x01,
                             0x0b,
                             0x6e,
                             0x6a,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x04,
                             0x02,
                             0x49,
                             PACKET_VARIABLE_SIZE,
                             0x31,
                             0x05,
                             0x09,
                             0x0f,
                             0x0d,
                             0x01,
                             0x04,
                             PACKET_VARIABLE_SIZE,
                             0x15,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x03,
                             0x09,
                             0x13,
                             0x03,
                             0x0e,
                             PACKET_VARIABLE_SIZE,
                             0x1c,
                             PACKET_VARIABLE_SIZE,
                             0x05,
                             0x02,
                             PACKET_VARIABLE_SIZE,
                             0x23,
                             0x10,
                             0x11,
                             PACKET_VARIABLE_SIZE,
                             0x09,
                             PACKET_VARIABLE_SIZE,
                             0x02,
                             PACKET_VARIABLE_SIZE,
                             0x0d,
                             0x02,
                             PACKET_VARIABLE_SIZE,
                             0x3e,
                             PACKET_VARIABLE_SIZE,
                             0x02,
                             0x27,
                             0x45,
                             0x02,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x42,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x0b,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x13,
                             0x41,
                             PACKET_VARIABLE_SIZE,
                             0x63,
                             PACKET_VARIABLE_SIZE,
                             0x09,
                             PACKET_VARIABLE_SIZE,
                             0x02,
                             PACKET_VARIABLE_SIZE,
                             0x1a,
                             PACKET_VARIABLE_SIZE,
                             0x102,
                             0x135,
                             0x33,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x03,
                             0x09,
                             0x09,
                             0x09,
                             0x95,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x04,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x05,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x0d,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x40,
                             0x09,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x03,
                             0x06,
                             0x09,
                             0x03,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x24,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x06,
                             0xcb,
                             0x01,
                             0x31,
                             0x02,
                             0x06,
                             0x06,
                             0x07,
                             PACKET_VARIABLE_SIZE,
                             0x01,
                             PACKET_VARIABLE_SIZE,
                             0x4e,
                             PACKET_VARIABLE_SIZE,
                             0x02,
                             0x19,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x10,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x9,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x09,
                             0xa,
                             0x4d,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x15,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x18,
                             PACKET_VARIABLE_SIZE,
                             0x15,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x6a,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             PACKET_VARIABLE_SIZE,
                             0x8,
                             PACKET_VARIABLE_SIZE };

bool IsDynLength(Byte packet)
{
    return (g_PacketSizes[packet] == 0);
}

void BringToFront(IntPtr hWnd)
{
    HWND handle = (HWND)hWnd.ToPointer();
    SetWindowPos(handle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    ShowWindow(handle, SW_SHOW);
    SetForegroundWindow(handle);
    SetFocus(handle);
}

void SendToServer(Byte *packet, int len)
{
    g_plugin->client.send_packet(packet, len);
}

void SendToClient(Byte *packet, int len)
{
    g_plugin->client.recv_packet(packet, len);
}

void Cast(int index)
{
    g_plugin->client.cast_spell(index);
}

void Attack(uint32_t serial)
{
    g_plugin->client.attack(serial);
}

String ^
    GetUOClientVersion() {
        return msclr::interop::marshal_as<String ^>(g_plugin->ClientVersionText);
    }

    ref class RazorThread
{
public:
    static ManualResetEventSlim ^ sync;

    static void Run()
    {
        ClientCommunication::UOClient::InstallLibrary =
            gcnew ClientCommunication::UOClient::InstallLibraryDelegate(InstallLibrary);
        ClientCommunication::UOClient::FindUOWindow =
            gcnew ClientCommunication::UOClient::FindUOWindowDelegate(FindUOWindow);
        ClientCommunication::UOClient::IsDynLength =
            gcnew ClientCommunication::UOClient::IsDynLengthDelegate(IsDynLength);
        ClientCommunication::UOClient::BringToFront =
            gcnew ClientCommunication::UOClient::BringToFrontDelegate(BringToFront);
        ClientCommunication::UOClient::SendToServer =
            gcnew ClientCommunication::UOClient::SendToServerDelegate(SendToServer);
        ClientCommunication::UOClient::SendToClient =
            gcnew ClientCommunication::UOClient::SendToClientDelegate(SendToClient);
        ClientCommunication::UOClient::Cast =
            gcnew ClientCommunication::UOClient::CastDelegate(Cast);
        ClientCommunication::UOClient::Attack =
            gcnew ClientCommunication::UOClient::AttackDelegate(Attack);
        ClientCommunication::UOClient::GetUOClientVersion =
            gcnew ClientCommunication::UOClient::GetUOClientVersionDelegate(GetUOClientVersion);

        Engine ^ e = gcnew Engine();
        e->Run(msclr::interop::marshal_as<String ^>(g_plugin->ClientPath));
    }
};

int InstallLibrary(IntPtr razorWindow)
{
    g_razor_hwnd = (HWND)razorWindow.ToPointer();

    RazorThread::sync->Set();

    return 0;
}

/* Called by Orion after the DLL loads. */
DLLFUNCTION void Initialize(struct orion_plugin *plugin)
{
    g_plugin = plugin;

    plugin->OnMessage = OnMessage;

    RazorThread::sync = gcnew ManualResetEventSlim(false);
    RazorThread::sync->Reset();

    ThreadStart ^ entry = gcnew ThreadStart(&RazorThread::Run);
    Thread ^ thread = gcnew Thread(entry);
    thread->Start();

    RazorThread::sync->Wait();
}
