

#ifndef GUISHOPITEM_H
#define GUISHOPITEM_H

class CGUIShopItem : public CBaseGUI
{
public:
    uint Count = 0;

    uint Price = 0;

    string Name = "";

    bool Selected = false;

    bool NameFromCliloc = false;

private:
    CGLTextTexture m_NameText{ CGLTextTexture() };
    CGLTextTexture m_CountText{ CGLTextTexture() };

    int m_ImageOffset{ 0 };

    int m_TextOffset{ 0 };

    int m_MaxOffset{ 0 };

public:
    CGUIShopItem(
        int serial,
        ushort graphic,
        ushort color,
        int count,
        int price,
        const string &name,
        int x,
        int y);
    virtual ~CGUIShopItem();

    void OnClick();

    void CreateNameText();

    void UpdateOffsets();

    void CreateCountText(int lostCount);

    virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(200, m_MaxOffset + 20); }

    virtual void PrepareTextures();

    virtual void SetShaderMode();

    virtual void Draw(bool checktrans = false);
    virtual bool Select();
};

#endif
