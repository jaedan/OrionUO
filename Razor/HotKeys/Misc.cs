using System;
using Assistant;

namespace Assistant.HotKeys
{
	public class UseHotKeys
	{
		public static void Initialize()
		{
			HotKey.Add( HKCategory.Misc, LocString.ClearDragDropQueue, new HotKeyCallback( DragDropManager.GracefulStop ) );
			
			HotKey.Add( HKCategory.Misc, LocString.LastSpell, new HotKeyCallback( LastSpell ) );
			HotKey.Add( HKCategory.Misc, LocString.LastSkill, new HotKeyCallback( LastSkill ) );
			HotKey.Add( HKCategory.Misc, LocString.LastObj, new HotKeyCallback( LastObj ) );
			HotKey.Add( HKCategory.Misc, LocString.AllNames, new HotKeyCallback( AllNames ) );
			HotKey.Add( HKCategory.Misc, LocString.Dismount, new HotKeyCallback( Dismount ) );
			
			HotKey.Add( HKCategory.Items, LocString.BandageSelf, new HotKeyCallback( BandageSelf ) );
			HotKey.Add( HKCategory.Items, LocString.BandageLT, new HotKeyCallback( BandageLastTarg ) );
            HotKey.Add( HKCategory.Items, LocString.BandageRessFriend, new HotKeyCallback(BandageRessFriend));
			HotKey.Add( HKCategory.Items, LocString.UseHand, new HotKeyCallback( UseItemInHand ) );

			HotKey.Add( HKCategory.Misc, LocString.PartyAccept, new HotKeyCallback( PartyAccept ) );
			HotKey.Add( HKCategory.Misc, LocString.PartyDecline, new HotKeyCallback( PartyDecline ) );
			
			HotKeyCallbackState call = new HotKeyCallbackState( OnUseItem );
			HotKey.Add( HKCategory.Items,					LocString.UseBandage,	call, (ushort)3617 );
			HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkHeal,	call, (ushort)3852 );
			HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkCure,	call, new object[]{(ushort)3847,(ushort)0} );
			HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkRef,		call, (ushort)3851 );
			HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkNS,		call, (ushort)3846 );
			HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkExp,		call, (ushort)3853 );
			HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkStr,		call, (ushort)3849 );
			HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkAg,		call, (ushort)3848 );
            //those are potions used on uoitalia, since you don't use them as they are here, you can remove or do whatever you want jaedan
            HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkMana,    call, (ushort)3845 );
            HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkInvis,   call, new object[]{(ushort)3847, (ushort)1185} );
            HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkSpeed,   call, new object[]{(ushort)3839, (ushort)51} );
            HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkUrti,    call, (ushort)195 );
            HotKey.Add( HKCategory.Items, HKSubCat.Potions, LocString.DrinkApple,	new HotKeyCallback( OnDrinkApple ) );
		}

		private static void PartyAccept()
		{
			if ( PacketHandlers.PartyLeader != Serial.Zero )
			{
				ClientCommunication.SendToServer( new AcceptParty( PacketHandlers.PartyLeader ) );
				PacketHandlers.PartyLeader = Serial.Zero;
			}
		}

		private static void PartyDecline()
		{
			if ( PacketHandlers.PartyLeader != Serial.Zero )
			{
				ClientCommunication.SendToServer( new DeclineParty( PacketHandlers.PartyLeader ) );
				PacketHandlers.PartyLeader = Serial.Zero;
			}
		}

		private static void Dismount()
		{
			if ( World.Player.GetItemOnLayer( Layer.Mount ) != null )
				ActionQueue.DoubleClick( true, World.Player.Serial );
			else
				World.Player.SendMessage( "You are not mounted." );
		}

		private static void AllNames()
		{
			bool textFlags = Config.GetBool( "LastTargTextFlags" );

			foreach ( Mobile m in World.MobilesInRange() )
			{
				if ( m != World.Player )
					ClientCommunication.SendToServer( new SingleClick( m ) );
				
				if ( textFlags )
					Targeting.CheckTextFlags( m );
			}

			foreach ( Item i in World.Items.Values )
			{
				if ( i.IsCorpse )
					ClientCommunication.SendToServer( new SingleClick( i ) );
			}
		}

		private static void LastSkill()
		{
			if ( World.Player != null && World.Player.LastSkill != -1 )
				ClientCommunication.SendToServer( new UseSkill( World.Player.LastSkill ) );
		}

		private static void LastObj()
		{
			if ( World.Player != null && World.Player.LastObject != Serial.Zero )
				PlayerData.DoubleClick ( World.Player.LastObject );
		}

		private static void LastSpell()
		{
			if ( World.Player != null && World.Player.LastSpell != -1 )
			{
				ushort id = (ushort)World.Player.LastSpell;
				object o = id;
				Spell.OnHotKey( ref o );
			}
		}

		public static void BandageLastTarg()
		{
			Item pack = World.Player.Backpack;
			if ( pack != null )
			{
				if ( !UseItem( pack, 3617 ) )
				{
					World.Player.SendMessage( MsgLevel.Warning, LocString.NoBandages );
				}
				else
				{
					Targeting.LastTarget( true );//force a targetself to be queued
					BandageTimer.Start();
				}
			}
		}

        public static void BandageRessFriend()
        {
            Item pack = World.Player.Backpack;
            if (pack != null)
            {
                if (!UseItem(pack, 3617))
                {
                    World.Player.SendMessage(MsgLevel.Warning, LocString.NoBandages);
                }
                else
                {
                    Targeting.ClearQueue();
                    Targeting.TargetCloseGreenDead(1);//force a targetself to be queued
                    BandageTimer.Start();
                }
            }
        }

		public static void BandageSelf()
		{
			Item pack = World.Player.Backpack;
			if ( pack != null )
			{
				if ( !UseItem( pack, 3617 ) )
				{
					World.Player.SendMessage( MsgLevel.Warning, LocString.NoBandages );
				}
				else
				{
					Targeting.ClearQueue();
					Targeting.TargetSelf( true );//force a targetself to be queued
					BandageTimer.Start();
				}
			}
		}

		private static bool DrinkApple( Item cont )
		{
			for (int i=0;i<cont.Contains.Count;i++)
			{
				Item item = cont.Contains[i];

				if ( item.ItemID == 12248 && item.Hue == 1160 )
				{
					PlayerData.DoubleClick( item );
					return true;
				}
				else if ( item.Contains != null && item.Contains.Count > 0 )
				{
					if ( DrinkApple( item ) )
						return true;
				}
			}

			return false;
		}

		private static void OnDrinkApple()
		{
			if ( World.Player.Backpack == null )
				return;

			if ( !DrinkApple( World.Player.Backpack ) )
				World.Player.SendMessage( LocString.NoItemOfType, (ItemID)12248 );
		}

		private static void OnUseItem( ref object state )
		{
			Item pack = World.Player.Backpack;
			if ( pack == null )
				return;
            ushort id;
            ushort color = ushort.MaxValue;
            if (state is object[] states)
            {
                id = (ushort)states[0];
                color = (ushort)states[1];
            }
            else
                id = (ushort)state;
            if ( id == 3852 && World.Player.Poisoned && Config.GetBool( "BlockHealPoison" ) )//&& ClientCommunication.AllowBit( FeatureBit.BlockHealPoisoned ) )
			{
				World.Player.SendMessage( MsgLevel.Force, LocString.HealPoisonBlocked );
				return;
			}
			
			if ( !UseItem( pack, id ) )
				World.Player.SendMessage( LocString.NoItemOfType, (ItemID)id );
		}

		private static void UseItemInHand()
		{
			Item item = World.Player.GetItemOnLayer( Layer.RightHand );
			if ( item == null )
				item = World.Player.GetItemOnLayer( Layer.LeftHand );

			if ( item != null )
				PlayerData.DoubleClick( item );
		}

		private static bool UseItem( Item cont, ushort find )
		{
            return UseItem(cont, find, ushort.MaxValue);
        }

        private static bool UseItem( Item cont, ushort find, ushort hue )
		{
			/*if ( !ClientCommunication.AllowBit( FeatureBit.PotionHotkeys ) )
				return false;*/

			for (int i=0;i<cont.Contains.Count;i++)
			{
				Item item = cont.Contains[i];

				if ( item.ItemID == find && (hue==ushort.MaxValue || hue==item.Hue))
				{
					PlayerData.DoubleClick( item );
					return true;
				}
				else if ( item.Contains != null && item.Contains.Count > 0 )
				{
					if ( UseItem( item, find ) )
						return true;
				}
			}

			return false;
		}
	}
}
