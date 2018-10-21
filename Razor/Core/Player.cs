using System;
using System.Collections;
using System.Collections.Generic;

namespace Assistant
{
    public enum LockType : byte
    {
        Up = 0,
        Down = 1,
        Locked = 2,
    }

    public enum MsgLevel
    {
        Debug = 0,
        Info = 0,
        Warning = 1,
        Error = 2,
        Force = 3,
    }

    public class Skill
    {
        public static int Count = 55;

        private LockType m_Lock;
        private ushort m_Value;
        private ushort m_Base;
        private ushort m_Cap;
        private short m_Delta;
        private int m_Idx;

        public Skill(int idx)
        {
            m_Idx = idx;
        }

        public int Index { get { return m_Idx; } }

        public LockType Lock
        {
            get { return m_Lock; }
            set { m_Lock = value; }
        }

        public ushort FixedValue
        {
            get { return m_Value; }
            set { m_Value = value; }
        }

        public ushort FixedBase
        {
            get { return m_Base; }
            set
            {
                m_Delta += (short)(value - m_Base);
                m_Base = value;
            }
        }

        public ushort FixedCap
        {
            get { return m_Cap; }
            set { m_Cap = value; }
        }

        public double Value
        {
            get { return m_Value / 10.0; }
            set { m_Value = (ushort)(value * 10.0); }
        }

        public double Base
        {
            get { return m_Base / 10.0; }
            set { m_Base = (ushort)(value * 10.0); }
        }

        public double Cap
        {
            get { return m_Cap / 10.0; }
            set { m_Cap = (ushort)(value * 10.0); }
        }

        public double Delta
        {
            get { return m_Delta / 10.0; }
            set { m_Delta = (short)(value * 10); }
        }
    }

    public enum SkillName
    {
        Alchemy = 0,
        Anatomy = 1,
        AnimalLore = 2,
        ItemID = 3,
        ArmsLore = 4,
        Parry = 5,
        Begging = 6,
        Blacksmith = 7,
        Fletching = 8,
        Peacemaking = 9,
        Camping = 10,
        Carpentry = 11,
        Cartography = 12,
        Cooking = 13,
        DetectHidden = 14,
        Discordance = 15,
        EvalInt = 16,
        Healing = 17,
        Fishing = 18,
        Forensics = 19,
        Herding = 20,
        Hiding = 21,
        Provocation = 22,
        Inscribe = 23,
        Lockpicking = 24,
        Magery = 25,
        MagicResist = 26,
        Tactics = 27,
        Snooping = 28,
        Musicianship = 29,
        Poisoning = 30,
        Archery = 31,
        SpiritSpeak = 32,
        Stealing = 33,
        Tailoring = 34,
        AnimalTaming = 35,
        TasteID = 36,
        Tinkering = 37,
        Tracking = 38,
        Veterinary = 39,
        Swords = 40,
        Macing = 41,
        Fencing = 42,
        Wrestling = 43,
        Lumberjacking = 44,
        Mining = 45,
        Meditation = 46,
        Stealth = 47,
        RemoveTrap = 48,
        Necromancy = 49,
        Focus = 50,
        Chivalry = 51,
        Bushido = 52,
        Ninjitsu = 53,
        SpellWeaving = 54,
    }

    public class PlayerData : Mobile
    {
        public int VisRange = 18;
        public int MultiVisRange { get { return VisRange + 5; } }

        private int m_MaxWeight = -1;

        private short m_FireResist, m_ColdResist, m_PoisonResist, m_EnergyResist, m_Luck;
        private ushort m_DamageMin, m_DamageMax;

        private ushort m_Str, m_Dex, m_Int;
        private LockType m_StrLock, m_DexLock, m_IntLock;
        private uint m_Gold;
        private ushort m_Weight;
        private Skill[] m_Skills;
        private ushort m_AR;
        private ushort m_StatCap;
        private byte m_Followers;
        private byte m_FollowersMax;
        private int m_Tithe;
        private sbyte m_LocalLight;
        private byte m_GlobalLight;
        private ushort m_Features;
        private byte m_Season;
        private int[] m_MapPatches = new int[10];

