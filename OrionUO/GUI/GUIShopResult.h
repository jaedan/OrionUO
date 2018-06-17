

#ifndef GUISHOPRESULT_H
#define GUISHOPRESULT_H

class CGUIShopResult : public CBaseGUI
{
public:
    uint Price = 0;

    string Name = "";

private:
    CGLTextTexture m_NameText{ CGLTextTexture() };

public:
    CGUIShopResult(class CGUIShopItem *shopItem, int x, int y);
    virtual ~CGUIShopResult();

    class CGUIMinMaxButtons *m_MinMaxButtons{ NULL };

    CBaseGUI *SelectedItem();

    virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(200, m_NameText.Height); }

    virtual void PrepareTextures();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};

#endif
