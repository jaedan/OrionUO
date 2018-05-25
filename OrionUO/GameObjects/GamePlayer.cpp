// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GamePlayer.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CPlayer* g_Player = NULL;
//----------------------------------------------------------------------------------
CPlayer::CPlayer(int serial)
    : CGameCharacter(serial)
{
    WISPFUN_DEBUG("c21_f1");
    CPacketSkillsRequest(Serial).Send();
}
//---------------------------------------------------------------------------
CPlayer::~CPlayer()
{
    WISPFUN_DEBUG("c21_f2");
}
//---------------------------------------------------------------------------
int CPlayer::GetWalkSpeed(bool run, bool onMount)
{
    WISPFUN_DEBUG("c177_f6");
    bool mounted = (onMount || (g_SpeedMode == CST_FAST_UNMOUNT || g_SpeedMode == CST_FAST_UNMOUNT_AND_CANT_RUN) || Flying());

    return CHARACTER_ANIMATION_DELAY_TABLE[mounted][run];
}
//----------------------------------------------------------------------------------
bool CPlayer::Walk(bool run, uchar direction)
{
    WISPFUN_DEBUG("c177_f7");
    if (LastStepRequestTime > g_Ticks || m_RequestedSteps.size() >= MAX_STEPS_COUNT || g_DeathScreenTimer || g_GameState != GS_GAME || AnimationFromServer)
        return false;

    if (g_SpeedMode >= CST_CANT_RUN)
        run = false;
    else if (!run)
        run = g_ConfigManager.AlwaysRun;

    int x, y;
    char z;
    uchar oldDirection;

    GetEndPosition(x, y, z, oldDirection);

    bool onMount = (FindLayer(OL_MOUNT) != NULL);

    char oldZ = z;
    ushort walkTime = TURN_DELAY;

    if ((oldDirection & 7) == (direction & 7)) //Повернуты куда надо
    {
        uchar newDir = direction;
        int newX = x;
        int newY = y;
        char newZ = z;

        if (!g_PathFinder.CanWalk(newDir, newX, newY, newZ))
            return false;

        if ((direction & 7) != newDir)
            direction = newDir;
        else {
            direction = newDir;
            x = newX;
            y = newY;
            z = newZ;

            walkTime = GetWalkSpeed(run, onMount);
        }
    } else {
        uchar newDir = direction;
        int newX = x;
        int newY = y;
        char newZ = z;

        if (!g_PathFinder.CanWalk(newDir, newX, newY, newZ)) {
            if ((oldDirection & 7) == newDir)
                return false;
        }

        if ((oldDirection & 7) == newDir) {
            x = newX;
            y = newY;
            z = newZ;

            walkTime = GetWalkSpeed(run, onMount);
        }

        direction = newDir;
    }

    CloseBank();

    if (!IsMoving() && !Walking()) {
        SetAnimation(0xFF);
    }

	if (run)
		direction += 0x80;

	Step step;
    step.Direction = direction;
    step.X = x;
    step.Y = y;
    step.Z = z;

	m_RequestedSteps.push_back(step);

    QueueStep(x, y, z, direction);

    CPacketWalkRequest(direction, SequenceNumber).Send();

    if (SequenceNumber == 0xFF)
        SequenceNumber = 1;
    else
        SequenceNumber++;

    g_World->MoveToTop(this);

    static bool lastRun = false;
    static bool lastMount = false;
    static int lastDir = -1;
    static int lastDelta = 0;
    static int lastStepTime = 0;

    //Высчитываем актуальную дельту с помощью разници во времени между прошлым и текущим шагом.
    int nowDelta = 0;

    if (lastDir == direction && lastMount == onMount && lastRun == run) {
        nowDelta = (g_Ticks - lastStepTime) - walkTime + lastDelta;

        if (abs(nowDelta) > 70)
            nowDelta = 0;

        lastDelta = nowDelta;
    } else
        lastDelta = 0;

    lastStepTime = g_Ticks;

    lastRun = run;
    lastMount = onMount;

    lastDir = direction;

    LastStepRequestTime = g_Ticks + walkTime - nowDelta;
    GetAnimationGroup();

    return true;
}
//---------------------------------------------------------------------------
void CPlayer::ResetSteps()
{
    SequenceNumber = 0;
    LastStepRequestTime = 0;
}
//----------------------------------------------------------------------------------
void CPlayer::DenyWalk(uchar sequence, int x, int y, char z)
{
    m_Steps.clear();
	m_RequestedSteps.clear();

    OffsetX = 0;
    OffsetY = 0;
    OffsetZ = 0;

    ResetSteps();

    if (x != -1) {
        SetX(x);
        SetY(y);
        SetZ(z);

        g_RemoveRangeXY.X = x;
        g_RemoveRangeXY.Y = y;

        UOI_PLAYER_XYZ_DATA xyzData = { g_RemoveRangeXY.X, g_RemoveRangeXY.Y, 0 };
        g_PluginManager.WindowProc(g_OrionWindow.Handle, UOMSG_UPDATE_REMOVE_POS, (WPARAM)&xyzData, 0);
    }
}
//----------------------------------------------------------------------------------
void CPlayer::ConfirmWalk(uchar sequence)
{
	Step& step = m_RequestedSteps.front();

	g_RemoveRangeXY.X = step.X;
	g_RemoveRangeXY.Y = step.Y;

	m_RequestedSteps.pop_front();

	UOI_PLAYER_XYZ_DATA xyzData = { g_RemoveRangeXY.X, g_RemoveRangeXY.Y, 0 };
    g_PluginManager.WindowProc(g_OrionWindow.Handle, UOMSG_UPDATE_REMOVE_POS, (WPARAM)&xyzData, 0);
}
//---------------------------------------------------------------------------
void CPlayer::CloseBank()
{
    CGameItem* bank = FindLayer(OL_BANK);

    if (bank != NULL && bank->Opened) {
        bank->Clear();
        bank->Opened = false;

        g_GumpManager.CloseGump(bank->Serial, 0, GT_CONTAINER);
    }
}
//---------------------------------------------------------------------------
/*!
Поиск бинтов в сумке (и подсумках)
@return Ссылка на бинт или NULL
*/
CGameItem* CPlayer::FindBandage()
{
    WISPFUN_DEBUG("c21_f11");
    CGameItem* item = FindLayer(OL_BACKPACK);

    if (item != NULL)
        item = item->FindItem(0x0E21);

    return item;
}
//---------------------------------------------------------------------------
void CPlayer::UpdateAbilities()
{
    WISPFUN_DEBUG("c21_f12");
    ushort equippedGraphic = 0;

    CGameItem* layerObject = FindLayer(OL_1_HAND);

    if (layerObject != NULL) {
        equippedGraphic = layerObject->Graphic;
    } else {
        layerObject = FindLayer(OL_2_HAND);

        if (layerObject != NULL)
            equippedGraphic = layerObject->Graphic;
    }

    g_Ability[0] = 0xFF;
    g_Ability[1] = 0xFF;

    if (equippedGraphic) {
        ushort graphics[2] = { equippedGraphic, 0 };
        ushort imageID = layerObject->GetStaticData()->AnimID;
        int count = 1;

        ushort testGraphic = equippedGraphic - 1;

        if (g_Orion.m_StaticData[testGraphic].AnimID == imageID) {
            graphics[1] = testGraphic;
            count = 2;
        } else {
            testGraphic = equippedGraphic + 1;

            if (g_Orion.m_StaticData[testGraphic].AnimID == imageID) {
                graphics[1] = testGraphic;
                count = 2;
            }
        }

        IFOR(i, 0, count)
        {
            switch (graphics[i]) {
            case 0x0901: // Gargish Cyclone
                g_Ability[0] = AT_MOVING_SHOT;
                g_Ability[1] = AT_INFUSED_THROW;
                break;
            case 0x0902: // Gargish Dagger
                g_Ability[0] = AT_INFECTING;
                g_Ability[1] = AT_SHADOW_STRIKE;
                break;
            case 0x0905: // Glass Staff
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x0906: // serpentstone staff
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x090C: // Glass Sword
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x0DF0:
            case 0x0DF1: // Black Staves
                g_Ability[0] = AT_WHIRLWIND_ATTACK;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x0DF2:
            case 0x0DF3:
            case 0x0DF4:
            case 0x0DF5: // Wands Type A-D
                g_Ability[0] = AT_DISMOUNT;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x0E81:
            case 0x0E82: // Shepherd's Crooks
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x0E85:
            case 0x0E86: // Pickaxes
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x0E87:
            case 0x0E88: // Pitchforks
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x0E89:
            case 0x0E8A: // Quarter Staves
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x0EC2:
            case 0x0EC3: // Cleavers
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_INFECTING;
                break;
            case 0x0EC4:
            case 0x0EC5: // Skinning Knives
                g_Ability[0] = AT_SHADOW_STRIKE;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x0F43:
            case 0x0F44: // Hatchets
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x0F45:
            case 0x0F46: // Double Axes
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x0F47:
            case 0x0F48: // Battle Axes
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x0F49:
            case 0x0F4A: // Axes
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x0F4B:
            case 0x0F4C:
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_WHIRLWIND_ATTACK;
                break;
            case 0x0F4D:
            case 0x0F4E: // Bardiches
                g_Ability[0] = AT_PARALYZING_BLOW;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x0F4F:
            case 0x0F50: // Crossbows
                g_Ability[0] = AT_CONCUSSION_BLOW;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x0F51:
            case 0x0F52: // Daggers
                g_Ability[0] = AT_INFECTING;
                g_Ability[1] = AT_SHADOW_STRIKE;
                break;
            case 0x0F5C:
            case 0x0F5D: // Maces
                g_Ability[0] = AT_CONCUSSION_BLOW;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x0F5E:
            case 0x0F5F: // Broadswords
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_ARMOR_IGNORE;
                break;
            case 0x0F60:
            case 0x0F61: // Longswords
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x0F62:
            case 0x0F63: // Spears
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x0FB5:
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_SHADOW_STRIKE;
                break;
            case 0x13AF:
            case 0x13B0: // War Axes
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_BLEED_ATTACK;
                break;
            case 0x13B1:
            case 0x13B2: // Bows
                g_Ability[0] = AT_PARALYZING_BLOW;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x13B3:
            case 0x13B4: // Clubs
                g_Ability[0] = AT_SHADOW_STRIKE;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x13B7:
            case 0x13B8: // Scimitars
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x13B9:
            case 0x13BA: // Viking Swords
                g_Ability[0] = AT_PARALYZING_BLOW;
                g_Ability[1] = AT_CRUSHING_BLOW;
                break;
            case 0x13FD: // Heavy Crossbows
                g_Ability[0] = AT_MOVING_SHOT;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x13E3: // Smith's Hammers
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_SHADOW_STRIKE;
                break;
            case 0x13F6: // Butcher Knives
                g_Ability[0] = AT_INFECTING;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x13F8: // Gnarled Staves
                g_Ability[0] = AT_CONCUSSION_BLOW;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x13FB: // Large Battle Axes
                g_Ability[0] = AT_WHIRLWIND_ATTACK;
                g_Ability[1] = AT_BLEED_ATTACK;
                break;
            case 0x13FF: // Katana
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_ARMOR_IGNORE;
                break;
            case 0x1401: // Kryss
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_INFECTING;
                break;
            case 0x1402:
            case 0x1403: // Short Spears
                g_Ability[0] = AT_SHADOW_STRIKE;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x1404:
            case 0x1405: // War Forks
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x1406:
            case 0x1407: // War Maces
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_BLEED_ATTACK;
                break;
            case 0x1438:
            case 0x1439: // War Hammers
                g_Ability[0] = AT_WHIRLWIND_ATTACK;
                g_Ability[1] = AT_CRUSHING_BLOW;
                break;
            case 0x143A:
            case 0x143B: // Mauls
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x143C:
            case 0x143D: // Hammer Picks
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x143E:
            case 0x143F: // Halberds
                g_Ability[0] = AT_WHIRLWIND_ATTACK;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x1440:
            case 0x1441: // Cutlasses
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_SHADOW_STRIKE;
                break;
            case 0x1442:
            case 0x1443: // Two Handed Axes
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_SHADOW_STRIKE;
                break;
            case 0x26BA: // Scythes
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x26BB: // Bone Harvesters
                g_Ability[0] = AT_PARALYZING_BLOW;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x26BC: // Scepters
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x26BD: // Bladed Staves
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x26BE: // Pikes
                g_Ability[0] = AT_PARALYZING_BLOW;
                g_Ability[1] = AT_INFECTING;
                break;
            case 0x26BF: // Double Bladed Staff
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_INFECTING;
                break;
            case 0x26C0: // Lances
                g_Ability[0] = AT_DISMOUNT;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x26C1: // Crescent Blades
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x26C2: // Composite Bows
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_MOVING_SHOT;
                break;
            case 0x26C3: // Repeating Crossbows
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_MOVING_SHOT;
                break;
            case 0x26C4: // also Scythes
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x26C5: // also Bone Harvesters
                g_Ability[0] = AT_PARALYZING_BLOW;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x26C6: // also Scepters
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x26C7: // also Bladed Staves
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x26C8: // also Pikes
                g_Ability[0] = AT_PARALYZING_BLOW;
                g_Ability[1] = AT_INFECTING;
                break;
            case 0x26C9: // also Double Bladed Staff
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_INFECTING;
                break;
            case 0x26CA: // also Lances
                g_Ability[0] = AT_DISMOUNT;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x26CB: // also Crescent Blades
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x26CC: // also Composite Bows
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_MOVING_SHOT;
                break;
            case 0x26CD: // also Repeating Crossbows
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_MOVING_SHOT;
                break;
            case 0x27A2: // No-Dachi
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_RIDING_SWIPE;
                break;
            case 0x27A3: // Tessen
                g_Ability[0] = AT_FEINT;
                g_Ability[1] = AT_BLOCK;
                break;
            case 0x27A4: // Wakizashi
                g_Ability[0] = AT_FRENZIED_WHIRLWIND;
                g_Ability[1] = AT_DOUBLE_STRIKE;
                break;
            case 0x27A5: // Yumi
                g_Ability[0] = AT_ARMOR_PIERCE;
                g_Ability[1] = AT_DOUBLE_SHOT;
                break;
            case 0x27A6: // Tetsubo
                g_Ability[0] = AT_FRENZIED_WHIRLWIND;
                g_Ability[1] = AT_CRUSHING_BLOW;
                break;
            case 0x27A7: // Lajatang
                g_Ability[0] = AT_DEFENSE_MASTERY;
                g_Ability[1] = AT_FRENZIED_WHIRLWIND;
                break;
            case 0x27A8: // Bokuto
                g_Ability[0] = AT_FEINT;
                g_Ability[1] = AT_NERVE_STRIKE;
                break;
            case 0x27A9: // Daisho
                g_Ability[0] = AT_FEINT;
                g_Ability[1] = AT_DOUBLE_STRIKE;
                break;
            case 0x27AA: // Fukya
                g_Ability[0] = AT_DISARM;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x27AB: // Tekagi
                g_Ability[0] = AT_DUAL_WIELD;
                g_Ability[1] = AT_TALON_STRIKE;
                break;
            case 0x27AD: // Kama
                g_Ability[0] = AT_WHIRLWIND_ATTACK;
                g_Ability[1] = AT_DEFENSE_MASTERY;
                break;
            case 0x27AE: // Nunchaku
                g_Ability[0] = AT_BLOCK;
                g_Ability[1] = AT_FEINT;
                break;
            case 0x27AF: // Sai
                g_Ability[0] = AT_BLOCK;
                g_Ability[1] = AT_ARMOR_PIERCE;
                break;
            case 0x27ED: // also No-Dachi
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_RIDING_SWIPE;
                break;
            case 0x27EE: // also Tessen
                g_Ability[0] = AT_FEINT;
                g_Ability[1] = AT_BLOCK;
                break;
            case 0x27EF: // also Wakizashi
                g_Ability[0] = AT_FRENZIED_WHIRLWIND;
                g_Ability[1] = AT_DOUBLE_STRIKE;
                break;
            case 0x27F0: // also Yumi
                g_Ability[0] = AT_ARMOR_PIERCE;
                g_Ability[1] = AT_DOUBLE_SHOT;
                break;
            case 0x27F1: // also Tetsubo
                g_Ability[0] = AT_FRENZIED_WHIRLWIND;
                g_Ability[1] = AT_CRUSHING_BLOW;
                break;
            case 0x27F2: // also Lajatang
                g_Ability[0] = AT_DEFENSE_MASTERY;
                g_Ability[1] = AT_FRENZIED_WHIRLWIND;
                break;
            case 0x27F3: // also Bokuto
                g_Ability[0] = AT_FEINT;
                g_Ability[1] = AT_NERVE_STRIKE;
                break;
            case 0x27F4: // also Daisho
                g_Ability[0] = AT_FEINT;
                g_Ability[1] = AT_DOUBLE_STRIKE;
                break;
            case 0x27F5: // also Fukya
                g_Ability[0] = AT_DISARM;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x27F6: // also Tekagi
                g_Ability[0] = AT_DUAL_WIELD;
                g_Ability[1] = AT_TALON_STRIKE;
                break;
            case 0x27F8: // Kama
                g_Ability[0] = AT_WHIRLWIND_ATTACK;
                g_Ability[1] = AT_DEFENSE_MASTERY;
                break;
            case 0x27F9: // Nunchaku
                g_Ability[0] = AT_BLOCK;
                g_Ability[1] = AT_FEINT;
                break;
            case 0x27FA: // Sai
                g_Ability[0] = AT_BLOCK;
                g_Ability[1] = AT_ARMOR_PIERCE;
                break;
            case 0x2D1E: // Elven Composite Longbows
                g_Ability[0] = AT_FORCE_ARROW;
                g_Ability[1] = AT_SERPENT_ARROW;
                break;
            case 0x2D1F: // Magical Shortbows
                g_Ability[0] = AT_LIGHTNING_ARROW;
                g_Ability[1] = AT_PSYCHIC_ATTACK;
                break;
            case 0x2D20: // Elven Spellblades
                g_Ability[0] = AT_PSYCHIC_ATTACK;
                g_Ability[1] = AT_BLEED_ATTACK;
                break;
            case 0x2D21: // Assassin Spikes
                g_Ability[0] = AT_INFECTING;
                g_Ability[1] = AT_SHADOW_STRIKE;
                break;
            case 0x2D22: // Leafblades
                g_Ability[0] = AT_FEINT;
                g_Ability[1] = AT_ARMOR_IGNORE;
                break;
            case 0x2D23: // War Cleavers
                g_Ability[0] = AT_DISARM;
                g_Ability[1] = AT_BLADEWEAVE;
                break;
            case 0x2D24: // Diamond Maces
                g_Ability[0] = AT_CONCUSSION_BLOW;
                g_Ability[1] = AT_CRUSHING_BLOW;
                break;
            case 0x2D25: // Wild Staves
                g_Ability[0] = AT_BLOCK;
                g_Ability[1] = AT_FORCE_OF_NATURE;
                break;
            case 0x2D26: // Rune Blades
                g_Ability[0] = AT_DISARM;
                g_Ability[1] = AT_BLADEWEAVE;
                break;
            case 0x2D27: // Radiant Scimitars
                g_Ability[0] = AT_WHIRLWIND_ATTACK;
                g_Ability[1] = AT_BLADEWEAVE;
                break;
            case 0x2D28: // Ornate Axes
                g_Ability[0] = AT_DISARM;
                g_Ability[1] = AT_CRUSHING_BLOW;
                break;
            case 0x2D29: // Elven Machetes
                g_Ability[0] = AT_DEFENSE_MASTERY;
                g_Ability[1] = AT_BLADEWEAVE;
                break;
            case 0x2D2A: // also Elven Composite Longbows
                g_Ability[0] = AT_FORCE_ARROW;
                g_Ability[1] = AT_SERPENT_ARROW;
                break;
            case 0x2D2B: // also Magical Shortbows
                g_Ability[0] = AT_LIGHTNING_ARROW;
                g_Ability[1] = AT_PSYCHIC_ATTACK;
                break;
            case 0x2D2C: // also Elven Spellblades
                g_Ability[0] = AT_PSYCHIC_ATTACK;
                g_Ability[1] = AT_BLEED_ATTACK;
                break;
            case 0x2D2D: // also Assassin Spikes
                g_Ability[0] = AT_INFECTING;
                g_Ability[1] = AT_SHADOW_STRIKE;
                break;
            case 0x2D2E: // also Leafblades
                g_Ability[0] = AT_FEINT;
                g_Ability[1] = AT_ARMOR_IGNORE;
                break;
            case 0x2D2F: // also War Cleavers
                g_Ability[0] = AT_DISARM;
                g_Ability[1] = AT_BLADEWEAVE;
                break;
            case 0x2D30: // also Diamond Maces
                g_Ability[0] = AT_CONCUSSION_BLOW;
                g_Ability[1] = AT_CRUSHING_BLOW;
                break;
            case 0x2D31: // also Wild Staves
                g_Ability[0] = AT_BLOCK;
                g_Ability[1] = AT_FORCE_OF_NATURE;
                break;
            case 0x2D32: // also Rune Blades
                g_Ability[0] = AT_DISARM;
                g_Ability[1] = AT_BLADEWEAVE;
                break;
            case 0x2D33: // also Radiant Scimitars
                g_Ability[0] = AT_WHIRLWIND_ATTACK;
                g_Ability[1] = AT_BLADEWEAVE;
                break;
            case 0x2D34: // also Ornate Axes
                g_Ability[0] = AT_DISARM;
                g_Ability[1] = AT_CRUSHING_BLOW;
                break;
            case 0x2D35: // also Elven Machetes
                g_Ability[0] = AT_DEFENSE_MASTERY;
                g_Ability[1] = AT_BLADEWEAVE;
                break;
            case 0x4067: // Boomerang
                g_Ability[0] = AT_MYSTIC_ARC;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x4068: // Dual Short Axes
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_INFECTING;
                break;
            case 0x406B: // Soul Glaive
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x406C: // Cyclone
                g_Ability[0] = AT_MOVING_SHOT;
                g_Ability[1] = AT_INFUSED_THROW;
                break;
            case 0x406D: // Dual Pointed Spear
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x406E: // Disc Mace
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x4072: // Blood Blade
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x4074: // Dread Sword
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x4075: // Gargish Talwar
                g_Ability[0] = AT_WHIRLWIND_ATTACK;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x4076: // Shortblade
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x48AE: // Gargish Cleaver
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_INFECTING;
                break;
            case 0x48B0: // Gargish Battle Axe
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x48B2: // Gargish Axe
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x48B4: // Gargish Bardiche
                g_Ability[0] = AT_PARALYZING_BLOW;
                g_Ability[1] = AT_DISMOUNT;
                break;
            case 0x48B6: // Gargish Butcher Knife
                g_Ability[0] = AT_INFECTING;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x48B8: // Gargish Gnarled Staff
                g_Ability[0] = AT_CONCUSSION_BLOW;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x48BA: // Gargish Katana
                g_Ability[0] = AT_DOUBLE_STRIKE;
                g_Ability[1] = AT_ARMOR_IGNORE;
                break;
            case 0x48BC: // Gargish Kryss
                g_Ability[0] = AT_ARMOR_IGNORE;
                g_Ability[1] = AT_INFECTING;
                break;
            case 0x48BE: // Gargish War Fork
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_DISARM;
                break;
            case 0x48CA: // Gargish Lance
                g_Ability[0] = AT_DISMOUNT;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x48C0: // Gargish War Hammer
                g_Ability[0] = AT_WHIRLWIND_ATTACK;
                g_Ability[1] = AT_CRUSHING_BLOW;
                break;
            case 0x48C2: // Gargish Maul
                g_Ability[0] = AT_CRUSHING_BLOW;
                g_Ability[1] = AT_CONCUSSION_BLOW;
                break;
            case 0x48C4: // Gargish Scyte
                g_Ability[0] = AT_BLEED_ATTACK;
                g_Ability[1] = AT_PARALYZING_BLOW;
                break;
            case 0x48C6: // Gargish Bone Harvester
                g_Ability[0] = AT_PARALYZING_BLOW;
                g_Ability[1] = AT_MORTAL_STRIKE;
                break;
            case 0x48C8: // Gargish Pike
                g_Ability[0] = AT_PARALYZING_BLOW;
                g_Ability[1] = AT_INFECTING;
                break;
            case 0x48CD: // Gargish Tessen
                g_Ability[0] = AT_FEINT;
                g_Ability[1] = AT_BLOCK;
                break;
            case 0x48CE: // Gargish Tekagi
                g_Ability[0] = AT_DUAL_WIELD;
                g_Ability[1] = AT_TALON_STRIKE;
                break;
            case 0x48D0: // Gargish Daisho
                g_Ability[0] = AT_FEINT;
                g_Ability[1] = AT_DOUBLE_STRIKE;
                break;
            default:
                break;
            }

            if (g_Ability[0] != 0xFF)
                break;
        }
    }

    if (g_Ability[0] == 0xFF) {
        g_Ability[0] = AT_DISARM;
        g_Ability[1] = AT_PARALYZING_BLOW;
    }

    g_GumpManager.UpdateContent(0, 0, GT_ABILITY);
    g_GumpManager.UpdateContent(1, 0, GT_ABILITY);

    g_GumpManager.UpdateContent(0, 0, GT_COMBAT_BOOK);
}
//---------------------------------------------------------------------------
