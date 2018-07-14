

#include "stdafx.h"

int CGumpStatusbar::m_StatusbarDefaultWidth = 154;
int CGumpStatusbar::m_StatusbarDefaultHeight = 59;

CGumpStatusbar::CGumpStatusbar(uint serial, short x, short y, bool minimized)
    : CGump(GT_STATUSBAR, serial, x, y)
{
    WISPFUN_DEBUG("c128_f1");
    if (minimized)
    {
        Minimized = true;
        MinimizedX = x;
        MinimizedY = y;
    }

    if (!g_ConfigManager.DisableNewTargetSystem && Serial == g_NewTargetSystem.Serial)
        g_GumpManager.UpdateGump(Serial, 0, GT_TARGET_SYSTEM);

    m_Locker.Serial = ID_GSB_LOCK_MOVING;
}

CGumpStatusbar::~CGumpStatusbar()
{
    WISPFUN_DEBUG("c128_f2");
    if (g_ConnectionManager.Connected() && g_PacketManager.GetClientVersion() >= CV_200 &&
        g_World != NULL && g_World->FindWorldObject(Serial) != NULL)
        CPacketCloseStatusbarGump(Serial).Send();

    RemoveFromGroup();
}

void CGumpStatusbar::InitToolTip()
{
    WISPFUN_DEBUG("c128_f3");
    uint id = g_SelectedObject.Serial;

    if (id > 0 && id < ID_GSB_TEXT_MAX)
    {
        static const wstring tooltip[ID_GSB_TEXT_MAX] = {
            L"Minimize the statusbar gump",
            L"",
            L"",
            L"",
            L"Remove bar from group",
            L"Open buff window",
            L"Change strength state",
            L"Change dexterity state",
            L"Change intelligence state",
            L"Strength",
            L"Dexterity",
            L"Intelligence",
            L"Sex",
            L"Armor",
            L"Hit Points",
            L"Stamina",
            L"Mana",
            L"Carrying Capacity",
            L"Hunger Satisfaction Remaining",
            L"Murder Count",
            L"Damage",
            L"Gold",
            L"Followers",
            L"Hours Remaining Until Murder Counts Removed",
            L"Criminal Time Remaining",
            L"Player Vs. Player Time Remaining",
            L"Bandage Timer",
        };

        const wstring &text = tooltip[id - 1];

        if (text.length())
            g_ToolTip.Set(text, 80);
    }
}

CGumpStatusbar *CGumpStatusbar::GetTopStatusbar()
{
    WISPFUN_DEBUG("c128_f4");
    if (!InGroup())
        return NULL;

    if (m_GroupPrev == NULL)
        return this;

    CGumpStatusbar *gump = m_GroupPrev;

    while (gump != NULL && gump->m_GroupPrev != NULL)
        gump = gump->m_GroupPrev;

    return gump;
}

CGumpStatusbar *CGumpStatusbar::GetNearStatusbar(int &x, int &y)
{
    WISPFUN_DEBUG("c128_f5");
    if (InGroup() || !Minimized)
        return NULL;

    int gumpWidth = m_StatusbarDefaultWidth;
    int gumpHeight = m_StatusbarDefaultHeight;

    int rangeX = gumpWidth / 2;
    int rangeY = gumpHeight / 2;

    const int rangeOffsetX = 60;
    const int rangeOffsetY = 24;

    CGump *gump = (CGump *)g_GumpManager.m_Items;

    while (gump != NULL)
    {
        if (gump != this && gump->GumpType == GT_STATUSBAR && gump->Minimized)
        {
            int gumpX = gump->MinimizedX;
            int offsetX = abs(x - gumpX);
            int passed = 0;

            if (x >= gumpX && x <= (gumpX + gumpWidth))
                passed = 2;
            else if (offsetX < rangeOffsetX)
                passed = 1;
            else
            {
                offsetX = abs(x - (gumpX + gumpWidth));

                if (offsetX < rangeOffsetX)
                    passed = -1;
                else if (x >= (gumpX - rangeX) && x <= (gumpX + gumpWidth + rangeX))
                    passed = 2;
            }

            int gumpY = gump->MinimizedY;

            if (abs(passed) == 1)
            {
                if (y < (gumpY - rangeY) || y > (gumpY + gumpHeight + rangeY))
                    passed = 0;
            }
            else if (passed == 2)
            {
                int offsetY = abs(y - gumpY);

                if (offsetY < rangeOffsetY)
                    passed = 2;
                else
                {
                    offsetY = abs(y - (gumpY + gumpHeight));

                    if (offsetY < rangeOffsetY)
                        passed = -2;
                    else
                        passed = 0;
                }
            }

            if (passed)
            {
                int testX = gumpX;
                int testY = gumpY;

                switch (passed)
                {
                    case -2:
                    {
                        testY += gumpHeight;
                        break;
                    }
                    case -1:
                    {
                        testX += gumpWidth;
                        break;
                    }
                    case 1:
                    {
                        testX -= gumpWidth;
                        break;
                    }
                    case 2:
                    {
                        testY -= gumpHeight;
                        break;
                    }
                    default:
                        break;
                }

                CGump *testGump = (CGump *)g_GumpManager.m_Items;

                while (testGump != NULL)
                {
                    if (testGump != this && testGump->GumpType == GT_STATUSBAR &&
                        testGump->Minimized)
                    {
                        if (testGump->MinimizedX == testX && testGump->MinimizedY == testY)
                            break;
                    }

                    testGump = (CGump *)testGump->m_Next;
                }

                if (testGump == NULL)
                {
                    x = testX;
                    y = testY;

                    break;
                }
            }
        }

        gump = (CGump *)gump->m_Next;
    }

    return (CGumpStatusbar *)gump;
}

