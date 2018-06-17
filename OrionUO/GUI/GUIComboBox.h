

#ifndef GUICOMBOBOX_H
#define GUICOMBOBOX_H

class CGUIComboBox : public CBaseGUI
{
    ushort OpenGraphic = 0;

    bool CompositeBackground = false;

protected:
    int m_ShowItemsCount = 0;

public:
    int GetShowItemsCount() { return m_ShowItemsCount; };
    void SetShowItemsCount(int val);

    int Width = 0;

    int OpenedWidth = 0;

    int StartIndex = 0;

    int SelectedIndex = -1;

    int ListingDirection = 0;

    uint ListingTimer = 0;

    bool ShowMaximizedCenter = false;

    int TextOffsetY = 0;

private:
    CBaseGUI *SkipToStart();

    int m_MinimizedArrowX{ 0 };

    int m_ArrowX{ 0 };

    int m_OffsetY{ 0 };

    int m_StepY{ 0 };

    int m_WorkWidth{ 0 };

    int m_WorkHeight{ 0 };

    class CGUIText *Text{ NULL };

public:
    CGUIComboBox(
        int serial,
        ushort graphic,
        bool compositeBackground,
        ushort openGraphic,
        int x,
        int y,
        int width,
        int showItemsCount,
        bool showMaximizedCenter);
    virtual ~CGUIComboBox();

    void SetText(class CGUIText *text) { Text = text; }

    void RecalculateWidth();

    virtual WISP_GEOMETRY::CSize GetSize();

    virtual void PrepareTextures();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();

    CBaseGUI *SelectedItem();

    int IsSelectedItem();
};

#endif
