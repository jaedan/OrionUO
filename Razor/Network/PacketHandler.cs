using System;
using System.Collections;

namespace Assistant
{
	public delegate void PacketViewerCallback( PacketReader p, PacketHandlerEventArgs args );
	public delegate void PacketFilterCallback( PacketReader p, PacketHandlerEventArgs args );

	public class PacketHandlerEventArgs
	{
		private bool m_Block;
		public bool Block
		{
			get{ return m_Block; }
			set{ m_Block = value; }
		}

		public PacketHandlerEventArgs()
		{
			Reinit();
		}

		public void Reinit()
		{
			m_Block = false;
		}
	}

	public class PacketHandler
	{
		private static Hashtable m_ClientViewers;
		private static Hashtable m_ServerViewers;

		private static Hashtable m_ClientFilters;
		private static Hashtable m_ServerFilters;

		static PacketHandler()
		{
			m_ClientViewers = new Hashtable();
			m_ServerViewers = new Hashtable();

			m_ClientFilters = new Hashtable();
			m_ServerFilters = new Hashtable();
		}

		internal static void RegisterClientToServerViewer( byte packetID, PacketViewerCallback callback )
		{
			ArrayList list = (ArrayList)m_ClientViewers[packetID];
			if ( list == null )
				m_ClientViewers[packetID] = list = new ArrayList();
			list.Add( callback );
		}

		internal static void RegisterServerToClientViewer(byte packetID, PacketViewerCallback callback )
		{
			ArrayList list = (ArrayList)m_ServerViewers[packetID];
			if ( list == null )
				m_ServerViewers[packetID] = list = new ArrayList();
			list.Add( callback );
		}

		internal static void RemoveClientToServerViewer(byte packetID, PacketViewerCallback callback )
		{
			ArrayList list = (ArrayList)m_ClientViewers[packetID];
			if ( list != null )
				list.Remove( callback );
		}

		internal static void RemoveServerToClientViewer(byte packetID, PacketViewerCallback callback )
		{
			ArrayList list = (ArrayList)m_ServerViewers[packetID];
			if ( list != null )
				list.Remove( callback );
		}

		internal static void RegisterClientToServerFilter(byte packetID, PacketFilterCallback callback )
		{
			ArrayList list = (ArrayList)m_ClientFilters[packetID];
			if ( list == null )
				m_ClientFilters[packetID] = list = new ArrayList();
			list.Add( callback );
		}

		internal static void RegisterServerToClientFilter(byte packetID, PacketFilterCallback callback )
		{
			ArrayList list = (ArrayList)m_ServerFilters[packetID];
			if ( list == null )
				m_ServerFilters[packetID] = list = new ArrayList();
			list.Add( callback );
		}

		internal static void RemoveClientToServerFilter(byte packetID, PacketFilterCallback callback )
		{
			ArrayList list = (ArrayList)m_ClientFilters[packetID];
			if ( list != null )
				list.Remove( callback );
		}

		internal static void RemoveServerToClientFilter(byte packetID, PacketFilterCallback callback )
		{
			ArrayList list = (ArrayList)m_ServerFilters[packetID];
			if ( list != null )
				list.Remove( callback );
		}

		public static bool OnServerPacket(PacketReader p)
		{
			bool result = false;

            ArrayList list = (ArrayList)m_ServerViewers[p.PacketID];
            if (list != null && list.Count > 0) {
                result = ProcessViewers(list, p);
			}

            list = (ArrayList)m_ServerFilters[p.PacketID];
            if (list != null && list.Count > 0) {
                result = ProcessFilters(list, p);
            }

            return result;
		}

		public static bool OnClientPacket(PacketReader p)
		{
            bool result = false;

            ArrayList list = (ArrayList)m_ClientViewers[p.PacketID];
            if (list != null && list.Count > 0) {
                result = ProcessViewers(list, p);
            }

            list = (ArrayList)m_ClientFilters[p.PacketID];
            if (list != null && list.Count > 0) {
                result = ProcessFilters(list, p);
            }

            return result;
        }

		private static PacketHandlerEventArgs m_Args = new PacketHandlerEventArgs();
		private static bool ProcessViewers( ArrayList list, PacketReader p )
		{
			m_Args.Reinit();

			if ( list != null )
			{
				for (int i=0;i<list.Count;i++)
				{
					p.MoveToData();

					try
					{
						((PacketViewerCallback)list[i])( p, m_Args );
					}
					catch ( Exception e )
					{
						Engine.LogCrash( e );
						new MessageDialog( "WARNING: Packet viewer exception!", true, e.ToString() ).Show();
					}
				}
			}

			return m_Args.Block;
		}

		private static bool ProcessFilters( ArrayList list, PacketReader p )
		{
			m_Args.Reinit();

			if ( list != null )
			{
				for (int i=0;i<list.Count;i++)
				{
					p.MoveToData();

					try
					{
						((PacketFilterCallback)list[i])( p, m_Args );
					}
					catch ( Exception e )
					{
						Engine.LogCrash( e );
						new MessageDialog( "WARNING: Packet filter exception!", true, e.ToString() ).Show();
					}
				}
			}

			return m_Args.Block;
		}
	}
}
