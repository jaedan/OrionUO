

#include "stdafx.h"

CEntryText *g_EntryPointer = NULL;

CEntryText::CEntryText(int maxLength, int width, int maxWidth, bool numberOnly)
    : MaxLength(maxLength)
    , Width(width)
    , MaxWidth(maxWidth)
    , NumberOnly(numberOnly)
{
    WISPFUN_DEBUG("c169_f1");
}

CEntryText::~CEntryText()
{
    WISPFUN_DEBUG("c169_f2");

    if (g_EntryPointer == this)
    {
        if (g_ConfigManager.GetConsoleNeedEnter())
            g_EntryPointer = NULL;
        else
        {
            if (g_GameState >= GS_GAME)
                g_EntryPointer = &g_GameConsole;
            else
                g_EntryPointer = NULL;
        }
    }

    Clear();
}

const char *CEntryText::c_str()
{
    WISPFUN_DEBUG("c169_f3");
    m_CText = ToString(Text);
    return m_CText.c_str();
}

void CEntryText::OnClick(
    CGump *gump, uchar font, bool unicode, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
    WISPFUN_DEBUG("c169_f4");

    if (g_EntryPointer != this)
    {
        CGump *gumpEntry = g_GumpManager.GetTextEntryOwner();

        if (gumpEntry != NULL)
            gumpEntry->FrameCreated = false;

        g_EntryPointer = this;
        Changed = true;
    }
    if (g_EntryPointer == this)
    {
        int oldPos = m_Position;

        if (unicode)
            m_Position =
                g_FontManager.CalculateCaretPosW(font, Text.c_str(), x, y, Width, align, flags);
        else
            m_Position = g_FontManager.CalculateCaretPosA(font, c_str(), x, y, Width, align, flags);

        if (oldPos != m_Position)
            Changed = true;
    }

    if (gump != NULL)
        gump->FrameCreated = false;
}

void CEntryText::OnKey(CGump *gump, WPARAM wParam)
{
    WISPFUN_DEBUG("c169_f5");
    switch (wParam)
    {
        case VK_HOME:
        {
            SetPos(0, gump);

            break;
        }
        case VK_END:
        {
            SetPos((int)Length(), gump);

            break;
        }
        case VK_LEFT:
        {
            AddPos(-1, gump);

            break;
        }
        case VK_RIGHT:
        {
            AddPos(1, gump);

            break;
        }
        case VK_BACK:
        {
            Remove(true, gump);

            break;
        }
        case VK_DELETE:
        {
            Remove(false, gump);

            break;
        }
        default:
            break;
    }
}

int CEntryText::GetLinesCountA(uchar font, TEXT_ALIGN_TYPE align, ushort flags, int width)
{
    WISPFUN_DEBUG("c169_f6");
    if (!width)
        width = Width;

    MULTILINES_FONT_INFO *info =
        g_FontManager.GetInfoA(font, c_str(), (int)Length(), align, flags, width);

    int count = 0;

    while (info != NULL)
    {
        MULTILINES_FONT_INFO *next = info->m_Next;
        delete info;
        info = next;
        count++;
    }

    return count;
}

int CEntryText::GetLinesCountW(uchar font, TEXT_ALIGN_TYPE align, ushort flags, int width)
{
    WISPFUN_DEBUG("c169_f7");
    if (!width)
        width = Width;

    MULTILINES_FONT_INFO *info =
        g_FontManager.GetInfoW(font, Data(), (int)Length(), align, flags, width);

    int count = 0;

    while (info != NULL)
    {
        MULTILINES_FONT_INFO *next = info->m_Next;
        delete info;
        info = next;
        count++;
    }

    return count;
}

bool CEntryText::Insert(wchar_t ch, CGump *gump)
{
    WISPFUN_DEBUG("c169_f8");
    if (ch == '\r')
        return false;

    if (m_Position < 0)
        m_Position = 0;

    if (m_Position > (int)Text.length())
        m_Position = (int)Text.length();

    if (MaxLength > 0)
    {
        if (NumberOnly)
        {
            wstring wstr = Text;
            wstr.insert(wstr.begin() + m_Position, ch);

            if (_wtoi(wstr.c_str()) >= MaxLength)
                return false;
        }
        else if ((int)Text.length() >= MaxLength)
            return false;
    }

    Text.insert(Text.begin() + m_Position, ch);

    m_Position++;

    Changed = true;

    if (gump != NULL)
        gump->FrameCreated = false;

    return true;
}

void CEntryText::Remove(bool left, CGump *gump)
{
    WISPFUN_DEBUG("c169_f9");

    if (left)
    {
        if (m_Position < 1)
            return;

        m_Position--;
    }
    else
    {
        if (m_Position >= (int)Text.length())
            return;
    }

    if (m_Position < Text.length())
        Text.erase(Text.begin() + m_Position);
    else
        Text.erase(Text.length() - 1);

    Changed = true;

    if (gump != NULL)
        gump->FrameCreated = false;
}

