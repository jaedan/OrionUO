

#ifndef GUIRADIO_H
#define GUIRADIO_H

class CGUIRadio : public CGUICheckbox
{
public:
    CGUIRadio(
        int serial, ushort graphic, ushort graphicChecked, ushort graphicDisabled, int x, int y);
    virtual ~CGUIRadio();
};

#endif
