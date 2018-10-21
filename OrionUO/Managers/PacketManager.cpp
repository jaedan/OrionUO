

#include "stdafx.h"

CPacketManager g_PacketManager;

#define UMSG(save, size)                                                                           \
    {                                                                                              \
        save, "?", size, DIR_BOTH, 0                                                               \
    }

#define SMSG(save, name, size)                                                                     \
    {                                                                                              \
        save, name, size, DIR_SEND, 0                                                              \
    }

#define SMSGH(save, name, size, smethod)                                                           \
    {                                                                                              \
        save, name, size, DIR_SEND, &CPacketManager::Handle##smethod                               \
    }

#define RMSG(save, name, size)                                                                     \
    {                                                                                              \
        save, name, size, DIR_RECV, 0                                                              \
    }

#define BMSG(save, name, size)                                                                     \
    {                                                                                              \
        save, name, size, DIR_BOTH, 0                                                              \
    }

#define RMSGH(save, name, size, rmethod)                                                           \
    {                                                                                              \
        save, name, size, DIR_RECV, &CPacketManager::Handle##rmethod                               \
    }
#define BMSGH(save, name, size, rmethod)                                                           \
    {                                                                                              \
        save, name, size, DIR_BOTH, &CPacketManager::Handle##rmethod                               \
    }

CPacketInfo CPacketManager::m_Packets[0x100] = {
    /* 0x0 */ SMSG(ORION_SAVE_PACKET, "Create Character", 0x68),
    /* 0x1 */ SMSG(ORION_SAVE_PACKET, "Disconnect", 0x05),
    /* 0x2 */ SMSG(ORION_IGNORE_PACKET, "Walk Request", 0x07),
    /* 0x3 */ SMSGH(ORION_SAVE_PACKET, "Client Talk", PACKET_VARIABLE_SIZE, ClientTalk),
    /* 0x4 */ BMSG(ORION_SAVE_PACKET, "Request God mode (God client)", 0x02),
    /* 0x5 */ SMSG(ORION_IGNORE_PACKET, "Attack", 0x05),
    /* 0x6 */ SMSG(ORION_IGNORE_PACKET, "Double Click", 0x05),
    /* 0x7 */ SMSG(ORION_SAVE_PACKET, "Pick Up Item", 0x07),
    /* 0x8 */ SMSG(ORION_SAVE_PACKET, "Drop Item", 0x0F),
    /* 0x9 */ SMSG(ORION_IGNORE_PACKET, "Single Click", 0x05),
    /* 0xA */ BMSG(ORION_SAVE_PACKET, "Edit (God client)", 0x0b),
    /* 0xB */ RMSGH(ORION_IGNORE_PACKET, "Damage Visualization", 0x07, Damage),
    /* 0xC */ BMSG(ORION_SAVE_PACKET, "Edit tiledata (God client)", PACKET_VARIABLE_SIZE),
    /* 0xD */ BMSG(ORION_SAVE_PACKET, "Edit NPC data (God client)", 0x03),
    /* 0xE */ BMSG(ORION_SAVE_PACKET, "Edit template data (God client)", 0x01),
    /* 0xF */ UMSG(ORION_SAVE_PACKET, 0x3d),
    /* 0x10 */ BMSG(ORION_SAVE_PACKET, "Edit hue data (God client)", 0xd7),
    /* 0x11 */ RMSGH(ORION_IGNORE_PACKET, "Character Status", PACKET_VARIABLE_SIZE, CharacterStatus),
    /* 0x12 */ SMSG(ORION_IGNORE_PACKET, "Perform Action", PACKET_VARIABLE_SIZE),
    /* 0x13 */ SMSG(ORION_IGNORE_PACKET, "Client Equip Item", 0x0a),
    /* 0x14 */ BMSG(ORION_SAVE_PACKET, "Change tile Z (God client)", 0x06),
    /* 0x15 */ BMSG(ORION_SAVE_PACKET, "Follow", 0x09),
    /* 0x16 */ RMSGH(ORION_SAVE_PACKET, "Health status bar update (0x16)", PACKET_VARIABLE_SIZE, NewHealthbarUpdate),
    /* 0x17 */ RMSGH(ORION_IGNORE_PACKET, "Health status bar update (KR)", PACKET_VARIABLE_SIZE, NewHealthbarUpdate),
    /* 0x18 */ BMSG(ORION_SAVE_PACKET, "Add script (God client)", PACKET_VARIABLE_SIZE),
    /* 0x19 */ BMSG(ORION_SAVE_PACKET, "Edit NPC speech (God client)", PACKET_VARIABLE_SIZE),
    /* 0x1A */ RMSGH(ORION_SAVE_PACKET, "Update Item", PACKET_VARIABLE_SIZE, UpdateItem),
    /* 0x1B */ RMSGH(ORION_SAVE_PACKET, "Enter World", 0x25, EnterWorld),
    /* 0x1C */ RMSGH(ORION_IGNORE_PACKET, "Server Talk", PACKET_VARIABLE_SIZE, Talk),
    /* 0x1D */ RMSGH(ORION_SAVE_PACKET, "Delete Object", 0x05, DeleteObject),
    /* 0x1E */ BMSG(ORION_SAVE_PACKET, "Animate?", 0x04),
    /* 0x1F */ BMSG(ORION_SAVE_PACKET, "Explode?", 0x08),
    /* 0x20 */ RMSGH(ORION_SAVE_PACKET, "Update Player", 0x13, UpdatePlayer),
    /* 0x21 */ RMSGH(ORION_IGNORE_PACKET, "Deny Walk", 0x08, DenyWalk),
    /* 0x22 */ BMSGH(ORION_IGNORE_PACKET, "Confirm Walk", 0x03, ConfirmWalk),
    /* 0x23 */ RMSGH(ORION_SAVE_PACKET, "Drag Animation", 0x1a, DragAnimation),
    /* 0x24 */ RMSGH(ORION_SAVE_PACKET, "Open Container", 0x09, OpenContainer),
    /* 0x25 */ RMSGH(ORION_SAVE_PACKET, "Update Contained Item", 0x15, UpdateContainedItem),
    /* 0x26 */ BMSG(ORION_SAVE_PACKET, "Kick client (God client)", 0x05),
    /* 0x27 */ RMSGH(ORION_SAVE_PACKET, "Deny Move Item", 0x02, DenyMoveItem),
    /* 0x28 */ RMSGH(ORION_SAVE_PACKET, "End dragging item", 0x05, EndDraggingItem),
    /* 0x29 */ RMSGH(ORION_SAVE_PACKET, "Drop Item Accepted", 0x01, DropItemAccepted),
    /* 0x2A */ RMSG(ORION_SAVE_PACKET, "Blood mode", 0x05),
    /* 0x2B */ BMSG(ORION_SAVE_PACKET, "Toggle God mode (God client)", 0x02),
    /* 0x2C */ BMSGH(ORION_IGNORE_PACKET, "Death Screen", 0x02, DeathScreen),
    /* 0x2D */ RMSGH(ORION_SAVE_PACKET, "Mobile Attributes", 0x11, MobileAttributes),
    /* 0x2E */ RMSGH(ORION_SAVE_PACKET, "Server Equip Item", 0x0f, EquipItem),
    /* 0x2F */ RMSG(ORION_SAVE_PACKET, "Combat Notification", 0x0a),
    /* 0x30 */ RMSG(ORION_SAVE_PACKET, "Attack ok", 0x05),
    /* 0x31 */ RMSG(ORION_SAVE_PACKET, "Attack end", PACKET_VARIABLE_SIZE),
    /* 0x32 */ BMSG(ORION_SAVE_PACKET, "Toggle hack mover (God client)", 0x02),
    /* 0x33 */ RMSGH(ORION_IGNORE_PACKET, "Pause Control", 0x02, PauseControl),
    /* 0x34 */ SMSG(ORION_IGNORE_PACKET, "Status Request", 0x0a),
    /* 0x35 */ BMSG(ORION_SAVE_PACKET, "Resource type (God client)", 0x28d),
    /* 0x36 */ BMSG(ORION_SAVE_PACKET, "Resource tile data (God client)", PACKET_VARIABLE_SIZE),
    /* 0x37 */ BMSG(ORION_SAVE_PACKET, "Move object (God client)", 0x08),
    /* 0x38 */ RMSGH(ORION_SAVE_PACKET, "Pathfinding", 0x07, Pathfinding),
    /* 0x39 */ BMSG(ORION_SAVE_PACKET, "Remove group (God client)", 0x09),
    /* 0x3A */ BMSGH(ORION_IGNORE_PACKET, "Update Skills", PACKET_VARIABLE_SIZE, UpdateSkills),
    /* 0x3B */ BMSGH(ORION_IGNORE_PACKET, "Vendor Buy Reply", PACKET_VARIABLE_SIZE, BuyReply),
    /* 0x3C */ RMSGH(ORION_SAVE_PACKET, "Update Contained Items", PACKET_VARIABLE_SIZE, UpdateContainedItems),
    /* 0x3D */ BMSG(ORION_SAVE_PACKET, "Ship (God client)", 0x02),
    /* 0x3E */ BMSG(ORION_SAVE_PACKET, "Versions (God client)", 0x25),
	/* 0x3F */ BMSGH(ORION_IGNORE_PACKET, "UltimaLive Commands", PACKET_VARIABLE_SIZE, OnUltimaLiveCommand),
    /* 0x40 */ BMSG(ORION_SAVE_PACKET, "Update terrains (God client)", 0xc9),
    /* 0x41 */ BMSG(ORION_SAVE_PACKET, "Update terrains (God client)", PACKET_VARIABLE_SIZE),
    /* 0x42 */ BMSG(ORION_SAVE_PACKET, "Update art (God client)", PACKET_VARIABLE_SIZE),
    /* 0x43 */ BMSG(ORION_SAVE_PACKET, "Update animation (God client)", 0x229),
    /* 0x44 */ BMSG(ORION_SAVE_PACKET, "Update hues (God client)", 0x2c9),
    /* 0x45 */ BMSG(ORION_SAVE_PACKET, "Version OK (God client)", 0x05),
    /* 0x46 */ BMSG(ORION_SAVE_PACKET, "New art (God client)", PACKET_VARIABLE_SIZE),
    /* 0x47 */ BMSG(ORION_SAVE_PACKET, "New terrain (God client)", 0x0b),
    /* 0x48 */ BMSG(ORION_SAVE_PACKET, "New animation (God client)", 0x49),
    /* 0x49 */ BMSG(ORION_SAVE_PACKET, "New hues (God client)", 0x5d),
    /* 0x4A */ BMSG(ORION_SAVE_PACKET, "Destroy art (God client)", 0x05),
    /* 0x4B */ BMSG(ORION_SAVE_PACKET, "Check version (God client)", 0x09),
    /* 0x4C */ BMSG(ORION_SAVE_PACKET, "Script names (God client)", PACKET_VARIABLE_SIZE),
    /* 0x4D */ BMSG(ORION_SAVE_PACKET, "Edit script (God client)", PACKET_VARIABLE_SIZE),
    /* 0x4E */ RMSGH(ORION_IGNORE_PACKET, "Personal Light Level", 0x06, PersonalLightLevel),
    /* 0x4F */ RMSGH(ORION_IGNORE_PACKET, "Global Light Level", 0x02, LightLevel),
    /* 0x50 */ BMSG(ORION_IGNORE_PACKET, "Board header", PACKET_VARIABLE_SIZE),
    /* 0x51 */ BMSG(ORION_IGNORE_PACKET, "Board message", PACKET_VARIABLE_SIZE),
    /* 0x52 */ BMSG(ORION_IGNORE_PACKET, "Post board message", PACKET_VARIABLE_SIZE),
    /* 0x53 */ RMSGH(ORION_SAVE_PACKET, "Error Code", 0x02, ErrorCode),
    /* 0x54 */ RMSGH(ORION_SAVE_PACKET, "Sound Effect", 0x0c, PlaySoundEffect),
    /* 0x55 */ RMSGH(ORION_IGNORE_PACKET, "Login Complete", 0x01, LoginComplete),
    /* 0x56 */ BMSGH(ORION_IGNORE_PACKET, "Map Data", 0x0b, MapData),
    /* 0x57 */ BMSG(ORION_SAVE_PACKET, "Update regions (God client)", 0x6e),
    /* 0x58 */ BMSG(ORION_SAVE_PACKET, "New region (God client)", 0x6a),
    /* 0x59 */ BMSG(ORION_SAVE_PACKET, "New content FX (God client)", PACKET_VARIABLE_SIZE),
    /* 0x5A */ BMSG(ORION_SAVE_PACKET, "Update content FX (God client)", PACKET_VARIABLE_SIZE),
    /* 0x5B */ RMSGH(ORION_IGNORE_PACKET, "Set Time", 0x04, SetTime),
    /* 0x5C */ BMSG(ORION_SAVE_PACKET, "Restart Version", 0x02),
    /* 0x5D */ SMSG(ORION_IGNORE_PACKET, "Select Character", 0x49),
    /* 0x5E */ BMSG(ORION_SAVE_PACKET, "Server list (God client)", PACKET_VARIABLE_SIZE),
    /* 0x5F */ BMSG(ORION_SAVE_PACKET, "Add server (God client)", 0x31),
    /* 0x60 */ BMSG(ORION_SAVE_PACKET, "Remove server (God client)", 0x05),
    /* 0x61 */ BMSG(ORION_SAVE_PACKET, "Destroy static (God client)", 0x09),
    /* 0x62 */ BMSG(ORION_SAVE_PACKET, "Move static (God client)", 0x0f),
    /* 0x63 */ BMSG(ORION_SAVE_PACKET, "Area load (God client)", 0x0d),
    /* 0x64 */ BMSG(ORION_SAVE_PACKET, "Area load request (God client)", 0x01),
    /* 0x65 */ RMSGH(ORION_IGNORE_PACKET, "Set Weather", 0x04, SetWeather),
    /* 0x66 */ BMSGH(ORION_IGNORE_PACKET, "Book Page Data", PACKET_VARIABLE_SIZE, BookData),
    /* 0x67 */ BMSG(ORION_SAVE_PACKET, "Simped? (God client)", 0x15),
    /* 0x68 */ BMSG(ORION_SAVE_PACKET, "Script attach (God client)", PACKET_VARIABLE_SIZE),
    /* 0x69 */ BMSG(ORION_SAVE_PACKET, "Friends (God client)", PACKET_VARIABLE_SIZE),
    /* 0x6A */ BMSG(ORION_SAVE_PACKET, "Notify friend (God client)", 0x03),
    /* 0x6B */ BMSG(ORION_SAVE_PACKET, "Key use (God client)", 0x09),
    /* 0x6C */ BMSGH(ORION_IGNORE_PACKET, "Target Data", 0x13, Target),
    /* 0x6D */ RMSGH(ORION_SAVE_PACKET, "Play Music", 0x03, PlayMusic),
    /* 0x6E */ RMSGH(ORION_IGNORE_PACKET, "Character Animation", 0x0e, CharacterAnimation),
    /* 0x6F */ BMSGH(ORION_IGNORE_PACKET, "Secure Trading", PACKET_VARIABLE_SIZE, SecureTrading),
    /* 0x70 */ RMSGH(ORION_IGNORE_PACKET, "Graphic Effect", 0x1c, GraphicEffect),
    /* 0x71 */ BMSGH(ORION_IGNORE_PACKET, "Bulletin Board Data", PACKET_VARIABLE_SIZE, BulletinBoardData),
    /* 0x72 */ BMSGH(ORION_IGNORE_PACKET, "War Mode", 0x05, Warmode),
    /* 0x73 */ BMSGH(ORION_IGNORE_PACKET, "Ping", 0x02, Ping),
    /* 0x74 */ RMSGH(ORION_IGNORE_PACKET, "Vendor Buy List", PACKET_VARIABLE_SIZE, BuyList),
    /* 0x75 */ SMSG(ORION_SAVE_PACKET, "Rename Character", 0x23),
    /* 0x76 */ RMSG(ORION_SAVE_PACKET, "New Subserver", 0x10),
    /* 0x77 */ RMSGH(ORION_SAVE_PACKET, "Update Character", 0x11, UpdateCharacter),
    /* 0x78 */ RMSGH(ORION_SAVE_PACKET, "Update Object", PACKET_VARIABLE_SIZE, UpdateObject),
    /* 0x79 */ BMSG(ORION_SAVE_PACKET, "Resource query (God client)", 0x09),
    /* 0x7A */ BMSG(ORION_SAVE_PACKET, "Resource data (God client)", PACKET_VARIABLE_SIZE),
    /* 0x7B */ RMSG(ORION_SAVE_PACKET, "Sequence?", 0x02),
    /* 0x7C */ RMSGH(ORION_IGNORE_PACKET, "Open Menu Gump", PACKET_VARIABLE_SIZE, OpenMenu),
    /* 0x7D */ SMSG(ORION_IGNORE_PACKET, "Menu Choice", 0x0d),
    /* 0x7E */ BMSG(ORION_SAVE_PACKET, "God view query (God client)", 0x02),
    /* 0x7F */ BMSG(ORION_SAVE_PACKET, "God view data (God client)", PACKET_VARIABLE_SIZE),
    /* 0x80 */ SMSG(ORION_IGNORE_PACKET, "First Login", 0x3e),
    /* 0x81 */ RMSG(ORION_SAVE_PACKET, "Change character", PACKET_VARIABLE_SIZE),
    /* 0x82 */ RMSGH(ORION_IGNORE_PACKET, "Login Error", 0x02, LoginError),
    /* 0x83 */ SMSG(ORION_IGNORE_PACKET, "Delete Character", 0x27),
    /* 0x84 */ BMSG(ORION_SAVE_PACKET, "Change password", 0x45),
    /* 0x85 */ RMSGH(ORION_IGNORE_PACKET, "Character List Notification", 0x02, CharacterListNotification),
    /* 0x86 */ RMSGH(ORION_IGNORE_PACKET, "Resend Character List", PACKET_VARIABLE_SIZE, ResendCharacterList),
    /* 0x87 */ BMSG(ORION_SAVE_PACKET, "Send resources (God client)", PACKET_VARIABLE_SIZE),
    /* 0x88 */ RMSGH(ORION_IGNORE_PACKET, "Open Paperdoll", 0x42, OpenPaperdoll),
    /* 0x89 */ RMSGH(ORION_SAVE_PACKET, "Corpse Equipment", PACKET_VARIABLE_SIZE, CorpseEquipment),
    /* 0x8A */ BMSG(ORION_SAVE_PACKET, "Trigger edit (God client)", PACKET_VARIABLE_SIZE),
    /* 0x8B */ RMSG(ORION_SAVE_PACKET, "Display sign gump", PACKET_VARIABLE_SIZE),
    /* 0x8C */ RMSGH(ORION_IGNORE_PACKET, "Relay Server", 0x0b, RelayServer),
    /* 0x8D */ SMSG(ORION_SAVE_PACKET, "UO3D create character", PACKET_VARIABLE_SIZE),
    /* 0x8E */ BMSG(ORION_SAVE_PACKET, "Move character (God client)", PACKET_VARIABLE_SIZE),
    /* 0x8F */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0x90 */ RMSGH(ORION_IGNORE_PACKET, "Display Map", 0x13, DisplayMap),
    /* 0x91 */ SMSG(ORION_IGNORE_PACKET, "Second Login", 0x41),
    /* 0x92 */ BMSG(ORION_SAVE_PACKET, "Update multi data (God client)", PACKET_VARIABLE_SIZE),
    /* 0x93 */ RMSGH(ORION_IGNORE_PACKET, "Open Book", 0x63, OpenBook),
    /* 0x94 */ BMSG(ORION_SAVE_PACKET, "Update skills data (God client)", PACKET_VARIABLE_SIZE),
    /* 0x95 */ BMSGH(ORION_IGNORE_PACKET, "Dye Data", 0x09, DyeData),
    /* 0x96 */ BMSG(ORION_SAVE_PACKET, "Game central monitor (God client)", PACKET_VARIABLE_SIZE),
    /* 0x97 */ RMSGH(ORION_SAVE_PACKET, "Move Player", 0x02, MovePlayer),
    /* 0x98 */ BMSG(ORION_SAVE_PACKET, "All Names (3D Client Only)", PACKET_VARIABLE_SIZE),
    /* 0x99 */ BMSGH(ORION_SAVE_PACKET, "Multi Placement", 0x1E, MultiPlacement),
    /* 0x9A */ BMSGH(ORION_SAVE_PACKET, "ASCII Prompt", PACKET_VARIABLE_SIZE, ASCIIPrompt),
    /* 0x9B */ SMSG(ORION_IGNORE_PACKET, "Help Request", 0x102),
    /* 0x9C */ BMSG(ORION_SAVE_PACKET, "Assistant request (God client)", 0x135),
    /* 0x9D */ BMSG(ORION_SAVE_PACKET, "GM single (God client)", 0x33),
    /* 0x9E */ RMSGH(ORION_IGNORE_PACKET, "Vendor Sell List", PACKET_VARIABLE_SIZE, SellList),
    /* 0x9F */ SMSG(ORION_IGNORE_PACKET, "Vendor Sell Reply", PACKET_VARIABLE_SIZE),
    /* 0xA0 */ SMSG(ORION_IGNORE_PACKET, "Select Server", 0x03),
    /* 0xA1 */ RMSGH(ORION_IGNORE_PACKET, "Update Hitpoints", 0x09, UpdateHitpoints),
    /* 0xA2 */ RMSGH(ORION_IGNORE_PACKET, "Update Mana", 0x09, UpdateMana),
    /* 0xA3 */ RMSGH(ORION_IGNORE_PACKET, "Update Stamina", 0x09, UpdateStamina),
    /* 0xA4 */ SMSG(ORION_SAVE_PACKET, "System Information", 0x95),
    /* 0xA5 */ RMSGH(ORION_SAVE_PACKET, "Open URL", PACKET_VARIABLE_SIZE, OpenUrl),
    /* 0xA6 */ RMSGH(ORION_IGNORE_PACKET, "Tip Window", PACKET_VARIABLE_SIZE, TipWindow),
    /* 0xA7 */ SMSG(ORION_SAVE_PACKET, "Request Tip", 0x04),
    /* 0xA8 */ RMSGH(ORION_IGNORE_PACKET, "Server List", PACKET_VARIABLE_SIZE, ServerList),
    /* 0xA9 */ RMSGH(ORION_IGNORE_PACKET, "Character List", PACKET_VARIABLE_SIZE, CharacterList),
    /* 0xAA */ RMSGH(ORION_IGNORE_PACKET, "Attack Reply", 0x05, AttackCharacter),
    /* 0xAB */ RMSGH(ORION_SAVE_PACKET, "Text Entry Dialog", PACKET_VARIABLE_SIZE, TextEntryDialog),
    /* 0xAC */ SMSG(ORION_SAVE_PACKET, "Text Entry Dialog Reply", PACKET_VARIABLE_SIZE),
    /* 0xAD */ SMSGH(ORION_IGNORE_PACKET, "Unicode Client Talk", PACKET_VARIABLE_SIZE, OnReceiveSendPacket),
    /* 0xAE */ RMSGH(ORION_IGNORE_PACKET, "Unicode Server Talk", PACKET_VARIABLE_SIZE, UnicodeTalk),
    /* 0xAF */ RMSGH(ORION_SAVE_PACKET, "Display Death", 0x0d, DisplayDeath),
    /* 0xB0 */ RMSGH(ORION_IGNORE_PACKET, "Open Gump", PACKET_VARIABLE_SIZE, OpenGump),
    /* 0xB1 */ SMSG(ORION_IGNORE_PACKET, "Gump Choice", PACKET_VARIABLE_SIZE),
    /* 0xB2 */ BMSG(ORION_SAVE_PACKET, "Chat Data", PACKET_VARIABLE_SIZE),
    /* 0xB3 */ RMSG(ORION_SAVE_PACKET, "Chat Text ?", PACKET_VARIABLE_SIZE),
    /* 0xB4 */ BMSG(ORION_SAVE_PACKET, "Target object list (God client)", PACKET_VARIABLE_SIZE),
    /* 0xB5 */ BMSGH(ORION_SAVE_PACKET, "Open Chat Window", 0x40, OpenChat),
    /* 0xB6 */ SMSG(ORION_SAVE_PACKET, "Popup Help Request", 0x09),
    /* 0xB7 */ RMSG(ORION_SAVE_PACKET, "Popup Help Data", PACKET_VARIABLE_SIZE),
    /* 0xB8 */ BMSGH(ORION_IGNORE_PACKET, "Character Profile", PACKET_VARIABLE_SIZE, CharacterProfile),
    /* 0xB9 */ RMSGH(ORION_SAVE_PACKET, "Enable locked client features", 0x05, EnableLockedFeatures),
    /* 0xBA */ RMSGH(ORION_IGNORE_PACKET, "Display Quest Arrow", 0x0A, DisplayQuestArrow),
    /* 0xBB */ SMSG(ORION_SAVE_PACKET, "Account ID ?", 0x09),
    /* 0xBC */ RMSGH(ORION_IGNORE_PACKET, "Season", 0x03, Season),
    /* 0xBD */ BMSGH(ORION_SAVE_PACKET, "Client Version", PACKET_VARIABLE_SIZE, ClientVersion),
    /* 0xBE */ BMSGH(ORION_SAVE_PACKET, "Assist Version", PACKET_VARIABLE_SIZE, AssistVersion),
    /* 0xBF */ BMSGH(ORION_SAVE_PACKET, "Extended Command", PACKET_VARIABLE_SIZE, ExtendedCommand),
    /* 0xC0 */ RMSGH(ORION_IGNORE_PACKET, "Graphical Effect", 0x24, GraphicEffect),
    /* 0xC1 */ RMSGH(ORION_IGNORE_PACKET, "Display cliloc String", PACKET_VARIABLE_SIZE, DisplayClilocString),
    /* 0xC2 */ BMSGH(ORION_SAVE_PACKET, "Unicode prompt", PACKET_VARIABLE_SIZE, UnicodePrompt),
    /* 0xC3 */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xC4 */ UMSG(ORION_SAVE_PACKET, 0x06),
    /* 0xC5 */ BMSG(ORION_SAVE_PACKET, "Invalid map (God client)", 0xcb),
    /* 0xC6 */ RMSG(ORION_SAVE_PACKET, "Invalid map enable", 0x01),
    /* 0xC7 */ RMSGH(ORION_IGNORE_PACKET, "Graphical Effect", 0x31, GraphicEffect),
    /* 0xC8 */ BMSGH(ORION_SAVE_PACKET, "Client View Range", 0x02, ClientViewRange),
    /* 0xC9 */ BMSG(ORION_SAVE_PACKET, "Trip time", 0x06),
    /* 0xCA */ BMSG(ORION_SAVE_PACKET, "UTrip time", 0x06),
    /* 0xCB */ UMSG(ORION_SAVE_PACKET, 0x07),
    /* 0xCC */ RMSGH(ORION_IGNORE_PACKET, "Localized Text Plus String", PACKET_VARIABLE_SIZE, DisplayClilocString),
    /* 0xCD */ UMSG(ORION_SAVE_PACKET, 0x01),
    /* 0xCE */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xCF */ UMSG(ORION_SAVE_PACKET, 0x4e),
    /* 0xD0 */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xD1 */ RMSGH(ORION_IGNORE_PACKET, "Logout", 0x02, Logout),
    /* 0xD2 */ RMSGH(ORION_SAVE_PACKET, "Update Character (New)", 0x19, UpdateCharacter),
    /* 0xD3 */ RMSGH(ORION_SAVE_PACKET, "Update Object (New)", PACKET_VARIABLE_SIZE, UpdateObject),
    /* 0xD4 */ BMSGH(ORION_IGNORE_PACKET, "Open Book (New)", PACKET_VARIABLE_SIZE, OpenBookNew),
    /* 0xD5 */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xD6 */ BMSGH(ORION_IGNORE_PACKET, "Mega cliloc", PACKET_VARIABLE_SIZE, MegaCliloc),
    /* 0xD7 */ SMSG(ORION_SAVE_PACKET, "+AoS command", PACKET_VARIABLE_SIZE),
    /* 0xD8 */ RMSGH(ORION_IGNORE_PACKET, "Custom house", PACKET_VARIABLE_SIZE, CustomHouse),
    /* 0xD9 */ SMSG(ORION_SAVE_PACKET, "+Metrics", 0x10c),
    /* 0xDA */ BMSG(ORION_SAVE_PACKET, "Mahjong game command", PACKET_VARIABLE_SIZE),
    /* 0xDB */ RMSG(ORION_SAVE_PACKET, "Character transfer log", PACKET_VARIABLE_SIZE),
    /* 0xDC */ RMSGH(ORION_IGNORE_PACKET, "OPL Info Packet", 9, OPLInfo),
    /* 0xDD */ RMSGH(ORION_IGNORE_PACKET, "Compressed Gump", PACKET_VARIABLE_SIZE, OpenCompressedGump),
    /* 0xDE */ RMSG(ORION_SAVE_PACKET, "Update characters combatants", PACKET_VARIABLE_SIZE),
    /* 0xDF */ RMSGH(ORION_SAVE_PACKET, "Buff/Debuff", PACKET_VARIABLE_SIZE, BuffDebuff),
    /* 0xE0 */ SMSG(ORION_SAVE_PACKET, "Bug Report KR", PACKET_VARIABLE_SIZE),
    /* 0xE1 */ SMSG(ORION_SAVE_PACKET, "Client Type KR/SA", PACKET_VARIABLE_SIZE),
    /* 0xE2 */ RMSGH(ORION_IGNORE_PACKET, "New Character Animation", 0xa, NewCharacterAnimation),
    /* 0xE3 */ RMSG(ORION_SAVE_PACKET, "KR Encryption Responce", PACKET_VARIABLE_SIZE),
    /* 0xE4 */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xE5 */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xE6 */ UMSG(ORION_SAVE_PACKET, 0x05),
    /* 0xE7 */ UMSG(ORION_SAVE_PACKET, 0x0C),
    /* 0xE8 */ UMSG(ORION_SAVE_PACKET, 0x0D),
    /* 0xE9 */ UMSG(ORION_SAVE_PACKET, 0x4B),
    /* 0xEA */ UMSG(ORION_SAVE_PACKET, 0x03),
    /* 0xEB */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xEC */ SMSG(ORION_SAVE_PACKET, "Equip Macro", PACKET_VARIABLE_SIZE),
    /* 0xED */ SMSG(ORION_SAVE_PACKET, "Unequip item macro", PACKET_VARIABLE_SIZE),
    /* 0xEE */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xEF */ SMSG(ORION_SAVE_PACKET, "KR/2D Client Login/Seed", 0x15),
    /* 0xF0 */ BMSGH(ORION_SAVE_PACKET, "Krrios client special", PACKET_VARIABLE_SIZE, KrriosClientSpecial),
    /* 0xF1 */ SMSG(ORION_SAVE_PACKET, "Client-Server Time Synchronization Request", PACKET_VARIABLE_SIZE),
    /* 0xF2 */ RMSG(ORION_SAVE_PACKET, "Client-Server Time Synchronization Response", PACKET_VARIABLE_SIZE),
    /* 0xF3 */ RMSGH(ORION_SAVE_PACKET, "Update Item (SA)", 0x1A, UpdateItemSA),
    /* 0xF4 */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xF5 */ RMSGH(ORION_IGNORE_PACKET, "Display New Map", 0x15, DisplayMap),
    /* 0xF6 */ RMSGH(ORION_SAVE_PACKET, "Boat moving", PACKET_VARIABLE_SIZE, BoatMoving),
    /* 0xF7 */ RMSGH(ORION_SAVE_PACKET, "Packets (0xF3) list", PACKET_VARIABLE_SIZE, PacketsList),
    /* 0xF8 */ SMSG(ORION_SAVE_PACKET, "Character Creation (7.0.16.0)", 0x6a),
    /* 0xF9 */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xFA */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xFB */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xFC */ BMSGH(ORION_SAVE_PACKET, "Orion messages", PACKET_VARIABLE_SIZE, OrionMessages),
    /* 0xFD */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE),
    /* 0xFE */ RMSG(ORION_SAVE_PACKET, "Razor Handshake", 0x8),
    /* 0xFF */ UMSG(ORION_SAVE_PACKET, PACKET_VARIABLE_SIZE)
};

