

#ifndef GUILINE_H
#define GUILINE_H

class CGUILine : public CBaseGUI
{
    int TargetX = 0;

    int TargetY = 0;

    uchar ColorR = 0;

    uchar ColorG = 0;

    uchar ColorB = 0;

    uchar ColorA = 0;

public:
    CGUILine(int x, int y, int targetX, int targetY, int polygonColor);
    virtual ~CGUILine();

    virtual void Draw(bool checktrans = false);
};

#endif
