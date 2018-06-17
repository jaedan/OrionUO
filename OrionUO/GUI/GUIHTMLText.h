

#ifndef GUIHTMLTEXT_H
#define GUIHTMLTEXT_H

class CGUIHTMLText : public CBaseGUI
{
public:
    uint TextID = 0;

    uint HTMLStartColor = 0;

    wstring Text = L"";

    uchar Font = 0;

    TEXT_ALIGN_TYPE Align = TS_LEFT;

    ushort TextFlags = 0;

    int Width = 0;

    CGUIHTMLText(
        int index,
        uchar font,
        ushort color,
        int x,
        int y,
        int width = 0,
        TEXT_ALIGN_TYPE align = TS_LEFT,
        ushort textFlags = 0,
        int htmlStartColor = 0xFFFFFFFF);
    virtual ~CGUIHTMLText();

    CGLHTMLTextTexture m_Texture{ CGLHTMLTextTexture() };

    virtual WISP_GEOMETRY::CSize GetSize()
    {
        return WISP_GEOMETRY::CSize(m_Texture.Width, m_Texture.Height);
    }

    void CreateTexture(bool backgroundCanBeColored);

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};

#endif
