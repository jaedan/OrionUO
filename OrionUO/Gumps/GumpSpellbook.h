

#ifndef GUMPSPELLBOOK_H
#define GUMPSPELLBOOK_H

struct SPELL_REQURIES
{
    uchar ManaCost;
    uchar MinSkill;
};

class CGumpSpellbook : public CGump
{
public:
    static const int SPELLBOOK_SPELLS_COUNT = 64;

    static string m_SpellName[SPELLBOOK_SPELLS_COUNT][2];

    static string m_SpellReagents[SPELLBOOK_SPELLS_COUNT];

private:
    static const int ID_GSB_BUTTON_PREV = 1;
    static const int ID_GSB_BUTTON_NEXT = 2;
    static const int ID_GSB_BUTTON_CIRCLE_1_2 = 3;
    static const int ID_GSB_BUTTON_CIRCLE_3_4 = 4;
    static const int ID_GSB_BUTTON_CIRCLE_5_6 = 5;
    static const int ID_GSB_BUTTON_CIRCLE_7_8 = 6;
    static const int ID_GSB_BUTTON_MINIMIZE = 7;
    static const int ID_GSB_LOCK_MOVING = 8;
    static const int ID_GSB_SPELL_ICON_LEFT = 100;
    static const int ID_GSB_SPELL_ICON_RIGHT = 1000;

    static const int MAX_SPELLS_COUNT = 64;

    static string m_SpellCircleName[8];

    int m_SpellCount = 0;
    uchar m_Spells[MAX_SPELLS_COUNT];
    int PageCount = 8;

    CGUIGumppic *m_Body = nullptr;
    CGUIText *m_TithingPointsText = nullptr;

    CGUIButton *m_PrevPage = nullptr;
    CGUIButton *m_NextPage = nullptr;

    CGUIGumppic *m_LastSpellPointer = nullptr;
    CGUIGumppic *m_LastSpellBookmark = nullptr;

    void GetTooltipBookInfo(int &dictionaryPagesCount, int &tooltipOffset);

    void GetSummaryBookInfo(
        int &maxSpellsCount,
        int &dictionaryPagesCount,
        int &spellsOnPage,
        int &spellIndexOffset,
        ushort &graphic,
        ushort &minimizedGraphic,
        ushort &iconStartGraphic);

    string GetSpellName(int offset, string &abbreviature, string &reagents);

public:
    CGumpSpellbook(uint serial, int x, int y);
    virtual ~CGumpSpellbook();

    static void InitStaticData();

    virtual void DelayedClick(CRenderObject *obj);

    void ChangePage(int newPage);

    virtual void InitToolTip();

    virtual void PrepareContent();

    virtual void UpdateContent();

    GUMP_BUTTON_EVENT_H;
    GUMP_TEXT_ENTRY_EVENT_H;

    virtual bool OnLeftMouseButtonDoubleClick();
};

#endif
