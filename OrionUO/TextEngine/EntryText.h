

#ifndef ENTRYTEXT_H
#define ENTRYTEXT_H

class CGump;

class CEntryText
{
public:
    int MaxLength = 0;
    int Width = 0;
    int MaxWidth = 0;
    bool Changed = true;
    ushort Color = 0;
    int DrawOffset = 0;
    WISP_GEOMETRY::CPoint2Di CaretPos = WISP_GEOMETRY::CPoint2Di();
    bool NumberOnly = false;

protected:
    int m_Position{ 0 };

    string m_CText = string{ "" };

    wstring Text = wstring{ L"" };

public:
    CEntryText(int maxLength = 0, int width = 0, int maxWidth = 0, bool numberOnly = false);
    virtual ~CEntryText();

    CGLTextTexture m_Texture;

    CGLTextTexture m_CaretTexture;

    void Clear();

    void Paste();

    int
    GetLinesCountA(uchar font, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0, int width = 0);

    int
    GetLinesCountW(uchar font, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0, int width = 0);

    bool Insert(wchar_t ch, CGump *gump = NULL);

    void Remove(bool left, CGump *gump = NULL);

    void RemoveSequence(int startPos, int length);

    WPARAM GetLastChar();

    void AddPos(int val, CGump *gump = NULL);

    void SetPos(int val, CGump *gump = NULL);

    const wchar_t *Data() const { return Text.c_str(); }

    const char *c_str();

    void SetText(const string &text);

    void SetText(const wstring &text);

    string GetTextA() const;

    wstring GetTextW() const;

    void OnClick(
        CGump *gump,
        uchar font,
        bool unicode,
        int x,
        int y,
        TEXT_ALIGN_TYPE align = TS_LEFT,
        ushort flags = 0);

    void OnKey(CGump *gump, WPARAM wParam);

    int Pos() const { return m_Position; }

    size_t Length() const { return Text.length(); }

    string CheckMaxWidthA(uchar font, string str);

    wstring CheckMaxWidthW(uchar font, wstring str);

    void FixMaxWidthA(uchar font);

    void FixMaxWidthW(uchar font);

    void CreateTextureA(
        uchar font, string str, ushort color, int width, TEXT_ALIGN_TYPE align, ushort flags);

    void CreateTextureW(
        uchar font, wstring str, ushort color, int width, TEXT_ALIGN_TYPE align, ushort flags);

    virtual void
    PrepareToDrawA(uchar font, ushort color, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

    virtual void
    PrepareToDrawW(uchar font, ushort color, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

    virtual void DrawA(
        uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

    virtual void DrawW(
        uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

    void DrawMaskA(
        uchar font, ushort color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

    void DrawMaskW(
        uchar font, ushort color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);
};

extern CEntryText *g_EntryPointer;

#endif
