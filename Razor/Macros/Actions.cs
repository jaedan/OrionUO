using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Assistant.Macros
{
	public delegate void MacroMenuCallback( object[] Args );
	public class MacroMenuItem : MenuItem
	{
		private MacroMenuCallback m_Call;
		private object[] m_Args;

		public MacroMenuItem( LocString name, MacroMenuCallback call, params object[] args ) : base( Language.GetString( name ) )
		{
			base.Click += new EventHandler( OnMenuClick );
			m_Call = call;
			m_Args = args;
		}

		private void OnMenuClick( object sender, System.EventArgs e )
		{
			m_Call( m_Args );
		}
	}

	public abstract class MacroAction
	{
		protected Macro m_Parent;

		public MacroAction()
		{
		}

		public override string ToString()
		{
			return String.Format( "?{0}?", GetType().Name );
		}

		public virtual string Serialize()
		{
			return GetType().FullName;
		}

		protected string DoSerialize( params object[] args )
		{
			StringBuilder sb = new StringBuilder( GetType().FullName );
			for (int i=0;i<args.Length;i++)
				sb.AppendFormat( "|{0}", args[i] );
			return sb.ToString();
		}

		public virtual MenuItem[] GetContextMenuItems()
		{
			return null;
		}

		public Macro Parent { get{ return m_Parent; } set{ m_Parent = value; } }

		public abstract bool Perform();
	}

	public abstract class MacroWaitAction : MacroAction
	{
		protected TimeSpan m_Timeout = TimeSpan.FromMinutes( 5 );
		private DateTime m_Start;
		private MacroMenuItem m_MenuItem = null;

		public MacroWaitAction()
		{
		}

		public abstract bool PerformWait();
		public TimeSpan Timeout { get{ return m_Timeout; } }
		public DateTime StartTime{ get{ return m_Start; } set{ m_Start = value; } }

		public MacroMenuItem EditTimeoutMenuItem 
		{
			get 
			{
				if ( m_MenuItem == null )
					m_MenuItem = new MacroMenuItem( LocString.EditTimeout, new MacroMenuCallback( EditTimeout ) );
				return m_MenuItem;
			}
		}

		private void EditTimeout( object[] args )
		{
			if ( InputBox.Show( Language.GetString( LocString.NewTimeout ), Language.GetString( LocString.ChangeTimeout ), ((int)(m_Timeout.TotalSeconds)).ToString() ) )
				m_Timeout = TimeSpan.FromSeconds( InputBox.GetInt( 60 ) );
		}

		public virtual bool CheckMatch( MacroAction a )
		{
			return false; // a.GetType() == this.GetType();
		}
	}

	public class MacroComment : MacroAction
	{
		private string m_Comment;

		public MacroComment( string comment )
		{
			if ( comment == null )
				comment = "";

			m_Comment = comment.Trim();
		}

		public override bool Perform()
		{
			return true;
		}

		public override string Serialize()
		{
			return ToString();
		}

		public string Comment { get { return m_Comment; } set { m_Comment = value; } }

		public override string ToString()
		{
			if ( m_Comment == null )
				m_Comment = "";

			return String.Format( "// {0}", m_Comment );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.Edit, new MacroMenuCallback( Edit ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void Edit( object[] args )
		{
			if ( InputBox.Show( Language.GetString( LocString.InsComment ), Language.GetString( LocString.InputReq ), m_Comment ) )
			{
				if ( m_Comment == null )
					m_Comment = "";

				m_Comment = InputBox.GetString();

				if ( m_Comment == null )
					m_Comment = "";

				if ( m_Parent != null )
					m_Parent.Update();
			}
		}
	}

	public class DoubleClickAction : MacroAction
	{
		private Serial m_Serial;
		private ushort m_Gfx;

		public DoubleClickAction( Serial obj, ushort gfx )
		{
			m_Serial = obj;
			m_Gfx = gfx;
		}

		public DoubleClickAction( string[] args )
		{
			m_Serial = Serial.Parse( args[1] );
			m_Gfx = Convert.ToUInt16( args[2] );
		}

		public override bool Perform()
		{
			PlayerData.DoubleClick( m_Serial );
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Serial.Value,  m_Gfx );
		}

		public override string ToString()
		{
			return Language.Format( LocString.DClickA1, m_Serial );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.ReTarget, new MacroMenuCallback( ReTarget ) ),
					new MacroMenuItem( LocString.Conv2DCT, new MacroMenuCallback( ConvertToByType ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void ConvertToByType( object[] args )
		{
			if ( m_Gfx != 0 && m_Serial.IsItem && m_Parent != null )
				m_Parent.Convert( this, new DoubleClickTypeAction( m_Gfx, m_Serial.IsItem ) );
		}

		private void ReTarget( object[] args )
		{
			Targeting.OneTimeTarget( new Targeting.TargetResponseCallback( OnReTarget ) );
			World.Player.SendMessage( MsgLevel.Force, LocString.SelTargAct );
		}

		private void OnReTarget( bool ground, Serial serial, Point3D pt, ushort gfx )
		{
			if ( serial.IsItem )
			{
				m_Serial = serial;
				m_Gfx = gfx;
			}

			Engine.MainWindow.ShowMe();

			if ( m_Parent != null )
				m_Parent.Update();
		}
	}

	public class DoubleClickTypeAction : MacroAction
	{
		private ushort m_Gfx;
		public bool m_Item;

		public DoubleClickTypeAction( string[] args )
		{
			m_Gfx = Convert.ToUInt16( args[1] );
			try
			{
				m_Item = Convert.ToBoolean( args[2] );
			}
			catch
			{
			}
		}

		public DoubleClickTypeAction( ushort gfx, bool item )
		{
			m_Gfx = gfx;
			m_Item = item;
		}

		public override bool Perform()
		{
			Serial click = Serial.Zero;

			if ( m_Item )
			{
				Item item = World.Player.Backpack?.FindItemByID( m_Gfx );
				List<Item> list = new List<Item>();
				if ( item == null )
				{
					foreach ( Item i in World.Items.Values )
					{
						if ( i.ItemID == m_Gfx && i.RootContainer == null )
							list.Add( i );
					}
					if ( list.Count == 0 )
					{
						foreach ( Item i in World.Items.Values )
						{
							if ( i.ItemID == m_Gfx )
							{
								if ( !i.IsInBank )
									list.Add( i );
							}
						}
					}

					if ( list.Count > 0 )
						click = list[Utility.Random(list.Count)].Serial;
				}
				else
				{
					click = item.Serial;
				}
			}
			else
			{
				List<Mobile> list = new List<Mobile>();
				foreach ( Mobile m in World.MobilesInRange() )
				{
					if ( m.Body == m_Gfx )
						list.Add( m );
				}

				if ( list.Count > 0 )
					click = list[Utility.Random(list.Count)].Serial;
			}

			if ( click != Serial.Zero )
				PlayerData.DoubleClick( click );
			else
				World.Player.SendMessage( MsgLevel.Force, LocString.NoItemOfType, m_Item ? ((ItemID)m_Gfx).ToString() : String.Format( "(Character) 0x{0:X}", m_Gfx ) );
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Gfx, m_Item );
		}

		public override string ToString()
		{
			return Language.Format( LocString.DClickA1, m_Item ? ((ItemID)m_Gfx).ToString() : String.Format( "(Character) 0x{0:X}", m_Gfx ) );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.ReTarget, new MacroMenuCallback( ReTarget ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void ReTarget( object[] args )
		{
			Targeting.OneTimeTarget( new Targeting.TargetResponseCallback( OnReTarget ) );
			World.Player.SendMessage( LocString.SelTargAct );
		}

		private void OnReTarget( bool ground, Serial serial, Point3D pt, ushort gfx )
		{
			m_Gfx = gfx;
			m_Item = serial.IsItem;

			Engine.MainWindow.ShowMe();
			if ( m_Parent != null )
				m_Parent.Update();
		}
	}

	public class LiftAction : MacroWaitAction
	{
		private ushort m_Amount;
		private Serial m_Serial;
		private ushort m_Gfx;
		
		private static Item m_LastLift;
		public static Item LastLift { get{ return m_LastLift; } set{ m_LastLift = value; } }

		public LiftAction( string[] args )
		{
			m_Serial = Serial.Parse( args[1] );
			m_Amount = Convert.ToUInt16( args[2] );
			m_Gfx = Convert.ToUInt16( args[3] );
		}

		public LiftAction( Serial ser, ushort amount, ushort gfx )
		{
			m_Serial = ser;
			m_Amount = amount;
			m_Gfx = gfx;
		}

		private int m_Id;

		public override bool Perform()
		{
			Item item = World.FindItem( m_Serial );
			if ( item != null )
			{
				//DragDropManager.Holding = item;
				m_LastLift = item;
				m_Id = DragDropManager.Drag( item, m_Amount <= item.Amount ? m_Amount : item.Amount );
			}
			else
			{
				World.Player.SendMessage( MsgLevel.Warning, LocString.MacroItemOutRange );
			}
			return false;
		}

		public override bool PerformWait()
		{
			return DragDropManager.LastIDLifted < m_Id;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Serial.Value, m_Amount, m_Gfx );
		}

		public override string ToString()
		{
			return Language.Format( LocString.LiftA10, m_Serial, m_Amount );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.ConvLiftByType, new MacroMenuCallback( ConvertToByType ) ),
					new MacroMenuItem( LocString.Edit, new MacroMenuCallback( EditAmount ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void EditAmount( object[] args )
		{
			if ( InputBox.Show( Engine.MainWindow,  Language.GetString( LocString.EnterAmount ), Language.GetString( LocString.InputReq ), m_Amount.ToString() ) )
			{
				m_Amount = (ushort)InputBox.GetInt( m_Amount );

				if ( m_Parent != null )
					m_Parent.Update();
			}
		}

		private void ConvertToByType( object[] args )
		{
			if ( m_Gfx != 0 && m_Parent != null )
				m_Parent.Convert( this, new LiftTypeAction( m_Gfx, m_Amount ) );
		}
	}

	public class LiftTypeAction : MacroWaitAction
	{
		private ushort m_Gfx;
		private ushort m_Amount;

		public LiftTypeAction( string[] args )
		{
			m_Gfx = Convert.ToUInt16( args[1] );
			m_Amount = Convert.ToUInt16( args[2] );
		}

		public LiftTypeAction( ushort gfx, ushort amount )
		{
			m_Gfx = gfx;
			m_Amount = amount;
		}

		private int m_Id;
		public override bool Perform()
		{
			Item item = World.Player.Backpack?.FindItemByID( m_Gfx );
			/*if ( item == null )
			{
				ArrayList list = new ArrayList();

				foreach ( Item i in World.Items.Values )
				{
					if ( i.ItemID == m_Gfx && ( i.RootContainer == null || i.IsChildOf( World.Player.Quiver ) ) )
						list.Add( i );
				}

				if ( list.Count > 0 )
					item = (Item)list[ Utility.Random( list.Count ) ];
			}*/

			if ( item != null )
			{
				//DragDropManager.Holding = item;
				ushort amount = m_Amount;
				if ( item.Amount < amount )
					amount = item.Amount;
				LiftAction.LastLift = item;
				//ActionQueue.Enqueue( new LiftRequest( item, amount ) );
				m_Id = DragDropManager.Drag( item, amount );
			}
			else
			{
				World.Player.SendMessage( MsgLevel.Warning, LocString.NoItemOfType, (ItemID)m_Gfx );
				//MacroManager.Stop();
			}
			return false;
		}

		public override bool PerformWait()
		{
			return DragDropManager.LastIDLifted < m_Id && !DragDropManager.Empty;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Gfx, m_Amount );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.Edit, new MacroMenuCallback( EditAmount ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void EditAmount( object[] args )
		{
			if ( InputBox.Show( Engine.MainWindow,  Language.GetString( LocString.EnterAmount ), Language.GetString( LocString.InputReq ), m_Amount.ToString() ) )
			{
				m_Amount = (ushort)InputBox.GetInt( m_Amount );

				if ( m_Parent != null )
					m_Parent.Update();
			}
		}

		public override string ToString()
		{
			return Language.Format( LocString.LiftA10, m_Amount, (ItemID)m_Gfx );
		}
	}

	public class DropAction : MacroAction
	{
		private Serial m_To;
		private Point3D m_At;
		private Layer m_Layer;

		public DropAction( string[] args )
		{
			m_To = Serial.Parse( args[1] );
			m_At = Point3D.Parse( args[2] );
			try
			{
				m_Layer = (Layer)Byte.Parse( args[3] );
			}
			catch
			{
				m_Layer = Layer.Invalid;
			}
		}

		public DropAction( Serial to, Point3D at ) : this(to,at,0)
		{
		}

		public DropAction( Serial to, Point3D at, Layer layer )
		{
			m_To = to;
			m_At = at;
			m_Layer = layer;
		}

		public override bool Perform()
		{
			if ( DragDropManager.Holding != null )
			{
				if ( m_Layer > Layer.Invalid && m_Layer <= Layer.LastUserValid )
				{
					Mobile m = World.FindMobile( m_To );
					if ( m != null )
						DragDropManager.Drop( DragDropManager.Holding, m, m_Layer );
				}
				else
				{
					DragDropManager.Drop( DragDropManager.Holding, m_To, m_At );
				}
			}
			else
			{
				World.Player.SendMessage( MsgLevel.Warning, LocString.MacroNoHold );
			}
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_To, m_At, (byte)m_Layer );
		}

		public override string ToString()
		{
			if ( m_Layer != Layer.Invalid )
				return Language.Format( LocString.EquipTo, m_To, m_Layer );
			else
				return Language.Format( LocString.DropA2, m_To.IsValid ? m_To.ToString() : "Ground", m_At );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_To.IsValid )
			{
				return null; // Dont allow conversion(s)
			}
			else
			{
				if ( m_MenuItems == null )
				{
					m_MenuItems = new MacroMenuItem[]
					{
						new MacroMenuItem( LocString.ConvRelLoc, new MacroMenuCallback( ConvertToRelLoc ) ),
					};
				}
				
				return m_MenuItems;
			}
		}

		private void ConvertToRelLoc( object[] args )
		{
			if ( !m_To.IsValid && m_Parent != null )
				m_Parent.Convert( this, new DropRelLocAction( (sbyte)(m_At.X - World.Player.Position.X), (sbyte)(m_At.Y - World.Player.Position.Y), (sbyte)(m_At.Z - World.Player.Position.Z) ) );
		}
	}

	public class DropRelLocAction : MacroAction
	{
		private sbyte[] m_Loc;

		public DropRelLocAction( string[] args )
		{
			m_Loc = new sbyte[3]
			{
				Convert.ToSByte( args[1] ),
				Convert.ToSByte( args[2] ),
				Convert.ToSByte( args[3] ),
			};
		}

		public DropRelLocAction( sbyte x, sbyte y, sbyte z )
		{
			m_Loc = new sbyte[3]{ x, y, z };
		}

		public override bool Perform()
		{
			if ( DragDropManager.Holding != null )
				DragDropManager.Drop( DragDropManager.Holding, null, new Point3D( (ushort)(World.Player.Position.X + m_Loc[0]), (ushort)(World.Player.Position.Y + m_Loc[1]), (short)(World.Player.Position.Z + m_Loc[2]) ) );
			else
				World.Player.SendMessage( LocString.MacroNoHold );
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Loc[0], m_Loc[1], m_Loc[2] );
		}

		public override string ToString()
		{
			return Language.Format( LocString.DropRelA3, m_Loc[0], m_Loc[1], m_Loc[2] );
		}
	}

	public class GumpResponseAction : MacroAction
	{
		private int m_ButtonID;
		private int[] m_Switches;
		private GumpTextEntry[] m_TextEntries;

		public GumpResponseAction( string[] args )
		{
			m_ButtonID = Convert.ToInt32( args[1] );
			m_Switches = new int[Convert.ToInt32( args[2] )];
			for(int i=0;i<m_Switches.Length;i++)
				m_Switches[i] = Convert.ToInt32( args[3+i] );
			m_TextEntries = new GumpTextEntry[Convert.ToInt32( args[3+m_Switches.Length] )];
			for(int i=0;i<m_TextEntries.Length;i++)
			{
				string[] split = args[4+m_Switches.Length+i].Split( '&' );
				m_TextEntries[i].EntryID = Convert.ToUInt16( split[0] );
				m_TextEntries[i].Text = split[1];
			}
		}

		public GumpResponseAction( int button, int[] switches, GumpTextEntry[] entries )
		{
			m_ButtonID = button;
			m_Switches = switches;
			m_TextEntries = entries;
		}

		public override bool Perform()
		{
			ClientCommunication.SendToClient( new CloseGump( World.Player.CurrentGumpI ) );
			ClientCommunication.SendToServer( new GumpResponse( World.Player.CurrentGumpS, World.Player.CurrentGumpI, m_ButtonID, m_Switches, m_TextEntries ) );
			World.Player.HasGump = false;
			return true;
		}

		public override string Serialize()
		{
            ArrayList list = new ArrayList(3 + m_Switches.Length + m_TextEntries.Length)
            {
                m_ButtonID,
                m_Switches.Length
            };
            list.AddRange( m_Switches );
			list.Add( m_TextEntries.Length );
			for(int i=0;i<m_TextEntries.Length;i++)
				list.Add( String.Format( "{0}&{1}", m_TextEntries[i].EntryID, m_TextEntries[i].Text ) );
			return DoSerialize( (object[])list.ToArray( typeof( object ) ) );
		}

		public override string ToString()
		{
			if ( m_ButtonID != 0 )
				return Language.Format( LocString.GumpRespB, m_ButtonID );
			else
				return Language.Format( LocString.CloseGump );
		}
	}

	public class MenuResponseAction : MacroAction
	{
		private ushort m_Index, m_ItemID, m_Hue;

		public MenuResponseAction( string[] args )
		{
			m_Index = Convert.ToUInt16( args[1] );
			m_ItemID = Convert.ToUInt16( args[2] );
			m_Hue = Convert.ToUInt16( args[3] );
		}

		public MenuResponseAction( ushort idx, ushort iid, ushort hue )
		{
			m_Index = idx;
			m_ItemID = iid;
			m_Hue = hue;
		}

		public override bool Perform()
		{
			ClientCommunication.SendToServer( new MenuResponse( World.Player.CurrentMenuS, World.Player.CurrentMenuI, m_Index, m_ItemID, m_Hue ) );
			World.Player.HasMenu = false;
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Index, m_ItemID, m_Hue );
		}

		public override string ToString()
		{
			return Language.Format( LocString.MenuRespA1, m_Index );
		}
	}

	public class AbsoluteTargetAction : MacroAction
	{
		private TargetInfo m_Info;

		public AbsoluteTargetAction( string[] args )
		{
            m_Info = new TargetInfo()
            {
                Type = Convert.ToByte(args[1]),
                Flags = Convert.ToByte(args[2]),
                Serial = Convert.ToUInt32(args[3]),
                X = Convert.ToUInt16(args[4]),
                Y = Convert.ToUInt16(args[5]),
                Z = Convert.ToInt16(args[6]),
                Gfx = Convert.ToUInt16(args[7])
            };
        }

		public AbsoluteTargetAction( TargetInfo info )
		{
            m_Info = new TargetInfo()
            {
                Type = info.Type,
                Flags = info.Flags,
                Serial = info.Serial,
                X = info.X,
                Y = info.Y,
                Z = info.Z,
                Gfx = info.Gfx
            };
        }

		public override bool Perform()
		{
			Targeting.Target( m_Info );
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Info.Type, m_Info.Flags, m_Info.Serial.Value, m_Info.X, m_Info.Y, m_Info.Z, m_Info.Gfx );
		}

		public override string ToString()
		{
			return Language.GetString( LocString.AbsTarg );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.ReTarget, new MacroMenuCallback( ReTarget ) ),
					new MacroMenuItem( LocString.ConvLT, new MacroMenuCallback( ConvertToLastTarget ) ),
					new MacroMenuItem( LocString.ConvTargType, new MacroMenuCallback( ConvertToByType ) ),
					new MacroMenuItem( LocString.ConvRelLoc, new MacroMenuCallback( ConvertToRelLoc ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void ReTarget( object[] args )
		{
			Targeting.OneTimeTarget( !m_Info.Serial.IsValid, new Targeting.TargetResponseCallback( ReTargetResponse ) );
			World.Player.SendMessage( MsgLevel.Force, LocString.SelTargAct );
		}

		private void ReTargetResponse( bool ground, Serial serial, Point3D pt, ushort gfx )
		{
			m_Info.Gfx = gfx;
			m_Info.Serial = serial;
			m_Info.Type = (byte)(ground ? 1 : 0);
			m_Info.X = pt.X;
			m_Info.Y = pt.Y;
			m_Info.Z = pt.Z;

			Engine.MainWindow.ShowMe();
			if ( m_Parent != null )
				m_Parent.Update();
		}

		private void ConvertToLastTarget( object[] args )
		{
			if ( m_Parent != null )
				m_Parent.Convert( this, new LastTargetAction() );
		}

		private void ConvertToByType( object[] args )
		{
			if ( m_Parent != null )
				m_Parent.Convert( this, new TargetTypeAction( m_Info.Serial.IsMobile, m_Info.Gfx ) );
		}

		private void ConvertToRelLoc( object[] args )
		{
			if ( m_Parent != null )
				m_Parent.Convert( this, new TargetRelLocAction( (sbyte)(m_Info.X - World.Player.Position.X), (sbyte)(m_Info.Y - World.Player.Position.Y) ) );//, (sbyte)(m_Info.Z - World.Player.Position.Z) ) );
		}
	}

	public class TargetTypeAction : MacroAction
	{
		private bool m_Mobile;
		private ushort m_Gfx;

		public TargetTypeAction( string[] args )
		{
			m_Mobile = Convert.ToBoolean( args[1] );
			m_Gfx = Convert.ToUInt16( args[2] );
		}

		public TargetTypeAction( bool mobile, ushort gfx )
		{
			m_Mobile = mobile;
			m_Gfx = gfx;
		}

		public override bool Perform()
		{
			List<UOEntity> list = new List<UOEntity>();
			if ( m_Mobile )
			{
				foreach ( Mobile find in World.MobilesInRange() )
				{
					if ( find.Body == m_Gfx )
						list.Add( find );
				}
			}
			else
			{
				foreach ( Item i in World.Items.Values )
				{
					if ( i.ItemID == m_Gfx && !i.IsInBank )
						list.Add( i );
				}
			}

			if ( list.Count > 0 )
				Targeting.Target( list[Utility.Random( list.Count )] );
			else
				World.Player.SendMessage( MsgLevel.Warning, LocString.NoItemOfType, m_Mobile ? String.Format( "Character [{0}]", m_Gfx ) : ((ItemID)m_Gfx).ToString() );
			
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Mobile, m_Gfx );
		}

		public override string ToString()
		{
			if ( m_Mobile )
				return Language.Format( LocString.TargByType, m_Gfx );
			else
				return Language.Format( LocString.TargByType, (ItemID)m_Gfx );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.ReTarget, new MacroMenuCallback( ReTarget ) ),
					new MacroMenuItem( LocString.ConvLT, new MacroMenuCallback( ConvertToLastTarget ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void ReTarget( object[] args )
		{
			Targeting.OneTimeTarget( false, new Targeting.TargetResponseCallback( ReTargetResponse ) );
			World.Player.SendMessage( MsgLevel.Force, LocString.SelTargAct );
		}

		private void ReTargetResponse( bool ground, Serial serial, Point3D pt, ushort gfx )
		{
			if ( !ground && serial.IsValid )
			{
				m_Mobile = serial.IsMobile;
				m_Gfx = gfx;
			}
			Engine.MainWindow.ShowMe();
			if ( m_Parent != null )
				m_Parent.Update();
		}

		private void ConvertToLastTarget( object[] args )
		{
			if ( m_Parent != null )
				m_Parent.Convert( this, new LastTargetAction() );
		}
	}

	public class TargetRelLocAction : MacroAction
	{
		private sbyte m_X, m_Y;

		public TargetRelLocAction( string[] args )
		{
			m_X = Convert.ToSByte( args[1] );
			m_Y = Convert.ToSByte( args[2] );
		}

		public TargetRelLocAction( sbyte x, sbyte y )
		{
			m_X = x;
			m_Y = y;
		}

		public override bool Perform()
		{
			ushort x = (ushort)(World.Player.Position.X + m_X);
			ushort y = (ushort)(World.Player.Position.Y + m_Y);
			short z = (short)World.Player.Position.Z;
			try
			{
				Ultima.HuedTile tile = Map.GetTileNear( World.Player.Map, x, y, z );
				Targeting.Target( new Point3D( x, y, tile.Z ), (ushort)tile.ID );	
			}
			catch ( Exception e )
			{
				World.Player.SendMessage( MsgLevel.Debug, "Error Executing TargetRelLoc: {0}", e.Message );
			}
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_X, m_Y );
		}

		public override string ToString()
		{
			return Language.Format( LocString.TargRelLocA3, m_X, m_Y, 0 );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.ReTarget, new MacroMenuCallback( ReTarget ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void ReTarget( object[] args )
		{
			Engine.MainWindow.ShowMe();

			Targeting.OneTimeTarget( true, new Targeting.TargetResponseCallback( ReTargetResponse ) );
			World.Player.SendMessage( LocString.SelTargAct );
		}

		private void ReTargetResponse( bool ground, Serial serial, Point3D pt, ushort gfx )
		{
			m_X = (sbyte)(pt.X - World.Player.Position.X);
			m_Y = (sbyte)(pt.Y - World.Player.Position.Y);
			// m_Z = (sbyte)(pt.Z - World.Player.Position.Z);
			if ( m_Parent != null )
				m_Parent.Update();
		}
	}

	public class LastTargetAction : MacroAction
	{
		public LastTargetAction()
		{
		}

		public override bool Perform()
		{
			if ( !Targeting.DoLastTarget() )//Targeting.LastTarget( true );
				Targeting.ResendTarget();
			return true;
		}

		public override string ToString()
		{
			return String.Format( "Exec: {0}", Language.GetString( LocString.LastTarget ) );
		}
	}

	public class SetLastTargetAction : MacroWaitAction
	{
		public SetLastTargetAction()
		{
		}

		public override bool Perform()
		{
			Targeting.TargetSetLastTarget();
			return !PerformWait();
		}

		public override bool PerformWait()
		{
			return !Targeting.LTWasSet;
		}

		public override string ToString()
		{
			return Language.GetString( LocString.SetLT );
		}
	}

	public class SpeechAction : MacroAction
	{
		private MessageType m_Type;
		private ushort m_Font;
		private ushort m_Hue;
		private string m_Lang;
		private ArrayList m_Keywords;
		private string m_Speech;

		public SpeechAction( string[] args )
		{
			m_Type = ((MessageType)Convert.ToInt32( args[1] )) & ~MessageType.Encoded;
			m_Hue = Convert.ToUInt16( args[2] );
			m_Font = Convert.ToUInt16( args[3] );
			m_Lang = args[4];

			int count = Convert.ToInt32( args[5] );
			if ( count > 0 )
			{
                m_Keywords = new ArrayList(count)
                {
                    Convert.ToUInt16(args[6])
                };
                for ( int i = 1; i < count; i++ )
					m_Keywords.Add( Convert.ToByte( args[6+i] ) );
			}

			m_Speech = args[6+count];
		}

		public SpeechAction( MessageType type, ushort hue, ushort font, string lang, ArrayList kw, string speech )
		{
			m_Type = type;
			m_Hue = hue;
			m_Font = font;
			m_Lang = lang;
			m_Keywords = kw;
			m_Speech = speech;
		}

        public override bool Perform()
        {
            int hue = m_Hue;

            if (m_Type != MessageType.Emote)
            {
                if (World.Player.SpeechHue == 0)
                    World.Player.SpeechHue = m_Hue;
                hue = World.Player.SpeechHue;
            }

            ClientCommunication.SendToServer(new ClientUniMessage(m_Type, hue, m_Font, m_Lang, m_Keywords, m_Speech));
            return true;
        }
        /*public override bool Perform()
		{
			if ( m_Speech.Length > 1 && m_Speech[0] == '-' )
			{
				string text = m_Speech.Substring( 1 );
				string[] split = text.Split( ' ', '\t' );
                CommandCallback call;
                Command.List.TryGetValue(split[0], out call);
				if ( call == null && text[0] == '-' )
				{
                    Command.List.TryGetValue("-", out call);
					if ( call != null && split.Length > 1 && split[1] != null && split[1].Length > 1 )
						split[1] = split[1].Substring( 1 );
				}

				if ( call != null )
				{
					List<string> list = new List<string>();
					for(int i=1;i<split.Length;i++)
					{
						if ( split[i] != null && split[i].Length > 0 )
							list.Add( split[i] );
					}
					call(list.ToArray());
					return true;
				}
			}

			int hue = m_Hue;

			if ( m_Type != MessageType.Emote )
			{
				if ( World.Player.SpeechHue == 0 )
					World.Player.SpeechHue = m_Hue;
				hue = World.Player.SpeechHue;
			}
			
			ClientCommunication.SendToServer( new ClientUniMessage( m_Type, hue, m_Font, m_Lang, m_Keywords, m_Speech ) );
			return true;
		}*/

		public override string Serialize()
		{
            List<object> list = new List<object>(6)
            {
                (int)m_Type,
                m_Hue,
                m_Font,
                m_Lang
            };
            if ( m_Keywords != null && m_Keywords.Count > 1 )
			{
				list.Add( (int)m_Keywords.Count );
				for(int i=0;i<m_Keywords.Count;i++)
					list.Add( m_Keywords[i] );
			}
			else
			{
				list.Add( "0" );
			}
			list.Add( m_Speech );

			return DoSerialize(list.ToArray());
		}

		public override string ToString()
		{
			//return Language.Format( LocString.SayQA1, m_Speech );
			StringBuilder sb = new StringBuilder();
			switch ( m_Type )
			{
				case MessageType.Emote:
					sb.Append( "Emote: " );
					break;
				case MessageType.Whisper:
					sb.Append( "Whisper: " );
					break;
				case MessageType.Yell:
					sb.Append( "Yell: " );
					break;
				case MessageType.Regular:
				default:
					sb.Append( "Say: " );
					break;
			}
			sb.Append( m_Speech );
			return sb.ToString();
		}
	}

	public class UseSkillAction : MacroAction
	{
		private int m_Skill;

		public UseSkillAction( string[] args )
		{
			m_Skill = Convert.ToInt32( args[1] );
		}

		public UseSkillAction( int sk )
		{
			m_Skill = sk;
		}

		public override bool Perform()
		{
			ClientCommunication.SendToServer( new UseSkill( m_Skill ) );
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Skill );
		}

		public override string ToString()
		{
			return Language.Format( LocString.UseSkillA1, Language.Skill2Str( m_Skill ) );
		}
	}

	public class ExtCastSpellAction : MacroAction
	{
		private Spell m_Spell;
		private Serial m_Book;

		public ExtCastSpellAction( string[] args )
		{
			m_Spell = Spell.Get( Convert.ToInt32( args[1] ) );
			m_Book = Serial.Parse( args[2] );
		}

		public ExtCastSpellAction( int s, Serial book )
		{
			m_Spell = Spell.Get( s );
			m_Book = book;
		}

		public ExtCastSpellAction( Spell s, Serial book )
		{
			m_Spell = s;
			m_Book = book;
		}

		public override bool Perform()
		{
			m_Spell.Cast(m_Spell.GetID());
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Spell.GetID(), m_Book.Value );
		}

		public override string ToString()
		{
			return Language.Format( LocString.CastSpellA1, m_Spell );
		}
	}

	public class BookCastSpellAction : MacroAction
	{
		private Spell m_Spell;
		private Serial m_Book;

		public BookCastSpellAction( string[] args )
		{
			m_Spell = Spell.Get( Convert.ToInt32( args[1] ) );
			m_Book = Serial.Parse( args[2] );
		}

		public BookCastSpellAction( int s, Serial book )
		{
			m_Spell = Spell.Get( s );
			m_Book = book;
		}

		public BookCastSpellAction( Spell s, Serial book )
		{
			m_Spell = s;
			m_Book = book;
		}

		public override bool Perform()
		{
			m_Spell.Cast(m_Spell.GetID());
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Spell.GetID(), m_Book.Value );
		}

		public override string ToString()
		{
			return Language.Format( LocString.CastSpellA1, m_Spell );
		}
	}

	public class MacroCastSpellAction : MacroAction
	{
		private Spell m_Spell;

		public MacroCastSpellAction( string[] args )
		{
			m_Spell = Spell.Get( Convert.ToInt32( args[1] ) );
		}

		public MacroCastSpellAction( int s )
		{
			m_Spell = Spell.Get( s );
		}

		public MacroCastSpellAction( Spell s )
		{
			m_Spell = s;
		}

		public override bool Perform()
		{
			m_Spell.Cast(m_Spell.GetID());
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Spell.GetID() );
		}

		public override string ToString()
		{
			return Language.Format( LocString.CastSpellA1, m_Spell );
		}
	}

	public class SetAbilityAction : MacroAction
	{
		private AOSAbility m_Ability;

		public SetAbilityAction( string[] args )
		{
			m_Ability = (AOSAbility)Convert.ToInt32( args[1] );
		}

		public SetAbilityAction( AOSAbility a )
		{
			m_Ability = a;
		}

		public override bool Perform()
		{
			ClientCommunication.SendToServer( new UseAbility( m_Ability ) );
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( (int)m_Ability );
		}

		public override string ToString()
		{
			return Language.Format( LocString.SetAbilityA1, m_Ability );
		}
	}

	public class DressAction : MacroWaitAction
	{
		private string m_Name;

		public DressAction( string[] args )
		{
			m_Name = args[1];
		}

		public DressAction( string name )
		{
			m_Name = name;
		}

		public override bool Perform()
		{
			DressList list = DressList.Find( m_Name );
			if ( list != null )
			{
				list.Dress();
				return false;
			}
			else
			{
				return true;
			}
		}

		public override bool PerformWait()
		{
			return !ActionQueue.Empty;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Name );
		}

		public override string ToString()
		{
			return Language.Format( LocString.DressA1, m_Name );
		}
	}

	public class UnDressAction : MacroWaitAction
	{
		private string m_Name;
		private byte m_Layer;

		public UnDressAction( string[] args )
		{
			try
			{
				m_Layer = Convert.ToByte( args[2] );
			}
			catch
			{
				m_Layer = 255;
			}

			if ( m_Layer == 255 )
				m_Name = args[1];
			else
				m_Name = "";
		}

		public UnDressAction( string name )
		{
			m_Name = name;
			m_Layer = 255;
		}

		public UnDressAction( byte layer )
		{
			m_Layer = layer;
			m_Name = "";
		}

		public override bool Perform()
		{
			if ( m_Layer == 255 )
			{
				DressList list = DressList.Find( m_Name );
				if ( list != null )
				{
					list.Undress();
					return false;
				}
				else
				{
					return true;
				}
			}
			else if ( m_Layer == 0 )
			{
				HotKeys.UndressHotKeys.OnUndressAll();
				return false;
			}
			else
			{
				return !HotKeys.UndressHotKeys.Unequip( (Layer)m_Layer );
			}
		}

		public override bool PerformWait()
		{
			return !ActionQueue.Empty;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Name, m_Layer );
		}

		public override string ToString()
		{
			if ( m_Layer == 255 )
				return Language.Format( LocString.UndressA1, m_Name );
			else if ( m_Layer == 0 )
				return Language.GetString( LocString.UndressAll );
			else
				return Language.Format( LocString.UndressLayerA1, (Layer)m_Layer );
		}
	}

	public class WaitForMenuAction : MacroWaitAction
	{
		private uint m_MenuID;

		public WaitForMenuAction( uint gid )
		{
			m_MenuID = gid;
		}

		public WaitForMenuAction( string[] args )
		{
			if ( args.Length > 1 )
				m_MenuID = Convert.ToUInt32( args[1] );

			try
			{
				m_Timeout = TimeSpan.FromSeconds( Convert.ToDouble( args[2] ) );
			}
			catch
			{
			}
		}

		public override bool Perform()
		{
			return !PerformWait();
		}

		public override bool PerformWait()
		{
			return !(World.Player.HasMenu && ( World.Player.CurrentGumpI == m_MenuID || m_MenuID == 0 ));
		}

		public override string ToString()
		{
			if ( m_MenuID == 0 )
				return Language.GetString( LocString.WaitAnyMenu );
			else
				return Language.Format( LocString.WaitMenuA1, m_MenuID );
		}

		public override string Serialize()
		{
			return DoSerialize( m_MenuID, m_Timeout.TotalSeconds );
		}

		public override bool CheckMatch(MacroAction a)
		{
			if ( a is WaitForMenuAction )
			{
				if ( m_MenuID == 0 || ((WaitForMenuAction)a).m_MenuID == m_MenuID )
					return true;
			}
			
			return false;
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.Edit, new MacroMenuCallback( Edit ) ),
					this.EditTimeoutMenuItem,
				};
			}
			
			return m_MenuItems;
		}

		private void Edit( object[] args )
		{
			new MacroInsertWait( this ).ShowDialog( Engine.MainWindow );
		}
	}

	public class WaitForGumpAction : MacroWaitAction
	{
		private uint m_GumpID;
		private bool m_Strict;

		public WaitForGumpAction()
		{
			m_GumpID = 0;
			m_Strict = false;
		}

		public WaitForGumpAction( uint gid )
		{
			m_GumpID = gid;
			m_Strict = false;
		}

		public WaitForGumpAction( string[] args )
		{
			m_GumpID = Convert.ToUInt32( args[1] );
			try
			{
				m_Strict = Convert.ToBoolean( args[2] );
			}
			catch
			{
				m_Strict = false;
			}

			try
			{
				m_Timeout = TimeSpan.FromSeconds( Convert.ToDouble( args[3] ) );
			}
			catch
			{
			}
		}

		public override bool Perform()
		{
			return !PerformWait();
		}

		public override bool PerformWait()
		{
			return !(World.Player.HasGump && ( World.Player.CurrentGumpI == m_GumpID || !m_Strict || m_GumpID == 0 ));
		}

		public override string ToString()
		{
			if ( m_GumpID == 0 || !m_Strict )
				return Language.GetString( LocString.WaitAnyGump );
			else
				return Language.Format( LocString.WaitGumpA1, m_GumpID );
		}

		public override string Serialize()
		{
			return DoSerialize( m_GumpID, m_Strict, m_Timeout.TotalSeconds );
		}

		public override bool CheckMatch( MacroAction a )
		{
			if ( a is WaitForGumpAction )
			{
				if ( m_GumpID == 0 || ((WaitForGumpAction)a).m_GumpID == m_GumpID )
					return true;
			}
			
			return false;
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.Edit, new MacroMenuCallback( Edit ) ),
					new MacroMenuItem( LocString.Null, new MacroMenuCallback( ToggleStrict ) ),
					this.EditTimeoutMenuItem,
				};
			}

			if ( !m_Strict )
				m_MenuItems[1].Text = String.Format( "Change to \"{0}\"", Language.Format( LocString.WaitGumpA1, m_GumpID ) );
			else
				m_MenuItems[1].Text = String.Format( "Change to \"{0}\"", Language.GetString( LocString.WaitAnyGump ) );
			m_MenuItems[1].Enabled = m_GumpID != 0 || m_Strict;
			
			return m_MenuItems;
		}

		private void Edit( object[] args )
		{
			new MacroInsertWait( this ).ShowDialog( Engine.MainWindow );
		}

		private void ToggleStrict( object[] args )
		{
			m_Strict = !m_Strict;
			if ( m_Parent != null )
				m_Parent.Update();
		}
	}

	public class WaitForTargetAction : MacroWaitAction
	{
		public WaitForTargetAction()
		{
			m_Timeout = TimeSpan.FromSeconds( 30.0 );
		}

		public WaitForTargetAction( string[] args )
		{
			try
			{
				m_Timeout = TimeSpan.FromSeconds( Convert.ToDouble( args[1] ) );
			}
			catch
			{
				m_Timeout = TimeSpan.FromSeconds( 30.0 );
			}
		}

		public override bool Perform()
		{
			return !PerformWait();
		}

		public override bool PerformWait()
		{
			return !Targeting.HasTarget;
		}

		public override string ToString()
		{
			return Language.GetString( LocString.WaitTarg );
		}

		public override string Serialize()
		{
			return DoSerialize( m_Timeout.TotalSeconds );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.Edit, new MacroMenuCallback( Edit ) ),
					this.EditTimeoutMenuItem,
				};
			}
			
			return m_MenuItems;
		}

		public override bool CheckMatch( MacroAction a )
		{
			return (a is WaitForTargetAction);
		}

		private void Edit( object[] args )
		{
			new MacroInsertWait( this ).ShowDialog( Engine.MainWindow );
		}
	}

	public class PauseAction : MacroWaitAction
	{
		public PauseAction( string[] args )
		{
			m_Timeout = TimeSpan.Parse( args[1] );
		}

		public PauseAction( int ms )
		{
			m_Timeout = TimeSpan.FromMilliseconds( ms );
		}

		public PauseAction( TimeSpan time )
		{
			m_Timeout = time;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Timeout );
		}

		public override bool Perform()
		{
			this.StartTime = DateTime.UtcNow;
			return !PerformWait();
		}

		public override bool PerformWait()
		{
			return ( StartTime + m_Timeout >= DateTime.UtcNow );
		}

		public override string ToString()
		{
			return Language.Format( LocString.PauseA1, m_Timeout.TotalSeconds );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.Edit, new MacroMenuCallback( Edit ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void Edit( object[] args )
		{
			new MacroInsertWait( this ).ShowDialog( Engine.MainWindow );
		}
	}

	public class WaitForStatAction : MacroWaitAction
	{
		private byte m_Direction;
		private int m_Value;
		private IfAction.IfVarType m_Stat;

		public byte Op { get{ return m_Direction; } }
		public int Amount { get { return m_Value; } }
		public IfAction.IfVarType Stat{ get{ return m_Stat; } }

		public WaitForStatAction( string[] args )
		{
			m_Stat = (IfAction.IfVarType)Convert.ToInt32( args[1] );
			m_Direction = Convert.ToByte( args[2] );
			m_Value = Convert.ToInt32( args[3] );

			try
			{
				m_Timeout = TimeSpan.FromSeconds( Convert.ToDouble( args[4] ) );
			}
			catch
			{
				m_Timeout = TimeSpan.FromMinutes( 60.0 );
			}
		}

		public WaitForStatAction( IfAction.IfVarType stat, byte dir, int val )
		{
			m_Stat = stat;
			m_Direction = dir;
			m_Value = val;

			m_Timeout = TimeSpan.FromMinutes( 60.0 );
		}

		public override string Serialize()
		{
			return DoSerialize( (int)m_Stat, m_Direction, m_Value, m_Timeout.TotalSeconds );
		}

		public override bool Perform()
		{
			return !PerformWait();
		}

		public override bool PerformWait()
		{
			if ( m_Direction > 0 )
			{ 
				// wait for m_Stat >= m_Value
				switch ( m_Stat )
				{
					case IfAction.IfVarType.Hits:
						return World.Player.Hits < m_Value;
					case IfAction.IfVarType.Mana:
						return World.Player.Mana < m_Value;
					case IfAction.IfVarType.Stamina:
						return World.Player.Stam < m_Value;
				}
			}
			else
			{
				// wait for m_Stat <= m_Value
				switch ( m_Stat )
				{
					case IfAction.IfVarType.Hits:
						return World.Player.Hits > m_Value;
					case IfAction.IfVarType.Mana:
						return World.Player.Mana > m_Value;
					case IfAction.IfVarType.Stamina:
						return World.Player.Stam > m_Value;
				}
			}

			return false;
		}

		public override string ToString()
		{
			return Language.Format( LocString.WaitA3, m_Stat, m_Direction > 0 ? ">=" : "<=", m_Value );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.Edit, new MacroMenuCallback( Edit ) ),
					this.EditTimeoutMenuItem,
				};
			}
			
			return m_MenuItems;
		}

		private void Edit( object[] args )
		{
			new MacroInsertWait( this ).ShowDialog( Engine.MainWindow );
		}
	}

	public class IfAction : MacroAction
	{
		public enum IfVarType : int
		{
			Hits = 0,
			Mana,
			Stamina,
			Poisoned,
			SysMessage,
			Weight,
			Mounted,
			RHandEmpty,
			LHandEmpty,

			BeginCountersMarker,

			Counter = 50,
		}

		private sbyte m_Direction;
		private object m_Value;
		private IfVarType m_Var;
		private string m_Counter;
		private Assistant.Counter m_CountObj;

		public sbyte Op { get{ return m_Direction; } }
		public object Value { get { return m_Value; } }
		public IfVarType Variable { get{ return m_Var; } }
		public string Counter { get { return m_Counter; } }
		
		public IfAction( string[] args )
		{
			m_Var = (IfVarType)Convert.ToInt32( args[1] );
			try
			{
				m_Direction = Convert.ToSByte( args[2] );
				if ( m_Direction > 1 )
					m_Direction = 0;
			}
			catch
			{
				m_Direction = -1;
			}

			if ( m_Var != IfVarType.SysMessage )
				m_Value = Convert.ToInt32( args[3] );
			else
				m_Value = args[3].ToLower();

			if ( m_Var == IfVarType.Counter )
				m_Counter = args[4];
		}

		public IfAction( IfVarType var, sbyte dir, int val )
		{
			m_Var = var;
			m_Direction = dir;
			m_Value = val;
		}

		public IfAction( IfVarType var, sbyte dir, int val, string counter )
		{
			m_Var = var;
			m_Direction = dir;
			m_Value = val;
			m_Counter = counter;
		}

		public IfAction( IfVarType var, string text )
		{
			m_Var = var;
			m_Value = text.ToLower();
		}

		public override string Serialize()
		{
			if ( m_Var == IfVarType.Counter && m_Counter != null )
				return DoSerialize( (int)m_Var, m_Direction, m_Value, m_Counter );
			else
				return DoSerialize( (int)m_Var, m_Direction, m_Value );
		}

		public override bool Perform()
		{
			return true;
		}

		public bool Evaluate()
		{
			switch ( m_Var )
			{
				case IfVarType.Hits:
				case IfVarType.Mana:
				case IfVarType.Stamina:
				case IfVarType.Weight:
				{
					int val = (int)m_Value;
					if ( m_Direction > 0 )
					{ 
						// if stat >= m_Value
						switch ( m_Var )
						{
							case IfVarType.Hits:
								return World.Player.Hits >= val;
							case IfVarType.Mana:
								return World.Player.Mana >= val;
							case IfVarType.Stamina:
								return World.Player.Stam >= val;
							case IfVarType.Weight:
								return World.Player.Weight >= val;
						}
					}
					else
					{
						// if stat <= m_Value
						switch ( m_Var )
						{
							case IfVarType.Hits:
								return World.Player.Hits <= val;
							case IfVarType.Mana:
								return World.Player.Mana <= val;
							case IfVarType.Stamina:
								return World.Player.Stam <= val;
							case IfVarType.Weight:
								return World.Player.Weight <= val;
						}
					}
					
					return false;
				}

				case IfVarType.Poisoned:
				{
					//if ( ClientCommunication.AllowBit( FeatureBit.BlockHealPoisoned ) )
						return World.Player.Poisoned;
					/*else
						return false;*/
				}

				case IfVarType.SysMessage:
				{
					string text = (string)m_Value;
					for(int i=PacketHandlers.SysMessages.Count-1;i>=0;i--)
					{
						string sys = (string)PacketHandlers.SysMessages[i];
						if ( sys.IndexOf( text ) != -1 )
						{
							PacketHandlers.SysMessages.RemoveRange( 0, i+1 );
							return true;
						}
					}
					return false;
				}

				case IfVarType.Mounted:
				{
					return World.Player.GetItemOnLayer( Layer.Mount ) != null;
				}

				case IfVarType.RHandEmpty:
				{
					return World.Player.GetItemOnLayer( Layer.RightHand ) == null;
				}

				case IfVarType.LHandEmpty:
				{
					return World.Player.GetItemOnLayer( Layer.LeftHand ) == null;
				}

				case IfVarType.Counter:
				{
					if ( m_CountObj == null )
					{
						foreach ( Assistant.Counter c in Assistant.Counter.List )
						{
							if ( c.Name == m_Counter )
							{
								m_CountObj = c;
								break;
							}
						}
					}

					if ( m_CountObj == null || !m_CountObj.Enabled )
						return false;

					if ( m_Direction > 0 )
						return m_CountObj.Amount >= (int)m_Value;
					else
						return m_CountObj.Amount <= (int)m_Value;
				}

				default:
					return false;
			}
		}

		public override string ToString()
		{
			switch ( m_Var )
			{
				case IfVarType.Hits: case IfVarType.Mana: case IfVarType.Stamina: case IfVarType.Weight:
					return String.Format( "If ( {0} {1} {2} )", m_Var, m_Direction > 0 ? ">=" : "<=", m_Value );
				case IfVarType.Poisoned:
					return "If ( Poisoned )";
				case IfVarType.SysMessage:
				{
					string str = (string)m_Value;
					if ( str.Length > 10 )
						str = str.Substring( 0, 7 ) + "...";
					return String.Format( "If ( SysMessage \"{0}\" )", str );
				}
				case IfVarType.Mounted:
					return "If ( Mounted )";
				case IfVarType.RHandEmpty:
					return "If ( R-Hand Empty )";
				case IfVarType.LHandEmpty:
					return "If ( L-Hand Empty )";
				case IfVarType.Counter:
					return String.Format( "If ( \"{0} count\" {1} {2} )", m_Counter, m_Direction > 0 ? ">=" : "<=", m_Value );
				default:
					return "If ( ??? )";
			}
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.Edit, new MacroMenuCallback( Edit ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void Edit( object[] args )
		{
			new MacroInsertIf( this ).ShowDialog( Engine.MainWindow );
		}
	}

	public class ElseAction : MacroAction
	{
		public ElseAction()
		{
		}

		public override bool Perform()
		{
			return true;
		}

		public override string ToString()
		{
			return "Else";
		}
	}

	public class EndIfAction : MacroAction
	{
		public EndIfAction()
		{
		}

		public override bool Perform()
		{
			return true;
		}

		public override string ToString()
		{
			return "End If";
		}
	}

	public class HotKeyAction : MacroAction
	{
		private KeyData m_Key;

		public HotKeyAction( KeyData hk )
		{
			m_Key = hk;
		}

		public HotKeyAction( string[] args )
		{
			try
			{
				int loc = Convert.ToInt32( args[1] );
				if ( loc != 0 )
					m_Key = HotKey.Get( loc );
			}
			catch
			{
			}

			if ( m_Key == null )
				m_Key = HotKey.Get( args[2] );

			if ( m_Key == null )
				throw new Exception( "HotKey not found." );
		}

		public override bool Perform()
		{
			if ( m_Key.DispName.IndexOf( Language.GetString( LocString.PlayA1 ).Replace( @"{0}", "" ) ) == -1 )//ClientCommunication.AllowBit( FeatureBit.LoopingMacros ) ||
                m_Key.Callback();
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Key.LocName, m_Key.StrName ?? "");
		}

		public override string ToString()
		{
			return String.Format( "Exec: {0}", m_Key.DispName );
		}
	}

	public class ForAction : MacroAction
	{
		private int m_Max, m_Count;

		public int Count { get { return m_Count; } set { m_Count = value; } }
		public int Max { get{ return m_Max; } }
		
		public ForAction( string[] args )
		{
			m_Max = Convert.ToInt32( args[1] );
		}

		public ForAction( int max )
		{
			m_Max = max;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Max );
		}

		public override bool Perform()
		{
			return true;
		}

		public override string ToString()
		{
			return String.Format( "For ( 1 to {0} )", m_Max );
		}

		private MenuItem[] m_MenuItems;
		public override MenuItem[] GetContextMenuItems()
		{
			if ( m_MenuItems == null )
			{
				m_MenuItems = new MacroMenuItem[]
				{
					new MacroMenuItem( LocString.Edit, new MacroMenuCallback( Edit ) ),
				};
			}
			
			return m_MenuItems;
		}

		private void Edit( object[] args )
		{
			if ( InputBox.Show( Language.GetString( LocString.NumIter ), "Input Box", m_Max.ToString() ) )
				m_Max = InputBox.GetInt( m_Max );
			if ( Parent != null )
				Parent.Update();
		}
	}

	public class EndForAction : MacroAction
	{
		public EndForAction()
		{
		}

		public override bool Perform()
		{
			return true;
		}

		public override string ToString()
		{
			return "End For";
		}
	}

	public class ContextMenuAction : MacroAction
	{
		private ushort m_CtxName;
		private ushort m_Idx;
		private Serial m_Entity;

		public ContextMenuAction( UOEntity ent, ushort idx, ushort ctxName )
		{
			m_Entity = ent != null ? ent.Serial : Serial.MinusOne;

			if ( World.Player != null && World.Player.Serial == m_Entity )
				m_Entity = Serial.Zero;

			m_Idx = idx;
			m_CtxName = ctxName;
		}

		public ContextMenuAction( string[] args )
		{
			m_Entity = Serial.Parse( args[1] );
			m_Idx = Convert.ToUInt16( args[2] );
			try
			{
				m_CtxName =	Convert.ToUInt16( args[3] );
			}
			catch
			{
			}
		}

		public override bool Perform()
		{
			Serial s = m_Entity;

			if ( s == Serial.Zero && World.Player != null )
				s = World.Player.Serial;

			ClientCommunication.SendToServer( new ContextMenuRequest( s ) );
			ClientCommunication.SendToServer( new ContextMenuResponse( s, m_Idx ) );
			return true;
		}

		public override string Serialize()
		{
			return DoSerialize( m_Entity, m_Idx, m_CtxName );
		}

		public override string ToString()
		{
			string ent;

			if ( m_Entity == Serial.Zero )
				ent = "(self)";
			else
				ent = m_Entity.ToString();
			return String.Format( "ContextMenu: {1} ({0})", ent, m_Idx );
		}
	}
}

