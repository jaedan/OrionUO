

#ifndef GAMEEFFECTDRAG_H
#define GAMEEFFECTDRAG_H

class CGameEffectDrag : public CGameEffect
{
public:
    int OffsetX = 0;

    int OffsetY = 0;

    uint LastMoveTime = 0;

    uchar MoveDelay = 20;

    CGameEffectDrag();

    virtual ~CGameEffectDrag();

    virtual void Update(class CGameObject *parent);
};

#endif
