

#ifndef MULREADER_H
#define MULREADER_H

class UOFileReader
{
public:
    UOFileReader() {}
    ~UOFileReader() {}

    USHORT_LIST GetGumpPixels(CIndexObject &io);

    CGLTexture *ReadGump(CIndexObject &io);

    USHORT_LIST GetArtPixels(ushort id, CIndexObject &io, bool run, short &width, short &height);

    CGLTexture *ReadArt(ushort id, CIndexObject &io, bool run);

    CGLTexture *ReadTexture(CIndexObject &io);

    CGLTexture *ReadLight(CIndexObject &io);
};

extern UOFileReader g_UOFileReader;

#endif
