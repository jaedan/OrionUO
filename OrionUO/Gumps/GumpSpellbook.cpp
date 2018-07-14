

#include "stdafx.h"

string CGumpSpellbook::m_SpellCircleName[8] = { "First Circle",   "Second Circle", "Third Circle",
                                                "Fourth Circle",  "Fifth Circle",  "Sixth Circle",
                                                "Seventh Circle", "Eighth Circle" };

CGumpSpellbook::CGumpSpellbook(uint serial, int x, int y)
    : CGump(GT_SPELLBOOK, serial, x, y)
{
    Draw2Page = true;
    memset(&m_Spells[0], 0, sizeof(m_Spells));
}

CGumpSpellbook::~CGumpSpellbook()
{
}

void CGumpSpellbook::InitStaticData()
{
    return;

    m_SpellCircleName[0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028384, false, "First Circle");
    m_SpellCircleName[1] =
        g_ClilocManager.Cliloc(g_Language)->GetA(1028385, false, "Second Circle");
    m_SpellCircleName[2] = g_ClilocManager.Cliloc(g_Language)->GetA(1028386, false, "Third Circle");
    m_SpellCircleName[3] =
        g_ClilocManager.Cliloc(g_Language)->GetA(1028387, false, "Fourth Circle");
    m_SpellCircleName[4] = g_ClilocManager.Cliloc(g_Language)->GetA(1028388, false, "Fifth Circle");
    m_SpellCircleName[5] = g_ClilocManager.Cliloc(g_Language)->GetA(1028389, false, "Sixth Circle");
    m_SpellCircleName[6] =
        g_ClilocManager.Cliloc(g_Language)->GetA(1028390, false, "Seventh Circle");
    m_SpellCircleName[7] =
        g_ClilocManager.Cliloc(g_Language)->GetA(1028391, false, "Eighth Circle");

    m_SpellName[0][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028320, false, "Clumsy");
    m_SpellName[1][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028321, false, "Create Food");
    m_SpellName[2][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028322, false, "Feeblemind");
    m_SpellName[3][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028323, false, "Heal");
    m_SpellName[4][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028324, false, "Magic Arrow");
    m_SpellName[5][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028325, false, "Night Sight");
    m_SpellName[6][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028326, false, "Reactive Armor");
    m_SpellName[7][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028327, false, "Weaken");
    m_SpellName[8][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028328, false, "Agility");
    m_SpellName[9][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028329, false, "Cunning");
    m_SpellName[10][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028330, false, "Cure");
    m_SpellName[11][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028331, false, "Harm");
    m_SpellName[12][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028332, false, "Magic Trap");
    m_SpellName[13][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028333, false, "Magic Untrap");
    m_SpellName[14][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028334, false, "Protection");
    m_SpellName[15][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028335, false, "Strength");
    m_SpellName[16][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028336, false, "Bless");
    m_SpellName[17][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028337, false, "Fireball");
    m_SpellName[18][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028338, false, "Magic Lock");
    m_SpellName[19][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028339, false, "Poison");
    m_SpellName[20][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028340, false, "Telekinesis");
    m_SpellName[21][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028341, false, "Teleport");
    m_SpellName[22][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028342, false, "Unlock");
    m_SpellName[23][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028343, false, "Wall of Stone");
    m_SpellName[24][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028344, false, "Arch Cure");
    m_SpellName[25][0] =
        g_ClilocManager.Cliloc(g_Language)->GetA(1028345, false, "Arch Protection");
    m_SpellName[26][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028346, false, "Curse");
    m_SpellName[27][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028347, false, "Fire Field");
    m_SpellName[28][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028348, false, "Greater Heal");
    m_SpellName[29][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028349, false, "Lightning");
    m_SpellName[30][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028350, false, "Mana Drain");
    m_SpellName[31][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028351, false, "Recall");
    m_SpellName[32][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028352, false, "Blade Spirits");
    m_SpellName[33][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028353, false, "Dispel Field");
    m_SpellName[34][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028354, false, "Incognito");
    m_SpellName[35][0] =
        g_ClilocManager.Cliloc(g_Language)->GetA(1028355, false, "Magic Reflection");
    m_SpellName[36][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028356, false, "Mind Blast");
    m_SpellName[37][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028357, false, "Paralyze");
    m_SpellName[38][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028358, false, "Poison Field");
    m_SpellName[39][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028359, false, "Summ. Creature");
    m_SpellName[40][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028360, false, "Dispel");
    m_SpellName[41][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028361, false, "Energy Bolt");
    m_SpellName[42][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028362, false, "Explosion");
    m_SpellName[43][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028363, false, "Invisibility");
    m_SpellName[44][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028364, false, "Mark");
    m_SpellName[45][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028365, false, "Mass Curse");
    m_SpellName[46][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028366, false, "Paralyze Field");
    m_SpellName[47][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028367, false, "Reveal");
    m_SpellName[48][0] =
        g_ClilocManager.Cliloc(g_Language)->GetA(1028368, false, "Chain Lightning");
    m_SpellName[49][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028369, false, "Energy Field");
    m_SpellName[50][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028370, false, "Flame Strike");
    m_SpellName[51][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028371, false, "Gate Travel");
    m_SpellName[52][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028372, false, "Mana Vampire");
    m_SpellName[53][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028373, false, "Mass Dispel");
    m_SpellName[54][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028374, false, "Meteor Swarm");
    m_SpellName[55][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028375, false, "Polymorph");
    m_SpellName[56][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028376, false, "Earthquake");
    m_SpellName[57][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028377, false, "Energy Vortex");
    m_SpellName[58][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028378, false, "Resurrection");
    m_SpellName[59][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028379, false, "Air Elemental");
    m_SpellName[60][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028380, false, "Summon Daemon");
    m_SpellName[61][0] =
        g_ClilocManager.Cliloc(g_Language)->GetA(1028381, false, "Earth Elemental");
    m_SpellName[62][0] = g_ClilocManager.Cliloc(g_Language)->GetA(1028382, false, "Fire Elemental");
    m_SpellName[63][0] =
        g_ClilocManager.Cliloc(g_Language)->GetA(1028383, false, "Water Elemental");
}

