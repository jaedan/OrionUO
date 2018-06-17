

#ifndef GLTEXTTEXTURE_H
#define GLTEXTTEXTURE_H

struct WEB_LINK_RECT
{
    ushort LinkID;

    int StartX;
    int StartY;

    int EndX;
    int EndY;
};

class CGLTextTexture : public CGLTexture
{
public:
    int LinesCount = 0;

    CGLTextTexture();
    virtual ~CGLTextTexture();

    bool Empty() { return (Texture == 0); }

    virtual void Clear();

    virtual void Draw(int x, int y, bool checktrans = false);

    virtual void ClearWebLink() {}

    virtual void AddWebLink(WEB_LINK_RECT &wl) {}

    virtual ushort WebLinkUnderMouse(int x, int y) { return 0; }
};

#endif