        private bool m_SkillsSent;
        private Timer m_CriminalTime;
        private DateTime m_CriminalStart = DateTime.MinValue;

        public PlayerData(Serial serial) : base(serial)
        {
            m_Skills = new Skill[Skill.Count];
            for (int i = 0; i < m_Skills.Length; i++)
                m_Skills[i] = new Skill(i);
        }

        public ushort Str
        {
            get { return m_Str; }
            set { m_Str = value; }
        }

        public ushort Dex
        {
            get { return m_Dex; }
            set { m_Dex = value; }
        }

        public ushort Int
        {
            get { return m_Int; }
            set { m_Int = value; }
        }

        public uint Gold
        {
            get { return m_Gold; }
            set { m_Gold = value; }
        }

        public ushort Weight
        {
            get { return m_Weight; }
            set { m_Weight = value; }
        }

        public ushort MaxWeight
        {
            get
            {
                if (m_MaxWeight == -1)
                    return (ushort)((m_Str * 3.5) + 40);
                else
                    return (ushort)m_MaxWeight;
            }
            set
            {
                m_MaxWeight = value;
            }
        }

        public short FireResistance
        {
            get { return m_FireResist; }
            set { m_FireResist = value; }
        }

        public short ColdResistance
        {
            get { return m_ColdResist; }
            set { m_ColdResist = value; }
        }

        public short PoisonResistance
        {
            get { return m_PoisonResist; }
            set { m_PoisonResist = value; }
        }

        public short EnergyResistance
        {
            get { return m_EnergyResist; }
            set { m_EnergyResist = value; }
        }

        public short Luck
        {
            get { return m_Luck; }
            set { m_Luck = value; }
        }

        public ushort DamageMin
        {
            get { return m_DamageMin; }
            set { m_DamageMin = value; }
        }

        public ushort DamageMax
        {
            get { return m_DamageMax; }
            set { m_DamageMax = value; }
        }

        public LockType StrLock
        {
            get { return m_StrLock; }
            set { m_StrLock = value; }
        }

        public LockType DexLock
        {
            get { return m_DexLock; }
            set { m_DexLock = value; }
        }

        public LockType IntLock
        {
            get { return m_IntLock; }
            set { m_IntLock = value; }
        }

        public ushort StatCap
        {
            get { return m_StatCap; }
            set { m_StatCap = value; }
        }

        public ushort AR
        {
            get { return m_AR; }
            set { m_AR = value; }
        }

        public byte Followers
        {
            get { return m_Followers; }
            set { m_Followers = value; }
        }

        public byte FollowersMax
        {
            get { return m_FollowersMax; }
            set { m_FollowersMax = value; }
        }

        public int Tithe
        {
            get { return m_Tithe; }
            set { m_Tithe = value; }
        }

        public Skill[] Skills { get { return m_Skills; } }

        public bool SkillsSent
        {
            get { return m_SkillsSent; }
            set { m_SkillsSent = value; }
        }

        public int CriminalTime
        {
            get
            {
                if (m_CriminalStart != DateTime.MinValue)
                {
					int sec = (int)(DateTime.UtcNow - m_CriminalStart).TotalSeconds;
                    if (sec > 300)
                    {
                        if (m_CriminalTime != null)
                            m_CriminalTime.Stop();
                        m_CriminalStart = DateTime.MinValue;
                        return 0;
                    }
                    else
                    {
                        return sec;
                    }
                }
                else
                {
                    return 0;
                }
            }
        }

        public override Point3D Position
        {
            get
            {
                return base.Position;
            }
            set
            {
                base.Position = value;

                if (Engine.MainWindow != null && Engine.MainWindow.MapWindow != null)
                {
                    Engine.MainWindow.MapWindow.PlayerMoved();
                }
            }
        }

        public override Direction Direction
        {
            get
            {
                return base.Direction;
            }
            set
            {
                base.Direction = value;
            }
        }