bool CGumpStatusbar::GetStatusbarGroupOffset(int &x, int &y)
{
    WISPFUN_DEBUG("c128_f6");
    if (InGroup() && Minimized && g_MouseManager.LeftButtonPressed &&
        g_PressedObject.LeftGump != NULL &&
        (g_PressedObject.LeftObject == NULL ||
         (g_PressedObject.LeftObject->IsGUI() &&
          ((CBaseGUI *)g_PressedObject.LeftObject)->MoveOnDrag)))
    {
        CGumpStatusbar *gump = GetTopStatusbar();

        while (gump != NULL)
        {
            if (gump != this && g_PressedObject.LeftGump == gump && gump->CanBeMoved())
            {
                WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

                x += offset.X;
                y += offset.Y;

                return true;
            }

            gump = gump->m_GroupNext;
        }
    }

    return false;
}

void CGumpStatusbar::UpdateGroup(int x, int y)
{
    WISPFUN_DEBUG("c128_f7");
    if (!InGroup())
        return;

    CGumpStatusbar *gump = GetTopStatusbar();

    while (gump != NULL)
    {
        if (gump != this)
        {
            gump->MinimizedX += x;
            gump->MinimizedY += y;

            g_GumpManager.MoveToBack(gump);
        }

        gump = gump->m_GroupNext;
    }

    g_GumpManager.MoveToBack(this);
}

void CGumpStatusbar::AddStatusbar(CGumpStatusbar *bar)
{
    WISPFUN_DEBUG("c128_f8");
    if (m_GroupNext == NULL)
    {
        m_GroupNext = bar;
        bar->m_GroupPrev = this;
        bar->m_GroupNext = NULL;
    }
    else
    {
        CGumpStatusbar *gump = m_GroupNext;

        while (gump != NULL && gump->m_GroupNext != NULL)
            gump = gump->m_GroupNext;

        gump->m_GroupNext = bar;
        bar->m_GroupPrev = gump;
        bar->m_GroupNext = NULL;
    }

    if (bar->m_StatusbarUnlocker != NULL)
    {
        bar->m_StatusbarUnlocker->Visible = bar->InGroup();
        bar->WantRedraw = true;
    }

    if (m_StatusbarUnlocker != NULL)
    {
        m_StatusbarUnlocker->Visible = InGroup();

        WantRedraw = true;
    }
}

void CGumpStatusbar::RemoveFromGroup()
{
    WISPFUN_DEBUG("c128_f9");
    if (m_GroupNext != NULL)
    {
        m_GroupNext->WantRedraw = true;
        m_GroupNext->m_GroupPrev = m_GroupPrev;

        if (m_GroupNext->m_StatusbarUnlocker != NULL)
            m_GroupNext->m_StatusbarUnlocker->Visible = m_GroupNext->InGroup();
    }

    if (m_GroupPrev != NULL)
    {
        m_GroupPrev->WantRedraw = true;
        m_GroupPrev->m_GroupNext = m_GroupNext;

        if (m_GroupPrev->m_StatusbarUnlocker != NULL)
            m_GroupPrev->m_StatusbarUnlocker->Visible = m_GroupPrev->InGroup();
    }

    m_GroupNext = NULL;
    m_GroupPrev = NULL;

    if (m_StatusbarUnlocker != NULL)
    {
        m_StatusbarUnlocker->Visible = InGroup();
        WantRedraw = true;
    }
}

void CGumpStatusbar::CalculateGumpState()
{
    WISPFUN_DEBUG("c128_f10");
    CGump::CalculateGumpState();

    if (g_GumpMovingOffset.X || g_GumpMovingOffset.Y)
    {
        if (g_Target.IsTargeting())
            g_GeneratedMouseDown = true;

        if (!InGroup())
        {
            int testX = g_MouseManager.Position.X;
            int testY = g_MouseManager.Position.Y;

            if (GetNearStatusbar(testX, testY) != NULL)
            {
                g_GumpTranslate.X = (float)testX;
                g_GumpTranslate.Y = (float)testY;
            }
        }
    }
    else if (InGroup())
    {
        int x = (int)g_GumpTranslate.X;
        int y = (int)g_GumpTranslate.Y;

        GetStatusbarGroupOffset(x, y);

        g_GumpTranslate.X = (float)x;
        g_GumpTranslate.Y = (float)y;
    }
}