void CEntryText::Clear()
{
    WISPFUN_DEBUG("c169_f10");

    Text = L"";
    m_CText = "";
    m_Position = 0;
    Changed = true;
    DrawOffset = 0;
    m_Texture.Clear();
    CaretPos.Reset();
}

void CEntryText::Paste()
{
    WISPFUN_DEBUG("c169_f11");
    if (OpenClipboard(g_OrionWindow.Handle))
    {
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);

        if (hData != NULL)
        {
            wstring text((wchar_t *)GlobalLock(hData));
            CGump *gump = g_GumpManager.GetTextEntryOwner();
            if (gump != NULL && gump->GumpType == GT_BOOK)
                gump->PasteClipboardData(text);
            else
            {
                IFOR (i, 0, (int)text.length())
                    Insert(text[i]);
            }

            GlobalUnlock(hData);
        }

        CloseClipboard();
    }
}

void CEntryText::AddPos(int val, CGump *gump)
{
    WISPFUN_DEBUG("c169_f12");

    m_Position += val;

    if (m_Position < 0)
        m_Position = 0;

    if (m_Position > (int)Text.length())
        m_Position = (int)Text.length();

    Changed = true;

    if (gump != NULL)
        gump->FrameCreated = false;
}

void CEntryText::SetPos(int val, CGump *gump)
{
    WISPFUN_DEBUG("c169_f13");

    m_Position = val;

    if (m_Position < 0)
        m_Position = 0;

    if (m_Position > (int)Text.length())
        m_Position = (int)Text.length();

    Changed = true;

    if (gump != NULL)
        gump->FrameCreated = false;
}

void CEntryText::SetText(const string &text)
{
    WISPFUN_DEBUG("c169_f14");

    wstring wtext = ToWString(text);

    SetText(wtext);
}

void CEntryText::SetText(const wstring &text)
{
    WISPFUN_DEBUG("c169_f15");

    Clear();

    Text = text;
    m_Position = (int)Text.length();

    if (m_Position < 0)
        m_Position = 0;

    if (MaxLength > 0)
    {
        if (NumberOnly)
        {
            string str = ToString(Text);

            while (true)
            {
                size_t len = str.length();

                if (std::atoi(str.c_str()) >= MaxLength && len > 0)
                    str.resize(len - 1);
                else
                    break;
            }
        }
        else if ((int)Text.length() >= MaxLength)
            Text.resize(MaxLength);
    }

    CGump *gump = g_GumpManager.GetTextEntryOwner();

    if (gump != NULL)
        gump->FrameCreated = false;
}

string CEntryText::CheckMaxWidthA(uchar font, string str)
{
    WISPFUN_DEBUG("c169_f16");

    if (MaxWidth > 0)
    {
        int width = g_FontManager.GetWidthA(font, str);
        size_t len = str.length();

        while (MaxWidth < width && len > 0)
        {
            str.erase(str.begin() + len);
            len--;
            width = g_FontManager.GetWidthA(font, str);
        }
    }

    return str;
}

wstring CEntryText::CheckMaxWidthW(uchar font, wstring str)
{
    WISPFUN_DEBUG("c169_f17");

    if (MaxWidth > 0)
    {
        int width = g_FontManager.GetWidthW(font, str);
        size_t len = str.length();

        while (MaxWidth < width && len > 0)
        {
            str.erase(str.begin() + len);
            len--;
            width = g_FontManager.GetWidthW(font, str);
        }
    }

    return str;
}

void CEntryText::FixMaxWidthA(uchar font)
{
    WISPFUN_DEBUG("c169_f18");
    c_str();

    if (MaxWidth > 0)
    {
        int width = g_FontManager.GetWidthA(font, m_CText);
        size_t len = m_CText.length();

        while (MaxWidth < width && len > 0)
        {
            Remove((m_Position > 0), NULL);
            len--;
            width = g_FontManager.GetWidthA(font, c_str());
        }
    }
}

void CEntryText::FixMaxWidthW(uchar font)
{
    WISPFUN_DEBUG("c169_f19");

    if (MaxWidth > 0)
    {
        int width = g_FontManager.GetWidthW(font, Text);
        size_t len = Text.length();

        while (MaxWidth < width && len > 0)
        {
            Remove((m_Position > 0), NULL);
            len--;
            width = g_FontManager.GetWidthW(font, Text);
        }
    }
}

void CEntryText::CreateTextureA(
    uchar font, string str, ushort color, int width, TEXT_ALIGN_TYPE align, ushort flags)
{
    WISPFUN_DEBUG("c169_f20");

    if (!str.length())
        Clear();
    else
    {
        m_Texture.Clear();

        if (m_Position)
        {
            CaretPos =
                g_FontManager.GetCaretPosA(font, str.c_str(), m_Position, width, align, flags);

            if (flags & UOFONT_FIXED)
            {
                if (DrawOffset)
                {
                    if (CaretPos.X + DrawOffset < 0)
                        DrawOffset = -CaretPos.X;
                    else if (Width + -DrawOffset < CaretPos.X)
                        DrawOffset = Width - CaretPos.X;
                }
                else if (Width + DrawOffset < CaretPos.X)
                    DrawOffset = Width - CaretPos.X;
                else
                    DrawOffset = 0;
            }
        }
        else
        {
            CaretPos.Reset();
            DrawOffset = 0;
        }

        g_FontManager.GenerateA(font, m_Texture, str, color, Width + abs(DrawOffset), align, flags);
    }
}

