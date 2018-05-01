using System;
using System.Collections;
using System.Threading;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Net;
using Microsoft.Win32;

namespace Assistant
{
	public class FeatureBit
	{
		public static readonly uint WeatherFilter	=  0;
		public static readonly uint LightFilter		=  1;
		public static readonly uint SmartLT			=  2;
		public static readonly uint RangeCheckLT	=  3;
		public static readonly uint UnequipBeforeCast= 5;
		public static readonly uint AutoPotionEquip	=  6;
		public static readonly uint BlockHealPoisoned= 7;
		public static readonly uint LoopingMacros	=  8; // includes fors and macros running macros
		public static readonly uint UseOnceAgent	=  9;
		public static readonly uint RestockAgent	= 10;
		public static readonly uint SellAgent		= 11;
		public static readonly uint BuyAgent		= 12;
		public static readonly uint PotionHotkeys	= 13;
		public static readonly uint RandomTargets	= 14;
		public static readonly uint ClosestTargets	= 15;
		public static readonly uint OverheadHealth	= 16;

		public static readonly uint MaxBit			= 16;
	}

    public unsafe sealed class ClientCommunication
    {
        public const int WM_USER = 0x400;

        public enum UOMSG_MOUSE_BUTTON
        {
            MOUSE_BUTTON_MID = 0,
            MOUSE_BUTTON_XTRA1,
            MOUSE_BUTTON_XTRA2,
        };

        public enum UOMSG_TYPE
        {
            First = RECV,

            RECV = WM_USER + 660,
            SEND,
            DRAW,
            SET_SERVER_NAME,
            SET_PLAYER_NAME,
            UPDATE_PLAYER_POSITION,
            CLOSE,
            DISCONNECT,
            MOUSEBUTTONDOWN,
            MOUSEBUTTONUP,
            MOUSEWHEEL,
            KEYDOWN,
            KEYUP,
            ACTIVEWINDOW,

            Last = ACTIVEWINDOW,
        }

        public struct UOMSG_PACKET
        {
            public byte[] packet;
            public int sz;
        }

        public struct UOMSG_NAME
        {
            public string name;
        }

        public struct UOMSG_POS
        {
            public uint x;
            public uint y;
            public uint z;
            public uint dir;
        }

        public static class UOClient
        {
            public delegate int InstallLibraryDelegate(IntPtr razorWindow);
            public delegate IntPtr FindUOWindowDelegate();
            public delegate bool IsDynLengthDelegate(byte packetID);
            public delegate void BringToFrontDelegate(IntPtr hWnd);
            public delegate void SendToServerDelegate(Byte* packet, int len);
            public delegate void SendToClientDelegate(Byte* packet, int len);
            public delegate void CastDelegate(int index);
            public delegate String GetUOClientVersionDelegate();

            public static InstallLibraryDelegate InstallLibrary;
            public static FindUOWindowDelegate FindUOWindow;
            public static IsDynLengthDelegate IsDynLength;
            public static BringToFrontDelegate BringToFront;
            public static SendToServerDelegate SendToServer;
            public static SendToClientDelegate SendToClient;
            public static CastDelegate Cast;
            public static GetUOClientVersionDelegate GetUOClientVersion;
        }

        [DllImport("msvcrt.dll", SetLastError = false)]
        static extern IntPtr memcpy(IntPtr dest, IntPtr src, int count);

        [DllImport("user32.dll")]
        internal static extern bool SetForegroundWindow(IntPtr hWnd);

        [DllImport("kernel32.dll")]
        private static extern IntPtr LoadLibrary(string path);
        [DllImport("kernel32.dll")]
        private static extern bool FreeLibrary(IntPtr hModule);
        [DllImport("kernel32.dll")]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string procName);

        [DllImport("Advapi32.dll")]
        private static extern int GetUserNameA(StringBuilder buff, int* len);

        public static string GetWindowsUserName()
        {
            int len = 1024;
            StringBuilder sb = new StringBuilder(len);
            if (GetUserNameA(sb, &len) != 0)
                return sb.ToString();
            else
                return "";
        }

        public static bool IsCalibrated() { return true; }

        public static void DoFeatures(int features) { }

        public static bool AllowBit(uint bit) { return true; }

        internal static void SetAllowDisconn(bool allowed) { }

        public static int HandleNegotiate(ulong word) { return 1; }

        internal static string GetUOVersion() { return "7.0.15.1"; }

        public static void PostCounterUpdate(int counter, int count) { }

        public static void PostSpellCast(int spell) { }

        public static void PostLogin(int serial) { }

        public static void PostMacroStop() { }

        public static void PostMapChange(int map) { }

        public static void PostSkillUpdate(int skill, int val) { }

        public static void PostRemoveMulti(Item item) { }

        public static void PostAddMulti(ItemID iid, Point3D Position) { }

        public static void PostHitsUpdate() { }

        public static void PostManaUpdate() { }

        public static void PostStamUpdate() { }

        public static int NotificationCount { get { return 0; } }

        static ClientCommunication()
        {
        }

        public static void SetMapWndHandle(Form mapWnd) { }

        public static void SetCustomNotoHue(int hue) { }

        private static DateTime m_ConnStart;
        public static DateTime ConnectionStart { get { return m_ConnStart; } }

        private static IPAddress m_LastConnection;
        public static IPAddress LastConnection { get { return m_LastConnection; } }

