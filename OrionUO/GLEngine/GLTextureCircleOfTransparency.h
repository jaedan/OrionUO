

#ifndef GLTEXTURECIRCLEOFTRANSPARENCY_H
#define GLTEXTURECIRCLEOFTRANSPARENCY_H

class CGLTextureCircleOfTransparency : public CGLTexture
{
public:
    int Radius = 0;
    int X = 0;
    int Y = 0;

    CGLTextureCircleOfTransparency();
    virtual ~CGLTextureCircleOfTransparency();

    static void CreatePixels(int radius, short &width, short &height, UINT_LIST &pixels);

    bool Create(int radius);

    virtual void Draw(int x, int y, bool checktrans = false);

    void Redraw();
};

extern CGLTextureCircleOfTransparency g_CircleOfTransparency;

#endif
