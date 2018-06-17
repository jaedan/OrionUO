

#ifndef GAMEEFFECTMOVING_H
#define GAMEEFFECTMOVING_H

class CGameEffectMoving : public CGameEffectDrag
{
public:
    float Angle = 0.0f;

    int OffsetZ = 0;

    CGameEffectMoving();

    virtual ~CGameEffectMoving();

    virtual void Update(class CGameObject *parent);
};

#endif
