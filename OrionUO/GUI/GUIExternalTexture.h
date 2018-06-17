

#ifndef GUIEXTERNALTEXTURE_H
#define GUIEXTERNALTEXTURE_H

class CGUIExternalTexture : public CBaseGUI
{
    bool DeleteTextureOnDestroy = false;

    short DrawWidth = 0;
    short DrawHeight = 0;

public:
    CGUIExternalTexture(
        CGLTexture *texture,
        bool deleteTextureOnDestroy,
        int x,
        int y,
        int drawWidth = 0,
        int drawHeight = 0);
    virtual ~CGUIExternalTexture();

    CGLTexture *m_Texture;

    virtual WISP_GEOMETRY::CSize GetSize();

    virtual void SetShaderMode();
    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};

#endif
