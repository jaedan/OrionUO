

#ifndef GUIBUFF_H
#define GUIBUFF_H

class CGUIBuff : public CGUIDrawObject
{
public:
    uint Timer = 0;

    wstring Text = L"";

    uint TooltipTimer = 0;

    bool DecAlpha = true;

    uchar Alpha = 0xFF;

    CGUIBuff(ushort graphic, int timer, const wstring &text);
    virtual ~CGUIBuff();

    virtual void Draw(bool checktrans = false);
};

#endif
