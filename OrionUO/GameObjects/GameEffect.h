

#ifndef GAMEEFFECT_H
#define GAMEEFFECT_H

class CGameEffect : public CRenderWorldObject
{
public:
    EFFECT_TYPE EffectType = EF_MOVING;

    uint DestSerial = 0;

    ushort DestX = 0;

    ushort DestY = 0;

    char DestZ = 0;

    uchar Speed = 0;

    uint Duration = 0;

    bool FixedDirection = false;

    bool Explode = false;

    uint RenderMode = 0;

    int AnimIndex = 0;

    int Increment = 0;

    uint LastChangeFrameTime = 0;

    CGameEffect();
    virtual ~CGameEffect();

    virtual void Draw(int x, int y);

    ushort GetCurrentGraphic();

    ushort CalculateCurrentGraphic();

    void ApplyRenderMode();

    void RemoveRenderMode();

    bool IsEffectObject() { return true; }

    virtual void Update(class CGameObject *parent);
};

#endif
