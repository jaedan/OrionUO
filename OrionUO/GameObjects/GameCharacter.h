

#ifndef GAMECHARACTER_H
#define GAMECHARACTER_H

class CGameCharacter : public CGameObject
{
public:
    short Hits = 0;

    short MaxHits = 0;

    short Mana = 0;

    short MaxMana = 0;

    short Stam = 0;

    short MaxStam = 0;

    bool Female = false;

    RACE_TYPE Race = RT_HUMAN;

    uchar Direction = 0;

    uchar Notoriety = 0;

    bool CanChangeName = false;

    uchar AnimationInterval = 0;

    uchar AnimationFrameCount = 0;

    uchar AnimationRepeatMode = 1;

    uchar AnimationGroup = 0xFF;

    bool AnimationRepeat = false;

    bool AnimationDirection = false;

    bool AnimationFromServer = false;

    uint LastStepSoundTime = 0;

    uint TimeToRandomFidget = 0;

    uchar StepSoundOffset = 0;

    int OffsetX = 0;

    int OffsetY = 0;

    int OffsetZ = 0;

    uint LastStepTime = 0;

    string Title = "";

    uchar HitsPercent = 0;

protected:
    void CorrectAnimationGroup(ushort graphic, ANIMATION_GROUPS group, uchar &animation);

    bool m_Dead = false;

public:
    CGameCharacter(int serial);

    virtual ~CGameCharacter();

    CTextContainer m_DamageTextControl{ CTextContainer(10) };

    deque<CWalkData> m_Steps;

    CGLTextTexture m_HitsTexture{ CGLTextTexture() };

    virtual void UpdateTextCoordinates();

    void UpdateHitsTexture(uchar hits);

    void ProcessGargoyleAnims(int &animGroup);

    int IsSitting();

    virtual void Draw(int x, int y);

    virtual void Select(int x, int y);

    void OnGraphicChange(int direction = 0);

    void ResetAnimationGroup(uchar val);

    void SetRandomFidgetAnimation();

    void SetAnimation(
        uchar id,
        uchar interval = 0,
        uchar frameCount = 0,
        uchar repeatCount = 0,
        bool repeat = false,
        bool frameDirection = false);

    ushort GetMountAnimation();

    uchar GetAnimationGroup(ushort checkGraphic = 0);

    void GetAnimationGroup(ANIMATION_GROUPS group, uchar &animation);

    bool Staying() { return AnimationGroup == 0xFF && m_Steps.empty(); }

    bool TestStepNoChangeDirection(uchar group);

    virtual bool Walking() { return (LastStepTime > (uint)(g_Ticks - WALKING_DELAY)); }

    virtual bool NoIterateAnimIndex()
    {
        return ((LastStepTime > (uint)(g_Ticks - WALKING_DELAY)) && m_Steps.empty());
    }

    void UpdateAnimationInfo(uchar &dir, bool canChange = false);

    bool IsHuman()
    {
        return (
            IN_RANGE(Graphic, 0x0190, 0x0193) || IN_RANGE(Graphic, 0x00B7, 0x00BA) ||
            IN_RANGE(Graphic, 0x025D, 0x0260) || IN_RANGE(Graphic, 0x029A, 0x029B) ||
            IN_RANGE(Graphic, 0x02B6, 0x02B7) || (Graphic == 0x03DB) || (Graphic == 0x03DF) ||
            (Graphic == 0x03E2));
    }

    bool Dead()
    {
        return (IN_RANGE(Graphic, 0x0192, 0x0193) || IN_RANGE(Graphic, 0x025F, 0x0260) ||
                IN_RANGE(Graphic, 0x02B6, 0x02B7)) ||
               m_Dead;
    }

    virtual CGameCharacter *GameCharacterPtr() { return this; }

    virtual CGameItem *FindSecureTradeBox();

    void SetDead(bool &dead);
};

#endif