void CGumpSpellbook::InitToolTip()
{
    WISPFUN_DEBUG("c127_f2");
    if (Minimized)
    {
        g_ToolTip.Set(L"Double click to maximize book gump");
        return;
    }

    int dictionaryPagesCount = 0;
    int tooltipOffset = 0;

    GetTooltipBookInfo(dictionaryPagesCount, tooltipOffset);

    if (Page >= dictionaryPagesCount)
    {
        uint serial = g_SelectedObject.Serial;

        if (serial >= ID_GSB_SPELL_ICON_LEFT)
        {
            if (serial >= ID_GSB_SPELL_ICON_RIGHT)
                serial -= ID_GSB_SPELL_ICON_RIGHT;
            else
                serial -= ID_GSB_SPELL_ICON_LEFT;

            g_ToolTip.Set(
                g_ClilocManager.Cliloc(g_Language)->GetW(tooltipOffset + serial, true), 150);
        }
    }
}

void CGumpSpellbook::PrepareContent()
{
    WISPFUN_DEBUG("c127_f3");
    int maxSpellsCount = 0;
    int spellsOnPage = 0;
    int dictionaryPagesCount = 0;
    int spellIndexOffset = 0;
    ushort graphic = 0;
    ushort minimizedGraphic = 0;
    ushort iconStartGraphic = 0;

    GetSummaryBookInfo(
        maxSpellsCount,
        dictionaryPagesCount,
        spellsOnPage,
        spellIndexOffset,
        graphic,
        minimizedGraphic,
        iconStartGraphic);

    if (g_PressedObject.LeftGump == this && Page >= dictionaryPagesCount &&
        g_PressedObject.LeftSerial >= ID_GSB_SPELL_ICON_LEFT)
    {
        WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

        if ((abs(offset.X) >= DRAG_PIXEL_RANGE || abs(offset.Y) >= DRAG_PIXEL_RANGE) ||
            (g_MouseManager.LastLeftButtonClickTimer + g_MouseManager.DoubleClickDelay < g_Ticks))
        {
            int index = g_PressedObject.LeftSerial;

            if (index < ID_GSB_SPELL_ICON_RIGHT)
                index -= ID_GSB_SPELL_ICON_LEFT;
            else
                index -= ID_GSB_SPELL_ICON_RIGHT;

            if (iconStartGraphic >= 0x5300 && iconStartGraphic < 0x5500)
                iconStartGraphic += 0x0020;

            g_GumpManager.AddGump(new CGumpSpell(
                index + spellIndexOffset + 1,
                g_MouseManager.Position.X - 20,
                g_MouseManager.Position.Y - 20,
                iconStartGraphic + index));

            g_OrionWindow.EmulateOnLeftMouseButtonDown();
        }
    }

    if (!Minimized && m_LastSpellPointer != NULL)
    {
        spellIndexOffset = 0;

        bool wantVisible = false;
        ushort graphicBookmark = 0x08AD;
        ushort graphicPointer = 0x08AF;
        int wantX = m_LastSpellPointer->GetX();
        int wantY = m_LastSpellPointer->GetY();

        if (Page < dictionaryPagesCount)
        {
            int offs = (spellsOnPage * Page);

            IFOR (j, 0, 2)
            {
                int y = 0;

                IFOR (i, 0, spellsOnPage)
                {
                    if (m_Spells[offs])
                    {
                        if (offs + spellIndexOffset == g_LastSpellIndex - 1)
                        {
                            wantVisible = true;
                            wantY = 52 + y;

                            if ((Page + j) % 2)
                            {
                                graphicBookmark = 0x08AE;
                                graphicPointer = 0x08B0;
                                wantX = 203;
                            }
                            else
                                wantX = 184;

                            offs++;
                            break;
                        }

                        y += 15;
                    }

                    offs++;
                }
            }
        }
        else
        {
            int page = dictionaryPagesCount;

            IFOR (i, 0, maxSpellsCount)
            {
                if (!m_Spells[i])
                    continue;

                if (page == Page || page == (Page + 1))
                {
                    if (i + spellIndexOffset == g_LastSpellIndex - 1)
                    {
                        wantVisible = true;
                        wantY = 40;

                        if (page % 2)
                        {
                            graphicBookmark = 0x08AE;
                            graphicPointer = 0x08B0;
                            wantX = 203;
                        }
                        else
                            wantX = 184;

                        break;
                    }
                }

                page++;
            }
        }

        if (wantVisible != m_LastSpellPointer->Visible || wantX != m_LastSpellPointer->GetX() ||
            wantY != m_LastSpellPointer->GetY())
        {
            m_LastSpellPointer->Visible = wantVisible;
            m_LastSpellPointer->Graphic = graphicPointer;
            m_LastSpellPointer->SetX(wantX);
            m_LastSpellPointer->SetY(wantY);

            m_LastSpellBookmark->Visible = wantVisible;
            m_LastSpellBookmark->Graphic = graphicBookmark;
            m_LastSpellBookmark->SetX(wantX);

            WantRedraw = true;
        }
    }
}

