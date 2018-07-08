

#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

enum class PlayerMovementState
{
    ANIMATE_IMMEDIATELY = 0,
    ANIMATE_ON_CONFIRM,
};

class CPlayer : public CGameCharacter
{
public:
    short Str = 0;
    short Int = 0;
    short Dex = 0;
    uchar LockStr = 0;
    uchar LockInt = 0;
    uchar LockDex = 0;
    ushort MaxWeight = 0;
    ushort Weight = 0;
    short Armor = 0;
    uint Gold = 0;
    bool Warmode = 0;
    ushort MurderCounts = 0;
    uchar Followers = 0;
    uchar MaxFollowers = 5;
    short MurderCountDecayHoursRemaining = 0;
    short CriminalTimerSecondsRemaining = 0;
    short PlayervsPlayerCooldownSecondsRemaining = 0;
    short BandageTimerSecondsRemaining = 0;
    short MaxPhysicalResistance = 0;
    short MaxFireResistance = 0;
    short MaxColdResistance = 0;
    short MaxPoisonResistance = 0;
    short MaxEnergyResistance = 0;
    short HungerSatisfactionMinutesRemaining = 0;
    short MinDamage = 0;
    short MaxDamage = 0;
    uint TithingPoints = 0;
    short DefenceChance = 0;
    short MaxDefenceChance = 0;
    short AttackChance = 0;
    short WeaponSpeed = 0;
    short WeaponDamage = 0;
    short LowerRegCost = 0;
    short SpellDamage = 0;
    short CastRecovery = 0;
    short CastSpeed = 0;
    short LowerManaCost = 0;

    short OldX = 0;
    short OldY = 0;
    char OldZ = 0;

    uint LastStepRequestTime = 0;
    uint8_t SequenceNumber = 0;
    std::deque<Step> m_RequestedSteps;
    PlayerMovementState m_MovementState = PlayerMovementState::ANIMATE_IMMEDIATELY;

    CPlayer(int serial);

    virtual ~CPlayer();

    bool Walk(Direction direction, bool run);

    void ResetSteps();

    void DenyWalk(uint8_t sequence, Direction direction, uint32_t x, uint32_t y, uint8_t z);

    void ConfirmWalk(uchar sequence);

    void CloseBank();

    class CGameItem *FindBandage();

    bool IsPlayer() { return true; }

    virtual bool Walking() { return (LastStepTime > (uint)(g_Ticks - PLAYER_WALKING_DELAY)); }

    virtual bool NoIterateAnimIndex() { return false; }

    void UpdateAbilities();
};

extern CPlayer *g_Player;

#endif
