

#ifndef GUISKILLITEM_H
#define GUISKILLITEM_H

class CGUISkillItem : public CBaseGUI
{
public:
    int Index = 0;

protected:
    uchar m_Status = 0;

public:
    uchar GetStatus() { return m_Status; };
    void SetStatus(uchar val);

private:
    CGLTextTexture m_NameText{ CGLTextTexture() };
    CGLTextTexture m_ValueText{ CGLTextTexture() };

    ushort GetStatusButtonGraphic();

public:
    CGUISkillItem(int serial, int useSerial, int statusSerial, int index, int x, int y);
    virtual ~CGUISkillItem();

    CGUIButton *m_ButtonUse{ NULL };
    CGUIButton *m_ButtonStatus{ NULL };

    void CreateValueText(bool showReal = false, bool showCap = false);

    virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(255, 17); }

    virtual void PrepareTextures();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();

    CBaseGUI *SelectedItem();
};

#endif
