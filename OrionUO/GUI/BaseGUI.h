

#ifndef BASEGUI_H
#define BASEGUI_H

class CBaseGUI : public CRenderObject
{
public:
    GUMP_OBJECT_TYPE Type = GOT_NONE;

    bool MoveOnDrag = false;

    bool PartialHue = false;

    bool CheckPolygon = false;

    bool Enabled = true;

    bool Visible = true;

    bool SelectOnly = false;

    bool DrawOnly = false;

    uint ClilocID = 0;

    CTextData *TextData = nullptr;

    CBaseGUI(GUMP_OBJECT_TYPE type, int serial, ushort graphic, ushort color, int x, int y);
    virtual ~CBaseGUI();

    virtual bool EntryPointerHere() { return false; }

    virtual void SetShaderMode() {}

    virtual ushort GetDrawGraphic() { return Graphic; }

    virtual void Draw(bool checktrans = false) {}

    virtual bool Select() { return false; }

    virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(); }

    virtual bool IsGUI() { return true; }

    virtual bool IsHTMLGump() { return false; }

    virtual bool IsControlHTML() { return false; }

    virtual bool IsPressedOuthit() { return false; }
};

#endif
