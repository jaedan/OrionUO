

#ifndef GUICHECKBOX_H
#define GUICHECKBOX_H

class CGUICheckbox : public CGUIDrawObject
{
public:
    ushort GraphicChecked = 0;

    ushort GraphicSelected = 0;

    ushort GraphicDisabled = 0;

    bool Checked = false;

    SLIDER_TEXT_POSITION TextPosition = STP_RIGHT;

    int TextX = 0;

    int TextY = 0;

    int DefaultTextOffset = 2;

protected:
    CGLTextTexture Text{ CGLTextTexture() };

    void UpdateTextPosition();

public:
    CGUICheckbox(
        int serial, ushort graphic, ushort graphicChecked, ushort graphicDisabled, int x, int y);
    virtual ~CGUICheckbox();

    void SetTextParameters(
        uchar font,
        const wstring &text,
        ushort color,
        SLIDER_TEXT_POSITION textPosition = STP_RIGHT,
        int textWidth = 0,
        TEXT_ALIGN_TYPE align = TS_LEFT,
        ushort textFlags = 0);
    void SetTextParameters(
        uchar font,
        const string &text,
        ushort color,
        SLIDER_TEXT_POSITION textPosition = STP_RIGHT,
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
