

#ifndef GUIMINMAXBUTTONS_H
#define GUIMINMAXBUTTONS_H

class CGUIMinMaxButtons : public CBaseGUI
{
public:
    int MinValue = 0;

    int MaxValue = 0;

    int Value = 0;

    bool HaveText = false;

    SLIDER_TEXT_POSITION TextPosition = STP_RIGHT;

    uchar Font = 0;

    ushort TextColor = 0;

    bool Unicode = true;

    int TextWidth = 0;

    TEXT_ALIGN_TYPE Align = TS_LEFT;

    ushort TextFlags = 0;

    int TextX = 0;

    int TextY = 0;

    int BaseScrollStep = 1;

    int ScrollStep = 1;

    uint LastScrollTime = 0;

    int DefaultTextOffset = 2;

private:
    CGLTextTexture Text{ CGLTextTexture() };

    int m_ScrollMode{ 0 };

public:
    CGUIMinMaxButtons(
        int serial, ushort graphic, int x, int y, int minValue, int maxValue, int value);
    virtual ~CGUIMinMaxButtons();

    virtual bool IsPressedOuthit() { return true; }

    virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(36, 18); }

    virtual void Scroll(int delay);

    virtual void OnClick();

    void UpdateText();

    void SetTextParameters(
        bool haveText,
        SLIDER_TEXT_POSITION textPosition,
        uchar font,
        ushort color,
        bool unicode,
        int textWidth = 0,
        TEXT_ALIGN_TYPE align = TS_LEFT,
        ushort textFlags = 0);

    virtual void PrepareTextures();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};

#endif