void CGumpSpellbook::GetTooltipBookInfo(int &dictionaryPagesCount, int &tooltipOffset)
{
    WISPFUN_DEBUG("c127_f4");
    int maxSpellsCount = 0;

    maxSpellsCount = SPELLBOOK_SPELLS_COUNT;
    tooltipOffset = 1061290;

    dictionaryPagesCount = (int)ceilf(maxSpellsCount / 8.0f);

    if (dictionaryPagesCount % 2)
        dictionaryPagesCount++;
}

void CGumpSpellbook::GetSummaryBookInfo(
    int &maxSpellsCount,
    int &dictionaryPagesCount,
    int &spellsOnPage,
    int &spellIndexOffset,
    ushort &graphic,
    ushort &minimizedGraphic,
    ushort &iconStartGraphic)
{
    WISPFUN_DEBUG("c127_f5");

    maxSpellsCount = SPELLBOOK_SPELLS_COUNT;
    graphic = 0x08AC;
    minimizedGraphic = 0x08BA;
    iconStartGraphic = 0x08C0;
    spellIndexOffset = 0;

    spellsOnPage = min(maxSpellsCount / 2, 8);

    dictionaryPagesCount = (int)ceilf(maxSpellsCount / 8.0f);

    if (dictionaryPagesCount % 2)
        dictionaryPagesCount++;
}

