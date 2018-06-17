

#ifndef COLORMANAGER_H
#define COLORMANAGER_H

struct FLOAT_HUES
{
    float Palette[32 * 3];
};

class CColorManager
{
private:
    vector<HUES_GROUP> m_HuesRange;

    vector<FLOAT_HUES> m_HuesFloat;

    int m_HuesCount{ 0 };

    USHORT_LIST m_Radarcol;

public:
    CColorManager();
    ~CColorManager();

    void Init();

    PHUES_GROUP GetHuesRangePointer() { return &m_HuesRange[0]; }

    int GetHuesCount() const { return m_HuesCount; }

    void SetHuesBlock(int index, PVERDATA_HUES_GROUP group);

    void CreateHuesPalette();

    void SendColorsToShader(ushort color);

    ushort Color32To16(int c);

    uint Color16To32(ushort c);

    ushort ConvertToGray(ushort c);

    ushort GetColor16(ushort c, ushort color);

    ushort GetRadarColorData(int c);

    uint GetPolygoneColor(ushort c, ushort color);

    uint GetUnicodeFontColor(ushort &c, ushort color);

    uint GetColor(ushort &c, ushort color);

    uint GetPartialHueColor(ushort &c, ushort color);

    ushort FixColor(ushort color, ushort defaultColor = 0);
};

extern CColorManager g_ColorManager;

#endif