#define IS_MOBILE(serial) (!(serial & 0x40000000))
#define IS_PLAYER(serial) (serial == g_PlayerSerial)

CPacketManager::CPacketManager()
    : WISP_NETWORK::CPacketReader()
{
    InitializeCriticalSection(&m_CSPluginNetwork);
}

CPacketManager::~CPacketManager()
{
    DeleteCriticalSection(&m_CSPluginNetwork);
}

bool CPacketManager::AutoLoginNameExists(const string &name)
{
    WISPFUN_DEBUG("c150_f1");
    if (!AutoLoginNames.length())
        return false;

    string search = string("|") + name + "|";

    return (AutoLoginNames.find(search) != string::npos);
}

#define CV_PRINT 0

#if CV_PRINT != 0
#define CVPRINT(s) LOG(s)
#else
#define CVPRINT(s)
#endif

int CPacketManager::GetPacketSize(const UCHAR_LIST &packet, int &offsetToSize)
{
    WISPFUN_DEBUG("c150_f3");
    if (packet.size())
        return m_Packets[packet[0]].Size;

    return 0;
}

void CPacketManager::SendMegaClilocRequests()
{
    WISPFUN_DEBUG("c150_f5");
    if (g_TooltipsEnabled && !m_MegaClilocRequests.empty())
    {
        for (int i : m_MegaClilocRequests)
            CPacketMegaClilocRequestOld(i).Send();

        m_MegaClilocRequests.clear();
    }
}

void CPacketManager::AddMegaClilocRequest(int serial)
{
    WISPFUN_DEBUG("c150_f6");
    for (int item : m_MegaClilocRequests)
    {
        if (item == serial)
            return;
    }

    m_MegaClilocRequests.push_back(serial);
}

void CPacketManager::OnReadFailed()
{
    WISPFUN_DEBUG("c150_f7");
    LOG("OnReadFailed...Disconnecting...\n");
    g_Orion.DisconnectGump();

    g_AbyssPacket03First = true;
    g_PluginManager.DisconnectNotify();

    g_ConnectionManager.Disconnect();
}

void CPacketManager::OnPacket()
{
    WISPFUN_DEBUG("c150_f8");
    uint ticks = g_Ticks;
    g_TotalRecvSize += (uint)Size;

    CPacketInfo &info = m_Packets[*Start];

    if (info.save)
    {
        time_t rawtime;
        struct tm timeinfo;
        char buffer[80];

        time(&rawtime);
        localtime_s(&timeinfo, &rawtime);
        strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &timeinfo);
        LOG("--- ^(%d) r(+%d => %d) %s Server:: %s\n",
            ticks - g_LastPacketTime,
            Size,
            g_TotalRecvSize,
            buffer,
            info.Name);
        LOG_DUMP(Start, (int)Size);
    }

    g_LastPacketTime = ticks;

    if (info.Direction != DIR_RECV && info.Direction != DIR_BOTH)
        LOG("message direction invalid: 0x%02X\n", *Start);
    else if (!g_PluginManager.RecvNotify(Start, (int)Size))
    {
        if (info.Handler != 0)
        {
            Ptr = Start + 1;

            if (!info.Size)
                Ptr += 2;

            (this->*(info.Handler))();
        }
    }
}

void CPacketManager::SavePluginReceivePacket(puchar buf, int size)
{
    WISPFUN_DEBUG("c150_f9");
    UCHAR_LIST packet(size);

    memcpy(&packet[0], &buf[0], size);

    EnterCriticalSection(&m_CSPluginNetwork);
    m_PluginData.push_front(packet);
    LeaveCriticalSection(&m_CSPluginNetwork);
}

void CPacketManager::ProcessPluginPackets()
{
    WISPFUN_DEBUG("c150_f10");
    EnterCriticalSection(&m_CSPluginNetwork);
    while (!m_PluginData.empty())
    {
        UCHAR_LIST &packet = m_PluginData.back();

        PluginReceiveHandler(&packet[0], (int)packet.size());
        packet.clear();

        m_PluginData.pop_back();
    }
    LeaveCriticalSection(&m_CSPluginNetwork);
}

void CPacketManager::PluginReceiveHandler(puchar buf, int size)
{
    WISPFUN_DEBUG("c150_f11");
    SetData(buf, size);

    uint ticks = g_Ticks;
    g_TotalRecvSize += (uint)Size;

    CPacketInfo &info = m_Packets[*Start];

    LOG("--- ^(%d) r(+%d => %d) Plugin->Client:: %s\n",
        ticks - g_LastPacketTime,
        Size,
        g_TotalRecvSize,
        info.Name);
    LOG_DUMP(Start, (int)Size);

    g_LastPacketTime = ticks;

    if (info.Direction != DIR_RECV && info.Direction != DIR_BOTH)
        LOG("message direction invalid: 0x%02X\n", *buf);
    else if (info.Handler != 0)
    {
        Ptr = Start + 1;

        if (!info.Size)
            Ptr += 2;

        (this->*(info.Handler))();
    }
}

#define PACKET_HANDLER(name) void CPacketManager::Handle##name()

PACKET_HANDLER(LoginError)
{
    WISPFUN_DEBUG("c150_f12");
    if (g_GameState == GS_MAIN_CONNECT || g_GameState == GS_SERVER_CONNECT ||
        g_GameState == GS_GAME_CONNECT)
    {
        g_ConnectionScreen.SetConnectionFailed(true);
        g_ConnectionScreen.SetErrorCode(ReadUInt8());
        g_ConnectionManager.Disconnect();
    }
}

PACKET_HANDLER(ServerList)
{
    WISPFUN_DEBUG("c150_f13");
    g_ServerList.ParsePacket(*this);
}

PACKET_HANDLER(RelayServer)
{
    WISPFUN_DEBUG("c150_f14");
    memset(&g_SelectedCharName[0], 0, sizeof(g_SelectedCharName));
    in_addr addr;
    puint paddr = (puint)Ptr;
    Move(4);
    addr.S_un.S_addr = *paddr;
    char relayIP[30] = { 0 };
    memcpy(&relayIP[0], inet_ntoa(addr), 29);
    int relayPort = ReadUInt16BE();
    g_Orion.RelayServer(relayIP, relayPort, Ptr);
    g_PacketLoginComplete = false;
    g_CurrentMap = 0;
}

PACKET_HANDLER(CharacterList)
{
    WISPFUN_DEBUG("c150_f15");
    HandleResendCharacterList();

    uchar locCount = ReadUInt8();

    g_CityList.Clear();

    IFOR (i, 0, locCount)
    {
        CCityItemNew *city = new CCityItemNew();

        city->LocationIndex = ReadUInt8();

        city->Name = ReadString(32);
        city->Area = ReadString(32);

        city->X = ReadUInt32BE();
        city->Y = ReadUInt32BE();
        city->Z = ReadUInt32BE();
        city->MapIndex = ReadUInt32BE();
        city->Cliloc = ReadUInt32BE();

        Move(4);

        g_CityList.AddCity(city);
    }

    g_ClientFlag = ReadUInt32BE();

    g_CharacterList.OnePerson = (bool)(g_ClientFlag & CLF_ONE_CHARACTER_SLOT);

    g_PopupEnabled = (bool)(g_ClientFlag & CLF_CONTEXT_MENU);
    g_TooltipsEnabled = (bool)((g_ClientFlag & CLF_PALADIN_NECROMANCER_TOOLTIPS));
    g_PaperdollBooks = (bool)(g_ClientFlag & CLF_PALADIN_NECROMANCER_TOOLTIPS);

    g_CharacterListScreen.UpdateContent();
}

PACKET_HANDLER(ResendCharacterList)
{
    WISPFUN_DEBUG("c150_f16");
    g_Orion.InitScreen(GS_CHARACTER);

    int numSlots = ReadInt8();
    if (*Start == 0x86)
        LOG("/======Resend chars===\n");
    else
        LOG("/======Chars===\n");

    g_CharacterList.Clear();
    g_CharacterList.Count = numSlots;

    int autoPos = -1;
    bool autoLogin = g_MainScreen.m_AutoLogin->Checked;
    bool haveCharacter = false;

    if (numSlots == 0)
        LOG("Warning!!! No slots in character list\n");
    else
    {
        int selectedPos = -1;

        IFOR (i, 0, numSlots)
        {
            string name = ReadString(30);
            Move(30);

            if (name.length())
            {
                haveCharacter = true;

                g_CharacterList.SetName(i, name);

                if (autoLogin && autoPos == -1 && AutoLoginNameExists(name))
                    autoPos = (int)i;

                if (name == g_CharacterList.LastCharacterName)
                {
                    g_CharacterList.Selected = (int)i;

                    if (autoLogin && selectedPos == -1)
                        selectedPos = (int)i;
                }
            }

            LOG("%d: %s\n", i, name.c_str());
        }

        if (autoLogin && autoPos == -1)
            autoPos = selectedPos;
    }

    if (autoLogin && numSlots)
    {
        if (autoPos == -1)
            autoPos = 0;

        g_CharacterList.Selected = autoPos;

        if (g_CharacterList.GetName(autoPos).length())
            g_Orion.CharacterSelection(autoPos);
    }

    if (*Start == 0x86)
        g_CharacterListScreen.UpdateContent();

    if (!haveCharacter)
        g_Orion.InitScreen(GS_PROFESSION_SELECT);
}

PACKET_HANDLER(LoginComplete)
{
    WISPFUN_DEBUG("c150_f17");
    g_PacketLoginComplete = true;

    g_Orion.LoginComplete(false);
}

PACKET_HANDLER(SetTime)
{
    WISPFUN_DEBUG("c150_f18");
    g_ServerTimeHour = ReadUInt8();
    g_ServerTimeMinute = ReadUInt8();
    g_ServerTimeSecond = ReadUInt8();
}

PACKET_HANDLER(EnterWorld)
{
    WISPFUN_DEBUG("c150_f19");
    uint serial = ReadUInt32BE();
    ConfigSerial = serial;
    bool loadConfig = false;

    if (g_World != NULL)
    {
        LOG("Warning!!! Duplicate enter world message\n");

        g_Orion.SavePlayerConfig();
        ConfigSerial = g_PlayerSerial;
        g_ConfigLoaded = false;
        loadConfig = true;
    }

    g_Orion.ClearWorld();

    g_World = new CGameWorld(serial);

    Move(4);

    if (strlen(g_SelectedCharName))
        g_Player->SetName(g_SelectedCharName);

    g_Player->Graphic = ReadUInt16BE();
    g_Player->OnGraphicChange();

    g_Player->SetX(ReadUInt16BE());
    g_Player->SetY(ReadUInt16BE());
    g_Player->SetZ((char)ReadUInt16BE());

    uchar dir = ReadUInt8();
    g_Player->Dir = (Direction)(dir & 0x7);
    g_Player->Run = dir & 0x80;

    g_RemoveRangeXY.X = g_Player->GetX();
    g_RemoveRangeXY.Y = g_Player->GetY();

    g_Player->OffsetX = 0;
    g_Player->OffsetY = 0;
    g_Player->OffsetZ = 0;

    LOG("Player 0x%08lX entered the world.\n", serial);

    g_MapManager.Init();
    g_MapManager.AddRender(g_Player);

    g_Orion.LoadPlayerConfig();

    g_LastSpellIndex = 1;
    g_LastSkillIndex = 1;

    CPacketClientVersion(g_Orion.ClientVersionText).Send();

    CPacketGameWindowSize().Send();

    CPacketLanguage(g_Language.c_str()).Send();

    g_Orion.Click(g_PlayerSerial);
    CPacketStatusRequest(g_PlayerSerial).Send();

    if (g_Player->Dead())
        g_Orion.ChangeSeason(ST_DESOLATION, DEATH_MUSIC_INDEX);

    if (loadConfig)
        g_Orion.LoginComplete(true);
    else
    {
        CServer *server = g_ServerList.GetSelectedServer();

        if (server != NULL)
            g_Orion.CreateTextMessageF(3, 0x0037, "Login confirm to %s", server->Name.c_str());
    }
}