string CGumpSpellbook::GetSpellName(int offset, string &abbreviature, string &reagents)
{
    WISPFUN_DEBUG("c127_f6");

    abbreviature = m_SpellName[offset][1];
    reagents = m_SpellReagents[offset];
    return m_SpellName[offset][0];
}

void CGumpSpellbook::UpdateContent()
{
    WISPFUN_DEBUG("c127_f8");
    m_Body = NULL;
    m_PrevPage = NULL;
    m_NextPage = NULL;
    m_LastSpellPointer = NULL;
    m_LastSpellBookmark = NULL;
    m_TithingPointsText = NULL;

    Clear();

    Add(new CGUIPage(-1));

    int maxSpellsCount = 0;
    int spellsOnPage = 0;
    int dictionaryPagesCount = 0;
    int spellIndexOffset = 0;
    ushort graphic = 0;
    ushort minimizedGraphic = 0;
    ushort iconStartGraphic = 0;

    GetSummaryBookInfo(
        maxSpellsCount,
        dictionaryPagesCount,
        spellsOnPage,
        spellIndexOffset,
        graphic,
        minimizedGraphic,
        iconStartGraphic);

    if (Minimized)
    {
        m_Body = (CGUIGumppic *)Add(new CGUIGumppic(minimizedGraphic, 0, 0));

        return;
    }

    m_Body = (CGUIGumppic *)Add(new CGUIGumppic(graphic, 0, 0));

    Add(new CGUIHitBox(ID_GSB_BUTTON_MINIMIZE, 6, 100, 16, 16, true));

    m_SpellCount = 0;
    memset(&m_Spells[0], 0, sizeof(m_Spells));

    CGameItem *spellbook = g_World->FindWorldItem(Serial);

    if (spellbook == NULL)
        return;

    QFOR(item, spellbook->m_Items, CGameItem *)
    {
        int currentCount = item->Count;

        if (currentCount > 0 && currentCount <= maxSpellsCount)
        {
            m_Spells[currentCount - 1] = 1;
            m_SpellCount++;
        }
    }

    PageCount = dictionaryPagesCount + m_SpellCount;

    int offs = 0;

    Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_1_2, 0x08B1, 0x08B1, 0x08B1, 58, 175));
    Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_1_2, 0x08B2, 0x08B2, 0x08B2, 93, 175));
    Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_3_4, 0x08B3, 0x08B3, 0x08B3, 130, 175));
    Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_3_4, 0x08B4, 0x08B4, 0x08B4, 164, 175));

    Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_5_6, 0x08B5, 0x08B5, 0x08B5, 227, 175));
    Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_5_6, 0x08B6, 0x08B6, 0x08B6, 260, 175));
    Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_7_8, 0x08B7, 0x08B7, 0x08B7, 297, 175));
    Add(new CGUIButton(ID_GSB_BUTTON_CIRCLE_7_8, 0x08B8, 0x08B8, 0x08B8, 332, 175));

    IFOR (page, 0, dictionaryPagesCount)
    {
        Add(new CGUIPage((int)page));

        int indexX = 106;
        int dataX = 62;
        int y = 0;
        uint spellSerial = ID_GSB_SPELL_ICON_LEFT;

        if (page % 2)
        {
            indexX = 269;
            dataX = 225;
            spellSerial = ID_GSB_SPELL_ICON_RIGHT;
        }

        CGUIText *text = (CGUIText *)Add(new CGUIText(0x0288, indexX, 10));
        text->CreateTextureA(6, "INDEX");

        text = (CGUIText *)Add(new CGUIText(0x0288, dataX, 30));
        text->CreateTextureA(6, m_SpellCircleName[page]);

        IFOR (i, 0, spellsOnPage)
        {
            if (m_Spells[offs])
            {
                CGUIHitBox *box = (CGUIHitBox *)Add(
                    new CGUIHitBox(spellSerial + offs, dataX, 52 + y, 100, 16, true));
                box->MoveOnDrag = true;

                CGUITextEntry *entry = (CGUITextEntry *)Add(new CGUITextEntry(
                    spellSerial + offs, 0x0288, 0, 0, dataX, 52 + y, 0, false, 9));
                string abbreviature;
                string reagents;
                entry->m_Entry.SetText(GetSpellName(offs, abbreviature, reagents));
                entry->CheckOnSerial = true;
                entry->ReadOnly = true;

                y += 15;
            }

            offs++;
        }
    }

    int page = dictionaryPagesCount;

    int topTextY = 10;

    IFOR (i, 0, maxSpellsCount)
    {
        if (!m_Spells[i])
            continue;

        int iconX = 62;
        int topTextX = 87;
        int iconTextX = 112;
        uint iconSerial = ID_GSB_SPELL_ICON_LEFT + (uint)i;

        if (page % 2)
        {
            iconX = 225;
            topTextX = 244;
            iconTextX = 275;
            iconSerial = ID_GSB_SPELL_ICON_RIGHT + (int)i;
        }

        Add(new CGUIPage(page));
        page++;

        string spellAbbreviature = "";
        string reagents = "";
        string spellName = GetSpellName((int)i, spellAbbreviature, reagents);

        CGUIText *text = (CGUIText *)Add(new CGUIText(0x0288, topTextX, topTextY));

        text->CreateTextureA(6, m_SpellCircleName[i / 8]);

        text = (CGUIText *)Add(new CGUIText(0x0288, iconTextX, 34));
        text->CreateTextureA(6, spellName, 80);

        int abbreviatureY = 26;

        if (text->m_Texture.Height < 24)
            abbreviatureY = 31;

        abbreviatureY += text->m_Texture.Height;

        text = (CGUIText *)Add(new CGUIText(0x0288, iconTextX, abbreviatureY));
        text->CreateTextureA(8, spellAbbreviature);

        CGUIGumppic *icon =
            (CGUIGumppic *)Add(new CGUIGumppic(iconStartGraphic + (int)i, iconX, 40));
        icon->Serial = iconSerial;

        Add(new CGUIGumppicTiled(0x0835, iconX, 88, 120, 0));

        text = (CGUIText *)Add(new CGUIText(0x0288, iconX, 92));
        text->CreateTextureA(6, "Reagents:");

        text = (CGUIText *)Add(new CGUIText(0x0288, iconX, 114));
        text->CreateTextureA(9, reagents);
    }

    Add(new CGUIPage(-1));

    m_PrevPage =
        (CGUIButton *)Add(new CGUIButton(ID_GSB_BUTTON_PREV, 0x08BB, 0x08BB, 0x08BB, 50, 8));
    m_PrevPage->Visible = (Page != 0);
    m_NextPage =
        (CGUIButton *)Add(new CGUIButton(ID_GSB_BUTTON_NEXT, 0x08BC, 0x08BC, 0x08BC, 321, 8));
    m_NextPage->Visible = (Page + 2 < PageCount);

    m_LastSpellBookmark = (CGUIGumppic *)Add(new CGUIGumppic(0x08AD, 184, 2));
    m_LastSpellPointer = (CGUIGumppic *)Add(new CGUIGumppic(0x08AF, 184, 52));
}

