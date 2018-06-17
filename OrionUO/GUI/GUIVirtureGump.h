

#ifndef GUIVIRTUREGUMP_H
#define GUIVIRTUREGUMP_H

class CGUIVirtureGump : public CGUIDrawObject
{
public:
    CGUIVirtureGump(ushort graphic, int x, int y);
    virtual ~CGUIVirtureGump();

    virtual bool Select();
};

#endif