PACKET_HANDLER(UpdateHitpoints)
{
    WISPFUN_DEBUG("c150_f20");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGameCharacter *obj = g_World->FindWorldCharacter(serial);

    if (obj == NULL)
        return;

    obj->MaxHits = ReadUInt16BE();
    obj->Hits = ReadUInt16BE();

    g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
    g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM);
}

PACKET_HANDLER(UpdateMana)
{
    WISPFUN_DEBUG("c150_f21");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGameCharacter *obj = g_World->FindWorldCharacter(serial);

    if (obj == NULL)
        return;

    obj->MaxMana = ReadUInt16BE();
    obj->Mana = ReadUInt16BE();

    g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
}

PACKET_HANDLER(UpdateStamina)
{
    WISPFUN_DEBUG("c150_f22");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGameCharacter *obj = g_World->FindWorldCharacter(serial);

    if (obj == NULL)
        return;

    obj->MaxStam = ReadUInt16BE();
    obj->Stam = ReadUInt16BE();

    g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
}

PACKET_HANDLER(MobileAttributes)
{
    WISPFUN_DEBUG("c150_f23");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGameCharacter *obj = g_World->FindWorldCharacter(serial);

    if (obj == NULL)
        return;

    obj->MaxHits = ReadUInt16BE();
    obj->Hits = ReadUInt16BE();

    obj->MaxMana = ReadUInt16BE();
    obj->Mana = ReadUInt16BE();

    obj->MaxStam = ReadUInt16BE();
    obj->Stam = ReadUInt16BE();

    g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
    g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM);
}

PACKET_HANDLER(NewHealthbarUpdate)
{
    WISPFUN_DEBUG("c150_f24");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGameCharacter *obj = g_World->FindWorldCharacter(serial);

    if (obj == NULL)
        return;

    ushort count = ReadUInt16BE();

    IFOR (i, 0, count)
    {
        ushort type = ReadUInt16BE();
        uchar enable = ReadUInt8();

        uchar flags = obj->GetFlags();

        if (type == 1)
        {
            uchar poisonFlag = 0x04;

            if (enable)
            {
                obj->SA_Poisoned = true;
            }
            else
            {
                obj->SA_Poisoned = false;
            }
        }
        else if (type == 2)
        {
            if (enable)
                flags |= 0x08;
            else
                flags &= ~0x08;
        }
        else if (type == 3)
        {
        }

        obj->SetFlags(flags);
    }

    g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
}

PACKET_HANDLER(UpdatePlayer)
{
    WISPFUN_DEBUG("c150_f25");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    ushort graphic = ReadUInt16BE();
    uchar graphicIncrement = ReadUInt8();
    ushort color = ReadUInt16BE();
    uchar flags = ReadUInt8();
    ushort x = ReadUInt16BE();
    ushort y = ReadUInt16BE();
    ushort serverID = ReadUInt16BE();
    uchar direction = ReadUInt8();
    char z = ReadUInt8();

    LOG("0x%08X 0x%04X %i 0x%04X 0x%02X %i %i %i %i %i\n",
        serial,
        graphic,
        graphicIncrement,
        color,
        flags,
        x,
        y,
        serverID,
        direction,
        z);
    g_World->UpdatePlayer(
        serial, graphic, graphicIncrement, 0, x, y, z, direction, color, flags, true);
}

PACKET_HANDLER(CharacterStatus)
{
    WISPFUN_DEBUG("c150_f26");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGameCharacter *obj = g_World->FindWorldCharacter(serial);
    if (obj == NULL)
        return;

    string name = ReadString(30);
    obj->SetName(name);

    obj->Hits = ReadUInt16BE();
    obj->MaxHits = ReadUInt16BE();

    obj->CanChangeName = (ReadUInt8() != 0);

    uchar flag = ReadUInt8();

    if (flag > 0)
    {
        if (ReadUInt8() == 0)
        {
            obj->Gender = GENDER_MALE;
            ;
        }
        else
        {
            obj->Gender = GENDER_FEMALE;
        }

        if (serial == g_PlayerSerial)
        {
            short newStr = ReadInt16BE();
            short newDex = ReadInt16BE();
            short newInt = ReadInt16BE();

            if (g_ConfigManager.StatReport && g_Player->Str)
            {
                short currentStr = g_Player->Str;
                short currentDex = g_Player->Dex;
                short currentInt = g_Player->Int;

                short deltaStr = newStr - currentStr;
                short deltaDex = newDex - currentDex;
                short deltaInt = newInt - currentInt;

                char str[64] = { 0 };
                if (deltaStr)
                {
                    sprintf_s(
                        str, "Your strength has changed by %d.  It is now %d.", deltaStr, newStr);
                    g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
                }

                if (deltaDex)
                {
                    sprintf_s(
                        str, "Your dexterity has changed by %d.  It is now %d.", deltaDex, newDex);
                    g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
                }

                if (deltaInt)
                {
                    sprintf_s(
                        str,
                        "Your intelligence has changed by %d.  It is now %d.",
                        deltaInt,
                        newInt);
                    g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
                }
            }

            g_Player->Str = newStr;
            g_Player->Dex = newDex;
            g_Player->Int = newInt;

            g_Player->Stam = ReadUInt16BE();
            g_Player->MaxStam = ReadUInt16BE();
            g_Player->Mana = ReadUInt16BE();
            g_Player->MaxMana = ReadUInt16BE();
            g_Player->Gold = ReadUInt32BE();
            g_Player->Armor = ReadInt16BE();
            g_Player->Weight = ReadInt16BE();

            if (flag >= 5)
            {
                g_Player->MaxWeight = ReadInt16BE();
                ReadUInt8(); // Race
            }
            else
            {
                g_Player->MaxWeight = 7 * (g_Player->Str / 2) + 40;
            }

            if (flag >= 3)
            {
                g_Player->MurderCounts = ReadUInt16BE();
                g_Player->Followers = ReadUInt8();
                g_Player->MaxFollowers = ReadUInt8();
            }

            if (flag >= 4)
            {
                g_Player->MurderCountDecayHoursRemaining = ReadInt16BE();
                g_Player->CriminalTimerSecondsRemaining = ReadInt16BE();
                g_Player->PlayervsPlayerCooldownSecondsRemaining = ReadInt16BE();
                g_Player->BandageTimerSecondsRemaining = ReadInt16BE();
                g_Player->HungerSatisfactionMinutesRemaining = ReadInt16BE();

                g_Player->MinDamage = ReadInt16BE();
                g_Player->MaxDamage = ReadInt16BE();
                g_Player->TithingPoints = ReadUInt32BE();
            }

            if (flag >= 6)
            {
                g_Player->MaxPhysicalResistance = ReadInt16BE();
                g_Player->MaxFireResistance = ReadInt16BE();
                g_Player->MaxColdResistance = ReadInt16BE();
                g_Player->MaxPoisonResistance = ReadInt16BE();
                g_Player->MaxEnergyResistance = ReadInt16BE();
                g_Player->DefenceChance = ReadInt16BE();
                g_Player->MaxDefenceChance = ReadInt16BE();
                g_Player->AttackChance = ReadInt16BE();
                g_Player->WeaponSpeed = ReadInt16BE();
                g_Player->WeaponDamage = ReadInt16BE();
                g_Player->LowerRegCost = ReadInt16BE();
                g_Player->SpellDamage = ReadInt16BE();
                g_Player->CastRecovery = ReadInt16BE();
                g_Player->CastSpeed = ReadInt16BE();
                g_Player->LowerManaCost = ReadInt16BE();
            }

            if (!g_ConnectionScreen.GetCompleted() && g_PacketLoginComplete)
                g_Orion.LoginComplete(false);
        }
    }

    g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
    g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM);
}

PACKET_HANDLER(UpdateItem)
{
    WISPFUN_DEBUG("c150_f27");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    if (serial == g_PlayerSerial)
        return;

    ushort count = 0;
    uchar graphicIncrement = 0;
    uchar direction = 0;
    ushort color = 0;
    uchar flags = 0;

    if (serial & 0x80000000)
    {
        serial &= 0x7FFFFFFF;
        count = 1;
    }

    ushort graphic = ReadUInt16BE();

    if (g_TheAbyss && (graphic & 0x7FFF) == 0x0E5C)
        return;

    if (graphic & 0x8000)
    {
        graphic &= 0x7FFF;
        graphicIncrement = ReadUInt8();
    }

    if (count)
        count = ReadUInt16BE();
    else
        count++;

    ushort x = ReadUInt16BE();

    if (x & 0x8000)
    {
        x &= 0x7FFF;
        direction = 1;
    }

    ushort y = ReadUInt16BE();

    if (y & 0x8000)
    {
        y &= 0x7FFF;
        color = 1;
    }

    if (y & 0x4000)
    {
        y &= 0x3FFF;
        flags = 1;
    }

    if (direction)
        direction = ReadUInt8();

    char z = ReadUInt8();

    if (color)
        color = ReadUInt16BE();

    if (flags)
        flags = ReadUInt8();

    if (IS_PLAYER(serial))
    {
        LOG("Error: Received packet 0x1A with the player's serial!\n");
        g_World->UpdatePlayer(
            serial, graphic, graphicIncrement, count, x, y, z, direction, color, flags, false);
    }
    else if (IS_MOBILE(serial))
    {
        LOG("Error: Received packet 0x1A with a serial matching a mobile!\n");
        g_World->UpdateMobile(
            serial, graphic, graphicIncrement, count, x, y, z, direction, color, flags);
    }
    else if (graphic >= 0x4000)
    {
        g_World->UpdateMulti(
            serial, graphic, graphicIncrement, count, x, y, z, direction, color, flags);
    }
    else
    {
        g_World->UpdateItem(
            serial, graphic, graphicIncrement, count, x, y, z, direction, color, flags);
    }
}

PACKET_HANDLER(UpdateItemSA)
{
    WISPFUN_DEBUG("c150_f28");
    if (g_World == NULL)
        return;

    Move(2);
    uint8_t dataType = ReadUInt8();
    uint serial = ReadUInt32BE();
    ushort graphic = ReadUInt16BE();
    uchar graphicIncrement = ReadUInt8();
    ushort count = ReadUInt16BE();
    ushort unknown = ReadUInt16BE();
    ushort x = ReadUInt16BE();
    ushort y = ReadUInt16BE();
    uchar z = ReadUInt8();
    uchar direction = ReadUInt8();
    ushort color = ReadUInt16BE();
    uchar flags = ReadUInt8();
    ushort unknown2 = ReadUInt16BE();

    if (IS_PLAYER(serial))
    {
        if (*Start != 0xF7)
        {
            LOG("Error: Received packet 0xF3 with the player's serial!\n");
        }
        g_World->UpdatePlayer(
            serial, graphic, graphicIncrement, count, x, y, z, direction, color, flags, false);
    }
    else if (IS_MOBILE(serial))
    {
        LOG("Error: Received packet 0xF3 with a serial matching a mobile!\n");
        g_World->UpdateMobile(
            serial, graphic, graphicIncrement, count, x, y, z, direction, color, flags);
    }
    else if (dataType == 2)
    {
        g_World->UpdateMulti(
            serial, graphic, graphicIncrement, count, x, y, z, direction, color, flags);
    }
    else
    {
        g_World->UpdateItem(
            serial, graphic, graphicIncrement, count, x, y, z, direction, color, flags);
    }
}

PACKET_HANDLER(UpdateObject)
{
    WISPFUN_DEBUG("c150_f29");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    ushort graphic = ReadUInt16BE();
    ushort x = ReadUInt16BE();
    ushort y = ReadUInt16BE();
    uchar z = ReadUInt8();
    uchar direction = ReadUInt8();
    ushort color = ReadUInt16BE();
    uchar flags = ReadUInt8();
    uchar notoriety = ReadUInt8();
    bool oldDead = false;

    if (IS_PLAYER(serial))
    {
        g_World->UpdatePlayer(serial, graphic, 0, 0, x, y, z, direction, color, flags, false);
    }
    else if (IS_MOBILE(serial))
    {
        g_World->UpdateMobile(serial, graphic, 0, 0, x, y, z, direction, color, flags);
    }
    else if (graphic >= 0x4000)
    {
        g_World->UpdateMulti(serial, graphic, 0, 0, x, y, z, direction, color, flags);
    }
    else
    {
        g_World->UpdateItem(serial, graphic, 0, 0, x, y, z, direction, color, flags);
    }

    CGameObject *obj = g_World->FindWorldObject(serial);

    if (obj == NULL)
        return;

    obj->ClearNotOpenedItems();

    if (obj->NPC)
    {
        ((CGameCharacter *)obj)->Notoriety = notoriety;
        g_GumpManager.UpdateContent(serial, 0, GT_PAPERDOLL);
    }

    if (*Start != 0x78)
        Move(6);

    uint itemSerial = ReadUInt32BE();

    while (itemSerial != 0 && !IsEOF())
    {
        ushort itemGraphic = ReadUInt16BE();
        uchar layer = ReadUInt8();
        ushort itemColor = 0;

        if (itemGraphic & 0x8000)
        {
            itemGraphic &= 0x7FFF;
            itemColor = ReadUInt16BE();
        }

        CGameItem *item = g_World->GetWorldItem(itemSerial);

        item->MapIndex = g_CurrentMap;

        item->Graphic = itemGraphic;
        item->Color = g_ColorManager.FixColor(itemColor);

        g_World->PutEquipment(item, obj, layer);
        item->OnGraphicChange();

        LOG("\t0x%08X:%04X [%d] %04X\n", item->Serial, item->Graphic, layer, item->Color);

        g_World->MoveToTop(item);

        itemSerial = ReadUInt32BE();
    }

    if (obj->IsPlayer())
    {
        if (oldDead != g_Player->Dead())
        {
            if (g_Player->Dead())
                g_Orion.ChangeSeason(ST_DESOLATION, DEATH_MUSIC_INDEX);
            else
                g_Orion.ChangeSeason(g_OldSeason, g_OldSeasonMusic);
        }

        g_GumpManager.UpdateContent(serial, 0, GT_PAPERDOLL);

        g_Player->UpdateAbilities();
    }
}

PACKET_HANDLER(EquipItem)
{
    WISPFUN_DEBUG("c150_f30");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGameItem *obj = g_World->GetWorldItem(serial);
    obj->MapIndex = g_CurrentMap;

    if (obj->Graphic && obj->Layer != OL_BACKPACK)
        obj->Clear();

    obj->Graphic = ReadUInt16BE();
    Move(1);
    int layer = ReadUInt8();
    uint cserial = ReadUInt32BE();
    obj->Color = g_ColorManager.FixColor(ReadUInt16BE());

    if (obj->Container != 0xFFFFFFFF)
    {
        g_GumpManager.UpdateContent(obj->Container, 0, GT_CONTAINER);
        g_GumpManager.UpdateContent(obj->Container, 0, GT_PAPERDOLL);
    }

    g_World->PutEquipment(obj, cserial, layer);
    obj->OnGraphicChange();

    if (g_NewTargetSystem.Serial == serial)
        g_NewTargetSystem.Serial = 0;

    if (layer >= OL_BUY_RESTOCK && layer <= OL_SELL)
        obj->Clear();
    else if (layer < OL_MOUNT)
        g_GumpManager.UpdateContent(cserial, 0, GT_PAPERDOLL);

    if (cserial == g_PlayerSerial && (layer == OL_1_HAND || layer == OL_2_HAND))
        g_Player->UpdateAbilities();
}

PACKET_HANDLER(UpdateContainedItem)
{
    WISPFUN_DEBUG("c150_f31");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    ushort graphic = ReadUInt16BE();
    uchar graphicIncrement = ReadUInt8();
    ushort count = ReadUInt16BE();
    ushort x = ReadUInt16BE();
    ushort y = ReadUInt16BE();

    Move(1);

    uint containerSerial = ReadUInt32BE();
    ushort color = ReadUInt16BE();

    g_World->UpdateContainedItem(
        serial, graphic, graphicIncrement, count, x, y, containerSerial, color);
}

PACKET_HANDLER(UpdateContainedItems)
{
    WISPFUN_DEBUG("c150_f32");
    if (g_World == NULL)
        return;

    ushort itemsCount = ReadUInt16BE();

    IFOR (i, 0, itemsCount)
    {
        uint serial = ReadUInt32BE();
        ushort graphic = ReadUInt16BE();
        uchar graphicIncrement = ReadUInt8();
        ushort count = ReadUInt16BE();
        ushort x = ReadUInt16BE();
        ushort y = ReadUInt16BE();

        Move(1);

        uint containerSerial = ReadUInt32BE();
        ushort color = ReadUInt16BE();

        if (!i)
        {
            CGameObject *container = g_World->FindWorldObject(containerSerial);

            if (container != NULL)
            {
                LOG("Making %08X empty...\n", containerSerial);

                if (container->IsCorpse())
                    container->ClearUnequipped();
                else
                    container->Clear();
            }
        }

        g_World->UpdateContainedItem(
            serial, graphic, graphicIncrement, count, x, y, containerSerial, color);
    }
}

PACKET_HANDLER(DenyMoveItem)
{
    WISPFUN_DEBUG("c150_f33");
    if (g_World == NULL)
        return;

    if (g_ObjectInHand.Enabled ||
        (g_ObjectInHand.Dropped && g_World->FindWorldItem(g_ObjectInHand.Serial) == NULL))
    {
        if (g_World->ObjectToRemove == g_ObjectInHand.Serial)
            g_World->ObjectToRemove = 0;

        if (!g_ObjectInHand.UpdatedInWorld)
        {
            if (!g_ObjectInHand.Layer && g_ObjectInHand.Container &&
                g_ObjectInHand.Container != 0xFFFFFFFF)
            {
                g_World->UpdateContainedItem(
                    g_ObjectInHand.Serial,
                    g_ObjectInHand.Graphic,
                    0,
                    g_ObjectInHand.TotalCount,
                    g_ObjectInHand.X,
                    g_ObjectInHand.Y,
                    g_ObjectInHand.Container,
                    g_ObjectInHand.Color);

                g_GumpManager.UpdateContent(g_ObjectInHand.Container, 0, GT_CONTAINER);
            }
            else
            {
                CGameItem *obj = g_World->GetWorldItem(g_ObjectInHand.Serial);

                if (obj != NULL)
                {
                    obj->Graphic = g_ObjectInHand.Graphic;
                    obj->Color = g_ObjectInHand.Color;
                    obj->Count = g_ObjectInHand.TotalCount;
                    obj->SetFlags(g_ObjectInHand.Flags);
                    obj->SetX(g_ObjectInHand.X);
                    obj->SetY(g_ObjectInHand.Y);
                    obj->SetZ(g_ObjectInHand.Z);
                    obj->OnGraphicChange();

                    CGameObject *container = g_World->FindWorldObject(g_ObjectInHand.Container);

                    if (container != NULL)
                    {
                        if (container->NPC)
                        {
                            g_World->PutEquipment(obj, container, g_ObjectInHand.Layer);

                            g_GumpManager.UpdateContent(obj->Container, 0, GT_PAPERDOLL);
                        }
                        else
                        {
                            g_World->RemoveObject(obj);
                            obj = NULL;
                        }
                    }
                    else
                        g_World->RemoveFromContainer(obj);

                    if (obj != NULL)
                        g_World->MoveToTop(obj);
                }
            }
        }

        g_GumpManager.CloseGump(g_ObjectInHand.Serial, 0, GT_DRAG);

        g_ObjectInHand.Clear();
    }

    uchar code = ReadUInt8();

    if (code < 5)
    {
        const string errorMessages[] = {
            "You can not pick that up.",
            "That is too far away.",
            "That is out of sight.",
            "That item does not belong to you.  You'll have to steal it.",
            "You are already holding an item."
        };

        g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, errorMessages[code]);
    }
}

PACKET_HANDLER(EndDraggingItem)
{
    WISPFUN_DEBUG("c150_f33_1");
    if (g_World == NULL)
        return;

    g_ObjectInHand.Enabled = false;
    g_ObjectInHand.Dropped = false;
}

PACKET_HANDLER(DropItemAccepted)
{
    WISPFUN_DEBUG("c150_f33_1");
    if (g_World == NULL)
        return;

    g_ObjectInHand.Enabled = false;
    g_ObjectInHand.Dropped = false;
}