void CGumpStatusbar::PrepareContent()
{
    WISPFUN_DEBUG("c128_f11");
    if (m_HitsBody != NULL)
    {
        CGameCharacter *obj = g_World->FindWorldCharacter(Serial);

        if (obj == NULL && m_HitsBody->Color != 0x0386)
        {
            m_Body->Color = 0x0386;
            m_HitsBody->Color = 0x0386;
            m_Entry->Color = 0x0386;
            m_Entry->ReadOnly = true;
        }
        else if (obj != NULL && m_HitsBody->Color == 0x0386)
        {
            m_Body->Color = g_ConfigManager.GetColorByNotoriety(obj->Notoriety);
            m_HitsBody->Color = 0;

            if (obj->CanChangeName)
            {
                m_Entry->ReadOnly = false;
                m_Entry->Color = 0x000E;
            }
            else
            {
                m_Entry->ReadOnly = true;
                m_Entry->Color = 0x0386;
            }
        }
    }
}

void CGumpStatusbar::UpdateContent()
{
    WISPFUN_DEBUG("c128_f12");
    Clear();
    m_StatusbarUnlocker = NULL;
    m_Body = NULL;
    m_HitsBody = NULL;
    m_Entry = NULL;
    bool useUOPGumps = g_FileManager.UseUOPGumps;
    CGUIText *text = NULL;

    if (Serial == g_PlayerSerial)
    {
        if (!Minimized)
        {
            static const int ROW_0_Y = 26;
            static const int ROW_1_Y = 56;
            static const int ROW_2_Y = 86;
            static const int ROW_3_Y = 116;
            static const int ROW_HEIGHT = 24;
            static const int ROW_PADDING = 2;

            static const int LOCKER_COLUMN_X = 10;
            static const int LOCKER_COLUMN_WIDTH = 10;

            static const int COLUMN_1_X = 20;
            static const int COLUMN_1_WIDTH = 80;
            static const int COLUMN_1_ICON_WIDTH = 35;

            static const int COLUMN_2_X = 100;
            static const int COLUMN_2_WIDTH = 60;
            static const int COLUMN_2_ICON_WIDTH = 20;

            static const int COLUMN_3_X = 160;
            static const int COLUMN_3_WIDTH = 60;
            static const int COLUMN_3_ICON_WIDTH = 30;

            static const int COLUMN_4_X = 220;
            static const int COLUMN_4_WIDTH = 80;
            static const int COLUMN_4_ICON_WIDTH = 35;

            static const int COLUMN_5_X = 300;
            static const int COLUMN_5_WIDTH = 80;
            static const int COLUMN_5_ICON_WIDTH = 55;

            Add(new CGUIGumppic(0x2A6C, 0, 0));

            Add(new CGUIGumppic(0x0805, 34, 12));
            int percent = CalculatePercents(g_Player->MaxHits, g_Player->Hits, 109);
            if (percent > 0)
            {
                ushort gumpid = 0x0806;
                if (g_Player->Poisoned())
                {
                    gumpid = 0x0808;
                }
                else if (g_Player->YellowHits())
                {
                    gumpid = 0x0809;
                }

                Add(new CGUIGumppicTiled(gumpid, 34, 12, percent, 0));
            }

            Add(new CGUIGumppic(0x0805, 34, 25));
            percent = CalculatePercents(g_Player->MaxMana, g_Player->Mana, 109);
            if (percent > 0)
            {
                Add(new CGUIGumppicTiled(0x0806, 34, 25, percent, 0));
            }

            Add(new CGUIGumppic(0x0805, 34, 38));
            percent = CalculatePercents(g_Player->MaxStam, g_Player->Stam, 109);
            if (percent > 0)
            {
                Add(new CGUIGumppicTiled(0x0806, 34, 38, percent, 0));
            }

            if (g_Player->GetName().length())
            {
                text = (CGUIText *)Add(new CGUIText(0x0386, 100, 10));
                text->CreateTextureA(1, g_Player->GetName(), 320, TS_CENTER);
            }

            Add(new CGUIButton(ID_GSB_BUFF_GUMP, 0x7538, 0x7538, 0x7538, 40, 50));

            uchar status = g_Player->LockStr;
            ushort gumpID = 0x0984;
            if (status == 1)
            {
                gumpID = 0x0986;
            }
            else if (status == 2)
            {
                gumpID = 0x082C;
            }
            Add(new CGUIButton(
                ID_GSB_BUFF_LOCKER_STR,
                gumpID,
                gumpID,
                gumpID,
                LOCKER_COLUMN_X,
                ROW_1_Y + ROW_HEIGHT - ROW_PADDING));
            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_1_X + COLUMN_1_ICON_WIDTH,
                ROW_1_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(1, std::to_string(g_Player->Str));
            Add(new CGUIHitBox(ID_GSB_TEXT_STR, COLUMN_1_X, ROW_1_Y, COLUMN_1_WIDTH, ROW_HEIGHT));

            status = g_Player->LockDex;
            gumpID = 0x0984;
            if (status == 1)
            {
                gumpID = 0x0986;
            }
            else if (status == 2)
            {
                gumpID = 0x082C;
            }
            Add(new CGUIButton(
                ID_GSB_BUFF_LOCKER_DEX,
                gumpID,
                gumpID,
                gumpID,
                LOCKER_COLUMN_X,
                ROW_2_Y + ROW_HEIGHT - ROW_PADDING));
            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_1_X + COLUMN_1_ICON_WIDTH,
                ROW_2_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(1, std::to_string(g_Player->Dex));
            Add(new CGUIHitBox(ID_GSB_TEXT_DEX, COLUMN_1_X, ROW_2_Y, COLUMN_1_WIDTH, ROW_HEIGHT));

            status = g_Player->LockInt;
            gumpID = 0x0984;
            if (status == 1)
            {
                gumpID = 0x0986;
            }
            else if (status == 2)
            {
                gumpID = 0x082C;
            }
            Add(new CGUIButton(
                ID_GSB_BUFF_LOCKER_INT,
                gumpID,
                gumpID,
                gumpID,
                LOCKER_COLUMN_X,
                ROW_3_Y + ROW_HEIGHT - ROW_PADDING));
            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_1_X + COLUMN_1_ICON_WIDTH,
                ROW_3_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(1, std::to_string(g_Player->Int));
            Add(new CGUIHitBox(ID_GSB_TEXT_INT, COLUMN_1_X, ROW_3_Y, COLUMN_1_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_2_X + COLUMN_2_ICON_WIDTH,
                ROW_1_Y + (ROW_HEIGHT / 2) - ROW_PADDING));
            text->CreateTextureA(1, std::to_string(g_Player->Hits), 40, TS_CENTER);
            Add(new CGUILine(
                COLUMN_2_X + COLUMN_2_ICON_WIDTH,
                ROW_1_Y + 23,
                COLUMN_2_X + COLUMN_2_ICON_WIDTH + 30,
                ROW_1_Y + 23,
                0xFF383838));
            text = (CGUIText *)Add(new CGUIText(
                0x0386, COLUMN_2_X + COLUMN_2_ICON_WIDTH, ROW_1_Y + ROW_HEIGHT - ROW_PADDING));
            text->CreateTextureA(1, std::to_string(g_Player->MaxHits), 40, TS_CENTER);
            Add(new CGUIHitBox(ID_GSB_TEXT_HITS, COLUMN_2_X, ROW_1_Y, COLUMN_2_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_2_X + COLUMN_2_ICON_WIDTH,
                ROW_2_Y + (ROW_HEIGHT / 2) - ROW_PADDING));
            text->CreateTextureA(1, std::to_string(g_Player->Stam), 40, TS_CENTER);
            Add(new CGUILine(
                COLUMN_2_X + COLUMN_2_ICON_WIDTH,
                ROW_2_Y + 23,
                COLUMN_2_X + COLUMN_2_ICON_WIDTH + 30,
                ROW_2_Y + 23,
                0xFF383838));
            text = (CGUIText *)Add(new CGUIText(
                0x0386, COLUMN_2_X + COLUMN_2_ICON_WIDTH, ROW_2_Y + ROW_HEIGHT - ROW_PADDING));
            text->CreateTextureA(1, std::to_string(g_Player->MaxStam), 40, TS_CENTER);
            Add(new CGUIHitBox(ID_GSB_TEXT_STAM, COLUMN_2_X, ROW_2_Y, COLUMN_2_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_2_X + COLUMN_2_ICON_WIDTH,
                ROW_3_Y + (ROW_HEIGHT / 2) - ROW_PADDING));
            text->CreateTextureA(1, std::to_string(g_Player->Mana), 40, TS_CENTER);
            Add(new CGUILine(
                COLUMN_2_X + COLUMN_2_ICON_WIDTH,
                ROW_3_Y + 23,
                COLUMN_2_X + COLUMN_2_ICON_WIDTH + 30,
                ROW_3_Y + 23,
                0xFF383838));
            text = (CGUIText *)Add(new CGUIText(
                0x0386, COLUMN_2_X + COLUMN_2_ICON_WIDTH, ROW_3_Y + ROW_HEIGHT - ROW_PADDING));
            text->CreateTextureA(1, std::to_string(g_Player->MaxMana), 40, TS_CENTER);
            Add(new CGUIHitBox(ID_GSB_TEXT_MANA, COLUMN_2_X, ROW_3_Y, COLUMN_2_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_3_X + COLUMN_3_ICON_WIDTH,
                ROW_1_Y + (ROW_HEIGHT / 2) - ROW_PADDING));
            text->CreateTextureA(1, std::to_string(g_Player->Followers), 40, TS_CENTER);
            Add(new CGUILine(
                COLUMN_3_X + COLUMN_3_ICON_WIDTH,
                ROW_1_Y + 23,
                COLUMN_3_X + COLUMN_3_ICON_WIDTH + 25,
                ROW_1_Y + 23,
                0xFF383838));
            text = (CGUIText *)Add(new CGUIText(
                0x0386, COLUMN_3_X + COLUMN_3_ICON_WIDTH, ROW_1_Y + ROW_HEIGHT - ROW_PADDING));
            text->CreateTextureA(1, std::to_string(g_Player->MaxFollowers), 40, TS_CENTER);
            Add(new CGUIHitBox(
                ID_GSB_TEXT_FOLLOWERS, COLUMN_3_X, ROW_1_Y, COLUMN_3_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_3_X + COLUMN_3_ICON_WIDTH,
                ROW_2_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(1, std::to_string(g_Player->Armor));
            Add(new CGUIHitBox(ID_GSB_TEXT_ARMOR, COLUMN_3_X, ROW_2_Y, COLUMN_3_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_3_X + COLUMN_3_ICON_WIDTH,
                ROW_3_Y + (ROW_HEIGHT / 2) - ROW_PADDING));
            text->CreateTextureA(1, std::to_string(g_Player->Weight), 40, TS_CENTER);
            Add(new CGUILine(
                COLUMN_3_X + COLUMN_3_ICON_WIDTH,
                ROW_3_Y + 23,
                COLUMN_3_X + COLUMN_3_ICON_WIDTH + 25,
                ROW_3_Y + 23,
                0xFF383838));
            text = (CGUIText *)Add(new CGUIText(
                0x0386, COLUMN_3_X + COLUMN_3_ICON_WIDTH, ROW_3_Y + ROW_HEIGHT - ROW_PADDING));
            text->CreateTextureA(1, std::to_string(g_Player->MaxWeight), 40, TS_CENTER);
            Add(new CGUIHitBox(
                ID_GSB_TEXT_WEIGHT, COLUMN_3_X, ROW_3_Y, COLUMN_3_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_4_X + COLUMN_4_ICON_WIDTH + 20,
                ROW_0_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(1, std::to_string(g_Player->HungerSatisfactionMinutesRemaining));
            Add(new CGUIHitBox(
                ID_GSB_TEXT_SATIETY, COLUMN_4_X, ROW_0_Y, COLUMN_4_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_4_X + COLUMN_4_ICON_WIDTH,
                ROW_1_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(1, std::to_string(g_Player->MurderCounts));
            Add(new CGUIHitBox(
                ID_GSB_TEXT_DAMAGE, COLUMN_4_X, ROW_1_Y, COLUMN_4_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_4_X + COLUMN_4_ICON_WIDTH,
                ROW_2_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(
                1, std::to_string(g_Player->MinDamage) + "-" + std::to_string(g_Player->MaxDamage));
            Add(new CGUIHitBox(
                ID_GSB_TEXT_DAMAGE, COLUMN_4_X, ROW_2_Y, COLUMN_4_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_4_X + COLUMN_4_ICON_WIDTH,
                ROW_3_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(1, std::to_string(g_Player->Gold));
            Add(new CGUIHitBox(ID_GSB_TEXT_GOLD, COLUMN_4_X, ROW_3_Y, COLUMN_4_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_5_X + COLUMN_5_ICON_WIDTH,
                ROW_0_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(1, std::to_string(g_Player->CriminalTimerSecondsRemaining));
            Add(new CGUIHitBox(
                ID_GSB_TEXT_SATIETY, COLUMN_5_X, ROW_0_Y, COLUMN_5_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_5_X + COLUMN_5_ICON_WIDTH,
                ROW_1_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(1, std::to_string(g_Player->MurderCountDecayHoursRemaining));
            Add(new CGUIHitBox(
                ID_GSB_TEXT_SATIETY, COLUMN_5_X, ROW_1_Y, COLUMN_5_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_5_X + COLUMN_5_ICON_WIDTH,
                ROW_2_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(
                1, std::to_string(g_Player->PlayervsPlayerCooldownSecondsRemaining));
            Add(new CGUIHitBox(
                ID_GSB_TEXT_SATIETY, COLUMN_5_X, ROW_2_Y, COLUMN_5_WIDTH, ROW_HEIGHT));

            text = (CGUIText *)Add(new CGUIText(
                0x0386,
                COLUMN_5_X + COLUMN_5_ICON_WIDTH,
                ROW_3_Y + ROW_HEIGHT - (3 * ROW_PADDING)));
            text->CreateTextureA(1, std::to_string(g_Player->BandageTimerSecondsRemaining));
            Add(new CGUIHitBox(
                ID_GSB_TEXT_SATIETY, COLUMN_5_X, ROW_3_Y, COLUMN_5_WIDTH, ROW_HEIGHT));

            Add(new CGUIHitBox(
                ID_GSB_MINIMIZE,
                COLUMN_5_X + COLUMN_5_WIDTH + 5,
                ROW_3_Y + ROW_HEIGHT + 5,
                20,
                20,
                true));
        }
        else
        {
            if (g_Party.Leader != 0 && !g_ConfigManager.GetOriginalPartyStatusbar())
            {
                CGUIGumppic *bodyGump = (CGUIGumppic *)Add(new CGUIGumppic(0x0803, 0, 0));
                bodyGump->SelectOnly = true;

                text = (CGUIText *)Add(new CGUIText(0x0386, 16, -2));
                text->CreateTextureA(3, "[* SELF *]");

                Add(new CGUIButton(ID_GSB_BUTTON_HEAL_1, 0x0938, 0x093A, 0x093A, 16, 20));
                Add(new CGUIButton(ID_GSB_BUTTON_HEAL_2, 0x0939, 0x093A, 0x093A, 16, 33));

                int color = 0;
                if (g_Player->Poisoned())
                    color = 63;
                else if (g_Player->YellowHits())
                    color = 353;

                Add(new CGUIGumppic(0x0028, 34, 20));

                int per = CalculatePercents(g_Player->MaxHits, g_Player->Hits, 96);
                Add(new CGUIShader(&g_ColorizerShader, true));
                if (per > 0)
                {
                    CGUIGumppic *gumppic =
                        (CGUIGumppic *)Add(new CGUIGumppicTiled(0x0029, 34, 20, per, 0));
                    gumppic->Color = color;
                }

                Add(new CGUIGumppic(0x0028, 34, 33));

                per = CalculatePercents(g_Player->MaxMana, g_Player->Mana, 96);

                if (per > 0)
                {
                    CGUIGumppic *gumppic =
                        (CGUIGumppic *)Add(new CGUIGumppicTiled(0x0029, 34, 33, per, 0));
                    gumppic->Color = 0x0482;
                }

                Add(new CGUIGumppic(0x0028, 34, 45));

                per = CalculatePercents(g_Player->MaxStam, g_Player->Stam, 96);

                if (per > 0)
                {
                    CGUIGumppic *gumppic =
                        (CGUIGumppic *)Add(new CGUIGumppicTiled(0x0029, 34, 45, per, 0));
                    gumppic->Color = 0x0075;
                }

                Add(new CGUIShader(&g_ColorizerShader, false));
            }
            else
            {
                if (g_Player->WarMode == WarModeState::War)
                    Add(new CGUIGumppic(0x0807, 0, 0));
                else
                    Add(new CGUIGumppic(0x0803, 0, 0));

                Add(new CGUIGumppic(0x0805, 34, 12));

                int per = CalculatePercents(g_Player->MaxHits, g_Player->Hits, 109);

                if (per > 0)
                {
                    ushort gumpid = 0x0806;
                    if (g_Player->Poisoned())
                        gumpid = 0x0808;
                    else if (g_Player->YellowHits())
                        gumpid = 0x0809;

                    Add(new CGUIGumppicTiled(gumpid, 34, 12, per, 0));
                }

                Add(new CGUIGumppic(0x0805, 34, 25));

                per = CalculatePercents(g_Player->MaxMana, g_Player->Mana, 109);

                if (per > 0)
                    Add(new CGUIGumppicTiled(0x0806, 34, 25, per, 0));

                Add(new CGUIGumppic(0x0805, 34, 38));

                per = CalculatePercents(g_Player->MaxStam, g_Player->Stam, 109);

                if (per > 0)
                    Add(new CGUIGumppicTiled(0x0806, 34, 38, per, 0));
            }

            m_StatusbarUnlocker = (CGUIButton *)Add(
                new CGUIButton(ID_GSB_BUTTON_REMOVE_FROM_GROUP, 0x082C, 0x082C, 0x082C, 136, 24));
            m_StatusbarUnlocker->CheckPolygon = true;
            m_StatusbarUnlocker->Visible = InGroup();
        }
    }
    else
    {
        if (g_Party.Contains(Serial) && !g_ConfigManager.GetOriginalPartyStatusbar())
        {
            IFOR (i, 0, 10)
            {
                if (g_Party.Member[i].Serial == Serial)
                {
                    CPartyObject &member = g_Party.Member[i];
                    if (member.Character == NULL)
                    {
                        member.Character = g_World->FindWorldCharacter(member.Serial);

                        if (member.Character == NULL)
                            break;
                    }

                    CGUIGumppic *bodyGump = (CGUIGumppic *)Add(new CGUIGumppic(0x0803, 0, 0));
                    bodyGump->SelectOnly = true;

                    string memberName = member.GetName((int)i);
                    bool outofRange = false;
                    if (member.Character->RemovedFromRender())
                    {
                        outofRange = true;
                    }
                    ushort textColor =
                        outofRange ?
                            912 :
                            g_ConfigManager.GetColorByNotoriety(member.Character->Notoriety);

                    Add(new CGUIHitBox(ID_GSB_TEXT_FIELD, 16, -2, 109, 16));
                    m_Entry = (CGUITextEntry *)Add(new CGUITextEntry(
                        ID_GSB_TEXT_FIELD,
                        textColor,
                        textColor,
                        textColor,
                        16,
                        -2,
                        150,
                        false,
                        1,
                        TS_LEFT,
                        UOFONT_FIXED));
                    m_Entry->m_Entry.SetText(memberName);
                    m_Entry->CheckOnSerial = true;

                    if (member.Character->CanChangeName)
                        m_Entry->ReadOnly = false;
                    else
                        m_Entry->ReadOnly = true;

                    if (!outofRange)
                    {
                        Add(new CGUIButton(ID_GSB_BUTTON_HEAL_1, 0x0938, 0x093A, 0x093A, 16, 20));
                        Add(new CGUIButton(ID_GSB_BUTTON_HEAL_2, 0x0939, 0x093A, 0x093A, 16, 33));
                    }
                    int color = 0;
                    if (member.Character->Poisoned())
                        color = 63;
                    else if (member.Character->YellowHits())
                        color = 353;

                    if (outofRange)
                        color = 912;
                    Add(new CGUIShader(&g_ColorizerShader, true));

                    CGUIGumppic *g = new CGUIGumppic(0x0028, 34, 20);
                    g->Color = outofRange ? color : 0;
                    Add(g);

                    int per =
                        CalculatePercents(member.Character->MaxHits, member.Character->Hits, 96);

                    if (per > 0)
                    {
                        CGUIGumppic *gumppic =
                            (CGUIGumppic *)Add(new CGUIGumppicTiled(0x0029, 34, 20, per, 0));
                        gumppic->Color = color;
                    }

                    g = new CGUIGumppic(0x0028, 34, 33);
                    g->Color = outofRange ? color : 0;
                    Add(g);

                    per = CalculatePercents(member.Character->MaxMana, member.Character->Mana, 96);

                    if (per > 0)
                    {
                        CGUIGumppic *gumppic =
                            (CGUIGumppic *)Add(new CGUIGumppicTiled(0x0029, 34, 33, per, 0));
                        gumppic->Color = outofRange ? color : 0x0482;
                    }

                    g = new CGUIGumppic(0x0028, 34, 45);
                    g->Color = outofRange ? color : 0;
                    Add(g);

                    per = CalculatePercents(member.Character->MaxStam, member.Character->Stam, 96);

                    if (per > 0)
                    {
                        CGUIGumppic *gumppic =
                            (CGUIGumppic *)Add(new CGUIGumppicTiled(0x0029, 34, 45, per, 0));
                        gumppic->Color = outofRange ? color : 0x0075;
                    }

                    Add(new CGUIShader(&g_ColorizerShader, false));

                    break;
                }
            }
        }
        else
        {
            Add(new CGUIShader(&g_ColorizerShader, true));

            ushort color = 0;
            ushort hitsColor = 0x0386;
            ushort textColor = 0x0386;
            CGameCharacter *obj = g_World->FindWorldCharacter(Serial);
            string objName = m_Name;
            bool canChangeName = false;

            if (obj != NULL)
            {
                hitsColor = 0;

                color = g_ConfigManager.GetColorByNotoriety(obj->Notoriety);

                if (obj->Notoriety == NT_CRIMINAL || obj->Notoriety == NT_SOMEONE_GRAY)
                    color = 0;

                objName = obj->GetName();
                m_Name = objName;

                if (obj->CanChangeName)
                {
                    textColor = 0x000E;
                    canChangeName = true;
                }
            }

            m_Body = (CGUIGumppic *)Add(new CGUIGumppic(0x0804, 0, 0));
            m_Body->Color = color;

            m_HitsBody = (CGUIGumppic *)Add(new CGUIGumppic(0x0805, 34, 38));
            m_HitsBody->Color = hitsColor;

            Add(new CGUIShader(&g_ColorizerShader, false));

            if (obj != NULL && obj->MaxHits > 0)
            {
                int per = CalculatePercents(obj->MaxHits, obj->Hits, 109);

                if (per > 0)
                {
                    ushort gumpid = 0x0806;
                    if (obj->Poisoned())
                        gumpid = 0x0808;
                    else if (obj->YellowHits())
                        gumpid = 0x0809;

                    Add(new CGUIGumppicTiled(gumpid, 34, 38, per, 0));
                }
            }

            Add(new CGUIHitBox(ID_GSB_TEXT_FIELD, 16, 14, 109, 16));
            m_Entry = (CGUITextEntry *)Add(new CGUITextEntry(
                ID_GSB_TEXT_FIELD,
                textColor,
                textColor,
                textColor,
                16,
                14,
                150,
                false,
                1,
                TS_LEFT,
                UOFONT_FIXED));
            m_Entry->m_Entry.SetText(objName);
            m_Entry->CheckOnSerial = true;

            if (canChangeName)
                m_Entry->ReadOnly = false;
            else
                m_Entry->ReadOnly = true;
        }

        m_StatusbarUnlocker = (CGUIButton *)Add(
            new CGUIButton(ID_GSB_BUTTON_REMOVE_FROM_GROUP, 0x082C, 0x082C, 0x082C, 136, 24));
        m_StatusbarUnlocker->CheckPolygon = true;
        m_StatusbarUnlocker->Visible = InGroup();
    }

    QFOR(item, m_Items, CBaseGUI *)
    {
        if (item->Serial != ID_GSB_MINIMIZE)
            item->MoveOnDrag = true;
    }
}

void CGumpStatusbar::OnLeftMouseButtonDown()
{
    WISPFUN_DEBUG("c128_f13");
    if (g_GeneratedMouseDown)
        return;

    if (g_Target.IsTargeting())
    {
        g_Target.SendTargetObject(Serial);
        g_MouseManager.CancelDoubleClick = true;
    }
    else
        CGump::OnLeftMouseButtonDown();
}

void CGumpStatusbar::GUMP_BUTTON_EVENT_C
{
    WISPFUN_DEBUG("c128_f14");
    if (g_GeneratedMouseDown)
        return;

    if (serial == ID_GSB_MINIMIZE && Serial == g_PlayerSerial)
    {
        Minimized = true;
        WantUpdateContent = true;
    }
    else if (serial == ID_GSB_LOCK_MOVING)
        LockMoving = !LockMoving;
    else if (serial == ID_GSB_BUTTON_HEAL_1)
    {
        g_Orion.CastSpell(29);
        g_PartyHelperTimer = g_Ticks + 500;
        g_PartyHelperTarget = Serial;
    }
    else if (serial == ID_GSB_BUTTON_HEAL_2)
    {
        g_Orion.CastSpell(11);
        g_PartyHelperTimer = g_Ticks + 500;
        g_PartyHelperTarget = Serial;
    }
    else if (serial == ID_GSB_BUTTON_REMOVE_FROM_GROUP)
    {
        CGumpStatusbar *oldGroup = m_GroupNext;

        if (oldGroup == NULL)
            oldGroup = m_GroupPrev;

        RemoveFromGroup();

        if (oldGroup != NULL)
        {
            oldGroup->UpdateGroup(0, 0);
            oldGroup->WantRedraw = true;
        }
    }
    else if (serial == ID_GSB_BUFF_GUMP)
        g_ConfigManager.ToggleBufficonWindow = true;
    else if (serial == ID_GSB_BUFF_LOCKER_STR)
    {
        g_Player->LockStr = (g_Player->LockStr + 1) % 3;
        WantUpdateContent = true;

        CPacketChangeStatLockStateRequest(0, g_Player->LockStr).Send();
    }
    else if (serial == ID_GSB_BUFF_LOCKER_DEX)
    {
        g_Player->LockDex = (g_Player->LockDex + 1) % 3;
        WantUpdateContent = true;

        CPacketChangeStatLockStateRequest(1, g_Player->LockDex).Send();
    }
    else if (serial == ID_GSB_BUFF_LOCKER_INT)
    {
        g_Player->LockInt = (g_Player->LockInt + 1) % 3;
        WantUpdateContent = true;

        CPacketChangeStatLockStateRequest(2, g_Player->LockInt).Send();
    }
}

bool CGumpStatusbar::OnLeftMouseButtonDoubleClick()
{
    WISPFUN_DEBUG("c128_f15");
    if (g_GeneratedMouseDown)
        return false;

    if (!g_PressedObject.LeftSerial && Serial == g_PlayerSerial && Minimized)
    {
        Minimized = false;

        if (InGroup())
        {
            CGumpStatusbar *oldGroup = m_GroupNext;

            if (oldGroup == NULL)
                oldGroup = m_GroupPrev;

            RemoveFromGroup();

            if (oldGroup != NULL)
            {
                oldGroup->UpdateGroup(0, 0);
                oldGroup->WantRedraw = true;
            }
        }

        WantUpdateContent = true;

        return true;
    }
    else if (Serial != g_PlayerSerial)
    {
        if (g_Player->WarMode == WarModeState::War)
            g_Orion.Attack(Serial);
        else
            g_Orion.DoubleClick(Serial);

        return true;
    }
    else if (!Minimized)
    {
        g_Orion.PaperdollReq(Serial);

        return true;
    }

    return false;
}

void CGumpStatusbar::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c128_f16");

    if (Serial != g_PlayerSerial)
    {
        string str = g_EntryPointer->c_str();

        if (g_EntryPointer->Pos() > 0)
            str.resize(g_EntryPointer->Pos());
        else
            str = "";

        if ((g_EntryPointer->Length() <= 15) && g_FontManager.GetWidthA(1, str) <= 100 &&
            ((wParam >= 'a' && wParam <= 'z') || (wParam >= 'A' && wParam <= 'Z')))
        {
            g_EntryPointer->Insert((wchar_t)wParam);
            WantRedraw = true;
        }
    }
}

void CGumpStatusbar::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c128_f17");
    switch (wParam)
    {
        case VK_RETURN:
        {
            if (g_EntryPointer->Length())
                SendRenameRequest();
            else
            {
                CGameObject *obj = g_World->FindWorldObject(Serial);

                if (obj != NULL)
                    g_EntryPointer->SetText(obj->GetName());
            }

            if (g_ConfigManager.GetConsoleNeedEnter())
                g_EntryPointer = NULL;
            else
                g_EntryPointer = &g_GameConsole;

            WantRedraw = true;

            break;
        }
        case VK_HOME:
        case VK_LEFT:
        case VK_RIGHT:
        case VK_BACK:
        case VK_DELETE:
        case VK_END:
        {
            g_EntryPointer->OnKey(this, wParam);
            WantRedraw = true;

            break;
        }
        case VK_ESCAPE:
        {
            CGameObject *obj = g_World->FindWorldObject(Serial);
            if (obj != NULL)
                g_EntryPointer->SetText(obj->GetName());

            if (g_ConfigManager.GetConsoleNeedEnter())
                g_EntryPointer = NULL;
            else
                g_EntryPointer = &g_GameConsole;

            WantRedraw = true;

            break;
        }
        default:
            break;
    }
}

void CGumpStatusbar::SendRenameRequest()
{
    WISPFUN_DEBUG("c128_f18");
    QFOR(item, m_Items, CBaseGUI *)
    {
        if (item->Type != GOT_TEXTENTRY)
            continue;

        CEntryText *entry = &((CGUITextEntry *)item)->m_Entry;

        if (entry->Length())
        {
            CPacketRenameRequest(Serial, entry->c_str()).Send();
        }
    }
}