void CGumpSpellbook::GUMP_BUTTON_EVENT_C
{
    WISPFUN_DEBUG("c127_f9");
    int newPage = -1;

    if (serial == ID_GSB_BUTTON_PREV)
    {
        if (Page > 0)
        {
            newPage = Page - 2;

            if (newPage < 0)
                newPage = 0;
        }
    }
    else if (serial == ID_GSB_BUTTON_NEXT)
    {
        if (Page < PageCount)
        {
            newPage = Page + 2;

            if (newPage >= PageCount)
                newPage = PageCount - 1;
        }
    }
    else if (serial == ID_GSB_BUTTON_CIRCLE_1_2)
        ChangePage(0);
    else if (serial == ID_GSB_BUTTON_CIRCLE_3_4)
        ChangePage(2);
    else if (serial == ID_GSB_BUTTON_CIRCLE_5_6)
        ChangePage(4);
    else if (serial == ID_GSB_BUTTON_CIRCLE_7_8)
        ChangePage(6);
    else if (serial == ID_GSB_BUTTON_MINIMIZE)
    {
        Minimized = true;
        WantUpdateContent = true;
    }
    else if (serial == ID_GSB_LOCK_MOVING)
        LockMoving = !LockMoving;
    else if (serial >= ID_GSB_SPELL_ICON_LEFT)
    {
        int maxSpellsCount = 0;
        int dictionaryPagesCount = 0;
        int spellsOnPage = 0;
        int spellIndexOffset = 0;
        ushort graphic = 0;
        ushort minimizedGraphic = 0;
        ushort iconStartGraphic = 0;

        GetSummaryBookInfo(
            maxSpellsCount,
            dictionaryPagesCount,
            spellsOnPage,
            spellIndexOffset,
            graphic,
            minimizedGraphic,
            iconStartGraphic);

        if (Page < dictionaryPagesCount)
        {
            IFOR (j, 0, 2)
            {
                IFOR (i, 0, spellsOnPage)
                {
                    intptr_t offs = i + (spellsOnPage * (Page + j));

                    if (!m_Spells[offs])
                        continue;

                    if (serial == offs + ID_GSB_SPELL_ICON_LEFT ||
                        serial == offs + ID_GSB_SPELL_ICON_RIGHT)
                    {
                        int c = dictionaryPagesCount;

                        IFOR (k, 0, maxSpellsCount)
                        {
                            if (m_Spells[k])
                            {
                                if (k == offs)
                                    break;

                                c++;
                            }
                        }

                        newPage = c;

                        break;
                    }
                }
            }
        }
    }

    if (newPage > -1 && !g_ClickObject.Enabled)
    {
        if (newPage % 2)
            newPage--;

        g_ClickObject.Init(g_PressedObject.LeftObject, this);
        g_ClickObject.Timer = g_Ticks + g_MouseManager.DoubleClickDelay;
        g_ClickObject.Page = newPage;
    }
}

