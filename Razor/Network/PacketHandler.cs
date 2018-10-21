using System;
using System.Collections.Generic;

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
		private static Dictionary<int, List<PacketViewerCallback>> m_ClientViewers;
		private static Dictionary<int, List<PacketViewerCallback>> m_ServerViewers;

		private static Dictionary<int, List<PacketFilterCallback>> m_ClientFilters;
		private static Dictionary<int, List<PacketFilterCallback>> m_ServerFilters;

		static PacketHandler()
		{
            m_ClientViewers = new Dictionary<int, List<PacketViewerCallback>>();
			m_ServerViewers = new Dictionary<int, List<PacketViewerCallback>>();

			m_ClientFilters = new Dictionary<int, List<PacketFilterCallback>>();
			m_ServerFilters = new Dictionary<int, List<PacketFilterCallback>>();
		}

		internal static void RegisterClientToServerViewer( byte packetID, PacketViewerCallback callback )
		{
            if (!m_ClientViewers.TryGetValue(packetID, out List<PacketViewerCallback> list) || list == null)
                m_ClientViewers[packetID] = list = new List<PacketViewerCallback>();
            list.Add( callback );
		}

		internal static void RegisterServerToClientViewer(byte packetID, PacketViewerCallback callback )
		{
            if (!m_ServerViewers.TryGetValue(packetID, out List<PacketViewerCallback> list) || list == null)
                m_ServerViewers[packetID] = list = new List<PacketViewerCallback>();
            list.Add( callback );
		}

		internal static void RemoveClientToServerViewer(byte packetID, PacketViewerCallback callback )
		{
            if (m_ClientViewers.TryGetValue(packetID, out List<PacketViewerCallback> list) && list != null)
                list.Remove(callback);
        }

		internal static void RemoveServerToClientViewer(byte packetID, PacketViewerCallback callback )
		{
            if (m_ServerViewers.TryGetValue(packetID, out List<PacketViewerCallback> list) && list != null)
                list.Remove(callback);
        }

		internal static void RegisterClientToServerFilter(byte packetID, PacketFilterCallback callback )
		{
            if (!m_ClientFilters.TryGetValue(packetID, out List<PacketFilterCallback> list) || list == null)
                m_ClientFilters[packetID] = list = new List<PacketFilterCallback>();
            list.Add( callback );
		}

		internal static void RegisterServerToClientFilter(byte packetID, PacketFilterCallback callback )
		{
            if (!m_ServerFilters.TryGetValue(packetID, out List<PacketFilterCallback> list) || list == null)
                m_ServerFilters[packetID] = list = new List<PacketFilterCallback>();
            list.Add( callback );
		}

		internal static void RemoveClientToServerFilter(byte packetID, PacketFilterCallback callback )
		{
            if (m_ClientFilters.TryGetValue(packetID, out List<PacketFilterCallback> list) && list != null)
                list.Remove(callback);
        }

		internal static void RemoveServerToClientFilter(byte packetID, PacketFilterCallback callback )
		{
            if (m_ServerFilters.TryGetValue(packetID, out List<PacketFilterCallback> list) && list != null)
                list.Remove(callback);
        }

		public static bool OnServerPacket(PacketReader p)
		{
			bool result = false;

            if (m_ServerViewers.TryGetValue(p.PacketID, out List<PacketViewerCallback> pvlist) && pvlist != null && pvlist.Count > 0)
                result = ProcessViewers(pvlist, p);
            if (m_ServerFilters.TryGetValue(p.PacketID, out List<PacketFilterCallback> pflist) && pflist != null && pflist.Count > 0)
                result |= ProcessFilters(pflist, p);

            return result;
		}

		public static bool OnClientPacket(PacketReader p)
		{
            bool result = false;
            if (m_ClientViewers.TryGetValue(p.PacketID, out List<PacketViewerCallback> pvlist) && pvlist != null && pvlist.Count > 0)
                result = ProcessViewers(pvlist, p);
            if (m_ClientFilters.TryGetValue(p.PacketID, out List<PacketFilterCallback> pflist) && pflist != null && pflist.Count > 0)
                result |= ProcessFilters(pflist, p);

            return result;
        }

		private static PacketHandlerEventArgs m_Args = new PacketHandlerEventArgs();
		private static bool ProcessViewers( List<PacketViewerCallback> list, PacketReader p )
		{
			m_Args.Reinit();

			if ( list != null )
			{
                int count = list.Count;
				for (int i=0;i<count;i++)
				{
					p.MoveToData();

					try
					{
                        list[i]( p, m_Args );
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

		private static bool ProcessFilters(List<PacketFilterCallback> list, PacketReader p )
		{
			m_Args.Reinit();

			if ( list != null )
			{
				for (int i=0;i<list.Count;i++)
				{
					p.MoveToData();

					try
					{
                        list[i]( p, m_Args );
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