void CEntryText::CreateTextureW(
    uchar font, wstring str, ushort color, int width, TEXT_ALIGN_TYPE align, ushort flags)
{
    WISPFUN_DEBUG("c169_f21");

    if (!str.length())
        Clear();
    else
    {
        m_Texture.Clear();

        if (m_Position)
        {
            CaretPos = g_FontManager.GetCaretPosW(font, str, m_Position, width, align, flags);

            if (flags & UOFONT_FIXED)
            {
                if (DrawOffset)
                {
                    if (CaretPos.X + DrawOffset < 0)
                        DrawOffset = -CaretPos.X;
                    else if (Width + -DrawOffset < CaretPos.X)
                        DrawOffset = Width - CaretPos.X;
                }
                else if (Width + DrawOffset < CaretPos.X)
                    DrawOffset = Width - CaretPos.X;
                else
                    DrawOffset = 0;
            }
        }
        else
        {
            CaretPos.Reset();
            DrawOffset = 0;
        }

        g_FontManager.GenerateW(font, m_Texture, str, color, 30, Width, align, flags);
    }
}

void CEntryText::PrepareToDrawA(uchar font, ushort color, TEXT_ALIGN_TYPE align, ushort flags)
{
    WISPFUN_DEBUG("c169_f22");

    if (Changed || Color != color)
    {
        FixMaxWidthA(font);

        CreateTextureA(font, m_CText, color, Width, align, flags);

        Changed = false;
        Color = color;

        if (this == g_EntryPointer)
            g_FontManager.GenerateA(font, m_CaretTexture, "_", color);
    }
}

void CEntryText::PrepareToDrawW(uchar font, ushort color, TEXT_ALIGN_TYPE align, ushort flags)
{
    WISPFUN_DEBUG("c169_f23");

    if (Changed || Color != color)
    {
        FixMaxWidthW(font);

        CreateTextureW(font, Text, color, Width, align, flags);

        Changed = false;
        Color = color;

        if (this == g_EntryPointer)
            g_FontManager.GenerateW(font, m_CaretTexture, L"_", color, 30);
    }
}

void CEntryText::DrawA(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
    WISPFUN_DEBUG("c169_f24");
    PrepareToDrawA(font, color, align, flags);

    m_Texture.Draw(x + DrawOffset, y);

    if (this == g_EntryPointer)
    {
        const int offsetTable[] = { 1, 2, 1, 1, 1, 2, 1, 1, 2, 2 };
        int offsY = offsetTable[font % 10];

        m_CaretTexture.Draw(x + DrawOffset + CaretPos.X, y + offsY + CaretPos.Y);
    }
}

void CEntryText::DrawW(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
    WISPFUN_DEBUG("c169_f25");
    PrepareToDrawW(font, color, align, flags);

    m_Texture.Draw(x + DrawOffset, y);

    if (this == g_EntryPointer)
        m_CaretTexture.Draw(x + DrawOffset + CaretPos.X, y + CaretPos.Y);
}

void CEntryText::DrawMaskA(
    uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
    WISPFUN_DEBUG("c169_f26");

    size_t len = Length();
    string str = "";
    IFOR (i, 0, len)
        str += "*";

    if (len)
        g_FontManager.DrawA(font, str, color, x + DrawOffset, y);

    if (this == g_EntryPointer)
    {
        const int offsetTable[] = { 1, 2, 1, 1, 1, 2, 1, 1, 2, 2 };
        int offsY = offsetTable[font % 10];

        if (m_Position)
        {
            str.resize(m_Position);
            x += g_FontManager.GetWidthA(font, str.c_str());
        }

        g_FontManager.DrawA(font, "_", color, x + DrawOffset, y + offsY);
    }
}

void CEntryText::DrawMaskW(
    uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align, ushort flags)
{
    WISPFUN_DEBUG("c169_f27");

    size_t len = Length();
    wstring str = L"";
    IFOR (i, 0, len)
        str += L"*";

    if (len)
        g_FontManager.DrawW(font, str, color, x + DrawOffset, y, 30, 0, TS_LEFT, flags);

    if (this == g_EntryPointer)
    {
        if (m_Position)
        {
            str.resize(m_Position);
            x += g_FontManager.GetWidthW(font, str);
        }

        g_FontManager.DrawW(font, L"_", color, x + DrawOffset, y, 30, 0, TS_LEFT, flags);
    }
}

WPARAM CEntryText::GetLastChar()
{
    if (Text.length() == 0)
        return 0;
    return Text[Text.length() - 1];
}

void CEntryText::RemoveSequence(int startPos, int length)
{
    Text.erase(startPos, length);
}

string CEntryText::GetTextA() const
{
    return m_CText;
}

wstring CEntryText::GetTextW() const
{
    return Text;
}
