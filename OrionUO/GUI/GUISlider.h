

#ifndef GUISLIDER_H
#define GUISLIDER_H

class CGUISlider : public CBaseGUI
{
public:
    ushort GraphicSelected = 0;

    ushort GraphicPressed = 0;

    ushort BackgroundGraphic = 0;

    bool CompositeBackground = false;

    bool Vertical = false;

    int Length = 0;

    float Percents = 0.0f;

    int Offset = 0;

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

    int ScrollStep = 15;

    uint LastScrollTime = 0;

    int DefaultTextOffset = 2;

private:
    CGLTextTexture Text{ CGLTextTexture() };

public:
    CGUISlider(
        int serial,
        ushort graphic,
        ushort graphicSelected,
        ushort graphicPressed,
        ushort backgroundGraphic,
        bool compositeBackground,
        bool vertical,
        int x,
        int y,
        int lenght,
        int minValue,
        int maxValue,
        int value);
    virtual ~CGUISlider();

    virtual bool IsPressedOuthit() { return true; }

    virtual WISP_GEOMETRY::CSize GetSize();

    virtual void OnScroll(bool up, int delay);

    virtual void OnClick(int x, int y);

    void UpdateText();

    virtual void CalculateOffset();

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

    virtual ushort GetDrawGraphic();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();

    virtual void OnMouseEnter();
    virtual void OnMouseExit();
};

#endif