        internal static bool ClientEncrypted { get { return false; } }
        internal static bool ServerEncrypted { get { return false; } }

        internal static int Initialize( IntPtr mainWindow )
		{
			return UOClient.InstallLibrary(mainWindow);
		}

		public static void Close()
		{
		}

        public static void SetNegotiate(bool negotiate) { }

        public static void RequestTitlebarUpdate() { }

        public static int GetZ(int x, int y, int z) { return World.Player.Position.Z; }

        public static void BeginCalibratePosition() { }

		public static void OnLogout()
		{
			OnLogout( true );
		}

		private static void OnLogout( bool fake )
		{
			World.Player = null;
			World.Items.Clear();
			World.Mobiles.Clear();
			Macros.MacroManager.Stop();
			ActionQueue.Stop();
			StealthSteps.Unhide();
			Engine.MainWindow.OnLogout();
			if( Engine.MainWindow.MapWindow != null )
				Engine.MainWindow.MapWindow.Close();
			PacketHandlers.Party.Clear();
			PacketHandlers.IgnoreGumps.Clear();
			Config.Save();
		}

        [StructLayout(LayoutKind.Sequential)]
        private struct PositionData
        {
            public UInt16 X;
            public UInt16 Y;
            public byte Z;
            public byte Direction;
        }

        internal static int OnMessage(UOMSG_TYPE msg, IntPtr wParam, IntPtr lParam) {

            int block = 0;

            switch (msg) {
                case UOMSG_TYPE.SET_SERVER_NAME:
                    break;
                case UOMSG_TYPE.SET_PLAYER_NAME:
                    m_ConnStart = DateTime.Now;
                    break;
                case UOMSG_TYPE.UPDATE_PLAYER_POSITION:
                {
                    PositionData* data = (PositionData*)wParam;
                    World.Player.Position = new Point3D(data->X, data->Y, data->Z);
                    World.Player.Direction = (Direction)data->Direction;
                    break;
                }
                case UOMSG_TYPE.RECV:
                    block = OnRecv((byte*)wParam, (int)lParam) ? 1 : 0; // packet, size
                    break;
                case UOMSG_TYPE.SEND:
                    block = OnSend((byte*)wParam, (int)lParam) ? 1 : 0; // packet, size
                    break;
                case UOMSG_TYPE.CLOSE:
                    OnLogout();
                    Engine.MainWindow.Close();
                    break;
                case UOMSG_TYPE.DISCONNECT:
                    OnLogout(false);
                    break;
                case UOMSG_TYPE.MOUSEBUTTONDOWN:
                    HotKey.OnMouse((int)wParam + 1, 0);
                    break;
                case UOMSG_TYPE.MOUSEBUTTONUP:
                    break;
                case UOMSG_TYPE.MOUSEWHEEL:
                    HotKey.OnMouse(0, (int)wParam);
                    break;
                case UOMSG_TYPE.KEYDOWN:
                    block = HotKey.OnKeyDown((int)wParam) ? 0 : 1;
                    break;
                case UOMSG_TYPE.KEYUP:
                    break;
                case UOMSG_TYPE.ACTIVEWINDOW:
                    /*
                    if (Config.GetBool("AlwaysOnTop")) {
                        if (lParam != 0 && !razor.TopMost) {
                            razor.TopMost = true;
                            SetForegroundWindow(FindUOWindow());
                        } else if (lParam == 0 && razor.TopMost) {
                            razor.TopMost = false;
                            razor.SendToBack();
                        }
                    }

                    // always use smartness for the map window
                    if (razor.MapWindow != null && razor.MapWindow.Visible) {
                        if (lParam != 0 && !razor.MapWindow.TopMost) {
                            razor.MapWindow.TopMost = true;
                            SetForegroundWindow(FindUOWindow());
                        } else if (lParam == 0 && razor.MapWindow.TopMost) {
                            razor.MapWindow.TopMost = false;
                            razor.MapWindow.SendToBack();
                        }
                    }
                    */
                    break;
            }

            return block;
        }

        internal static IntPtr FindUOWindow()
        {
            return UOClient.FindUOWindow();
        }

        internal static void BringToFront(IntPtr hWnd)
        {
            UOClient.BringToFront(hWnd);
        }

        internal static void SendToServer(Packet p)
		{
            var len = p.Length;
            fixed (byte* packet = p.Compile()) {
                UOClient.SendToServer(packet, (int)len);
            }
		}

        internal static void ForceSendToServer(Packet p)
        {
            SendToServer(p);
        }

		internal static void SendToClient(Packet p)
		{
            var len = p.Length;
            fixed (byte* packet = p.Compile()) {
                UOClient.SendToClient(packet, (int)len);
            }
        }

        internal static void ForceSendToClient(Packet p)
        {
            SendToClient(p);
        }

        internal static void Cast(int index)
        {
            UOClient.Cast(index);
        }

        private static bool OnRecv(byte *packet, int len)
		{
            PacketReader p = new PacketReader(packet, len, UOClient.IsDynLength(packet[0]));

            return PacketHandler.OnServerPacket(p);
        }

        private static bool OnSend(byte* packet, int len)
		{
            PacketReader p = new PacketReader(packet, len, UOClient.IsDynLength(packet[0]));

            return PacketHandler.OnClientPacket(p);
        }

        public static void ProcessPlaybackData(BinaryReader reader) { }
	}
}

