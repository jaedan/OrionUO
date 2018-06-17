

#ifndef GUISKILLGROUP_H
#define GUISKILLGROUP_H

class CGUISkillGroup : public CBaseGUI
{
protected:
    bool m_Minimized = false;

public:
    bool GetMinimized() { return m_Minimized; };
    void SetMinimized(bool val);

public:
    CGUISkillGroup(int serial, int minimizeSerial, class CSkillGroupObject *group, int x, int y);
    virtual ~CGUISkillGroup();

    CGUIButton *m_Minimizer{ NULL };
    CGUITextEntry *m_Name{ NULL };

    void UpdateDataPositions();

    virtual WISP_GEOMETRY::CSize GetSize();

    virtual void PrepareTextures();

    virtual bool EntryPointerHere();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();

    CBaseGUI *SelectedItem();
};

#endif
