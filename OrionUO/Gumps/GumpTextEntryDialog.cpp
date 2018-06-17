

#include "stdafx.h"

CGumpTextEntryDialog::CGumpTextEntryDialog(
    uint serial, short x, short y, uchar variant, int maxLength, string text, string description)
    : CGump(GT_TEXT_ENTRY_DIALOG, serial, x, y)
    , Text(text)
    , m_Description(description)
    , Variant(variant)
    , m_MaxLength(maxLength)
{
    WISPFUN_DEBUG("c130_f1");
    NoMove = true;
    Blocked = true;

    if (!g_GrayMenuCount)
    {
        g_Orion.InitScreen(GS_GAME_BLOCKED);
        g_GameBlockedScreen.Code = 2;
    }

    g_GrayMenuCount++;
}

CGumpTextEntryDialog::~CGumpTextEntryDialog()
{
}

void CGumpTextEntryDialog::PrepareContent()
{
    WISPFUN_DEBUG("c130_f2");
    if (m_TextField != NULL && m_Entry != NULL)
    {
        ushort newGraphic = 0x0475;

        if (g_EntryPointer == &m_Entry->m_Entry)
            newGraphic = 0x0477;
        else if (g_SelectedObject.Object == m_Entry)
            newGraphic = 0x0476;

        if (m_TextField->Graphic != newGraphic)
        {
            m_TextField->Graphic = newGraphic;
            WantRedraw = true;
        }
    }
}

void CGumpTextEntryDialog::UpdateContent()
{
    WISPFUN_DEBUG("c130_f3");
    Clear();

    Add(new CGUIGumppic(0x0474, 0, 0));

    CGUIText *text = (CGUIText *)Add(new CGUIText(0x0386, 60, 50));
    text->CreateTextureA(2, Text);

    text = (CGUIText *)Add(new CGUIText(0x0386, 60, 108));
    text->CreateTextureA(2, m_Description);

    CGump *gumpEntry = g_GumpManager.GetTextEntryOwner();

    if (gumpEntry != NULL)
        gumpEntry->WantRedraw = true;

    m_TextField = (CGUIGumppic *)Add(new CGUIGumppic(0x0477, 60, 130));
    m_TextField->Serial = ID_GTED_TEXT_FIELD;

    m_Entry = (CGUITextEntry *)Add(
        new CGUITextEntry(ID_GTED_TEXT_FIELD, 0x0386, 0x0386, 0x0386, 71, 137, 0, false, 1));
    m_Entry->m_Entry.MaxLength = m_MaxLength;
    m_Entry->m_Entry.NumberOnly = (Variant == 2);
    m_Entry->CheckOnSerial = true;
    g_EntryPointer = &m_Entry->m_Entry;

    Add(new CGUIButton(ID_GTED_BUTTON_OKAY, 0x047B, 0x047D, 0x047C, 117, 190));

    if (!NoClose)
        Add(new CGUIButton(ID_GTED_BUTTON_CANCEL, 0x0478, 0x047A, 0x0479, 204, 190));
}

void CGumpTextEntryDialog::GUMP_BUTTON_EVENT_C
{
    WISPFUN_DEBUG("c130_f4");
    if (serial == ID_GTED_BUTTON_OKAY)
        SendTextEntryDialogResponse(true);
    else if (serial == ID_GTED_BUTTON_CANCEL)
        SendTextEntryDialogResponse(false);
}

void CGumpTextEntryDialog::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c130_f5");
    if (Variant == 2)
    {
        if (wParam >= '0' && wParam <= '9')
        {
            g_EntryPointer->Insert((wchar_t)wParam);

            int val = atoi(g_EntryPointer->c_str());

            if (val > m_MaxLength)
                g_EntryPointer->Remove(true);
            else
                WantRedraw = true;
        }
    }
    else if (Variant == 1)
    {
        if ((int)g_EntryPointer->Length() < m_MaxLength)
        {
            g_EntryPointer->Insert((wchar_t)wParam);
            WantRedraw = true;
        }
    }
}

void CGumpTextEntryDialog::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
    WISPFUN_DEBUG("c130_f6");
    switch (wParam)
    {
        case VK_RETURN:
        case VK_ESCAPE:
        {
            SendTextEntryDialogResponse(wParam == VK_RETURN);

            break;
        }
        case VK_HOME:
        case VK_END:
        case VK_LEFT:
        case VK_RIGHT:
        case VK_BACK:
        case VK_DELETE:
        {
            g_EntryPointer->OnKey(this, wParam);
            WantRedraw = true;

            break;
        }
        default:
            break;
    }
}

void CGumpTextEntryDialog::SendTextEntryDialogResponse(bool mode)
{
    WISPFUN_DEBUG("c130_f7");
    if (!RemoveMark && m_Entry != NULL)
    {
        CPacketTextEntryDialogResponse(this, &m_Entry->m_Entry, mode).Send();
    }

    RemoveMark = true;
}