void CGumpSpellbook::GUMP_TEXT_ENTRY_EVENT_C
{
    WISPFUN_DEBUG("c127_f10");
    if (serial >= ID_GSB_SPELL_ICON_LEFT)
        OnButton(serial);
}

bool CGumpSpellbook::OnLeftMouseButtonDoubleClick()
{
    WISPFUN_DEBUG("c127_f11");
    bool result = false;

    if (Minimized)
    {
        Minimized = false;
        WantUpdateContent = true;

        result = true;
    }
    else
    {
        if (g_PressedObject.LeftSerial == ID_GSB_BUTTON_PREV)
        {
            ChangePage(0);

            WantRedraw = true;

            result = true;
        }
        else if (g_PressedObject.LeftSerial == ID_GSB_BUTTON_NEXT)
        {
            int newPage = PageCount - 1;

            if (newPage % 2)
                newPage--;

            ChangePage(newPage);

            WantRedraw = true;

            result = true;
        }
        else
        {
            if (g_PressedObject.LeftSerial >= ID_GSB_SPELL_ICON_LEFT)
            {
                int spellIndex = g_PressedObject.LeftSerial - ID_GSB_SPELL_ICON_RIGHT + 1;

                if (g_PressedObject.LeftSerial < ID_GSB_SPELL_ICON_RIGHT)
                    spellIndex = g_PressedObject.LeftSerial - ID_GSB_SPELL_ICON_LEFT + 1;

                g_Orion.CastSpell(spellIndex);

                Minimized = true;
                WantUpdateContent = true;

                result = true;
            }
        }
    }

    return result;
}