PACKET_HANDLER(DeleteObject)
{
    WISPFUN_DEBUG("c150_f34");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    if (serial == g_PlayerSerial)
        return;

    CGameObject *obj = g_World->FindWorldObject(serial);

    if (obj != NULL)
    {
        bool updateAbilities = false;
        uint cont = obj->Container & 0x7FFFFFFF;

        if (obj->Container != 0xFFFFFFFF)
        {
            CGameObject *top = obj->GetTopObject();

            if (top != NULL)
            {
                if (top->IsPlayer())
                {
                    CGameItem *item = (CGameItem *)obj;
                    updateAbilities = (item->Layer == OL_1_HAND || item->Layer == OL_2_HAND);
                }

                CGameObject *tradeBox = top->FindSecureTradeBox();

                if (tradeBox != NULL)
                    g_GumpManager.UpdateContent(0, tradeBox->Serial, GT_TRADE);
            }

            if (cont == g_PlayerSerial && ((CGameItem *)obj)->Layer == OL_NONE)
                g_ObjectInHand.Enabled = false;

            if (!obj->NPC && ((CGameItem *)obj)->Layer != OL_NONE)
                g_GumpManager.UpdateContent(cont, 0, GT_PAPERDOLL);

            CGump *gump = g_GumpManager.UpdateContent(cont, 0, GT_CONTAINER);

            if (obj->Graphic == 0x0EB0)
            {
                g_GumpManager.CloseGump(serial, cont, GT_BULLETIN_BOARD_ITEM);

                CGumpBulletinBoard *bbGump =
                    (CGumpBulletinBoard *)g_GumpManager.UpdateGump(cont, 0, GT_BULLETIN_BOARD);

                if (bbGump != NULL && bbGump->m_HTMLGump != NULL)
                {
                    QFOR(go, bbGump->m_HTMLGump->m_Items, CBaseGUI *)
                    {
                        if (go->Serial == serial)
                        {
                            bbGump->m_HTMLGump->Delete(go);

                            int posY = 0;

                            QFOR(go1, bbGump->m_HTMLGump->m_Items, CBaseGUI *)
                            {
                                if (go1->Type == GOT_BB_OBJECT)
                                {
                                    go1->SetY(posY);
                                    posY += 18;
                                }
                            }

                            bbGump->m_HTMLGump->CalculateDataSize();

                            break;
                        }
                    }
                }
            }
        }

        if (!g_CorpseManager.InList(0, serial))
        {
            if (obj->NPC)
            {
                if (g_Party.Contains(obj->Serial))
                {
                    g_GumpManager.UpdateContent(obj->Serial, 0, GT_STATUSBAR);

                    obj->RemoveRender();
                }
                else
                    g_World->RemoveObject(obj);
            }
            else
            {
                g_World->RemoveObject(obj);

                if (updateAbilities)
                    g_Player->UpdateAbilities();
            }
        }
    }
}

PACKET_HANDLER(UpdateCharacter)
{
    WISPFUN_DEBUG("c150_f35");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    CGameCharacter *obj = g_World->FindWorldCharacter(serial);

    if (obj == NULL)
        return;

    ushort graphic = ReadUInt16BE();
    ushort x = ReadUInt16BE();
    ushort y = ReadUInt16BE();
    char z = ReadUInt8();
    uchar direction = ReadUInt8();
    ushort color = ReadUInt16BE();
    uchar flags = ReadUInt8();
    uchar notoriety = ReadUInt8();

    obj->Notoriety = notoriety;

    if (obj->IsPlayer())
    {
        obj->SetFlags(flags);
    }
    else
    {
        g_World->UpdateMobile(serial, graphic, 0, 0, x, y, z, direction, color, flags);
    }

    g_World->MoveToTop(obj);
}

PACKET_HANDLER(Warmode)
{
    WISPFUN_DEBUG("c150_f36");

    WarModeState newState = (WarModeState)ReadUInt8();

    if (g_Player->m_WarModeRequests.empty())
    {
        /* Unsolicited. */
        g_Player->SetWarMode(newState);
        return;
    }

    WarModeState expectedState = g_Player->m_WarModeRequests.front();
    g_Player->m_WarModeRequests.pop_front();

    if (newState != expectedState)
    {
        g_Player->SetWarMode(newState);
        g_Player->m_WarModeRequests.clear();
    }
}

PACKET_HANDLER(PauseControl)
{
    WISPFUN_DEBUG("c150_f37");
}

PACKET_HANDLER(OpenPaperdoll)
{
    WISPFUN_DEBUG("c150_f38");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGameCharacter *obj = g_World->FindWorldCharacter(serial);

    string text = ReadString(60);

    uchar flags = ReadUInt8();

    if (obj != NULL)
        obj->Title = text;

    CGumpPaperdoll *gump = (CGumpPaperdoll *)g_GumpManager.UpdateGump(serial, 0, GT_PAPERDOLL);

    if (gump == NULL)
    {
        gump = new CGumpPaperdoll(serial, 0, 0, false);
        g_GumpManager.AddGump(gump);
    }

    gump->CanLift = (flags & 0x02);
    gump->UpdateDescription(text);
}

PACKET_HANDLER(ClientVersion)
{
    WISPFUN_DEBUG("c150_f39");
    CPacketClientVersion(g_Orion.ClientVersionText).Send();
}

PACKET_HANDLER(Ping)
{
    WISPFUN_DEBUG("c150_f40");
    g_Ping = 0;
}

PACKET_HANDLER(SetWeather)
{
    WISPFUN_DEBUG("c150_f41");
    g_Weather.Reset();

    uchar type = ReadUInt8();
    g_Weather.Type = type;
    g_Weather.Count = ReadUInt8();

    bool showMessage = (g_Weather.Count > 0);

    if (g_Weather.Count > 70)
        g_Weather.Count = 70;

    g_Weather.Temperature = ReadUInt8();
    g_Weather.Timer = g_Ticks + WEATHER_TIMER;
    g_Weather.Generate();

    switch (type)
    {
        case 0:
        {
            if (showMessage)
                g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "It begins to rain.");

            break;
        }
        case 1:
        {
            if (showMessage)
                g_Orion.CreateTextMessage(
                    TT_SYSTEM, 0xFFFFFFFF, 3, 0, "A fierce storm approaches.");

            break;
        }
        case 2:
        {
            if (showMessage)
                g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "It begins to snow.");

            break;
        }
        case 3:
        {
            if (showMessage)
                g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "A storm is brewing.");

            break;
        }
        case 0xFE:
        case 0xFF:
        {
            g_Weather.Timer = 0;
            break;
        }
        default:
            break;
    }
}

PACKET_HANDLER(PersonalLightLevel)
{
    WISPFUN_DEBUG("c150_f42");
    uint serial = ReadUInt32BE();

    if (serial == g_PlayerSerial)
    {
        uchar level = ReadUInt8();

        if (level > 0x1F)
            level = 0x1F;

        g_PersonalLightLevel = level;

        g_GameScreen.CalculateLightLevel();
    }
}

PACKET_HANDLER(LightLevel)
{
    WISPFUN_DEBUG("c150_f43");
    uchar level = ReadUInt8();

    if (level > 0x1F)
        level = 0x1F;

    g_LightLevel = level;

    g_GameScreen.CalculateLightLevel();
}

PACKET_HANDLER(EnableLockedFeatures)
{
    WISPFUN_DEBUG("c150_f44");
    g_LockedClientFeatures = ReadUInt32BE();

    g_ChatEnabled = (bool)(g_LockedClientFeatures & LFF_T2A);

    g_AnimationManager.UpdateAnimationAddressTable();
}

PACKET_HANDLER(OpenContainer)
{
    WISPFUN_DEBUG("c150_f45");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    ushort gumpid = ReadUInt16BE();

    CGump *gump = NULL;

    if (gumpid == 0xFFFF)
    {
        int gameWindowCenterX =
            (g_ConfigManager.GameWindowX - 4) + g_ConfigManager.GameWindowWidth / 2;
        int gameWindowCenterY =
            (g_ConfigManager.GameWindowY - 4) + g_ConfigManager.GameWindowHeight / 2;

        int x = gameWindowCenterX - 200;
        int y = gameWindowCenterY - 100;

        if (x < 0)
            x = 0;

        if (y < 0)
            y = 0;

        gump = new CGumpSpellbook(serial, x, y);
        g_Orion.PlaySoundEffect(0x0055);
    }
    else if (gumpid == 0x0030)
    {
        g_GumpManager.CloseGump(serial, 0, GT_SHOP);

        CGameCharacter *vendor = g_World->FindWorldCharacter(serial);

        if (vendor != NULL)
        {
            CGumpShop *buyGump = new CGumpShop(serial, true, 150, 5);
            gump = buyGump;
            buyGump->Visible = true;

            IFOR (layer, OL_BUY_RESTOCK, OL_BUY + 1)
            {
                CGameItem *item = vendor->FindLayer((int)layer);

                if (item == NULL)
                {
                    LOG("Buy layer %i not found!\n", layer);
                    continue;
                }

                item = (CGameItem *)item->m_Items;

                if (item == NULL)
                {
                    LOG("Buy items not found!\n");
                    continue;
                }

                bool reverse = (item->GetX() > 1);

                if (reverse)
                {
                    while (item != NULL && item->m_Next != NULL)
                        item = (CGameItem *)item->m_Next;
                }

                CGUIHTMLGump *htmlGump = buyGump->m_ItemList[0];

                int currentY = 0;

                QFOR(shopItem, htmlGump->m_Items, CBaseGUI *)
                {
                    if (shopItem->Type == GOT_SHOPITEM)
                        currentY += shopItem->GetSize().Height;
                }

                while (item != NULL)
                {
                    CGUIShopItem *shopItem = (CGUIShopItem *)htmlGump->Add(new CGUIShopItem(
                        item->Serial,
                        item->Graphic,
                        item->Color,
                        item->Count,
                        item->Price,
                        item->GetName(),
                        0,
                        currentY));
                    shopItem->NameFromCliloc = item->NameFromCliloc;

                    if (!currentY)
                    {
                        shopItem->Selected = true;
                        shopItem->CreateNameText();
                        shopItem->UpdateOffsets();
                    }

                    currentY += shopItem->GetSize().Height;

                    if (reverse)
                        item = (CGameItem *)item->m_Prev;
                    else
                        item = (CGameItem *)item->m_Next;
                }

                htmlGump->CalculateDataSize();
            }
        }
        else
            LOG("Buy vendor not found!\n");
    }
    else
    {
        ushort graphic = 0xFFFF;

        IFOR (i, 0, (int)g_ContainerOffset.size())
        {
            if (gumpid == g_ContainerOffset[i].Gump)
            {
                graphic = (ushort)i;
                break;
            }
        }

        if (graphic == 0xFFFF)
            return;

        g_ContainerRect.Calculate(gumpid);

        gump = new CGumpContainer(serial, gumpid, g_ContainerRect.X, g_ContainerRect.Y);
        gump->Graphic = graphic;
        g_Orion.ExecuteGump(gumpid);
    }

    if (gump == NULL)
        return;

    gump->ID = gumpid;

    if (gumpid != 0x0030)
    {
        if (g_ContainerStack.size())
        {
            for (deque<CContainerStackItem>::iterator cont = g_ContainerStack.begin();
                 cont != g_ContainerStack.end();
                 ++cont)
            {
                if (cont->Serial == serial)
                {
                    gump->SetX(cont->X);
                    gump->SetY(cont->Y);
                    gump->Minimized = cont->Minimized;
                    gump->MinimizedX = cont->MinimizedX;
                    gump->MinimizedY = cont->MinimizedY;
                    gump->LockMoving = cont->LockMoving;

                    if (gumpid != 0xFFFF)
                    {
                        if (cont->Minimized)
                            gump->Page = 1;
                        else
                            gump->Page = 2;
                    }

                    g_ContainerStack.erase(cont);

                    break;
                }
            }

            if (g_CheckContainerStackTimer < g_Ticks)
                g_ContainerStack.clear();
        }

        CGameItem *obj = g_World->FindWorldItem(serial);

        if (obj != NULL)
        {
            obj->Opened = true;
            if (!obj->IsCorpse())// && gumpid != 0xFFFF) //this is only for sphere
                g_World->ClearContainer(obj);
        }
    }

    g_GumpManager.AddGump(gump);
}

PACKET_HANDLER(UpdateSkills)
{
    WISPFUN_DEBUG("c150_f46");
    if (g_World == NULL)
        return;

    uchar type = ReadUInt8();
    bool haveCap = ((type && type <= 0x03) || type == 0xDF);
    bool isSingleUpdate = (type == 0xFF || type == 0xDF);
    LOG("Skill update type %i (cap=%d)\n", type, haveCap);

    if (type == 0xFE)
    {
        int count = ReadInt16BE();
        g_SkillsManager.Clear();

        IFOR (i, 0, count)
        {
            bool haveButton = (ReadUInt8() != 0);
            int nameLength = ReadUInt8();
            g_SkillsManager.Add(CSkill(haveButton, ReadString(nameLength)));
        }

        g_SkillsManager.Sort();

        return;
    }

    CGumpSkills *gump = (CGumpSkills *)g_GumpManager.UpdateGump(0, 0, GT_SKILLS);

    if (!isSingleUpdate && (type == 1 || type == 3 || g_SkillsManager.SkillsRequested))
    {
        g_SkillsManager.SkillsRequested = false;

        if (gump == NULL)
        {
            gump = new CGumpSkills(0, 0, false, 250);
            g_GumpManager.AddGump(gump);
        }

        gump->Visible = true;
    }

    while (!IsEOF())
    {
        ushort id = ReadUInt16BE();

        if (IsEOF())
            break;

        if (!id && !type)
            break;
        else if (!type || type == 0x02)
            id--;

        ushort baseVal = ReadUInt16BE();
        ushort realVal = ReadUInt16BE();
        uchar lock = ReadUInt8();
        ushort cap = 1000;

        if (haveCap)
            cap = ReadUInt16BE();

        CSkill *skill = g_SkillsManager.Get(id);

        if (id < g_SkillsManager.Count && skill != NULL)
        {
            if (isSingleUpdate)
            {
                float change = (float)(baseVal / 10.0f) - skill->BaseValue;

                if (change)
                {
                    char str[128] = { 0 };
                    sprintf_s(
                        str,
                        "Your skill in %s has %s by %.1f%%.  It is now %.1f%%.",
                        skill->Name.c_str(),
                        ((change < 0) ? "decreased" : "increased"),
                        std::abs(change),
                        skill->BaseValue + change);

                    g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x58, str);
                }
            }

            skill->BaseValue = (float)(baseVal / 10.0f);
            skill->Value = (float)(realVal / 10.0f);
            skill->Cap = (float)(cap / 10.0f);
            skill->Status = lock;

            if (gump != NULL)
                gump->UpdateSkillValue(id);
        }

        if (isSingleUpdate)
            break;
    }

    g_SkillsManager.UpdateSkillsSum();

    if (gump != NULL)
        gump->UpdateSkillsSum();

    LOG("Skill(s) updated!\n");
}

PACKET_HANDLER(ExtendedCommand)
{
    WISPFUN_DEBUG("c150_f47");
    ushort cmd = ReadUInt16BE();

    switch (cmd)
    {
        case 0:
            break;
        case 1:
        {
            break;
        }
        case 2:
        {
            break;
        }
        case 4:
        {
            uint id = ReadUInt32BE();
            uint button = ReadUInt32BE();

            for (CGump *gump = (CGump *)g_GumpManager.m_Items; gump != NULL;)
            {
                CGump *next = (CGump *)gump->m_Next;

                if (gump->GumpType == GT_GENERIC && gump->ID == id)
                    ((CGumpGeneric *)gump)->SendGumpResponse(button);

                gump = next;
            }

            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            g_Party.ParsePacketData(*this);

            break;
        }
        case 8:
        {
            g_Orion.ChangeMap(ReadUInt8());

            break;
        }
        case 0xC:
        {
            uint serial = ReadUInt32BE();
            g_GumpManager.CloseGump(serial, 0, GT_STATUSBAR);

            break;
        }
        case 0x10:
        {
            uint serial = ReadUInt32BE();
            CGameItem *item = g_World->FindWorldItem(serial);
            if (item == NULL)
                return;

            item->JournalPrefix = "";
            wstring str = L"";
            int clilocNum = ReadInt32BE();
            if (clilocNum)
            {
                str = g_ClilocManager.Cliloc(g_Language)->GetW(clilocNum, true);
                if (str.length() > 0)
                {
                    item->SetName(ToString(str));
                }

                g_Orion.CreateUnicodeTextMessage(
                    TT_OBJECT, serial, g_ConfigManager.ChatFont, 0x3B2, str);
            }

            str = L"";
            ushort crafterNameLen = 0;
            uint next = ReadUInt32BE();
            if (next == 0xFFFFFFFD)
            {
                crafterNameLen = ReadUInt16BE();
                if (crafterNameLen)
                    str = L"Crafted by " + DecodeUTF8(ReadString(crafterNameLen));
            }

            if (crafterNameLen != 0)
                next = ReadUInt32BE();
            if (next == 0xFFFFFFFC)
                str += L"[Unidentified";

            puchar end = Start + Size - 4;
            uchar count = 0;
            while (Ptr < end)
            {
                if (count != 0 || next == 0xFFFFFFFD || next == 0xFFFFFFFC)
                    next = ReadInt32BE();
                short charges = ReadInt16BE();
                wstring attrsString = g_ClilocManager.Cliloc(g_Language)->GetW(next, true);
                if (charges == -1)
                {
                    if (count > 0)
                    {
                        str += L"/";
                        str += attrsString;
                    }
                    else
                    {
                        str += L" [";
                        str += attrsString;
                    }
                }
                else
                {
                    str += L"\n[";
                    str += attrsString;
                    str += L" : ";
                    str += std::to_wstring(charges);
                    str += L"]";
                    count += 20;
                }
                count++;
            }
            if ((count < 20 && count > 0) || (next == 0xFFFFFFFC && count == 0))
                str += L"]";

            if (str.length())
                g_Orion.CreateUnicodeTextMessage(
                    TT_OBJECT, serial, g_ConfigManager.ChatFont, 0x3B2, str);
            CPacketMegaClilocRequestOld(serial).Send();
            break;
        }
        case 0x14:
        {
            CGumpPopupMenu::Parse(*this);

            break;
        }
        case 0x16:
        {
            uint id = ReadUInt32BE();
            uint serial = ReadUInt32BE();

            switch (id)
            {
                case 1:
                {
                    g_GumpManager.CloseGump(serial, 0, GT_PAPERDOLL);
                    break;
                }
                case 2:
                {
                    g_GumpManager.CloseGump(serial, 0, GT_STATUSBAR);
                    break;
                }
                case 8:
                {
                    g_GumpManager.CloseGump(serial, 0, GT_PROFILE);
                    break;
                }
                case 0xC:
                {
                    g_GumpManager.CloseGump(serial, 0, GT_CONTAINER);
                    break;
                }
                default:
                    break;
            }

            break;
        }
        case 0x18:
        {
            g_MapManager.ApplyPatches(*this);

            break;
        }
        case 0x19:
        {
            uchar version = ReadUInt8();
            uint serial = ReadUInt32BE();

            switch (version)
            {
                case 0:
                {
                    CGameCharacter *bonded = g_World->FindWorldCharacter(serial);
                    if (bonded == NULL)
                        break;

                    bool dead = ReadUInt8();
                    bonded->SetDead(dead);
                    break;
                }
                case 2:
                {
                    if (serial == g_PlayerSerial)
                    {
                        uchar updateGump = ReadUInt8();
                        uchar state = ReadUInt8();
                        g_DrawStatLockers = true;

                        g_Player->LockStr = (state >> 4) & 3;
                        g_Player->LockDex = (state >> 2) & 3;
                        g_Player->LockInt = state & 3;

                        CGump *statusbar = g_GumpManager.GetGump(g_PlayerSerial, 0, GT_STATUSBAR);

                        if (statusbar != NULL && !statusbar->Minimized)
                            statusbar->WantUpdateContent = true;
                    }
                    break;
                }
                case 5:
                {
                    CGameCharacter *character = g_World->FindWorldCharacter(serial);
                    if (character == NULL)
                        break;

                    if (Size == 19)
                    {
                        bool dead = ReadUInt8();
                        character->SetDead(dead);
                    }
                    break;
                }
            }
            break;
        }
        case 0x1B:
        {
            Move(2);
            uint serial = ReadUInt32BE();

            CGameItem *spellbook = g_World->FindWorldItem(serial);

            if (spellbook == NULL)
            {
                LOG("Where is a spellbook?!?\n");
                return;
            }

            g_World->ClearContainer(spellbook);

            ushort graphic = ReadUInt16BE();
            uint16_t bookType = ReadUInt16BE();

            IFOR (j, 0, 2)
            {
                uint spells = 0;

                IFOR (i, 0, 4)
                    spells |= (ReadUInt8() << (i * 8));

                IFOR (i, 0, 32)
                {
                    if (spells & (1 << i))
                    {
                        CGameItem *spellItem = new CGameItem();
                        spellItem->Graphic = 0x1F2E;
                        spellItem->Count = ((int)j * 32) + (int)i + 1;

                        spellbook->AddItem(spellItem);
                    }
                }
            }

            break;
        }
        case 0x1D:
        {
            break;
        }
        case 0x20:
        {
            break;
        }
        case 0x21:
        {
            IFOR (i, 0, 2)
            {
                g_Ability[i] &= 0x7F;
                g_GumpManager.UpdateContent((int)i, 0, GT_ABILITY);
            }

            break;
        }
        case 0x22:
        {
            if (g_World == NULL)
                return;

            Move(1);
            uint serial = ReadUInt32BE();
            CGameCharacter *character = g_World->FindWorldCharacter(serial);

            if (character != NULL)
            {
                int damage = ReadUInt8();

                CTextData *text = new CTextData();
                text->Unicode = false;
                text->Font = 3;
                text->Serial = serial;
                text->Color = (serial == g_PlayerSerial ? 0x0034 : 0x0021);
                text->Type = TT_OBJECT;
                text->Text = std::to_string(damage);
                text->GenerateTexture(0);
                text->SetX(text->m_Texture.Width / 2);
                int height = text->m_Texture.Height;

                CTextData *head = (CTextData *)character->m_DamageTextControl.Last();

                if (head != NULL)
                {
                    height += head->GetY();

                    if (height > 0)
                    {
                        if (height > 100)
                            height = 0;

                        text->SetY(height);
                    }
                }

                character->m_DamageTextControl.Add(text);
                text->Timer = g_Ticks + DAMAGE_TEXT_NORMAL_DELAY;
            }

            break;
        }
        case 0x26:
        {
            uchar val = ReadUInt8();

            if (val > CST_FAST_UNMOUNT_AND_CANT_RUN)
                val = 0;

            g_SpeedMode = (CHARACTER_SPEED_TYPE)val;

            break;
        }
        default:
            break;
    }
}