        public override void OnPositionChanging(Point3D newPos)
        {
            List<Mobile> mlist = new List<Mobile>( World.Mobiles.Values );
			for (int i=0;i<mlist.Count;i++)
            {
				Mobile m = mlist[i];
                if (m != this)
                {
                    if (!Utility.InRange(m.Position, newPos, VisRange))
                        m.Remove();
                    else
                        Targeting.CheckLastTargetRange(m);
                }
            }
            mlist = null;
            List<Item> ilist = new List<Item>( World.Items.Values );
            ScavengerAgent s = ScavengerAgent.Instance;
			for (int i=0;i<ilist.Count;i++)
            {
				Item item = ilist[i];
                if (item.Deleted || item.Container != null)
                    continue;

                int dist = Utility.Distance(item.GetWorldPosition(), newPos);
                if (item != DragDropManager.Holding && (dist > MultiVisRange || (!item.IsMulti && dist > VisRange)))
                    item.Remove();
                else if (!IsGhost && Visible && dist <= 2 && s.Enabled && item.Movable)
                    s.Scavenge(item);
            }
            ilist = null;
            base.OnPositionChanging(newPos);
        }

        public override void OnMapChange(byte old, byte cur)
        {
            List<Mobile> list = new List<Mobile>( World.Mobiles.Values );
            for (int i = 0; i < list.Count; i++)
            {
				Mobile m = list[i];
                if (m != this && m.Map != cur)
                    m.Remove();
            }
            list = null;
            World.Items.Clear();
            Counter.Reset();
            for (int i = 0; i < Contains.Count; i++)
            {
				Item item = Contains[i];
                World.AddItem(item);
                item.Contains.Clear();
            }

            if (Config.GetBool("AutoSearch") && Backpack != null)
                PlayerData.DoubleClick(Backpack);

            ClientCommunication.PostMapChange(cur);

            if (Engine.MainWindow != null && Engine.MainWindow.MapWindow != null)
                Engine.MainWindow.MapWindow.PlayerMoved();
        }

        protected override void OnNotoChange(byte old, byte cur)
        {
            if ((old == 3 || old == 4) && (cur != 3 && cur != 4))
            {
                // grey is turning off
                // SendMessage( "You are no longer a criminal." );
                if (m_CriminalTime != null)
                    m_CriminalTime.Stop();
                m_CriminalStart = DateTime.MinValue;
                ClientCommunication.RequestTitlebarUpdate();
            }
            else if ((cur == 3 || cur == 4) && (old != 3 && old != 4 && old != 0))
            {
                // grey is turning on
                ResetCriminalTimer();
            }
        }

        public void ResetCriminalTimer()
        {
			if ( m_CriminalStart == DateTime.MinValue || DateTime.UtcNow - m_CriminalStart >= TimeSpan.FromSeconds( 1 ) )
            {
				m_CriminalStart = DateTime.UtcNow;
                if (m_CriminalTime == null)
                    m_CriminalTime = new CriminalTimer(this);
                m_CriminalTime.Start();
                ClientCommunication.RequestTitlebarUpdate();
            }
        }

        private class CriminalTimer : Timer
        {
            private PlayerData m_Player;
            public CriminalTimer(PlayerData player) : base(TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1))
            {
                m_Player = player;
            }

            protected override void OnTick()
            {
                ClientCommunication.RequestTitlebarUpdate();
            }
        }

        internal void SendMessage(MsgLevel lvl, LocString loc, params object[] args)
        {
            SendMessage(lvl, Language.Format(loc, args));
        }

        internal void SendMessage(MsgLevel lvl, LocString loc)
        {
            SendMessage(lvl, Language.GetString(loc));
        }

        internal void SendMessage(LocString loc, params object[] args)
        {
            SendMessage(MsgLevel.Info, Language.Format(loc, args));
        }

        internal void SendMessage(LocString loc)
        {
            SendMessage(MsgLevel.Info, Language.GetString(loc));
        }

        internal void SendMessage(MsgLevel lvl, string format, params object[] args)
        {
            SendMessage(lvl, String.Format(format, args));
        }

