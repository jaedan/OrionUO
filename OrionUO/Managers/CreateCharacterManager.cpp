

#include "stdafx.h"

CCreateCharacterManager g_CreateCharacterManager;

const CC_HAIR_STYLE CCreateCharacterManager::m_HumanMaleHairStyleTable[HUMAN_MALE_HAIR_COUNT] = {
    { 0, 0, "NONE" },
    { 0x0753, 0x203B, "Short" },
    { 0x0754, 0x203C, "Long" },
    { 0x0757, 0x203D, "Pony Tail" },
    { 0x0755, 0x2044, "Mohawk" },
    { 0x074F, 0x2045, "Pageboy" },
    { 0x0752, 0x204A, "Topknot" },
    { 0x0751, 0x2047, "Curly" },
    { 0x0758, 0x2048, "Receding" },
    { 0x074E, 0x2049, "2 Tails" }
};

const CC_HAIR_STYLE CCreateCharacterManager::m_HumanFemaleHairStyleTable[HUMAN_FEMALE_HAIR_COUNT] =
    { { 0, 0, "NONE" },
      { 0x0737, 0x203B, "Short" },
      { 0x0732, 0x203C, "Long" },
      { 0x0735, 0x203D, "Pony Tail" },
      { 0x0733, 0x2044, "Mohawk" },
      { 0x0734, 0x2045, "Pageboy" },
      { 0x0730, 0x204A, "Topknot" },
      { 0x072F, 0x2047, "Curly" },
      { 0x0736, 0x2048, "Receding" },
      { 0x072C, 0x2049, "2 Tails" },
      { 0x0731, 0x2046, "Buns" } };

const CC_HAIR_STYLE CCreateCharacterManager::m_HumanBeardStyleTable[HUMAN_MALE_FACIAL_HAIR_COUNT] =
    { { 0, 0, "NONE" },
      { 0x0759, 0x2040, "Goatee" },
      { 0x075B, 0x203E, "Long beard" },
      { 0x075D, 0x203F, "Short beard" },
      { 0x075C, 0x2041, "Moustache" },
      { 0x075E, 0x204B, "Short beard/moustache" },
      { 0x075A, 0x204C, "Long beard/moustache" },
      { 0x075F, 0x204D, "Vandyke" } };

const ushort CCreateCharacterManager::m_HumanSkinTone[HUMAN_SKIN_TONE_COUNT] = {
    0x03E9, 0x03F1, 0x03F9, 0x0401, 0x0409, 0x0411, 0x0419, 0x0421, 0x03EA, 0x03F2, 0x03FA,
    0x0402, 0x040A, 0x0412, 0x041A, 0x0421, 0x03EB, 0x03F3, 0x03FB, 0x0403, 0x040B, 0x0413,
    0x041B, 0x0421, 0x03EC, 0x03F4, 0x03FC, 0x0404, 0x040C, 0x0414, 0x041C, 0x0421, 0x03ED,
    0x03F5, 0x03FD, 0x0405, 0x040D, 0x0415, 0x041D, 0x0421, 0x03EE, 0x03F6, 0x03FE, 0x0406,
    0x040E, 0x0416, 0x041E, 0x0421, 0x03EF, 0x03F7, 0x03FF, 0x0407, 0x040F, 0x0417, 0x041F,
    0x0421, 0x03F0, 0x03F8, 0x0400, 0x0408, 0x0410, 0x0418, 0x0420, 0x0421
};

const ushort CCreateCharacterManager::m_HumanHairColor[HUMAN_HAIR_COLOR_COUNT] = {
    0x044D, 0x0455, 0x045D, 0x0465, 0x046D, 0x0475, 0x044E, 0x0456, 0x045E, 0x0466, 0x046E, 0x0476,
    0x044F, 0x0457, 0x045F, 0x0467, 0x046F, 0x0477, 0x0450, 0x0458, 0x0460, 0x0468, 0x0470, 0x0478,
    0x0451, 0x0459, 0x0461, 0x0469, 0x0471, 0x0479, 0x0452, 0x045A, 0x0462, 0x046A, 0x0472, 0x047A,
    0x0453, 0x045B, 0x0463, 0x046B, 0x0473, 0x047B, 0x0454, 0x045C, 0x0464, 0x046C, 0x0474, 0x047C
};