PACKET_HANDLER(DenyWalk)
{
    WISPFUN_DEBUG("c150_f48");
    if (g_Player == NULL)
        return;

    g_Ping = 0;

    uint8_t sequence = ReadUInt8();
    uint16_t x = ReadUInt16BE();
    uint16_t y = ReadUInt16BE();
    Direction dir = (Direction)(ReadUInt8() & 0x7);
    uint8_t z = ReadUInt8();

    g_Player->DenyWalk(sequence, dir, x, y, z);

    g_World->MoveToTop(g_Player);
}

PACKET_HANDLER(ConfirmWalk)
{
    WISPFUN_DEBUG("c150_f49");
    if (g_Player == NULL)
        return;

    uchar sequence = ReadUInt8();

    uchar newnoto = ReadUInt8() & (~0x40);

    if (!newnoto || newnoto >= 7)
        newnoto = 0x01;

    g_Player->Notoriety = newnoto;

    g_Player->ConfirmWalk(sequence);

    g_World->MoveToTop(g_Player);
}

PACKET_HANDLER(OpenUrl)
{
    WISPFUN_DEBUG("c150_f50");
    g_Orion.GoToWebLink(ReadString(0));
}

PACKET_HANDLER(Target)
{
    WISPFUN_DEBUG("c150_f51");
    g_Target.SetData(*this);

    if (g_PartyHelperTimer > g_Ticks && g_PartyHelperTarget)
    {
        g_Target.SendTargetObject(g_PartyHelperTarget);
        g_PartyHelperTimer = 0;
        g_PartyHelperTarget = 0;
    }
}

PACKET_HANDLER(Talk)
{
    WISPFUN_DEBUG("c150_f52");
    if (g_World == NULL)
    {
        if (g_GameState == GS_GAME_CONNECT)
        {
            uint serial = ReadUInt32BE();
            ushort graphic = ReadUInt16BE();
            SPEECH_TYPE type = (SPEECH_TYPE)ReadUInt8();
            ushort textColor = ReadUInt16BE();
            ushort font = ReadUInt16BE();

            string name(ReadString(0));
            string str = "";

            if (Size > 44)
            {
                Ptr = Start + 44;
                g_ConnectionScreen.SetConnectionFailed(true);
                g_ConnectionScreen.SetText(ReadString(0));
            }
        }

        return;
    }

    uint serial = ReadUInt32BE();
    ushort graphic = ReadUInt16BE();
    SPEECH_TYPE type = (SPEECH_TYPE)ReadUInt8();
    ushort textColor = ReadUInt16BE();
    ushort font = ReadUInt16BE();
    string name(ReadString(0));

    if (!serial && !graphic && type == ST_NORMAL && font == 0xFFFF && textColor == 0xFFFF &&
        ToLowerA(name) == "system")
    {
        uchar sbuffer[0x28] = { 0x03, 0x00, 0x28, 0x20, 0x00, 0x34, 0x00, 0x03, 0xdb, 0x13,
                                0x14, 0x3f, 0x45, 0x2c, 0x58, 0x0f, 0x5d, 0x44, 0x2e, 0x50,
                                0x11, 0xdf, 0x75, 0x5c, 0xe0, 0x3e, 0x71, 0x4f, 0x31, 0x34,
                                0x05, 0x4e, 0x18, 0x1e, 0x72, 0x0f, 0x59, 0xad, 0xf5, 0x00 };

        g_Orion.Send(sbuffer, 0x28);

        return;
    }

    if (font >= g_FontManager.FontCount)
        return;

    string str = "";

    if (Size > 44)
    {
        Ptr = Start + 44;
        str = ReadString(0);
    }

    LOG("%s: %s\n", name.c_str(), str.c_str());

    CGameObject *obj = g_World->FindWorldObject(serial);

    if (type == ST_BROADCAST || serial == 0xFFFFFFFF || !serial ||
        (ToLowerA(name) == "system" && obj == NULL))
    {
        g_Orion.CreateTextMessage(TT_SYSTEM, serial, (uchar)font, textColor, str);
    }
    else
    {
        if (type == ST_EMOTE)
        {
            textColor = g_ConfigManager.EmoteColor;
            str = "*" + str + "*";
        }

        if (obj != NULL)
        {
            obj->JournalPrefix = "";
            if (!obj->GetName().length())
            {
                obj->SetName(name);

                if (obj->NPC)
                    g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
            }

            if (type == ST_SYSTEM)
                obj->JournalPrefix = "You see: ";
            else if (obj->GetName().length())
                obj->JournalPrefix = obj->GetName() + ": ";
        }

        g_Orion.CreateTextMessage(TT_OBJECT, serial, (uchar)font, textColor, str);
    }
}

PACKET_HANDLER(UnicodeTalk)
{
    WISPFUN_DEBUG("c150_f53");
    if (g_World == NULL)
    {
        if (g_GameState == GS_GAME_CONNECT)
        {
            uint serial = ReadUInt32BE();
            ushort graphic = ReadUInt16BE();
            SPEECH_TYPE type = (SPEECH_TYPE)ReadUInt8();
            ushort textColor = ReadUInt16BE();
            ushort font = ReadUInt16BE();
            uint language = ReadUInt32BE();

            string name(ReadString(0));

            if (Size > 48)
            {
                Ptr = Start + 48;
                g_ConnectionScreen.SetConnectionFailed(true);
                g_ConnectionScreen.SetText(ToString(ReadWString((Size - 48) / 2)));
            }
        }

        return;
    }

    uint serial = ReadUInt32BE();
    ushort graphic = ReadUInt16BE();
    SPEECH_TYPE type = (SPEECH_TYPE)ReadUInt8();
    ushort textColor = ReadUInt16BE();
    ushort font = ReadUInt16BE();
    uint language = ReadUInt32BE();
    string name(ReadString(0));

    if (!serial && !graphic && type == ST_NORMAL && font == 0xFFFF && textColor == 0xFFFF &&
        ToLowerA(name) == "system")
    {
        uchar sbuffer[0x28] = { 0x03, 0x00, 0x28, 0x20, 0x00, 0x34, 0x00, 0x03, 0xdb, 0x13,
                                0x14, 0x3f, 0x45, 0x2c, 0x58, 0x0f, 0x5d, 0x44, 0x2e, 0x50,
                                0x11, 0xdf, 0x75, 0x5c, 0xe0, 0x3e, 0x71, 0x4f, 0x31, 0x34,
                                0x05, 0x4e, 0x18, 0x1e, 0x72, 0x0f, 0x59, 0xad, 0xf5, 0x00 };

        g_Orion.Send(sbuffer, 0x28);

        return;
    }

    wstring str = L"";

    if (Size > 48)
    {
        Ptr = Start + 48;
        str = ReadWString((Size - 48) / 2);
    }

    LOG("%s: %s\n", name.c_str(), ToString(str).c_str());

    CGameObject *obj = g_World->FindWorldObject(serial);

    if (type == ST_GUILD_CHAT)
    {
        type = ST_BROADCAST;
        textColor = g_ConfigManager.GuildMessageColor;
        str = L"[Guild][" + ToWString(name) + L"]: " + str;
    }
    else if (type == ST_ALLIANCE_CHAT)
    {
        type = ST_BROADCAST;
        textColor = g_ConfigManager.AllianceMessageColor;
        str = L"[Alliance][" + ToWString(name) + L"]: " + str;
    }

    if (type == ST_BROADCAST || serial == 0xFFFFFFFF || !serial ||
        (ToLowerA(name) == "system" && obj == NULL))
        g_Orion.CreateUnicodeTextMessage(
            TT_SYSTEM, serial, (uchar)g_ConfigManager.SpeechFont, textColor, str);
    else
    {
        if (type == ST_EMOTE && !textColor)
        {
            textColor = g_ConfigManager.EmoteColor;
        }

        if (obj != NULL)
        {
            obj->JournalPrefix = "";

            if (!obj->GetName().length())
            {
                obj->SetName(name);

                if (obj->NPC)
                    g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
            }

            if (type == ST_SYSTEM)
                obj->JournalPrefix = "You see: ";
            else if (obj->GetName().length())
                obj->JournalPrefix = obj->GetName() + ": ";
        }

        g_Orion.CreateUnicodeTextMessage(
            TT_OBJECT, serial, (uchar)g_ConfigManager.SpeechFont, textColor, str);
    }
}

PACKET_HANDLER(ClientTalk)
{
    WISPFUN_DEBUG("c150_f54");
    if (!g_AbyssPacket03First)
    {
        bool parse = true;

        if (Start[4] == 0x78)
        {
            Size = ReadInt16BE(2);
            Start += 4;
        }
        else if (Start[4] == 0x3C)
        {
            Size = ReadInt16BE(2);
            Start += 4;
        }
        else if (Start[4] == 0x25)
        {
            Size = 0x14;
            Start += 4;
        }
        else if (Start[4] == 0x2E)
        {
            Size = 0x0F;
            Start += 4;
        }
        else
            parse = false;

        if (parse)
            OnPacket();
    }

    g_AbyssPacket03First = false;
}

PACKET_HANDLER(OnReceiveSendPacket)
{
	OnPacket();
}

PACKET_HANDLER(MultiPlacement)
{
    WISPFUN_DEBUG("c150_f55");
    if (g_World == NULL)
        return;

    g_Target.SetMultiData(*this);
}

PACKET_HANDLER(GraphicEffect)
{
    WISPFUN_DEBUG("c150_f56");
    if (g_World == NULL)
        return;

    uchar type = ReadUInt8();

    if (type > 3)
    {
        if (type == 4 && *Start == 0x70)
        {
            Move(8);
            ushort val = ReadInt16BE();

            if (val > SET_TO_BLACK_VERY_FAST)
                val = SET_TO_BLACK_VERY_FAST;

            g_ScreenEffectManager.Use(SEM_SUNSET, (SCREEN_EFFECT_TYPE)val, true);
        }

        return;
    }

    uint sourceSerial = ReadUInt32BE();
    uint destSerial = ReadUInt32BE();
    ushort graphic = ReadUInt16BE();
    short sourceX = ReadInt16BE();
    short sourceY = ReadInt16BE();
    char sourceZ = ReadInt8();
    short destX = ReadInt16BE();
    short destY = ReadInt16BE();
    char destZ = ReadInt8();
    uchar speed = ReadUInt8();
    short duration = (short)ReadUInt8() * 50;

    Move(2);
    uchar fixedDirection = ReadUInt8();
    uchar explode = ReadUInt8();

    uint color = 0;
    uint renderMode = 0;

    if (*Start != 0x70)
    {
        color = ReadUInt32BE();
        renderMode = ReadUInt32BE() % 7;

        if (*Start == 0xC7)
        {
        }
    }

    CGameEffect *effect = NULL;
    if (!type)
    {
        effect = new CGameEffectMoving();

        if (!speed)
            speed++;

        ((CGameEffectMoving *)effect)->MoveDelay = 20 / speed;
    }
    else
        effect = new CGameEffect();

    effect->EffectType = (EFFECT_TYPE)type;
    effect->Serial = sourceSerial;
    effect->DestSerial = destSerial;
    effect->Graphic = graphic;

    CGameCharacter *sourceObject = g_World->FindWorldCharacter(sourceSerial);

    if (sourceObject != NULL)
    {
        effect->SetX(sourceObject->GetX());
        effect->SetY(sourceObject->GetY());
        effect->SetZ(sourceObject->GetZ());
    }
    else
    {
        effect->SetX(sourceX);
        effect->SetY(sourceY);
        effect->SetZ(sourceZ);
    }

    CGameCharacter *destObject = g_World->FindWorldCharacter(destSerial);

    if (destObject != NULL)
    {
        effect->DestX = destObject->GetX();
        effect->DestY = destObject->GetY();
        effect->DestZ = destObject->GetZ();
    }
    else
    {
        effect->DestX = destX;
        effect->DestY = destY;
        effect->DestZ = destZ;
    }

    uintptr_t addressAnimData = (uintptr_t)g_FileManager.m_AnimdataMul.Start;

    if (addressAnimData)
    {
        PANIM_DATA pad = (PANIM_DATA)(addressAnimData + ((graphic * 68) + 4 * ((graphic / 8) + 1)));

        effect->Speed = pad->FrameInterval * 45;
    }
    else
        effect->Speed = speed + 6;

    effect->Duration = g_Ticks + duration;
    effect->FixedDirection = (fixedDirection != 0);
    effect->Explode = (explode != 0);

    effect->Color = (ushort)color;
    effect->RenderMode = renderMode;

    g_EffectManager.AddEffect(effect);
}

PACKET_HANDLER(DeathScreen)
{
    WISPFUN_DEBUG("c150_f57");
    uchar action = ReadUInt8();

    if (action != 1)
    {
        g_GameScreen.SmoothScreenAction = 0;
        g_ScreenEffectManager.Use(SEM_SUNSET, SET_TO_BLACK, true);

        g_Weather.Reset();
        g_Target.Reset();

        g_GumpManager.CloseGump(0, 0, GT_DRAG);

        if (g_ConfigManager.GetMusic())
            g_Orion.PlayMusic(42, true);

        g_Player->SetWarMode(WarModeState::Peace);

        g_DeathScreenTimer = g_Ticks + DEATH_SCREEN_DELAY;
    }
}

PACKET_HANDLER(PlaySoundEffect)
{
    WISPFUN_DEBUG("c150_f58");
    Move(1);
    ushort index = ReadUInt16BE();
    ushort volume = ReadUInt16BE();
    ushort xCoord = ReadUInt16BE();
    ushort yCoord = ReadUInt16BE();

    int distance = GetDistance(g_Player, WISP_GEOMETRY::CPoint2Di(xCoord, yCoord));

    g_Orion.PlaySoundEffect(index, g_SoundManager.GetVolumeValue(distance));
}

PACKET_HANDLER(PlayMusic)
{
    WISPFUN_DEBUG("c150_f59");
    ushort index = ReadUInt16BE();

    if (!g_ConfigManager.GetMusic() || GetForegroundWindow() != g_OrionWindow.Handle ||
        g_ConfigManager.GetMusicVolume() < 1)
        return;

    g_Orion.PlayMusic(index);
}

PACKET_HANDLER(DragAnimation)
{
    WISPFUN_DEBUG("c150_f60");
    if (g_World == NULL)
        return;

    ushort graphic = ReadUInt16BE();
    graphic += ReadUInt8();

    ushort color = ReadUInt16BE();
    ushort count = ReadUInt16BE();

    uint sourceSerial = ReadUInt32BE();
    short sourceX = ReadInt16BE();
    short sourceY = ReadInt16BE();
    char sourceZ = ReadInt8();
    uint destSerial = ReadUInt32BE();
    short destX = ReadInt16BE();
    short destY = ReadInt16BE();
    char destZ = ReadInt8();

    if (graphic == 0x0EED)
        graphic = 0x0EEF;
    else if (graphic == 0x0EEA)
        graphic = 0x0EEC;
    else if (graphic == 0x0EF0)
        graphic = 0x0EF2;

    CGameCharacter *sourceObj = g_World->FindWorldCharacter(sourceSerial);

    if (sourceObj == NULL)
        sourceSerial = 0;
    else
    {
        sourceX = sourceObj->GetX();
        sourceY = sourceObj->GetY();
        sourceZ = sourceObj->GetZ();
    }

    CGameCharacter *destObj = g_World->FindWorldCharacter(destSerial);

    if (destObj == NULL)
        destSerial = 0;
    else
    {
        destX = destObj->GetX();
        destY = destObj->GetY();
        destZ = destObj->GetZ();
    }

    CGameEffect *effect = NULL;

    uchar speed = 5;

    if (!sourceSerial || !destSerial)
    {
        effect = new CGameEffectMoving();
        effect->EffectType = EF_MOVING;
        effect->FixedDirection = true;

        ((CGameEffectMoving *)effect)->MoveDelay = 20 / speed;
    }
    else
    {
        effect = new CGameEffectDrag();
        effect->EffectType = EF_DRAG;
    }

    effect->Graphic = graphic;
    effect->Color = color;
    effect->Serial = sourceSerial;
    effect->DestSerial = destSerial;
    effect->SetX(sourceX);
    effect->SetY(sourceY);
    effect->SetZ(sourceZ);
    effect->DestX = destX;
    effect->DestY = destY;
    effect->DestZ = destZ;
    effect->Duration = g_Ticks + 5000;

    uintptr_t addressAnimData = (uintptr_t)g_FileManager.m_AnimdataMul.Start;

    if (addressAnimData)
    {
        PANIM_DATA pad = (PANIM_DATA)(addressAnimData + ((graphic * 68) + 4 * ((graphic / 8) + 1)));

        effect->Speed = pad->FrameInterval * 45;
    }
    else
        effect->Speed = speed + 6;

    g_EffectManager.AddEffect(effect);
}

PACKET_HANDLER(CorpseEquipment)
{
    WISPFUN_DEBUG("c150_f61");
    if (g_World == NULL)
        return;

    uint cserial = ReadUInt32BE();

    puchar end = Start + Size;

    int layer = ReadUInt8();

    while (layer && Ptr < end)
    {
        uint serial = ReadUInt32BE();

        CGameItem *obj = g_World->FindWorldItem(serial);

        if (obj != NULL && obj->Container == cserial)
            g_World->PutEquipment(obj, cserial, layer);

        layer = ReadUInt8();
    }
}

PACKET_HANDLER(ASCIIPrompt)
{
    WISPFUN_DEBUG("c150_f62");
    if (g_World == NULL)
        return;

    g_ConsolePrompt = PT_ASCII;
    memcpy(&g_LastASCIIPrompt[0], &Start[0], 11);
}

PACKET_HANDLER(UnicodePrompt)
{
    WISPFUN_DEBUG("c150_f63");
    if (g_World == NULL)
        return;

    g_ConsolePrompt = PT_UNICODE;
    memcpy(&g_LastUnicodePrompt[0], &Start[0], 11);
}

PACKET_HANDLER(CharacterAnimation)
{
    WISPFUN_DEBUG("c150_f64");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    CGameCharacter *obj = g_World->FindWorldCharacter(serial);

    if (obj != NULL)
    {
        ushort action = ReadUInt16BE();
        ushort frameCount = ReadUInt16BE();
        frameCount = 0;
        ushort repeatMode = ReadUInt16BE();
        bool frameDirection = (ReadUInt8() == 0);
        bool repeat = (ReadUInt8() != 0);
        uchar delay = ReadUInt8();
        obj->SetAnimation(
            g_AnimationManager.GetReplacedObjectAnimation(obj, action),
            delay,
            (uchar)frameCount,
            (uchar)repeatMode,
            repeat,
            frameDirection);
        obj->AnimationFromServer = true;
    }
}

PACKET_HANDLER(NewCharacterAnimation)
{
    WISPFUN_DEBUG("c150_f65");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    CGameCharacter *obj = g_World->FindWorldCharacter(serial);

    if (obj != NULL)
    {
        ushort type = ReadUInt16BE();
        ushort action = ReadUInt16BE();
        uchar mode = ReadUInt8();

        uchar group = g_AnimationManager.GetObjectNewAnimation(obj, type, action, mode);

        obj->SetAnimation(group);

        obj->AnimationRepeatMode = 1;
        obj->AnimationDirection = true;

        if ((type == 1 || type == 2) && obj->Graphic == 0x0015)
            obj->AnimationRepeat = true;

        obj->AnimationFromServer = true;
    }
}

PACKET_HANDLER(DisplayQuestArrow)
{
    WISPFUN_DEBUG("c150_f66");
    g_QuestArrow.Timer = g_Ticks + 1000;
    g_QuestArrow.Enabled = (ReadUInt8() != 0);
    g_QuestArrow.X = ReadUInt16BE();
    g_QuestArrow.Y = ReadUInt16BE();
}

PACKET_HANDLER(ClientViewRange)
{
    WISPFUN_DEBUG("c150_f67");
    g_ConfigManager.UpdateRange = ReadUInt8();
}

PACKET_HANDLER(KrriosClientSpecial)
{
    WISPFUN_DEBUG("c150_f68");
    uchar type = ReadUInt8();

    if (type == 0xFE)
        CPacketRazorAnswer().Send();
}

PACKET_HANDLER(AssistVersion)
{
    WISPFUN_DEBUG("c150_f69");
    uint version = ReadUInt32BE();

    CPacketAssistVersion(version, g_Orion.ClientVersionText).Send();
}

PACKET_HANDLER(CharacterListNotification)
{
    WISPFUN_DEBUG("c150_f70");
    g_Orion.InitScreen(GS_DELETE);
    g_ConnectionScreen.SetType(CST_CHARACTER_LIST);
    g_ConnectionScreen.SetConnectionFailed(true);
    g_ConnectionScreen.SetErrorCode(ReadUInt8());
}