		internal void SendMessage( string format, params object[] args )
		{
			SendMessage( MsgLevel.Info, String.Format( format, args ) );
		}

        internal void SendMessage(string text)
        {
            SendMessage(MsgLevel.Info, text);
        }

        internal void SendMessage(MsgLevel lvl, string text)
        {
            if (lvl >= (MsgLevel)Config.GetInt("MessageLevel") && text.Length > 0)
            {
                int hue;
                switch (lvl)
                {
                    case MsgLevel.Error:
                    case MsgLevel.Warning:
                        hue = Config.GetInt("WarningColor");
                        break;

                    default:
                        hue = Config.GetInt("SysColor");
                        break;
                }

                ClientCommunication.SendToClient(new UnicodeMessage(0xFFFFFFFF, -1, MessageType.Regular, hue, 3, Language.CliLocName, "System", text));

                PacketHandlers.SysMessages.Add(text.ToLower());

                if (PacketHandlers.SysMessages.Count >= 25)
                    PacketHandlers.SysMessages.RemoveRange(0, 10);
            }
        }

        public uint CurrentGumpS, CurrentGumpI;
        public bool HasGump;
        public uint CurrentMenuS;
        public ushort CurrentMenuI;
        public bool HasMenu;

        private ushort m_SpeechHue;
        public ushort SpeechHue { get { return m_SpeechHue; } set { m_SpeechHue = value; } }

        public sbyte LocalLightLevel { get { return m_LocalLight; } set { m_LocalLight = value; } }
        public byte GlobalLightLevel { get { return m_GlobalLight; } set { m_GlobalLight = value; } }
        public byte Season { get { return m_Season; } set { m_Season = value; } }
        public ushort Features { get { return m_Features; } set { m_Features = value; } }
        public int[] MapPatches { get { return m_MapPatches; } set { m_MapPatches = value; } }

        private int m_LastSkill = -1;
        public int LastSkill { get { return m_LastSkill; } set { m_LastSkill = value; } }

        private Serial m_LastObj = Serial.Zero;
        public Serial LastObject { get { return m_LastObj; } }

        private int m_LastSpell = -1;
        public int LastSpell { get { return m_LastSpell; } set { m_LastSpell = value; } }

        public static bool DoubleClick(object clicked)
        {
            return DoubleClick(clicked, true);
        }

        public static bool DoubleClick(object clicked, bool silent)
        {
            Serial s;
            if (clicked is Mobile)
                s = ((Mobile)clicked).Serial.Value;
            else if (clicked is Item)
                s = ((Item)clicked).Serial.Value;
            else if (clicked is Serial)
                s = ((Serial)clicked).Value;
            else
                s = Serial.Zero;

            if (s != Serial.Zero)
            {
                Item free = null, pack = World.Player.Backpack;
                if (s.IsItem && pack != null && Config.GetBool("PotionEquip") )// && ClientCommunication.AllowBit(FeatureBit.AutoPotionEquip))
                {
                    Item i = World.FindItem(s);
                    if (i != null && i.IsPotion && i.ItemID != 3853) // dont unequip for exploison potions
                    {
                        // dont worry about uneqipping RuneBooks or SpellBooks
                        Item left = World.Player.GetItemOnLayer(Layer.LeftHand);
                        Item right = World.Player.GetItemOnLayer(Layer.RightHand);

                        if (left != null && (right != null || left.IsTwoHanded))
                            free = left;
                        else if (right != null && right.IsTwoHanded)
                            free = right;

                        if (free != null)
                        {
                            if (DragDropManager.HasDragFor(free.Serial))
                                free = null;
                            else
                                DragDropManager.DragDrop(free, pack);
                        }
                    }
                }

                ActionQueue.DoubleClick(silent, s);

                if (free != null)
                    DragDropManager.DragDrop(free, World.Player, free.Layer, true);

                if (s.IsItem)
                    World.Player.m_LastObj = s;
            }

            return false;
        }
    }
}
