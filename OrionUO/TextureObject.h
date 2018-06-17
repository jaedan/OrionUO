

#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

struct UOPAnimationData
{
    std::string path;
    unsigned int offset;
    unsigned int compressedLength;
    unsigned int decompressedLength;
    std::fstream *fileStream;
};

class CTextureAnimationFrame : public CGLTexture
{
public:
    short CenterX = 0;
    short CenterY = 0;

    CTextureAnimationFrame();
    virtual ~CTextureAnimationFrame();
};

class CTextureAnimationDirection
{
public:
    uchar FrameCount = 0;

    size_t BaseAddress = 0;

    uint BaseSize = 0;

    size_t PatchedAddress = 0;

    uint PatchedSize = 0;

    int FileIndex = 0;

    size_t Address = 0;

    uint Size = 0;

    uint LastAccessTime = 0;

    bool IsUOP = false;

    bool IsVerdata = false;

    CTextureAnimationDirection();
    virtual ~CTextureAnimationDirection();

    CTextureAnimationFrame *m_Frames;
};

class CTextureAnimationGroup
{
public:
    CTextureAnimationGroup();
    virtual ~CTextureAnimationGroup();

    CTextureAnimationDirection m_Direction[5];
    UOPAnimationData m_UOPAnimData;
};

#endif