PACKET_HANDLER(ErrorCode)
{
    WISPFUN_DEBUG("c150_f71");
    uchar code = ReadUInt8();

    g_Orion.InitScreen(GS_DELETE);
    g_ConnectionScreen.SetType(CST_GAME_LOGIN);
    g_ConnectionScreen.SetErrorCode(code);

    if (code > 7)
        g_ConnectionScreen.SetErrorCode(3);
}

PACKET_HANDLER(AttackCharacter)
{
    WISPFUN_DEBUG("c150_f72");
    g_LastAttackObject = ReadUInt32BE();

    if (g_LastAttackObject != 0 && g_World != NULL)
    {
        CGameCharacter *obj = g_World->FindWorldCharacter(g_LastAttackObject);

        if (obj != NULL && !obj->MaxHits)
            CPacketStatusRequest(g_LastAttackObject).Send();
    }
}

PACKET_HANDLER(Season)
{
    WISPFUN_DEBUG("c150_f73");
    if (g_World == NULL)
        return;

    uchar season = ReadUInt8();

    if (season > ST_DESOLATION)
        season = 0;

    if ((g_Player->Dead()) && (SEASON_TYPE)season != ST_DESOLATION)
        return;

    g_OldSeason = (SEASON_TYPE)season;
    g_OldSeasonMusic = ReadUInt8();

    if (g_Season == ST_DESOLATION)
        g_OldSeasonMusic = DEATH_MUSIC_INDEX;

    g_Orion.ChangeSeason((SEASON_TYPE)season, g_OldSeasonMusic);
}

PACKET_HANDLER(DisplayDeath)
{
    WISPFUN_DEBUG("c150_f74");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    uint corpseSerial = ReadUInt32BE();
    uint running = ReadUInt32BE();

    CGameCharacter *owner = g_World->FindWorldCharacter(serial);

    if (owner == NULL)
        return;

    serial |= 0x80000000;

    g_World->ReplaceObject(owner, serial);

    if (corpseSerial)
        g_CorpseManager.Add(CCorpse(corpseSerial, serial, owner->Dir, running != 0));

    uchar group = g_AnimationManager.GetDieGroupIndex(owner->Graphic, running != 0);

    owner->SetAnimation(group, 0, 5, 1, false, false);
}

PACKET_HANDLER(OpenChat)
{
    WISPFUN_DEBUG("c150_f75");
    uchar newbuf[4] = { 0xf0, 0x00, 0x04, 0xff };
    g_Orion.Send(newbuf, 4);
}

PACKET_HANDLER(DisplayClilocString)
{
    WISPFUN_DEBUG("c150_f76");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    ushort graphic = ReadUInt16BE();
    uchar type = ReadUInt8();
    ushort color = ReadUInt16BE();
    ushort font = ReadUInt16BE();
    uint cliloc = ReadUInt32BE();

    uchar flags = 0;

    if (*Start == 0xCC)
        flags = ReadUInt8();

    string name = ReadString(30).c_str();

    wstring affix = L"";
    if (*Start == 0xCC)
        affix = DecodeUTF8(ReadString()).c_str();

    wstring message =
        g_ClilocManager.ParseArgumentsToClilocString(cliloc, false, (wchar_t *)Ptr).c_str();
    message += affix;

    CGameObject *obj = g_World->FindWorldObject(serial);

    if (serial == 0xFFFFFFFF || !serial || (ToLowerA(name) == "system" && obj == NULL))
        g_Orion.CreateUnicodeTextMessage(
            TT_SYSTEM, serial, g_ConfigManager.ChatFont, color, message);
    else
    {
        if (obj != NULL)
        {
            if (!name.length())
            {
                obj->JournalPrefix = "You see: ";
            }
            else
            {
                obj->JournalPrefix = name + ": ";
                obj->SetName(name);

                if (obj->NPC)
                    g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
            }
        }

        g_Orion.CreateUnicodeTextMessage(
            TT_OBJECT, serial, g_ConfigManager.ChatFont, color, message);
    }
}

PACKET_HANDLER(MegaCliloc)
{
    WISPFUN_DEBUG("c150_f77");
    if (g_World == NULL)
        return;

    ushort unknown = ReadUInt16BE();

    if (unknown > 1)
        return;

    uint serial = ReadUInt32BE();

    Move(2);
    uint clilocRevision = ReadUInt32BE();

    puchar end = Start + Size;

    WSTRING_LIST list;

    while (Ptr < end)
    {
        uint cliloc = ReadUInt32BE();

        if (!cliloc)
            break;

        int len = ReadInt16BE();

        wstring argument = L"";

        if (len > 0)
        {
            argument = wstring((wchar_t *)Ptr, len / 2);
            Ptr += len;
        }

        wstring str = g_ClilocManager.ParseArgumentsToClilocString(cliloc, true, argument);

        bool canAdd = true;

        for (const wstring &tempStr : list)
        {
            if (tempStr == str)
            {
                canAdd = false;
                break;
            }
        }

        if (canAdd)
            list.push_back(str);
    }

    CGameItem *container = NULL;

    CGameObject *obj = g_World->FindWorldObject(serial);

    if (obj != NULL)
        container = g_World->FindWorldItem(obj->Container);

    bool inBuyList = false;

    if (container != NULL)
        inBuyList =
            (container->Layer == OL_BUY || container->Layer == OL_BUY_RESTOCK ||
             container->Layer == OL_SELL);

    bool first = true;
    wstring name = L"";
    wstring data(L"");

    if (!list.empty())
    {
        for (const wstring &str : list)
        {
            if (first)
            {
                name = str;

                if (obj != NULL && !obj->NPC)
                {
                    obj->SetName(ToString(str));
                    obj->GenerateObjectHandlesTexture(str);
                }

                first = false;
            }
            else
            {
                if (data.length())
                    data += L"\n";

                data += str;
            }
        }
    }

    g_ObjectPropertiesManager.Add(serial, CObjectProperty(serial, clilocRevision, name, data));

    if (obj != NULL && g_ToolTip.m_Object == obj)
        g_ObjectPropertiesManager.Reset();

    if (inBuyList && container->Serial)
    {
        CGumpShop *gump = (CGumpShop *)g_GumpManager.GetGump(container->Serial, 0, GT_SHOP);

        if (gump != NULL)
        {
            CGUIHTMLGump *htmlGump = gump->m_ItemList[0];

            QFOR(shopItem, htmlGump->m_Items, CBaseGUI *)
            {
                if (shopItem->Type == GOT_SHOPITEM && shopItem->Serial == serial &&
                    ((CGUIShopItem *)shopItem)->NameFromCliloc)
                {
                    CGUIShopItem *si = (CGUIShopItem *)shopItem;

                    int oldHeight = si->GetSize().Height;

                    si->Name = ToString(name);
                    si->CreateNameText();
                    si->UpdateOffsets();

                    int delta = si->GetSize().Height - oldHeight;

                    if (delta && shopItem->m_Next != NULL)
                    {
                        QFOR(shopItem2, shopItem->m_Next, CBaseGUI *)
                        {
                            if (shopItem2->Type == GOT_SHOPITEM)
                                ((CGUIShopItem *)shopItem2)
                                    ->SetY(((CGUIShopItem *)shopItem2)->GetY() + delta);
                        }
                    }

                    break;
                }
            }

            htmlGump->CalculateDataSize();
        }
    }
}

PACKET_HANDLER(Damage)
{
    WISPFUN_DEBUG("c150_f78");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    CGameCharacter *character = g_World->FindWorldCharacter(serial);

    if (character != NULL)
    {
        int damage = ReadUInt16BE();

        CTextData *text = new CTextData();
        text->Unicode = false;
        text->Font = 3;
        text->Serial = serial;
        text->Color = (serial == g_PlayerSerial ? 0x0034 : 0x0021);
        text->Type = TT_OBJECT;
        text->Text = std::to_string(damage);
        text->GenerateTexture(0);
        text->SetX(text->m_Texture.Width / 2);
        int height = text->m_Texture.Height;

        CTextData *head = (CTextData *)character->m_DamageTextControl.Last();

        if (head != NULL)
        {
            height += head->GetY();

            if (height > 0)
            {
                if (height > 100)
                    height = 0;

                text->SetY(height);
            }
        }

        character->m_DamageTextControl.Add(text);
        text->Timer = g_Ticks + DAMAGE_TEXT_NORMAL_DELAY;
    }
}

PACKET_HANDLER(BuffDebuff)
{
    WISPFUN_DEBUG("c150_f79");
    if (g_World == NULL)
        return;

    const int tableCount = 126;

    static const ushort table[tableCount] = {
        0x754C, 0x754A, 0x0000, 0x0000, 0x755E, 0x7549, 0x7551, 0x7556, 0x753A, 0x754D, 0x754E,
        0x7565, 0x753B, 0x7543, 0x7544, 0x7546, 0x755C, 0x755F, 0x7566, 0x7554, 0x7540, 0x7568,
        0x754F, 0x7550, 0x7553, 0x753E, 0x755D, 0x7563, 0x7562, 0x753F, 0x7559, 0x7557, 0x754B,
        0x753D, 0x7561, 0x7558, 0x755B, 0x7560, 0x7541, 0x7545, 0x7552, 0x7569, 0x7548, 0x755A,
        0x753C, 0x7547, 0x7567, 0x7542, 0x758A, 0x758B, 0x758C, 0x758D, 0x0000, 0x758E, 0x094B,
        0x094C, 0x094D, 0x094E, 0x094F, 0x0950, 0x753E, 0x5011, 0x7590, 0x7591, 0x7592, 0x7593,
        0x7594, 0x7595, 0x7596, 0x7598, 0x7599, 0x759B, 0x759C, 0x759E, 0x759F, 0x75A0, 0x75A1,
        0x75A3, 0x75A4, 0x75A5, 0x75A6, 0x75A7, 0x75C0, 0x75C1, 0x75C2, 0x75C3, 0x75C4, 0x75F2,
        0x75F3, 0x75F4, 0x75F5, 0x75F6, 0x75F7, 0x75F8, 0x75F9, 0x75FA, 0x75FB, 0x75FC, 0x75FD,
        0x75FE, 0x75FF, 0x7600, 0x7601, 0x7602, 0x7603, 0x7604, 0x7605, 0x7606, 0x7607, 0x7608,
        0x7609, 0x760A, 0x760B, 0x760C, 0x760D, 0x760E, 0x760F, 0x7610, 0x7611, 0x7612, 0x7613,
        0x7614, 0x7615, 0x75C5, 0x75F6, 0x761B
    };

    const ushort buffIconStart = 0x03E9;

    uint serial = ReadUInt32BE();

    ushort iconID = ReadUInt16BE() - buffIconStart;

    if (iconID < tableCount)
    {
        CGumpBuff *gump = (CGumpBuff *)g_GumpManager.UpdateGump(0, 0, GT_BUFF);

        if (gump != NULL)
        {
            ushort mode = ReadUInt16BE();

            if (mode)
            {
                Move(12);

                ushort timer = ReadUInt16BE();

                Move(3);

                uint titleCliloc = ReadUInt32BE();
                uint descriptionCliloc = ReadUInt32BE();
                uint wtfCliloc = ReadUInt32BE();

                Move(4);

                wstring title = g_ClilocManager.Cliloc(g_Language)->GetW(titleCliloc, true).c_str();
                wstring description = L"";
                wstring wtf = L"";

                if (descriptionCliloc)
                {
                    wstring arguments = ReadWString(0, false);

                    description = L'\n' + g_ClilocManager.ParseArgumentsToClilocString(
                                              descriptionCliloc, true, arguments);

                    if (description.length() < 2)
                        description = L"";
                }

                if (wtfCliloc)
                    wtf = L'\n' + g_ClilocManager.Cliloc(g_Language)->GetW(wtfCliloc, true).c_str();

                wstring text = L"<left>" + title + description + wtf + L"</left>";

                gump->AddBuff(table[iconID], timer, text);
            }
            else
                gump->DeleteBuff(table[iconID]);
        }
    }
}

PACKET_HANDLER(SecureTrading)
{
    WISPFUN_DEBUG("c150_f80");
    if (g_World == NULL)
        return;

    uchar type = ReadUInt8();
    uint serial = ReadUInt32BE();

    if (type == 0)
    {
        uint id1 = ReadUInt32BE();
        uint id2 = ReadUInt32BE();
        uchar hasName = ReadUInt8();

        CGumpSecureTrading *gump = new CGumpSecureTrading(id1, 0, 0, id1, id2);

        CGameObject *obj = g_World->FindWorldObject(id1);

        if (obj != NULL)
        {
            obj = obj->GetTopObject()->FindSecureTradeBox();

            if (obj != NULL)
                obj->Clear();
        }

        obj = g_World->FindWorldObject(id2);

        if (obj != NULL)
        {
            obj = obj->GetTopObject()->FindSecureTradeBox();

            if (obj != NULL)
                obj->Clear();
        }

        if (hasName && *Ptr)
            gump->Text = ReadString(0);

        g_GumpManager.AddGump(gump);
    }
    else if (type == 1)
        g_GumpManager.CloseGump(serial, 0, GT_TRADE);
    else if (type == 2)
    {
        CGumpSecureTrading *gump =
            (CGumpSecureTrading *)g_GumpManager.UpdateGump(serial, 0, GT_TRADE);

        if (gump != NULL)
        {
            uint id1 = ReadUInt32BE();
            uint id2 = ReadUInt32BE();

            gump->StateMy = (id1 != 0);
            gump->StateOpponent = (id2 != 0);
        }
    }
}

PACKET_HANDLER(TextEntryDialog)
{
    WISPFUN_DEBUG("c150_f81");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    uchar parentID = ReadUInt8();
    uchar buttonID = ReadUInt8();

    short textLen = ReadInt16BE();
    string text = ReadString(textLen);

    bool haveCancel = !ReadUInt8();
    uchar variant = ReadUInt8();
    int maxLength = ReadUInt32BE();

    short descLen = ReadInt16BE();
    string desc = ReadString(descLen);

    CGumpTextEntryDialog *gump =
        new CGumpTextEntryDialog(serial, 143, 172, variant, maxLength, text, desc);
    gump->NoClose = haveCancel;
    gump->ParentID = parentID;
    gump->ButtonID = buttonID;

    g_GumpManager.AddGump(gump);
}

PACKET_HANDLER(OpenMenu)
{
    WISPFUN_DEBUG("c150_f82");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    uint id = ReadUInt16BE();

    uchar nameLen = ReadUInt8();
    string name = ReadString(nameLen);

    uchar count = ReadUInt8();

    if (unpack16(Ptr))
    {
        CGumpMenu *gump = new CGumpMenu(serial, id, 0, 0);

        gump->Add(new CGUIGumppic(0x0910, 0, 0));

        gump->Add(new CGUIColoredPolygon(0, 0, 40, 42, 217, 49, 0xFF000001));

        CGUIText *text = (CGUIText *)gump->Add(new CGUIText(0x0386, 39, 18));
        text->CreateTextureA(1, name, 200, TS_LEFT, UOFONT_FIXED);

        CGUIHTMLGump *htmlGump = (CGUIHTMLGump *)gump->Add(
            new CGUIHTMLGump(CGumpMenu::ID_GM_HTMLGUMP, 0, 40, 42, 217, 49, false, true));
        htmlGump->Initalize(true);

        htmlGump->Add(new CGUIShader(&g_ColorizerShader, true));

        int posX = 0;

        IFOR (i, 0, count)
        {
            ushort graphic = ReadUInt16BE();
            ushort color = ReadUInt16BE();

            nameLen = ReadUInt8();
            name = ReadString(nameLen);

            WISP_GEOMETRY::CSize size = g_Orion.GetStaticArtDimension(graphic);

            if (size.Width && size.Height)
            {
                int posY = size.Height;

                if (posY >= 47)
                    posY = 0;
                else
                    posY = ((47 - posY) / 2);

                CGUIMenuObject *menuObject = (CGUIMenuObject *)htmlGump->Add(
                    new CGUIMenuObject((int)i + 1, graphic, color, posX, posY, name));

                posX += size.Width;
            }
        }

        htmlGump->Add(new CGUIShader(&g_ColorizerShader, false));

        htmlGump->CalculateDataSize();

        gump->m_TextObject = (CGUIText *)gump->Add(new CGUIText(0x0386, 42, 105));

        g_GumpManager.AddGump(gump);
    }
    else
    {
        int x = (g_OrionWindow.GetSize().Width / 2) - 200;
        int y = (g_OrionWindow.GetSize().Height / 2) - ((121 + (count * 21)) / 2);

        CGumpGrayMenu *gump = new CGumpGrayMenu(serial, id, x, y);

        CGUIResizepic *background =
            (CGUIResizepic *)gump->Add(new CGUIResizepic(0, 0x13EC, 0, 0, 400, 11111));

        CGUIText *text = (CGUIText *)gump->Add(new CGUIText(0x0386, 20, 16));
        text->CreateTextureA(1, name, 370);

        int offsetY = 35 + text->m_Texture.Height;
        int gumpHeight = 70 + offsetY;

        IFOR (i, 0, count)
        {
            Move(4);

            nameLen = ReadUInt8();
            name = ReadString(nameLen);

            gump->Add(new CGUIRadio((int)i + 1, 0x138A, 0x138B, 0x138A, 20, offsetY));

            offsetY += 2;

            text = (CGUIText *)gump->Add(new CGUIText(0x0386, 50, offsetY));
            text->CreateTextureA(1, name, 340);

            int addHeight = text->m_Texture.Height;

            if (addHeight < 21)
                addHeight = 21;

            offsetY += addHeight - 1;
            gumpHeight += addHeight;
        }

        offsetY += 5;

        gump->Add(
            new CGUIButton(CGumpGrayMenu::ID_GGM_CANCEL, 0x1450, 0x1450, 0x1451, 70, offsetY));
        gump->Add(
            new CGUIButton(CGumpGrayMenu::ID_GGM_CONTINUE, 0x13B2, 0x13B2, 0x13B3, 200, offsetY));

        background->Height = gumpHeight;

        g_GumpManager.AddGump(gump);
    }
}

void CPacketManager::AddHTMLGumps(CGump *gump, vector<HTMLGumpDataInfo> &list)
{
    WISPFUN_DEBUG("c150_f83");
    IFOR (i, 0, (int)list.size())
    {
        HTMLGumpDataInfo &data = list[i];

        CGUIHTMLGump *htmlGump = (CGUIHTMLGump *)gump->Add(new CGUIHTMLGump(
            data.TextID + 1,
            0x0BB8,
            data.GumpCoords->X,
            data.GumpCoords->Y,
            data.Width,
            data.Height,
            data.HaveBackground,
            data.HaveScrollbar));
        htmlGump->DrawOnly = (data.HaveScrollbar == 0);

        int width = htmlGump->Width;

        if (data.HaveScrollbar)
            width -= 16;

        uint htmlColor = 0xFFFFFFFF;
        ushort color = 0;

        if (data.Color)
        {
            if (data.Color == 0x00FFFFFF)
                htmlColor = 0xFFFFFFFE;
            else
                htmlColor = (g_ColorManager.Color16To32((ushort)data.Color) << 8) | 0xFF;
        }
        else if (!data.HaveBackground)
        {
            color = 0xFFFF;

            if (!data.HaveScrollbar)
                htmlColor = 0x010101FF;
        }
        else
        {
            width -= 9;
            htmlColor = 0x010101FF;
        }

        CGUIHTMLText *htmlText = (CGUIHTMLText *)htmlGump->Add(
            new CGUIHTMLText(data.TextID, (uchar)1, color, 0, 0, width, TS_LEFT, 0, htmlColor));

        if (data.IsXMF)
        {
			if (!data.Args.empty())
				htmlText->Text = g_ClilocManager.ParseArgumentsToClilocString(data.TextID, false, data.Args);
			else
				htmlText->Text = g_ClilocManager.Cliloc(g_Language)->GetW(data.TextID);
				
            htmlText->CreateTexture(!data.HaveBackground);
            htmlGump->CalculateDataSize();
        }
    }

    list.clear();
}