CCreateCharacterManager::CCreateCharacterManager()
{
}

CCreateCharacterManager::~CCreateCharacterManager()
{
}

void CCreateCharacterManager::SetFemale(bool female)
{
    WISPFUN_DEBUG("c140_f1");
    m_Female = female;

    if (HairStyle >= GetCurrentHairCount())
        HairStyle = 0;

    if (!m_Female && BeardStyle >= GetCurrentFacialHairCount())
        BeardStyle = 0;
}

void CCreateCharacterManager::Init()
{
    WISPFUN_DEBUG("c140_f3");
    IFOR (i, 1, HUMAN_MALE_HAIR_COUNT)
        g_Orion.ExecuteGump(m_HumanMaleHairStyleTable[i].GumpID);

    IFOR (i, 1, HUMAN_FEMALE_HAIR_COUNT)
        g_Orion.ExecuteGump(m_HumanFemaleHairStyleTable[i].GumpID);

    IFOR (i, 1, HUMAN_MALE_FACIAL_HAIR_COUNT)
        g_Orion.ExecuteGump(m_HumanBeardStyleTable[i].GumpID);
}

void CCreateCharacterManager::Clear()
{
    WISPFUN_DEBUG("c140_f4");
    m_Female = false;
    HairStyle = 1;
    BeardStyle = 0;

    SkinTone = 0x03EA;
    ShirtColor = 0x0084;
    PantsColor = 0x035F;
    HairColor = 0x044E;
    BeardColor = 0x044E;
}

int CCreateCharacterManager::GetCurrentHairCount()
{
    WISPFUN_DEBUG("c140_f5");

    if (m_Female)
    {
        return HUMAN_FEMALE_HAIR_COUNT;
    }
    else
    {
        return HUMAN_MALE_HAIR_COUNT;
    }
}

int CCreateCharacterManager::GetCurrentFacialHairCount()
{
    WISPFUN_DEBUG("c140_f6");

    if (m_Female)
    {
        return 0;
    }
    else
    {
        return HUMAN_MALE_FACIAL_HAIR_COUNT;
    }
}

ushort CCreateCharacterManager::GetBodyGump()
{
    WISPFUN_DEBUG("c140_f7");

    if (m_Female)
    {
        return 0x0760;
    }
    else
    {
        return 0x0761;
    }
}

ushort CCreateCharacterManager::GetShirtGump()
{
    WISPFUN_DEBUG("c140_f8");

    if (m_Female)
    {
        return 0x0714;
    }
    else
    {
        return 0x0739;
    }
}

ushort CCreateCharacterManager::GetPantsGump()
{
    WISPFUN_DEBUG("c140_f9");

    if (m_Female)
    {
        return 0x0764;
    }
    else
    {
        return 0x0738;
    }
}

ushort CCreateCharacterManager::GetBootsGump()
{
    WISPFUN_DEBUG("c140_f10");

    if (m_Female)
    {
        return 0x0763;
    }
    else
    {
        return 0x0762;
    }
}

pushort CCreateCharacterManager::GetSkinTonePtr()
{
    WISPFUN_DEBUG("c140_f11");

    return (pushort)m_HumanSkinTone;
}

pushort CCreateCharacterManager::GetHairColorPtr()
{
    WISPFUN_DEBUG("c140_f12");

    return (pushort)m_HumanHairColor;
}

CC_HAIR_STYLE CCreateCharacterManager::GetHair(uchar pos) const
{
    WISPFUN_DEBUG("c140_f13");

    if (m_Female)
    {
        if (pos < HUMAN_FEMALE_HAIR_COUNT)
            return m_HumanFemaleHairStyleTable[pos];
    }
    else
    {
        if (pos < HUMAN_MALE_HAIR_COUNT)
            return m_HumanMaleHairStyleTable[pos];
    }

    return m_HumanMaleHairStyleTable[0];
}

CC_HAIR_STYLE CCreateCharacterManager::GetBeard(uchar pos) const
{
    WISPFUN_DEBUG("c140_f14");

    if (pos < HUMAN_MALE_FACIAL_HAIR_COUNT)
        return m_HumanBeardStyleTable[pos];

    return m_HumanBeardStyleTable[0];
}
