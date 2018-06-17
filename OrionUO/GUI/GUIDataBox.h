

#ifndef GUIDATABOX_H
#define GUIDATABOX_H

class CGUIDataBox : public CGUIPolygonal
{
public:
    CGUIDataBox();
    virtual ~CGUIDataBox();

    virtual void PrepareTextures();

    virtual bool EntryPointerHere();

    virtual bool Select() { return true; }
};

#endif