PACKET_HANDLER(OpenGump)
{
    WISPFUN_DEBUG("c150_f84");
    if (g_World == NULL)
        return;

    vector<HTMLGumpDataInfo> htmlGumlList;

    uint serial = ReadUInt32BE();
    uint id = ReadUInt32BE();
    int x = ReadInt32BE();
    int y = ReadInt32BE();

    std::unordered_map<uint, GumpCoords>::iterator found = m_GumpsCoordsCache.find(id);

    if (found != m_GumpsCoordsCache.end())
    {
        x = found->second.X;
        y = found->second.Y;
    }
    else
    {
        SetCachedGumpCoords(id, x, y);
    }

    CGumpGeneric *gump = new CGumpGeneric(serial, x, y, id);

    ushort commandsLength = ReadUInt16BE();
    string commands = ReadString(commandsLength);

    WISP_FILE::CTextFileParser parser("", " ", "", "{}");
    WISP_FILE::CTextFileParser cmdParser("", " ", "", "");
    WISP_FILE::CTextFileParser tilepicGraphicParser("", ",", "", "");

    STRING_LIST commandList = parser.GetTokens(commands.c_str());
    CBaseGUI *lastGumpObject = NULL;

    bool EntryChanged = false;
    int FirstPage = 0;
    int CurrentPage = 0;
    CEntryText *ChangeEntry;

    for (const string &str : commandList)
    {
        STRING_LIST list = cmdParser.GetTokens(str.c_str(), false);

        int listSize = (int)list.size();

        if (!listSize)
            continue;

        string cmd = ToLowerA(list[0]);

        CBaseGUI *go = NULL;

        if (cmd == "nodispose")
        {
        }
        else if (cmd == "nomove")
        {
            gump->NoMove = true;
        }
        else if (cmd == "noclose")
        {
            gump->NoClose = true;
        }
        else if (cmd == "page")
        {
            if (listSize >= 2)
            {
                AddHTMLGumps(gump, htmlGumlList);

                int page = ToInt(list[1]);
                go = new CGUIPage(page);
                if (FirstPage == 0)
                    FirstPage = page;
                CurrentPage = page;
            }
        }
        else if (cmd == "group")
        {
            if (listSize >= 2)
            {
                int group = ToInt(list[1]);
                go = new CGUIGroup(group);
            }
        }
        else if (cmd == "endgroup")
        {
            go = new CGUIGroup(0);
        }
        else if (cmd == "resizepic")
        {
            if (listSize >= 6)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int graphic = ToInt(list[3]);
                int width = ToInt(list[4]);
                int height = ToInt(list[5]);

                go = new CGUIResizepic(0, graphic, x, y, width, height);
                go->DrawOnly = true;
            }
        }
        else if (cmd == "checkertrans")
        {
            if (listSize >= 5)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int width = ToInt(list[3]);
                int height = ToInt(list[4]);

                go = new CGUIChecktrans(x, y, width, height);
            }
        }
        else if (cmd == "button")
        {
            if (listSize >= 5)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int up = ToInt(list[3]);
                int down = ToInt(list[4]);
                int action = 0;
                int toPage = 0;
                int index = 0;

                if (listSize >= 6)
                    action = ToInt(list[5]);
                if (listSize >= 7)
                    toPage = ToInt(list[6]);
                if (listSize >= 8)
                    index = ToInt(list[7]);

                if (action != 0)
                    toPage = -1;

                go = new CGUIButton(index, up, up, down, x, y);
                go->CheckPolygon = true;

                ((CGUIButton *)go)->ToPage = toPage;
            }
        }
        else if (cmd == "buttontileart")
        {
            if (listSize >= 12)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int up = ToInt(list[3]);
                int down = ToInt(list[4]);
                int action = ToInt(list[5]);
                int toPage = ToInt(list[6]);
                int index = ToInt(list[7]);
                int tileGraphic = ToInt(list[8]);
                int tileColor = ToInt(list[9]);
                int tileX = ToInt(list[10]);
                int tileY = ToInt(list[11]);

                if (action != 0)
                    toPage = -1;

                go = new CGUIButtonTileart(
                    index, up, up, down, x, y, tileGraphic, tileColor, tileX, tileY);

                ((CGUIButton *)go)->ToPage = toPage;
            }
        }
        else if (cmd == "checkbox")
        {
            if (listSize >= 5)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int up = ToInt(list[3]);
                int down = ToInt(list[4]);
                int state = 0;
                int index = 0;

                if (listSize >= 6)
                    state = ToInt(list[5]);
                if (listSize >= 7)
                    index = ToInt(list[6]);

                go = new CGUICheckbox(index, up, down, up, x, y);

                ((CGUICheckbox *)go)->Checked = (state != 0);
            }
        }
        else if (cmd == "radio")
        {
            if (listSize >= 5)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int up = ToInt(list[3]);
                int down = ToInt(list[4]);
                int state = 0;
                int index = 0;

                if (listSize >= 6)
                    state = ToInt(list[5]);
                if (listSize >= 7)
                    index = ToInt(list[6]);

                go = new CGUIRadio(index, up, down, up, x, y);

                ((CGUIRadio *)go)->Checked = (state != 0);
            }
        }
        else if (cmd == "text")
        {
            if (listSize >= 5)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int color = ToInt(list[3]);
                int index = ToInt(list[4]);

                if (color)
                    color++;

                go = new CGUIGenericText(index, color, x, y);
                go->DrawOnly = true;
            }
        }
        else if (cmd == "croppedtext")
        {
            if (listSize >= 7)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int width = ToInt(list[3]);
                int height = ToInt(list[4]);
                int color = ToInt(list[5]);
                int index = ToInt(list[6]);

                if (color)
                    color++;

                go = new CGUIGenericText(index, color, x, y, width);
                go->DrawOnly = true;
            }
        }
        else if (cmd == "textentry")
        {
            if (listSize >= 8)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int width = ToInt(list[3]);
                int height = ToInt(list[4]);
                int color = ToInt(list[5]);
                int index = ToInt(list[6]);
                int textIndex = ToInt(list[7]);

                gump->Add(new CGUIHitBox(index + 1, x, y, width, height));
                gump->Add(new CGUIScissor(true, x, y, 0, 0, width, height));
                go = new CGUIGenericTextEntry(index + 1, textIndex, color, x, y);
                ((CGUIGenericTextEntry *)go)->CheckOnSerial = true;
                ((CGUITextEntry *)go)->m_Entry.Width = width;
                if (!EntryChanged)
                {
                    if (CurrentPage == 0 || CurrentPage == FirstPage)
                    {
                        ChangeEntry = &((CGUITextEntry *)go)->m_Entry;
                        EntryChanged = true;
                    }
                }
            }
        }
        else if (cmd == "textentrylimited")
        {
            if (listSize >= 9)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int width = ToInt(list[3]);
                int height = ToInt(list[4]);
                int color = ToInt(list[5]);
                int index = ToInt(list[6]);
                int textIndex = ToInt(list[7]);
                int length = ToInt(list[8]);

                gump->Add(new CGUIHitBox(index + 1, x, y, width, height));
                gump->Add(new CGUIScissor(true, x, y, 0, 0, width, height));
                go = new CGUIGenericTextEntry(index + 1, textIndex, color, x, y, width, length);
                ((CGUIGenericTextEntry *)go)->CheckOnSerial = true;
                ((CGUITextEntry *)go)->m_Entry.Width = width;
                if (!EntryChanged)
                {
                    if (CurrentPage == 0 || CurrentPage == FirstPage)
                    {
                        ChangeEntry = &((CGUITextEntry *)go)->m_Entry;
                        EntryChanged = true;
                    }
                }
            }
        }
        else if (cmd == "tilepic" || cmd == "tilepichue")
        {
            if (listSize >= 4)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int color = 0;
                int graphic = 0;

                if (cmd == "tilepic")
                {
                    STRING_LIST graphicList = tilepicGraphicParser.GetTokens(list[3].c_str());

                    if (graphicList.size() >= 1)
                    {
                        graphic = ToInt(graphicList[0]);

                        if (graphicList.size() >= 2)
                            color = ToInt(graphicList[1]);
                    }
                }
                else
                    graphic = ToInt(list[3]);

                if (listSize >= 5)
                    color = ToInt(list[4]);

                if (color)
                    gump->Add(new CGUIShader(&g_ColorizerShader, true));

                go = new CGUITilepic(graphic, color, x, y);
                go->DrawOnly = true;
            }
        }
        else if (cmd == "gumppic")
        {
            if (listSize >= 4)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int graphic = ToInt(list[3]);
                int color = 0;

                if (listSize >= 5)
                {
                    WISP_FILE::CTextFileParser gumppicParser("", "=", "", "");
                    STRING_LIST hueList = gumppicParser.GetTokens(list[4].c_str());

                    if (hueList.size() > 1)
                        color = ToInt(hueList[1]);
                    else
                        color = ToInt(hueList[0]);

                    if (listSize >= 6)
                    {
                        STRING_LIST classList = gumppicParser.GetTokens(list[5].c_str());

                        if (hueList.size() > 1)
                        {
                            if (ToLowerA(classList[0]) == "class" &&
                                ToLowerA(Trim(classList[1])) == "virtuegumpitem")
                            {
                                go = new CGUIVirtureGump(graphic, x, y);
                            }
                        }
                    }
                }

                if (go == NULL)
                {
                    if (color)
                        gump->Add(new CGUIShader(&g_ColorizerShader, true));

                    go = new CGUIGumppic(graphic, x, y);
                    go->Color = color + 1;
                    go->DrawOnly = true;
                }
            }
        }
        else if (cmd == "gumppictiled")
        {
            if (listSize >= 6)
            {
                int x = ToInt(list[1]);
                int y = ToInt(list[2]);
                int width = ToInt(list[3]);
                int height = ToInt(list[4]);
                int graphic = ToInt(list[5]);

                if (width > 0 && height > 0)
                {
                    go = new CGUIGumppicTiled(graphic, x, y, width, height);
                    go->DrawOnly = true;
                }
            }
        }
        else if (cmd == "htmlgump" || cmd == "xmfhtmlgump" || cmd == "xmfhtmlgumpcolor")
        {
            if (listSize >= 8)
            {
                HTMLGumpDataInfo htmlInfo = { 0 };
                htmlInfo.IsXMF = (cmd != "htmlgump");
                GumpCoords *gumpCoords = new GumpCoords{ ToInt(list[1]), ToInt(list[2]) };
                htmlInfo.GumpCoords = gumpCoords;
                htmlInfo.Width = ToInt(list[3]);
                htmlInfo.Height = ToInt(list[4]);
                htmlInfo.TextID = ToInt(list[5]);
                htmlInfo.HaveBackground = ToInt(list[6]);
                htmlInfo.HaveScrollbar = ToInt(list[7]);
                htmlInfo.Color = 0;

                if (cmd == "xmfhtmlgumpcolor" && listSize >= 9)
                {
                    htmlInfo.Color = ToInt(list[8]);

                    if (htmlInfo.Color == 0x7FFF)
                        htmlInfo.Color = 0x00FFFFFF;
                }

                htmlGumlList.push_back(htmlInfo);
            }
        }
        else if (cmd == "xmfhtmltok")
        {
            if (listSize >= 9)
            {
                HTMLGumpDataInfo htmlInfo = { 0 };
                htmlInfo.IsXMF = true;
                GumpCoords *gumpCoords = new GumpCoords{ ToInt(list[1]), ToInt(list[2]) };
                htmlInfo.GumpCoords = gumpCoords;
                htmlInfo.Width = ToInt(list[3]);
                htmlInfo.Height = ToInt(list[4]);
                htmlInfo.HaveBackground = ToInt(list[5]);
                htmlInfo.HaveScrollbar = ToInt(list[6]);
                htmlInfo.Color = ToInt(list[7]);

                if (htmlInfo.Color == 0x7FFF)
                    htmlInfo.Color = 0x00FFFFFF;

                htmlInfo.TextID = ToInt(list[8]);
				listSize--;
				if (list[9].size() > 1 && list[listSize].size() > 1)
				{
					for (int i = 9; i <= listSize; i++)
					{
						htmlInfo.Args.append(list[i].begin() + ((i == 9) ? 1 : 0), list[i].end() - ((i == listSize) ? 1 : 0));
						if (i < listSize)
							htmlInfo.Args.append(L" ");
					}
				}

                htmlGumlList.push_back(htmlInfo);
            }
        }
        else if (cmd == "tooltip")
        {
            if (listSize >= 2 && lastGumpObject != NULL)
                lastGumpObject->ClilocID = ToInt(list[1]);
        }
        else if (cmd == "mastergump")
        {
            if (listSize >= 2)
                gump->MasterGump = ToInt(list[1]);
        }

        if (go != NULL)
        {
            lastGumpObject = go;
            gump->Add(go);

            if ((go->Type == GOT_TILEPIC || go->Type == GOT_GUMPPIC) && go->Color)
                gump->Add(new CGUIShader(&g_ColorizerShader, false));
            else if (go->Type == GOT_TEXTENTRY)
                gump->Add(new CGUIScissor(false));
        }
    }

    AddHTMLGumps(gump, htmlGumlList);

    int textLinesCount = ReadInt16BE();

    IFOR (i, 0, textLinesCount)
    {
        int linelen = ReadInt16BE();

        if (linelen)
            gump->AddText((int)i, ReadWString(linelen, true));
        else
            gump->AddText((int)i, L"");
    }

    if (EntryChanged)
        g_EntryPointer = ChangeEntry;

    g_GumpManager.AddGump(gump);
}

PACKET_HANDLER(OpenCompressedGump)
{
    WISPFUN_DEBUG("c150_f85");
    if (g_World == NULL)
        return;

    uint senderID = ReadUInt32BE();
    uint gumpID = ReadUInt32BE();
    uint x = ReadUInt32BE();
    uint y = ReadUInt32BE();
    uLongf cLen = ReadUInt32BE() - 4;
    uLongf dLen = ReadUInt32BE();

    if (cLen < 1)
    {
        LOG("CLen=%d\nServer Sends bad Compressed Gumpdata!\n", cLen);

        return;
    }
    else if ((int)(28 + cLen) > Size)
    {
        LOG("Server Sends bad Compressed Gumpdata!\n");

        return;
    }

    UCHAR_LIST decLayoutData(dLen);
    LOG("Gump layout:\n\tSenderID=0x%08X\n\tGumpID=0x%08X\n\tCLen=%d\n\tDLen=%d\nDecompressing layout gump data...\n",
        senderID,
        gumpID,
        cLen,
        dLen);

    int z_err = uncompress(&decLayoutData[0], &dLen, Ptr, cLen);

    if (z_err != Z_OK)
    {
        LOG("Decompress layout gump error %d\n", z_err);

        return;
    }

    LOG("Layout gump data decompressed!\n");

    Move(cLen);

    uint linesCount = ReadUInt32BE();
    uint cTLen = 0;
    uLongf dTLen = 0;
    UCHAR_LIST gumpDecText;

    if (linesCount > 0)
    {
        cTLen = ReadUInt32BE();
        dTLen = ReadUInt32BE();

        gumpDecText.resize(dTLen);

        LOG("Decompressing text gump data...\n");

        z_err = uncompress(&gumpDecText[0], &dTLen, Ptr, cTLen);

        if (z_err != Z_OK)
        {
            LOG("Decompress text gump error %d\n", z_err);

            return;
        }

        LOG("Text gump data decompressed!\nGump text lines:\n\tLinesCount=%d\n\tCTLen=%d\n\tDTLen=%d\n",
            linesCount,
            cTLen,
            dTLen);
    }

    int newsize = 21 + dLen + 2 + dTLen;

    UCHAR_LIST newbufData(newsize);
    puchar newbuf = &newbufData[0];
    newbuf[0] = 0xb0;
    pack16(newbuf + 1, newsize);
    pack32(newbuf + 3, senderID);
    pack32(newbuf + 7, gumpID);
    pack32(newbuf + 11, x);
    pack32(newbuf + 15, y);
    pack16(newbuf + 19, (ushort)dLen);
    memcpy(newbuf + 21, &decLayoutData[0], dLen);
    pack16(newbuf + 21 + dLen, (ushort)linesCount);

    if (linesCount > 0)
        memcpy(newbuf + 23 + dLen, &gumpDecText[0], dTLen);
    else
        newbuf[newsize - 1] = 0x00;

    LOG("Gump decompressed! newsize=%d\n", newsize);

    Size = newsize;
    Start = newbuf;
    End = Start + Size;

    OnPacket();
}

PACKET_HANDLER(DyeData)
{
    WISPFUN_DEBUG("c150_f86");
    uint serial = ReadUInt32BE();
    Move(2);
    ushort graphic = ReadUInt16BE();

    WISP_GEOMETRY::CSize gumpSize = g_Orion.GetGumpDimension(0x0906);

    int x = (WORD)((g_OrionWindow.GetSize().Width / 2) - (gumpSize.Width / 2));
    int y = (WORD)((g_OrionWindow.GetSize().Height / 2) - (gumpSize.Height / 2));

    CGumpDye *gump = new CGumpDye(serial, x, y, graphic);

    g_GumpManager.AddGump(gump);
}

PACKET_HANDLER(DisplayMap)
{
    WISPFUN_DEBUG("c150_f87");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    ushort gumpid = ReadUInt16BE();

    ushort startX = ReadUInt16BE();
    ushort startY = ReadUInt16BE();
    ushort endX = ReadUInt16BE();
    ushort endY = ReadUInt16BE();
    ushort width = ReadUInt16BE();
    ushort height = ReadUInt16BE();

    CGumpMap *gump = new CGumpMap(serial, gumpid, startX, startY, endX, endY, width, height);

    if (*Start == 0xF5)
    {
        ushort facet = 0;

        if (*Start == 0xF5)
            facet = ReadUInt16BE();

        g_MultiMap.LoadFacet(gump, gump->m_Texture, facet);
    }
    else
        g_MultiMap.LoadMap(gump, gump->m_Texture);

    g_GumpManager.AddGump(gump);

    CGameItem *obj = g_World->FindWorldItem(serial);

    if (obj != NULL)
        obj->Opened = true;
}

PACKET_HANDLER(MapData)
{
    WISPFUN_DEBUG("c150_f88");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    CGumpMap *gump = (CGumpMap *)g_GumpManager.UpdateGump(serial, 0, GT_MAP);

    if (gump != NULL && gump->m_DataBox != NULL)
    {
        switch ((MAP_MESSAGE)ReadUInt8())
        {
            case MM_ADD:
            {
                Move(1);

                short x = ReadUInt16BE();
                short y = ReadUInt16BE();

                gump->m_DataBox->Add(new CGUIGumppic(0x139B, x, y));
                gump->WantRedraw = true;

                break;
            }
            case MM_INSERT:
            {
                break;
            }
            case MM_MOVE:
            {
                break;
            }
            case MM_REMOVE:
            {
                break;
            }
            case MM_CLEAR:
            {
                gump->m_DataBox->Clear();
                gump->WantRedraw = true;

                break;
            }
            case MM_EDIT_RESPONSE:
            {
                gump->SetPlotState(ReadUInt8());

                break;
            }
            default:
                break;
        }
    }
}

PACKET_HANDLER(TipWindow)
{
    WISPFUN_DEBUG("c150_f89");
    uchar flag = ReadUInt8();

    if (flag != 1)
    {
        uint serial = ReadUInt32BE();
        short len = ReadInt16BE();

        string str = ReadString(len);

        int x = 20;
        int y = 20;

        if (!flag)
        {
            x = 200;
            y = 100;
        }

        CGumpTip *gump = new CGumpTip(serial, x, y, str, flag != 0);

        g_GumpManager.AddGump(gump);
    }
}

PACKET_HANDLER(CharacterProfile)
{
    WISPFUN_DEBUG("c150_f90");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    wstring topText = ToWString(ReadString(0));

    wstring bottomText = ReadWString(0);
    wstring dataText = ReadWString(0);

    CGumpProfile *gump = new CGumpProfile(serial, 170, 90, topText, bottomText, dataText);

    g_GumpManager.AddGump(gump);
}

PACKET_HANDLER(BulletinBoardData)
{
    WISPFUN_DEBUG("c150_f91");
    if (g_World == NULL)
        return;

    switch (ReadUInt8())
    {
        case 0:
        {
            uint serial = ReadUInt32BE();

            CGameItem *item = g_World->FindWorldItem(serial);

            if (item != NULL)
            {
                CGumpBulletinBoard *bbGump =
                    (CGumpBulletinBoard *)g_GumpManager.UpdateGump(serial, 0, GT_BULLETIN_BOARD);

                if (bbGump != NULL)
                {
                    CBaseGUI *bbItem = (CBaseGUI *)bbGump->m_HTMLGump->m_Items;

                    while (bbItem != NULL)
                    {
                        CBaseGUI *bbNext = (CBaseGUI *)bbItem->m_Next;

                        if (bbItem->Type == GOT_BB_OBJECT)
                            bbGump->m_HTMLGump->Delete(bbItem);

                        bbItem = bbNext;
                    }

                    bbGump->m_HTMLGump->CalculateDataSize();
                }

                item->Opened = true;
            }

            string str((char *)Ptr);

            int x = (g_OrionWindow.GetSize().Width / 2) - 245;
            int y = (g_OrionWindow.GetSize().Height / 2) - 205;

            CGumpBulletinBoard *gump = new CGumpBulletinBoard(serial, x, y, str);

            g_GumpManager.AddGump(gump);

            break;
        }
        case 1:
        {
            uint boardSerial = ReadUInt32BE();

            CGumpBulletinBoard *gump =
                (CGumpBulletinBoard *)g_GumpManager.GetGump(boardSerial, 0, GT_BULLETIN_BOARD);

            if (gump != NULL)
            {
                uint serial = ReadUInt32BE();
                uint parentID = ReadUInt32BE();

                int len = ReadUInt8();
                wstring text = (len > 0 ? DecodeUTF8(ReadString(len)) : L"") + L" - ";

                len = ReadUInt8();
                text += (len > 0 ? DecodeUTF8(ReadString(len)) : L"") + L" - ";

                len = ReadUInt8();
                text += (len > 0 ? DecodeUTF8(ReadString(len)) : L"");

                int posY = (gump->m_HTMLGump->GetItemsCount() - 5) * 18;

                if (posY < 0)
                    posY = 0;

                gump->m_HTMLGump->Add(new CGUIBulletinBoardObject(serial, 0, posY, text));
                gump->m_HTMLGump->CalculateDataSize();
            }

            break;
        }
        case 2:
        {
            uint boardSerial = ReadUInt32BE();

            CGumpBulletinBoard *gump =
                (CGumpBulletinBoard *)g_GumpManager.GetGump(boardSerial, 0, GT_BULLETIN_BOARD);

            if (gump != NULL)
            {
                uint serial = ReadUInt32BE();

                int len = ReadUInt8();
                wstring poster = (len > 0 ? DecodeUTF8(ReadString(len)) : L"");

                len = ReadUInt8();
                wstring subject = (len > 0 ? DecodeUTF8(ReadString(len)) : L"");

                len = ReadUInt8();
                wstring dataTime = (len > 0 ? DecodeUTF8(ReadString(len)) : L"");

                Move(4);

                uchar unknown = ReadUInt8();

                if (unknown > 0)
                {
                    Move(unknown * 4);
                }

                uchar lines = ReadUInt8();
                wstring data = L"";

                IFOR (i, 0, lines)
                {
                    uchar linelen = ReadUInt8();

                    if (data.length())
                        data += L"\n";

                    if (linelen > 0)
                        data += DecodeUTF8(ReadString(linelen));
                }

                uchar variant = 1 + (int)(poster == ToWString(g_Player->GetName()));
                g_GumpManager.AddGump(new CGumpBulletinBoardItem(
                    serial, 0, 0, variant, boardSerial, poster, subject, dataTime, data));
            }

            break;
        }
        default:
            break;
    }
}

