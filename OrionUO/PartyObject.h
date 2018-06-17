

#ifndef PARTYOBJECT_H
#define PARTYOBJECT_H

class CPartyObject
{
public:
    uint Serial = 0;

    CPartyObject();
    ~CPartyObject() {}
    class CGameCharacter *Character{ NULL };
    string GetName(int index);
};

#endif