void CGumpSpellbook::DelayedClick(CRenderObject *obj)
{
    WISPFUN_DEBUG("c127_f12");
    if (obj != NULL)
    {
        ChangePage(g_ClickObject.Page);
        WantRedraw = true;
    }
}

void CGumpSpellbook::ChangePage(int newPage)
{
    WISPFUN_DEBUG("c127_f13");
    Page = newPage;

    m_PrevPage->Visible = (Page != 0);
    m_NextPage->Visible = (Page + 2 < PageCount);

    g_Orion.PlaySoundEffect(0x0055);
}

string CGumpSpellbook::m_SpellName[SPELLBOOK_SPELLS_COUNT][2] = { { "Clumsy", "U J" },
                                                                  { "Create Food", "I M Y" },
                                                                  { "Feeblemind", "R W" },
                                                                  { "Heal", "I M" },
                                                                  { "Magic Arrow", "I P Y" },
                                                                  { "Night Sight", "I L" },
                                                                  { "Reactive Armor", "F S" },
                                                                  { "Weaken", "D M" },
                                                                  { "Agility", "E U" },
                                                                  { "Cunning", "U W" },
                                                                  { "Cure", "A N" },
                                                                  { "Harm", "A M" },
                                                                  { "Magic Trap", "I J" },
                                                                  { "Magic Untrap", "A J" },
                                                                  { "Protection", "U S" },
                                                                  { "Strength", "U M" },
                                                                  { "Bless", "R S" },
                                                                  { "Fireball", "V F" },
                                                                  { "Magic Lock", "A P" },
                                                                  { "Poison", "I N" },
                                                                  { "Telekinesis", "O P Y" },
                                                                  { "Teleport", "R P" },
                                                                  { "Unlock", "E P" },
                                                                  { "Wall of Stone", "I S Y" },
                                                                  { "Arch Cure", "V A N" },
                                                                  { "Arch Protection", "V U S" },
                                                                  { "Curse", "D S" },
                                                                  { "Fire Field", "I F G" },
                                                                  { "Greater Heal", "I V M" },
                                                                  { "Lightning", "P O G" },
                                                                  { "Mana Drain", "O R" },
                                                                  { "Recall", "K O P" },
                                                                  { "Blade Spirits", "I H J Y" },
                                                                  { "Dispel Field", "A G" },
                                                                  { "Incognito", "K I E" },
                                                                  { "Magic Reflection", "I J S" },
                                                                  { "Mind Blast", "P C W" },
                                                                  { "Paralyze", "A E P" },
                                                                  { "Poison Field", "I N G" },
                                                                  { "Summ. Creature", "K X" },
                                                                  { "Dispel", "A O" },
                                                                  { "Energy Bolt", "C P" },
                                                                  { "Explosion", "V O F" },
                                                                  { "Invisibility", "A L X" },
                                                                  { "Mark", "K P Y" },
                                                                  { "Mass Curse", "V D S" },
                                                                  { "Paralyze Field", "I E G" },
                                                                  { "Reveal", "W Q" },
                                                                  { "Chain Lightning", "V O G" },
                                                                  { "Energy Field", "I S G" },
                                                                  { "Flame Strike", "K V F" },
                                                                  { "Gate Travel", "V R P" },
                                                                  { "Mana Vampire", "O S" },
                                                                  { "Mass Dispel", "V A O" },
                                                                  { "Meteor Swarm", "F K D Y" },
                                                                  { "Polymorph", "V Y R" },
                                                                  { "Earthquake", "I V P" },
                                                                  { "Energy Vortex", "V C P" },
                                                                  { "Resurrection", "A C" },
                                                                  { "Air Elemental", "K V X H" },
                                                                  { "Summon Daemon", "K V X C" },
                                                                  { "Earth Elemental", "K V X Y" },
                                                                  { "Fire Elemental", "K V X F" },
                                                                  { "Water Elemental",
                                                                    "K V X A" } };