PACKET_HANDLER(OpenBook)
{
    WISPFUN_DEBUG("c150_f92");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    uchar flags = ReadUInt8();
    Move(1);
    WORD pageCount = ReadUInt16BE();

    CGumpBook *gump = new CGumpBook(
        serial, 0, 0, pageCount, flags != 0, 1);

    gump->m_EntryTitle->m_Entry.SetText(ReadString(60));
    gump->m_EntryAuthor->m_Entry.SetText(ReadString(30));

    g_GumpManager.AddGump(gump);
}

PACKET_HANDLER(OpenBookNew)
{
    WISPFUN_DEBUG("c150_f93");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();
    uchar flag1 = ReadUInt8();
    uchar flag2 = ReadUInt8();
    ushort pageCount = ReadUInt16BE();

    CGumpBook *gump = new CGumpBook(serial, 0, 0, pageCount, (flag1 + flag2) != 0, true);

    int titleLen = ReadUInt16BE();

    if (titleLen > 0)
        gump->m_EntryTitle->m_Entry.SetText(ReadString(titleLen));

    int authorLen = ReadUInt16BE();

    if (authorLen > 0)
        gump->m_EntryAuthor->m_Entry.SetText(ReadString(authorLen));

    g_GumpManager.AddGump(gump);
}

PACKET_HANDLER(BookData)
{
    WISPFUN_DEBUG("c150_f94");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGumpBook *gump = (CGumpBook *)g_GumpManager.GetGump(serial, 0, GT_BOOK);

    if (gump != NULL)
    {
        ushort pageCount = ReadUInt16BE();

        IFOR (i, 0, pageCount)
        {
            ushort page = ReadUInt16BE();

            if (page > gump->PageCount)
                continue;

            ushort lineCount = ReadUInt16BE();

            wstring str = L"";

            IFOR (j, 0, lineCount)
            {
                if (j)
                    str += L'\n';

                wstring temp = DecodeUTF8(ReadString(0));

                while (temp.length() && (temp.back() == L'\n' || temp.back() == L'\r'))
                    temp.resize(temp.length() - 1);

                str += temp;
            }

            gump->SetPageData(page, str);
        }
    }
}

PACKET_HANDLER(BuyList)
{
    WISPFUN_DEBUG("c150_f95");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGameItem *container = g_World->FindWorldItem(serial);

    if (container == NULL)
    {
        LOG("Error!!! Buy container is not found!!!\n");
        return;
    }

    uint vendorSerial = container->Container;

    CGameCharacter *vendor = g_World->FindWorldCharacter(vendorSerial);

    if (vendor == NULL)
    {
        LOG("Error!!! Buy vendor is not found!!!\n");
        return;
    }

    CGumpShop *gump = (CGumpShop *)g_GumpManager.GetGump(vendorSerial, 0, GT_SHOP);

    if (gump != NULL && (gump->Serial != vendorSerial || !gump->IsBuyGump))
    {
        g_GumpManager.RemoveGump(gump);
        gump = NULL;
    }

    if (gump == NULL)
    {
        gump = new CGumpShop(vendorSerial, true, 150, 5);
        g_GumpManager.AddGump(gump);
    }

    gump->WantRedraw = true;

    if (container->Layer == OL_BUY_RESTOCK || container->Layer == OL_BUY)
    {
        uchar count = ReadUInt8();

        CGameItem *item = (CGameItem *)container->m_Items;

        if (item == NULL)
            return;

        bool reverse = (item->GetX() > 1);

        if (reverse)
        {
            while (item != NULL && item->m_Next != NULL)
                item = (CGameItem *)item->m_Next;
        }

        CGUIHTMLGump *htmlGump = gump->m_ItemList[0];

        int currentY = 0;

        QFOR(shopItem, htmlGump->m_Items, CBaseGUI *)
        {
            if (shopItem->Type == GOT_SHOPITEM)
                currentY += shopItem->GetSize().Height;
        }

        IFOR (i, 0, count)
        {
            if (item == NULL)
            {
                LOG("Error!!! Buy item is not found!!!\n");
                break;
            }

            item->Price = ReadUInt32BE();

            uchar nameLen = ReadUInt8();
            string name = ReadString(nameLen);

            int clilocNum = 0;

            if (Int32TryParse(name, clilocNum))
            {
                item->SetName(g_ClilocManager.Cliloc(g_Language)->GetA(clilocNum, true));
                item->NameFromCliloc = true;
            }
            else
            {
                item->SetName(name);
                item->NameFromCliloc = false;
            }

            if (reverse)
                item = (CGameItem *)item->m_Prev;
            else
                item = (CGameItem *)item->m_Next;
        }

        htmlGump->CalculateDataSize();
    }
    else
        LOG("Unknown layer for buy container!!!\n");
}

PACKET_HANDLER(SellList)
{
    WISPFUN_DEBUG("c150_f96");
    if (g_World == NULL)
        return;

    uint serial = ReadUInt32BE();

    CGameCharacter *vendor = g_World->FindWorldCharacter(serial);

    if (vendor == NULL)
    {
        LOG("Error!!! Sell vendor is not found!!!\n");
        return;
    }

    ushort itemsCount = ReadUInt16BE();

    if (!itemsCount)
    {
        LOG("Error!!! Sell list is empty.\n");
        return;
    }

    g_GumpManager.CloseGump(0, 0, GT_SHOP);

    CGumpShop *gump = new CGumpShop(serial, false, 100, 0);
    CGUIHTMLGump *htmlGump = gump->m_ItemList[0];

    int currentY = 0;

    IFOR (i, 0, itemsCount)
    {
        uint itemSerial = ReadUInt32BE();
        ushort graphic = ReadUInt16BE();
        ushort color = ReadUInt16BE();
        ushort count = ReadUInt16BE();
        ushort price = ReadUInt16BE();
        int nameLen = ReadInt16BE();
        string name = ReadString(nameLen);

        int clilocNum = 0;
        bool nameFromCliloc = false;

        if (Int32TryParse(name, clilocNum))
        {
            name = g_ClilocManager.Cliloc(g_Language)->GetA(clilocNum, true);
            nameFromCliloc = true;
        }

        CGUIShopItem *shopItem = (CGUIShopItem *)htmlGump->Add(
            new CGUIShopItem(itemSerial, graphic, color, count, price, name, 0, currentY));
        shopItem->NameFromCliloc = nameFromCliloc;

        if (!i)
        {
            shopItem->Selected = true;
            shopItem->CreateNameText();
            shopItem->UpdateOffsets();
        }

        currentY += shopItem->GetSize().Height;
    }

    htmlGump->CalculateDataSize();

    g_GumpManager.AddGump(gump);
}

PACKET_HANDLER(BuyReply)
{
    WISPFUN_DEBUG("c150_f97");
    uint serial = ReadUInt32BE();
    uchar flag = ReadUInt8();

    if (!flag)
        g_GumpManager.CloseGump(serial, 0, GT_SHOP);
}

PACKET_HANDLER(Logout)
{
    WISPFUN_DEBUG("c150_f98");
    g_Orion.LogOut();
}

PACKET_HANDLER(OPLInfo)
{
    WISPFUN_DEBUG("c150_f99");
    if (g_TooltipsEnabled)
    {
        uint serial = ReadUInt32BE();
        uint revision = ReadUInt32BE();

        if (!g_ObjectPropertiesManager.RevisionCheck(serial, revision))
            AddMegaClilocRequest(serial);
    }
}

PACKET_HANDLER(CustomHouse)
{
    WISPFUN_DEBUG("c150_f100");
}

PACKET_HANDLER(OrionMessages)
{
    WISPFUN_DEBUG("c150_f101");
    ushort command = ReadUInt16BE();
    uchar type = command >> 12;
    command &= 0x0FFF;

    if (type)
        return;

    switch (command)
    {
        case OCT_ORION_FEATURES:
        {
            g_OrionFeaturesFlags = ReadUInt32BE();

            g_ConfigManager.UpdateFeatures();

            break;
        }
        case OCT_ORION_IGNORE_TILES_IN_FILTER:
        {
            g_Orion.m_IgnoreInFilterTiles.clear();

            ushort count = ReadUInt16BE();

            IFOR (i, 0, count)
                g_Orion.m_IgnoreInFilterTiles.push_back(
                    std::pair<ushort, ushort>(ReadUInt16BE(), 0));

            ushort countRange = ReadUInt16BE();

            IFOR (i, 0, countRange)
            {
                ushort rangeStart = ReadUInt16BE();
                ushort rangeEnd = ReadUInt16BE();
                g_Orion.m_IgnoreInFilterTiles.push_back(
                    std::pair<ushort, ushort>(rangeStart, rangeEnd));
            }

            break;
        }
        case OCT_ORION_VERSION:
        {
            CPacketOrionVersion(g_Orion.OrionVersionNumeric).Send();
            break;
        }
        case OCT_CLOSE_GENERIC_GUMP_WITHOUT_RESPONSE:
        {
            uint serial = ReadUInt32BE();
            uint id = ReadUInt32BE();
            uchar all = ReadUInt8();

            QFOR(gump, g_GumpManager.m_Items, CGump *)
            {
                if (gump->GumpType == GT_GENERIC && gump->Serial == serial && gump->ID == id)
                {
                    gump->RemoveMark = true;

                    if (!all)
                        break;
                }
            }

            break;
        }
        case OCT_SELECT_MENU:
        {
            uint serial = ReadUInt32BE();
            uint id = ReadUInt32BE();
            uint code = ReadUInt32BE();

            if (!serial && !id)
            {
                for (CGump *gump = (CGump *)g_GumpManager.m_Items; gump != NULL;)
                {
                    CGump *next = (CGump *)gump->m_Next;

                    if (gump->GumpType == GT_MENU || gump->GumpType == GT_GRAY_MENU)
                    {
                        CPacketMenuResponse(gump, code).Send();
                        g_GumpManager.RemoveGump(gump);
                    }

                    gump = next;
                }

                break;
            }

            CGump *gump = g_GumpManager.GetGump(serial, id, GT_MENU);

            if (gump == NULL)
            {
                gump = g_GumpManager.GetGump(serial, id, GT_GRAY_MENU);

                if (gump != NULL)
                {
                    CPacketGrayMenuResponse(gump, code).Send();
                    g_GumpManager.RemoveGump(gump);
                }
            }
            else
            {
                CPacketMenuResponse(gump, code).Send();
                g_GumpManager.RemoveGump(gump);
            }

            break;
        }
        case OCT_CAST_SPELL_REQUEST:
        {
            int id = ReadUInt32BE();

            if (id >= 0)
            {
                g_LastSpellIndex = id;

                CPacketCastSpell(id).Send();
            }

            break;
        }
        case OCT_USE_SKILL_REQUEST:
        {
            int id = ReadUInt32BE();

            if (id >= 0)
            {
                g_LastSkillIndex = id;

                CPacketUseSkill(id).Send();
            }

            break;
        }
        case OCT_DRAW_STATUSBAR:
        {
            uint serial = ReadUInt32BE();
            int x = ReadInt32BE();
            int y = ReadInt32BE();
            bool minimized = (ReadUInt8() != 0);

            if (serial != g_PlayerSerial)
                minimized = true;

            CGump *gump = g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);

            if (gump != NULL)
            {
                gump->Minimized = minimized;

                if (gump->Minimized)
                {
                    gump->MinimizedX = x;
                    gump->MinimizedY = y;
                }
                else
                {
                    gump->SetX(x);
                    gump->SetY(y);
                }
            }
            else
            {
                CPacketStatusRequest(serial).Send();
                g_GumpManager.AddGump(new CGumpStatusbar(serial, x, y, minimized));
            }

            break;
        }
        case OCT_CLOSE_STATUSBAR:
        {
            uint serial = ReadUInt32BE();
            g_GumpManager.CloseGump(serial, 0, GT_STATUSBAR);
            break;
        }
        case OCT_SECURE_TRADE_CHECK:
        {
            uint id1 = ReadUInt32BE();

            CGumpSecureTrading *gump =
                (CGumpSecureTrading *)g_GumpManager.UpdateGump(id1, 0, GT_TRADE);

            if (gump != NULL)
            {
                gump->StateMy = (ReadUInt8() != 0);
                CPacketTradeResponse(gump, 2).Send();
            }

            break;
        }
        case OCT_SECURE_TRADE_CLOSE:
        {
            uint id1 = ReadUInt32BE();

            CGumpSecureTrading *gump =
                (CGumpSecureTrading *)g_GumpManager.GetGump(id1, 0, GT_TRADE);

            if (gump != NULL)
            {
                gump->RemoveMark = true;
                CPacketTradeResponse(gump, 1).Send();
            }

            break;
        }
        case OCT_UNICODE_SPEECH_REQUEST:
        {
            ushort color = ReadUInt16BE();
            wstring text = ReadWString(0);

            CGameConsole::Send(text, color);

            break;
        }
        case OCT_RENAME_MOUNT_REQUEST:
        {
            uint serial = ReadUInt32BE();
            string text = ReadString(0);

            CPacketRenameRequest(serial, text).Send();

            break;
        }
        case OCT_RECONNECT:
        {
            g_Orion.StartReconnect();
            break;
        }
        case OCT_PLAY_MACRO:
        {
            int count = ReadUInt16BE();

            static CMacro existsMacros(0, false, false, false);
            existsMacros.Clear();

            g_MacroPointer = NULL;

            IFOR (m, 0, count)
            {
                string name = ReadString(0);
                string param = ReadString(0);
                MACRO_CODE macroCode = MC_NONE;

                IFOR (i, 0, CMacro::MACRO_ACTION_NAME_COUNT)
                {
                    if (name == CMacro::m_MacroActionName[i])
                    {
                        macroCode = (MACRO_CODE)i;
                        break;
                    }
                }

                if (macroCode == MC_NONE)
                {
                    LOG("Invalid macro name: %s\n", name.c_str());
                    continue;
                }

                CMacroObject *macro = CMacro::CreateMacro(macroCode);

                if (param.length())
                {
                    if (macro->HaveString())
                        ((CMacroObjectString *)macro)->String = param;
                    else
                    {
                        IFOR (i, 0, CMacro::MACRO_ACTION_COUNT)
                        {
                            if (param == CMacro::m_MacroAction[i])
                            {
                                macro->SubCode = (MACRO_SUB_CODE)i;
                                break;
                            }
                        }
                    }
                }

                existsMacros.Add(macro);
            }

            g_MacroPointer = (CMacroObject *)existsMacros.m_Items;

            g_MacroManager.WaitingBandageTarget = false;
            g_MacroManager.WaitForTargetTimer = 0;
            g_MacroManager.Execute();

            break;
        }
        case OCT_MOVE_PAPERDOLL:
        {
            uint serial = ReadUInt32BE();
            int x = ReadInt32BE();
            int y = ReadInt32BE();

            CGump *gump = g_GumpManager.UpdateContent(serial, 0, GT_PAPERDOLL);

            if (gump != NULL)
            {
                if (gump->Minimized)
                {
                    gump->MinimizedX = x;
                    gump->MinimizedY = y;
                }
                else
                {
                    gump->SetX(x);
                    gump->SetY(y);
                }
            }

            break;
        }
        case OCT_USE_ABILITY:
        {
            CGumpAbility::OnAbilityUse(ReadUInt8() % 2);

            break;
        }
        default:
            break;
    }
}

PACKET_HANDLER(PacketsList)
{
    WISPFUN_DEBUG("c150_f102");
    if (g_World == NULL)
        return;

    int count = ReadUInt16BE();

    IFOR (i, 0, count)
    {
        uchar id = ReadUInt8();

        if (id == 0xF3)
            HandleUpdateItemSA();
        else
        {
            LOG("Unknown packet ID=0x%02X in packet 0xF7!!!\n", id);
            break;
        }
    }
}

PACKET_HANDLER(MovePlayer)
{
    WISPFUN_DEBUG("c150_f103");
    if (g_World == NULL)
        return;

    uchar direction = ReadUInt8();
    g_Player->Walk((Direction)(direction & 0x7), direction & 0x80);
}

PACKET_HANDLER(Pathfinding)
{
    WISPFUN_DEBUG("c150_f104");
    if (g_World == NULL)
        return;

    ushort x = ReadInt16BE();
    ushort y = ReadInt16BE();
    ushort z = ReadInt16BE();
    g_PathFinder.WalkTo(x, y, z, 0);
}

void CPacketManager::SetCachedGumpCoords(uint id, int x, int y)
{
    std::unordered_map<uint, GumpCoords>::iterator found = m_GumpsCoordsCache.find(id);

    if (found != m_GumpsCoordsCache.end())
    {
        found->second.X = x;
        found->second.Y = y;
    }
    else
    {
        m_GumpsCoordsCache[id] = GumpCoords{ x, y };
    }
}

PACKET_HANDLER(BoatMoving)
{
    WISPFUN_DEBUG("c150_f105");

    uint boatSerial = ReadUInt32BE();

    CGameObject *boat = g_World->FindWorldObject(boatSerial);
    if (boat == NULL)
        return;

    uchar boatSpeed = ReadUInt8();
    uchar movingDirection = ReadUInt8();
    uchar facingDirection = ReadUInt8();
    ushort boatX = ReadUInt16BE();
    ushort boatY = ReadUInt16BE();
    ushort boatZ = ReadUInt16BE();
    ushort boatObjectsCount = ReadUInt16BE();

    g_World->UpdateMulti(
        boatSerial,
        boat->Graphic,
        0,
        boat->Count,
        boatX,
        boatY,
        boatZ,
        facingDirection,
        boat->Color,
        boat->GetFlags());

    for (ushort i = 0; i < boatObjectsCount; i++)
    {
        uint boatObjectSerial = ReadUInt32BE();
        ushort boatObjectX = ReadUInt16BE();
        ushort boatObjectY = ReadUInt16BE();
        ushort boatObjectZ = ReadUInt16BE();

        g_World->MoveObject(boatObjectSerial, boatObjectX, boatObjectY, boatObjectZ);
    }
}
//----------------------------------------------------------------------------------
PACKET_HANDLER(OnUltimaLiveCommand)
{
	WISPFUN_DEBUG("c150_f903");
	Move(10);
	//live map streaming, this is work in progress
	uchar command = ReadUInt8();//commands
	if (command == 0xFF)//hash query - crc check
	{
		Move(-11);//we return to the block number contained in packet...
		uint32_t blocknum = ReadUInt32BE();//byte 003 to 006 - central block number for the query (block that player is standing in)
		Move(7);//sequence number is not used at the moment...also the command type can be skipped...
		uint8_t mapNumber = ReadUInt8();//byte 14 - map file index number
		if (g_MapManager.GetActualMap() == mapNumber)
		{
			CMapBlock *block = g_MapManager.GetBlock(blocknum);
			/*if (block != NULL)
			{
				unsigned char* pData = new uint8_t[192];
				block->Block
				uint16_t crc = 0;
				uint8_t* pBlockData = readLandBlock(mapNumber, blockNumber);
				uint32_t staticsLength = 0;

				uint8_t* pStaticsData = m_pFileManager->readStaticsBlock(mapNumber, blockNumber, staticsLength);

				if (pBlockData != NULL)
				{
					crc = fletcher16(pBlockData, pStaticsData, staticsLength);
					delete pBlockData;
				}

				if (pStaticsData != NULL)
				{
					delete pStaticsData;
				}
			}*/
		}
	}
	else if (command == 0x00)//update statics
	{

	}
	else if (command == 0xF1) //processes
	{

	}
	else if (command == 0x01)//Update Map Definitions
	{
		/*Move(-7);
		int count = ReadInt32BE();
		Move(4);
		int newNumMaps = (count * 7) / 9;
		for (int i = 0; i < newNumMaps; i++)
		{
			int offset = 15 + (i * 9);
			uint8_t mapNumber = ReadUInt8(); //iteration byte 000         -  map file index number
			uint16_t width = ReadUInt16BE();   //iteration byte 001 to 002  -  map width
			uint16_t height = ReadUInt16BE();  //iteration byte 003 to 004  -  map height
			uint16_t wrapX = ReadUInt16BE();   //iteration byte 005 to 006  -  wrap around dimension X
			uint16_t wrapY = ReadUInt16BE();   //iteration byte 007 to 008  -  wrap around dimension Y
			if ((width > 0) && (height > 0) && (wrapX > 0) && (wrapY > 0))
			{
				g_MapSize[i].Width = width;
				g_MapBlockSize[i].Width = width / 8;
				g_MapSize[i].Height = height;
				g_MapBlockSize[i].Height = height / 8;
				
			}
#ifdef _DEBUG
			else
			{
				LOG("Received bad definition for map %i\n", i);
			}
#endif
		}*/
	}
	else if (command == 0x02)//Live Login Complete
	{
		//we should send confirmation and prepare the directory where we will place our new modified statics...
	}
	else if (command == 0xF0)//extended commands
	{
		
	}
}
//----------------------------------------------------------------------------------
