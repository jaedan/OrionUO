

#ifndef GUIGENERICTEXT_H
#define GUIGENERICTEXT_H

class CGUIGenericText : public CGUIText
{
public:
    uint TextID = 0;

    int MaxWidth = 0;

    CGUIGenericText(int index, ushort color, int x, int y, int maxWidth = 0);
    virtual ~CGUIGenericText();

    void CreateTexture(const wstring &str);
};

#endif
