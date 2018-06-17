

#ifndef GUIHITBOX_H
#define GUIHITBOX_H

class CGUIHitBox : public CGUIPolygonal
{
public:
    int ToPage = -1;

    CGUIHitBox(int serial, int x, int y, int width, int height, bool callOnMouseUp = false);
    virtual ~CGUIHitBox();

    virtual void OnMouseEnter();
    virtual void OnMouseExit();
};

#endif
