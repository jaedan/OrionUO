using System;
using System.Collections.Generic;
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
		public static readonly ulong WeatherFilter	    = 0;
		public static readonly ulong LightFilter	    = 1;
		public static readonly ulong SmartLT		    = 2;	
		public static readonly ulong RangeCheckLT	    = 3;
		public static readonly ulong AutoOpenDoors	    = 4;
		public static readonly ulong UnequipBeforeCast  = 5;
		public static readonly ulong AutoPotionEquip	= 6;
		public static readonly ulong BlockHealPoisoned  = 7;
		public static readonly ulong LoopingMacros	    = 8; // includes fors and macros running macros
		public static readonly ulong UseOnceAgent   	= 9;
		public static readonly ulong RestockAgent   	= 10;
		public static readonly ulong SellAgent		    = 11;
		public static readonly ulong BuyAgent		    = 12;
		public static readonly ulong PotionHotkeys	    = 13;
		public static readonly ulong RandomTargets	    = 14;
		public static readonly ulong ClosestTargets 	= 15;
		public static readonly ulong OverheadHealth 	= 16;
        public static readonly ulong AutoLootAgent      = 17;
        public static readonly ulong BoneCutterAgent    = 18;
        public static readonly ulong JScriptMacros      = 19;
        public static readonly ulong AutoRemount        = 20;
        public static readonly ulong AutoBandage        = 21;
        public static readonly ulong EnemyTargetShare   = 22;
        public static readonly ulong FilterSeason       = 23;
        public static readonly ulong SpellTargetShare   = 24;

        public static readonly ulong MaxBit			    = 24;
	}

    public unsafe sealed class ClientCommunication
    {
        /*public enum UOMSG_MOUSE_BUTTON
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

            Last = KEYUP
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
        }*/
        public const int WM_USER = 0x400;
        private enum ASSISTANTMSG_TYPE
		{
			RECV = WM_USER + 760,
			SEND,
			CAST_SPELL,
			ATTACK,
			TITLEBAR,
		};

        public static class UOClient
        {
            public delegate int InstallLibraryDelegate(IntPtr razorWindow);
            public delegate IntPtr FindUOWindowDelegate();
            public delegate bool IsDynLengthDelegate(byte packetID);
            public delegate void BringToFrontDelegate(IntPtr hWnd);
            public delegate void SendToServerDelegate(Byte* packet, int len);
            public delegate void SendToClientDelegate(Byte* packet, int len);
            public delegate void CastDelegate(int index);
            public delegate void AttackDelegate(uint serial);
            public delegate String GetUOClientVersionDelegate();

            public static InstallLibraryDelegate InstallLibrary;
            public static FindUOWindowDelegate FindUOWindow;
            public static IsDynLengthDelegate IsDynLength;
            public static BringToFrontDelegate BringToFront;
            public static SendToServerDelegate SendToServer;
            public static SendToClientDelegate SendToClient;
            public static CastDelegate Cast;
            public static AttackDelegate Attack;
            public static GetUOClientVersionDelegate GetUOClientVersion;
        }

        [DllImport("user32.dll")]
        private static extern uint SendMessage(IntPtr hWnd, uint Msg, int wParam, IntPtr lParam);

        public static bool IsCalibrated() { return true; }

        //public static void DoFeatures(int features) { }

        //internal static void SetAllowDisconn(bool allowed) { }

        //public static int HandleNegotiate(ulong word) { return 1; }

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

        public static DateTime ConnectionStart { get; internal set; }

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

        private static Timer m_TBTimer;
        public static void RequestTitlebarUpdate(bool force = false)
        {
            if (m_TBTimer == null)
                m_TBTimer = new TitleBarThrottle();

            if (!m_TBTimer.Running)
                m_TBTimer.Start();
            else if (force)
                UpdateTitleBar();
        }

        private class TitleBarThrottle : Timer
        {
            public TitleBarThrottle() : base(TimeSpan.FromSeconds(0.1))
            {
            }

            protected override void OnTick()
            {
                UpdateTitleBar();
            }
        }

        private static StringBuilder m_TBBuilder = new StringBuilder();
        private static string m_LastPlayerName = "";

        private static void UpdateTitleBar()
        {
            if (World.Player != null && Config.GetBool("TitleBarDisplay"))
            {
                // reuse the same sb each time for less damn allocations
                m_TBBuilder.Remove(0, m_TBBuilder.Length);
                m_TBBuilder.Insert(0, Config.GetString("TitleBarText"));
                StringBuilder sb = m_TBBuilder;
                //StringBuilder sb = new StringBuilder( Config.GetString( "TitleBarText" ) ); // m_TitleCapacity 

                PlayerData p = World.Player;

                if (p.Name != m_LastPlayerName)
                {
                    m_LastPlayerName = p.Name;

                    Engine.MainWindow.UpdateTitle();
                }

                if (Config.GetBool("ShowNotoHue"))
                    sb.Replace(@"{char}", String.Format("~#{0:X6}{1}~#~", p.GetNotorietyColor() & 0x00FFFFFF, p.Name));
                else
                    sb.Replace(@"{char}", p.Name);
                sb.Replace(@"{shard}", World.ShardName);

                if (p.CriminalTime != 0)
                    sb.Replace(@"{crimtime}", String.Format("~^C0C0C0{0}~#~", p.CriminalTime));
                else
                    sb.Replace(@"{crimtime}", "-");

                sb.Replace(@"{str}", p.Str.ToString());
                sb.Replace(@"{hpmax}", p.HitsMax.ToString());
                if (p.Poisoned)
                    sb.Replace(@"{hp}", String.Format("~#FF8000{0}~#~", p.Hits));
                else
                    sb.Replace(@"{hp}", EncodeColorStat(p.Hits, p.HitsMax));
                sb.Replace(@"{dex}", World.Player.Dex.ToString());
                sb.Replace(@"{stammax}", World.Player.StamMax.ToString());
                sb.Replace(@"{stam}", EncodeColorStat(p.Stam, p.StamMax));
                sb.Replace(@"{int}", World.Player.Int.ToString());
                sb.Replace(@"{manamax}", World.Player.ManaMax.ToString());
                sb.Replace(@"{mana}", EncodeColorStat(p.Mana, p.ManaMax));

                sb.Replace(@"{ar}", p.AR.ToString());
                sb.Replace(@"{tithe}", p.Tithe.ToString());

                sb.Replace(@"{physresist}", p.AR.ToString());
                sb.Replace(@"{fireresist}", p.FireResistance.ToString());
                sb.Replace(@"{coldresist}", p.ColdResistance.ToString());
                sb.Replace(@"{poisonresist}", p.PoisonResistance.ToString());
                sb.Replace(@"{energyresist}", p.EnergyResistance.ToString());

                sb.Replace(@"{luck}", p.Luck.ToString());

                sb.Replace(@"{damage}", String.Format("{0}-{1}", p.DamageMin, p.DamageMax));

                if (World.Player.Weight >= World.Player.MaxWeight)
                    sb.Replace(@"{weight}", String.Format("~#FF0000{0}~#~", World.Player.Weight));
                else
                    sb.Replace(@"{weight}", World.Player.Weight.ToString());
                sb.Replace(@"{maxweight}", World.Player.MaxWeight.ToString());

                sb.Replace(@"{followers}", World.Player.Followers.ToString());
                sb.Replace(@"{followersmax}", World.Player.FollowersMax.ToString());

                sb.Replace(@"{gold}", World.Player.Gold.ToString());
                if (BandageTimer.Running)
                    sb.Replace(@"{bandage}", String.Format("~#FF8000{0}~#~", BandageTimer.Count));
                else
                    sb.Replace(@"{bandage}", "-");

                if (StealthSteps.Counting)
                    sb.Replace(@"{stealthsteps}", StealthSteps.Count.ToString());
                else
                    sb.Replace(@"{stealthsteps}", "-");

                string statStr = String.Format("{0}{1:X2}{2:X2}{3:X2}",
                    (int)(p.GetStatusCode()),
                    (int)(World.Player.HitsMax == 0 ? 0 : (double)World.Player.Hits / World.Player.HitsMax * 99),
                    (int)(World.Player.ManaMax == 0 ? 0 : (double)World.Player.Mana / World.Player.ManaMax * 99),
                    (int)(World.Player.StamMax == 0 ? 0 : (double)World.Player.Stam / World.Player.StamMax * 99));

                sb.Replace(@"{statbar}", String.Format("~SR{0}", statStr));
                sb.Replace(@"{mediumstatbar}", String.Format("~SL{0}", statStr));
                sb.Replace(@"{largestatbar}", String.Format("~SX{0}", statStr));

                bool dispImg = Config.GetBool("TitlebarImages");
                for (int i = 0; i < Counter.List.Count; i++)
                {
                    Counter c = Counter.List[i];
                    if (c.Enabled)
                        sb.Replace(String.Format("{{{0}}}", c.Format), c.GetTitlebarString(dispImg && c.DisplayImage));
                }

                SetTitleStr(sb.ToString());
            }
            else
            {
                SetTitleStr("");
            }
        }

        private static string m_LastStr;
        private static IntPtr m_TitleData = Marshal.AllocCoTaskMem(1024);

        private static void SetTitleStr(string str)
        {
            if (m_LastStr == str)
                return;

            m_LastStr = str;
            byte[] copy = System.Text.Encoding.ASCII.GetBytes(str);
            Array.Resize<byte>(ref copy, copy.Length + 1);
            int clen = copy.Length;
            copy[clen - 1] = 0;

            if (clen > 0)
            {
                if (clen >= 512)
                    clen = 511;
                Marshal.Copy(copy, 0, m_TitleData, clen);
            }

            bool active = Config.GetBool("TitleBarDisplay") && clen>1;
            SendMessage(FindUOWindow(), (uint)ASSISTANTMSG_TYPE.TITLEBAR, active ? clen : 0, active ? m_TitleData : IntPtr.Zero);
            //PostMessage(FindUOWindow(), WM_CUSTOMTITLE, IntPtr.Zero, IntPtr.Zero);
        }

        public static string EncodeColorStat(int val, int max)
        {
            double perc = ((double)val) / ((double)max);

            if (perc <= 0.25)
                return String.Format("~#FF0000{0}~#~", val);
            else if (perc <= 0.75)
                return String.Format("~#FFFF00{0}~#~", val);
            else
                return val.ToString();
        }

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

        /*internal static int OnMessage(UOMSG_TYPE msg, IntPtr wParam, IntPtr lParam)
        {
            int block = 0;
            switch (msg)
            {
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
                    Engine.MainWindow.OnClientClose();
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
            }

            return block;
        }*/

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

        internal static void Attack(uint serial)
        {
            UOClient.Attack(serial);
        }

        internal static bool OnRecv(byte *packet, int len)
		{
            PacketReader p = new PacketReader(packet, len, UOClient.IsDynLength(packet[0]));

            return PacketHandler.OnServerPacket(p);
        }

        internal static bool OnSend(byte* packet, int len)
		{
            PacketReader p = new PacketReader(packet, len, UOClient.IsDynLength(packet[0]));

            return PacketHandler.OnClientPacket(p);
        }

        public static void ProcessPlaybackData(BinaryReader reader) { }
	}
}

