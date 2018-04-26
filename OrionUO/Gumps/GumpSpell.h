

#ifndef GUMPSPELL_H
#define GUMPSPELL_H

class CGumpSpell : public CGump
{
public:
    bool BigIcon = false;

private:
    static const int ID_GS_LOCK_MOVING = 1;
    static const int ID_GS_BUTTON_REMOVE_FROM_GROUP = 2;

    void GetTooltipSpellInfo(int &tooltipOffset, int &spellIndexOffset);

protected:
    virtual void CalculateGumpState();

    CGUIButton *m_SpellUnlocker = nullptr;

public:
    CGumpSpell(uint serial, short x, short y, ushort graphic);
    virtual ~CGumpSpell();

    CGUIAlphaBlending *m_Blender = nullptr;

    CGumpSpell *m_GroupNext = nullptr;
    CGumpSpell *m_GroupPrev = nullptr;

    CGumpSpell *GetTopSpell();
    CGumpSpell *GetNearSpell(int &x, int &y);
    bool GetSpellGroupOffset(int &x, int &y);
    void UpdateGroup(int x, int y);
    void AddSpell(CGumpSpell *spell);
    void RemoveFromGroup();

    bool InGroup() { return (m_GroupNext != NULL || m_GroupPrev != NULL); }

    virtual void InitToolTip();

    virtual void PrepareContent();

    GUMP_BUTTON_EVENT_H;

    virtual bool OnLeftMouseButtonDoubleClick();
};

#endif
