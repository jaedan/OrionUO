

#ifndef GUIGENERICTEXTENTRY_H
#define GUIGENERICTEXTENTRY_H

class CGUIGenericTextEntry : public CGUITextEntry
{
public:
    uint TextID = 0;

    CGUIGenericTextEntry(
        int serial, int index, ushort color, int x, int y, int maxWidth = 0, int maxLength = 0);
    virtual ~CGUIGenericTextEntry();
};

#endif
