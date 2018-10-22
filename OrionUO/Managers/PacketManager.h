

#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H

class CPacketManager;
typedef void (CPacketManager::*PACKET_FUNCTION)();

#ifdef _DEBUG
#define ORION_SAVE_ALL_PACKETS 1
#define ORION_SAVE_PACKET 1
#else
#define ORION_SAVE_ALL_PACKETS 0
#define ORION_SAVE_PACKET 0
#endif

#if ORION_SAVE_ALL_PACKETS == 1
#define ORION_IGNORE_PACKET 1
#else
#define ORION_IGNORE_PACKET 0
#endif

enum PACKET_DIRECTION
{
    DIR_SEND = 0,
    DIR_RECV,
    DIR_BOTH
};

class CPacketInfo
{
public:
    bool save;

    const char *Name;

    int Size;

    PACKET_DIRECTION Direction;

    PACKET_FUNCTION Handler;
};

struct GumpCoords
{
    int X;
    int Y;
};

struct HTMLGumpDataInfo
{
    GumpCoords *GumpCoords;
    int Width;
    int Height;
    int TextID;
    int HaveBackground;
    int HaveScrollbar;
    int Color;
    bool IsXMF;
    wstring Args;
};

#define HANDLER_PACKET(name) void Handle##name()

class CPacketManager : public WISP_NETWORK::CPacketReader
{
public:
    string AutoLoginNames = "";
    uint ConfigSerial = 0;

private:
    static CPacketInfo m_Packets[0x100];

    std::unordered_map<uint, GumpCoords> m_GumpsCoordsCache;

    bool AutoLoginNameExists(const string &name);

    void AddHTMLGumps(class CGump *gump, vector<HTMLGumpDataInfo> &list);

    UINT_LIST m_MegaClilocRequests;

    deque<UCHAR_LIST> m_PluginData;

    CRITICAL_SECTION m_CSPluginNetwork;

protected:
    virtual void OnPacket();

    virtual void OnReadFailed();

    HANDLER_PACKET(LoginError);
    HANDLER_PACKET(ServerList);
    HANDLER_PACKET(RelayServer);
    HANDLER_PACKET(CharacterList);
    HANDLER_PACKET(ResendCharacterList);
    HANDLER_PACKET(LoginComplete);
    HANDLER_PACKET(SetTime);
    HANDLER_PACKET(EnterWorld);
    HANDLER_PACKET(UpdateHitpoints);
    HANDLER_PACKET(UpdateMana);
    HANDLER_PACKET(UpdateStamina);
    HANDLER_PACKET(MobileAttributes);
    HANDLER_PACKET(NewHealthbarUpdate);
    HANDLER_PACKET(UpdatePlayer);
    HANDLER_PACKET(CharacterStatus);
    HANDLER_PACKET(UpdateItem);
    HANDLER_PACKET(UpdateItemSA);
    HANDLER_PACKET(UpdateObject);
    HANDLER_PACKET(EquipItem);
    HANDLER_PACKET(UpdateContainedItem);
    HANDLER_PACKET(UpdateContainedItems);
    HANDLER_PACKET(DenyMoveItem);
    HANDLER_PACKET(EndDraggingItem);
    HANDLER_PACKET(DropItemAccepted);
    HANDLER_PACKET(DeleteObject);
    HANDLER_PACKET(UpdateCharacter);
    HANDLER_PACKET(Warmode);
    HANDLER_PACKET(PauseControl);
    HANDLER_PACKET(OpenPaperdoll);
    HANDLER_PACKET(ClientVersion);
    HANDLER_PACKET(Ping);
    HANDLER_PACKET(SetWeather);
    HANDLER_PACKET(PersonalLightLevel);
    HANDLER_PACKET(LightLevel);
    HANDLER_PACKET(EnableLockedFeatures);
    HANDLER_PACKET(OpenContainer);
    HANDLER_PACKET(UpdateSkills);
    HANDLER_PACKET(ExtendedCommand);
    HANDLER_PACKET(DenyWalk);
    HANDLER_PACKET(ConfirmWalk);
    HANDLER_PACKET(OpenUrl);
    HANDLER_PACKET(Target);
    HANDLER_PACKET(Talk);
    HANDLER_PACKET(UnicodeTalk);
    HANDLER_PACKET(ClientTalk);
    HANDLER_PACKET(MultiPlacement);
    HANDLER_PACKET(GraphicEffect);
    HANDLER_PACKET(DeathScreen);
    HANDLER_PACKET(PlaySoundEffect);
    HANDLER_PACKET(PlayMusic);
    HANDLER_PACKET(DragAnimation);
    HANDLER_PACKET(CorpseEquipment);
    HANDLER_PACKET(ASCIIPrompt);
    HANDLER_PACKET(UnicodePrompt);
    HANDLER_PACKET(CharacterAnimation);
    HANDLER_PACKET(NewCharacterAnimation);
    HANDLER_PACKET(DisplayQuestArrow);
    HANDLER_PACKET(ClientViewRange);
    HANDLER_PACKET(KrriosClientSpecial);
    HANDLER_PACKET(AssistVersion);
    HANDLER_PACKET(CharacterListNotification);
    HANDLER_PACKET(ErrorCode);
    HANDLER_PACKET(AttackCharacter);
    HANDLER_PACKET(Season);
    HANDLER_PACKET(DisplayDeath);
    HANDLER_PACKET(OpenChat);
    HANDLER_PACKET(DisplayClilocString);
    HANDLER_PACKET(MegaCliloc);
    HANDLER_PACKET(Damage);
    HANDLER_PACKET(BuffDebuff);
    HANDLER_PACKET(SecureTrading);
    HANDLER_PACKET(TextEntryDialog);
    HANDLER_PACKET(OpenMenu);
    HANDLER_PACKET(OpenGump);
    HANDLER_PACKET(OpenCompressedGump);
    HANDLER_PACKET(DyeData);
    HANDLER_PACKET(DisplayMap);
    HANDLER_PACKET(MapData);
    HANDLER_PACKET(TipWindow);
    HANDLER_PACKET(CharacterProfile);
    HANDLER_PACKET(BulletinBoardData);
    HANDLER_PACKET(OpenBook);
    HANDLER_PACKET(OpenBookNew);
    HANDLER_PACKET(BookData);
    HANDLER_PACKET(BuyList);
    HANDLER_PACKET(SellList);
    HANDLER_PACKET(BuyReply);
    HANDLER_PACKET(Logout);
    HANDLER_PACKET(OPLInfo);
    HANDLER_PACKET(CustomHouse);
    HANDLER_PACKET(OrionMessages);
    HANDLER_PACKET(PacketsList);
    HANDLER_PACKET(MovePlayer);
    HANDLER_PACKET(Pathfinding);
    HANDLER_PACKET(BoatMoving);
    HANDLER_PACKET(OnReceiveSendPacket);

public:
    CPacketManager();
    virtual ~CPacketManager();

    virtual int GetPacketSize(const UCHAR_LIST &packet, int &offsetToSize);

    CPacketInfo GetInfo(uchar buf) const { return m_Packets[buf]; }

    void SendMegaClilocRequests();

    void AddMegaClilocRequest(int serial);

    void SavePluginReceivePacket(puchar buf, int size);

    void ProcessPluginPackets();

    void PluginReceiveHandler(puchar buf, int size);

    void SetCachedGumpCoords(uint id, int x, int y);
};

extern CPacketManager g_PacketManager;

#endif