string CGumpSpellbook::m_SpellReagents[SPELLBOOK_SPELLS_COUNT] = {
    "Bloodmoss\nNightshade",
    "Garlic\nGinseng\nMandrake root",
    "Ginseng\nNightshade",
    "Garlic\nGinseng\nSpiders silk",
    "Sulfurous ash",
    "Sulfurous ash\nSpiders silk",
    "Garlic\nSulfurous ash\nSpiders silk",
    "Garlic\nNightshade",
    "Bloodmoss\nMandrake root",
    "Mandrake root\nNightshade",
    "Garlic\nGinseng",
    "Nightshade\nSpiders silk",
    "Garlic\nSulfurous ash\nSpiders silk",
    "Bloodmoss\nSulfurous ash",
    "Garlic\nGinseng\nSulfurous ash",
    "Mandrake root\nNightshade",
    "Garlic\nMandrake root",
    "Black pearl",
    "Sulfurous ash\nBloodmoss\nGarlic",
    "Nightshade",
    "Bloodmoss\nMandrake root",
    "Bloodmoss\nMandrake root",
    "Bloodmoss\nSulfurous ash",
    "Bloodmoss\nGarlic",
    "Garlic\nGinseng\nMandrake root",
    "Garlic\nGinseng\nMandrake root\nSulfurous ash",
    "Garlic\nNightshade\nSulfurous ash",
    "Black pearl\nSpiders silk\nSulfurous ash",
    "Garlic\nGinseng\nMandrake root\nSpiders silk",
    "Mandrake root\nSulfurous ash",
    "Black pearl\nMandrake root\nSpiders silk",
    "Black pearl\nBloodmoss\nMandrake root",
    "Black pearl\nMandrake root\nNightshade",
    "Garlic\nBlack pearl\nSpiders silk\nSulfurous ash",
    "Bloodmoss\nGarlic\nNightshade",
    "Garlic\nMandrake root\nSpiders silk",
    "Black pearl\nMandrake root\nNightshade\nSulfurous ash",
    "Garlic\nMandrake root\nSpiders silk",
    "Black pearl\nNightshade\nSpiders silk",
    "Bloodmoss\nMandrake root\nSpiders silk",
    "Garlic\nMandrake root\nSulfurous ash",
    "Black pearl\nNightshade",
    "Bloodmoss\nMandrake root",
    "Bloodmoss\nNightshade",
    "Black pearl\nBloodmoss\nMandrake root",
    "Garlic\nMandrake root\nNightshade\nSulfurous ash",
    "Black pearl\nGinseng\nSpiders silk",
    "Bloodmoss\nSulfurous ash",
    "Black pearl\nMandrake root\nBloodmoss\nSulfurous ash",
    "Black pearl\nMandrake root\nSpiders silk\nSulfurous ash",
    "Spiders silk\nSulfurous ash",
    "Black pearl\nMandrake root\nSulfurous ash",
    "Black pearl\nBloodmoss\nMandrake root\nSpiders silk",
    "Black pearl\nGarlic\nMandrake root\nSulfurous ash",
    "Bloodmoss\nSpiders silk\nMandrake root\nSulfurous ash",
    "Bloodmoss\nMandrake root\nSpiders silk",
    "Bloodmoss\nGinseng\nMandrake root\nSulfurous ash",
    "Black pearl\nBloodmoss\nMandrake root\nNightshade",
    "Bloodmoss\nGarlic\nGinseng",
    "Bloodmoss\nMandrake root\nSpiders silk",
    "Bloodmoss\nMandrake root\nSpiders silk\nSulfurous ash",
    "Bloodmoss\nMandrake root\nSpiders silk",
    "Bloodmoss\nMandrake root\nSpiders silk\nSulfurous ash",
    "Bloodmoss\nMandrake root\nSpiders silk"
};
