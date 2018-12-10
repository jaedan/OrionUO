#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

enum class WarModeState
{
    Peace = 0,
    War = 1,
    Toggle = 2
};

class CPlayer : public CGameCharacter
{
private:
    uint m_NextAllowedStepTime = 0;
    uint8_t m_SequenceNumber = 0;
    uint8_t m_StepsOutstanding = 0;
    uint16_t m_MovementX = 0;
    uint16_t m_MovementY = 0;
    uint8_t m_MovementZ = 0;
    Direction m_MovementDir = Direction::DIR_NORTH;
    int m_Resynchronizing = 0;

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
    WarModeState WarMode = WarModeState::Peace;
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

    CPlayer(int serial);
    virtual ~CPlayer();

    std::deque<WarModeState> m_WarModeRequests;
    void RequestWarMode(WarModeState state);
    bool SetWarMode(WarModeState state);

    bool Walk(Direction direction, bool run);
    void ConfirmWalk();
    virtual void ForcePosition(int x, int y, char z, Direction dir) override;
    void Resynchronize();

    void CloseBank();

    class CGameItem *FindBandage();

    bool IsPlayer() { return true; }

    virtual bool Walking() { return (LastStepTime > (uint)(g_Ticks - PLAYER_WALKING_DELAY)); }

    virtual bool NoIterateAnimIndex() { return false; }

    void UpdateAbilities();
};

extern CPlayer *g_Player;

#endif
