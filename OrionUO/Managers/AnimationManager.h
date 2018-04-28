

#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

static const float UPPER_BODY_RATIO = 0.35f;
static const float MID_BODY_RATIO = 0.60f;
static const float LOWER_BODY_RATIO = 0.94f;

struct FRAME_OUTPUT_INFO
{
    int StartX;
    int StartY;
    int EndX;
    int EndY;
};

struct ANIMATION_DIMENSIONS
{
    int Width;
    int Height;
    int CenterX;
    int CenterY;
};
struct UOPFrameData
{
    puchar dataStart;
    short frameId;
    uint pixelDataOffset;
};

class CEquipConvData
{
public:
    ushort Graphic = 0;
    ushort Gump = 0;
    ushort Color = 0;

    CEquipConvData(ushort graphic, ushort gump, ushort color)
        : Graphic(graphic)
        , Gump(gump)
        , Color(color)
    {
    }
    ~CEquipConvData() {}
};

typedef unordered_map<ushort, CEquipConvData> EQUIP_CONV_DATA_MAP;
typedef unordered_map<ushort, EQUIP_CONV_DATA_MAP> EQUIP_CONV_BODY_MAP;

class CAnimationManager : public WISP_DATASTREAM::CDataReader
{
public:
    ushort Color = 0;

    uchar AnimGroup = 0;

    uchar Direction = 0;

    ushort AnimID = 0;

    uchar AnimGroupCount = PAG_ANIMATION_COUNT;

private:
    size_t m_AddressIdx[6];
    size_t m_SizeIdx[6];

    int m_CharacterFrameHeight{ 0 };

    static void PrepareTargetAttackGump(
        CTargetGump &gump, int drawX, int drawY, ushort targetColor, int per, CGameCharacter &obj);

    int m_CharacterFrameStartY{ 0 };

    int m_StartCharacterWaistY{ 0 };

    int m_StartCharacterKneesY{ 0 };

    int m_StartCharacterFeetY{ 0 };

    int m_Sitting{ 0 };

    bool m_UseBlending{ false };

    EQUIP_CONV_BODY_MAP m_EquipConv;

    CEquipConvData *m_EquipConvItem{ false };

    static const int USED_LAYER_COUNT = 23;
    static const int m_UsedLayers[8][USED_LAYER_COUNT];

    vector<std::pair<ushort, uchar>> m_GroupReplaces[2];

    deque<CTextureAnimationDirection *> m_UsedAnimList;

    bool TestPixels(
        class CGameObject *obj, int x, int y, bool mirror, uchar &frameIndex, ushort id = 0x0000);

    void FixSittingDirection(uchar &layerDirection, bool &mirror, int &x, int &y);

    void Draw(class CGameObject *obj, int x, int y, bool mirror, uchar &frameIndex, int id = 0);

    void DrawIntoFrameBuffer(class CGameCharacter *obj, int x, int y);

    bool DrawEquippedLayers(
        bool selection,
        class CGameObject *obj,
        int drawX,
        int drawY,
        bool mirror,
        uchar layerDir,
        uchar animIndex,
        int lightOffset);

    bool IsCovered(int layer, class CGameObject *owner);

    bool m_Transform = false;

    ushort m_CharacterLayerGraphic[25];
    ushort m_CharacterLayerAnimID[25];

    bool TryReadUOPAnimDimins(CTextureAnimationDirection &direction);

    vector<UOPFrameData> ReadUOPFrameDataOffsets();

    void ReadUOPFrameData(
        short &imageCenterX,
        short &imageCenterY,
        short &imageWidth,
        short &imageHeight,
        pushort &palette,
        UOPFrameData &frameData);

    uchar GetObjectNewAnimationType_0(CGameCharacter *obj, ushort action, uchar mode);
    uchar GetObjectNewAnimationType_1_2(CGameCharacter *obj, ushort action, uchar mode);
    uchar GetObjectNewAnimationType_3(CGameCharacter *obj, ushort action, uchar mode);
    uchar GetObjectNewAnimationType_4(CGameCharacter *obj, ushort action, uchar mode);
    uchar GetObjectNewAnimationType_5(CGameCharacter *obj, ushort action, uchar mode);
    uchar GetObjectNewAnimationType_6_14(CGameCharacter *obj, ushort action, uchar mode);
    uchar GetObjectNewAnimationType_7(CGameCharacter *obj, ushort action, uchar mode);
    uchar GetObjectNewAnimationType_8(CGameCharacter *obj, ushort action, uchar mode);
    uchar GetObjectNewAnimationType_9_10(CGameCharacter *obj, ushort action, uchar mode);
    uchar GetObjectNewAnimationType_11(CGameCharacter *obj, ushort action, uchar mode);

    void ReadFrameDimensionData(ANIMATION_DIMENSIONS &result, uchar frameIndex, bool isCorpse);
    void ReadFramesPixelData(CTextureAnimationDirection &direction);

public:
    CAnimationManager();
    ~CAnimationManager();

    CIndexAnimation m_DataIndex[MAX_ANIMATIONS_DATA_INDEX_COUNT];

    void Init(int graphic, size_t addressIdx, size_t sizeIdx)
    {
        m_AddressIdx[graphic] = addressIdx;
        m_SizeIdx[graphic] = sizeIdx;
    }

    EQUIP_CONV_BODY_MAP &GetEquipConv() { return m_EquipConv; }

    void InitIndexReplaces(puint verdata);

    void UpdateAnimationAddressTable();

    void Load(puint verdata);

    void ClearUnusedTextures(uint ticks);

    bool LoadDirectionGroup(CTextureAnimationDirection &direction);

    void GetAnimDirection(uchar &dir, bool &mirror);

    void GetSittingAnimDirection(uchar &dir, bool &mirror, int &x, int &y);

    void DrawCharacter(class CGameCharacter *obj, int x, int y);

    bool CharacterPixelsInXY(class CGameCharacter *obj, int x, int y);

    void DrawCorpse(class CGameItem *obj, int x, int y);

    bool CorpsePixelsInXY(class CGameItem *obj, int x, int y);

    uchar GetDieGroupIndex(ushort id, bool second);

    ANIMATION_GROUPS GetGroupIndex(ushort id);

    bool AnimationExists(ushort graphic, uchar group);

    ANIMATION_DIMENSIONS
    GetAnimationDimensions(uchar frameIndex, ushort id, uchar dir, uchar animGroup, bool isCorpse);

    ANIMATION_DIMENSIONS GetAnimationDimensions(
        class CGameObject *obj,
        uchar frameIndex = 0xFF,
        uchar defaultDirection = 0,
        uchar defaultGroup = 0);

    void CalculateFrameInformation(
        FRAME_OUTPUT_INFO &info, class CGameObject *obj, bool mirror, uchar animIndex);

    struct DRAW_FRAME_INFORMATION
    CollectFrameInformation(class CGameObject *gameObject, bool checkLayers = true);

    uchar GetReplacedObjectAnimation(CGameCharacter *obj, ushort index);

    uchar GetObjectNewAnimation(CGameCharacter *obj, ushort type, ushort action, uchar delay);
};

extern CAnimationManager g_AnimationManager;

#endif
