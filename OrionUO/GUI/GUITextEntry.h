

#ifndef GUITEXTENTRY_H
#define GUITEXTENTRY_H

class CGUITextEntry : public CBaseGUI
{
public:
    ushort ColorSelected = 0;

    ushort ColorFocused = 0;

    bool Unicode = false;

    uchar Font = 0;

    TEXT_ALIGN_TYPE Align = TS_LEFT;

    ushort TextFlags = 0;

    bool CheckOnSerial = false;

    bool ReadOnly = false;

    bool Focused = false;

    bool UseGlobalColor = false;

    uchar GlobalColorR = 0;

    uchar GlobalColorG = 0;

    uchar GlobalColorB = 0;

    uchar GlobalColorA = 0;

    uchar GlobalColorSelectedR = 0;

    uchar GlobalColorSelectedG = 0;

    uchar GlobalColorSelectedB = 0;

    uchar GlobalColorSelectedA = 0;

    uchar GlobalColorFocusedR = 0;

    uchar GlobalColorFocusedG = 0;

    uchar GlobalColorFocusedB = 0;

    uchar GlobalColorFocusedA = 0;

    char FocusedOffsetY = 0;

    CGUITextEntry(
        int serial,
        ushort color,
        ushort colorSelected,
        ushort colorFocused,
        int x,
        int y,
        int maxWidth = 0,
        bool unicode = true,
        uchar font = 0,
        TEXT_ALIGN_TYPE align = TS_LEFT,
        ushort textFlags = 0,
        int maxLength = 0);
    virtual ~CGUITextEntry();

    class CEntryText m_Entry;

    virtual void PrepareTextures();

    virtual bool EntryPointerHere();

    virtual WISP_GEOMETRY::CSize GetSize();

    void SetGlobalColor(bool use, int color, int selected, int focused);

    void OnClick(CGump *gump, int x, int y);

    virtual void OnMouseEnter();
    virtual void OnMouseExit();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};

#endif
