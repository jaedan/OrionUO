

#ifndef TOOLTIP_H
#define TOOLTIP_H

class CToolTip
{
public:
    uint Timer = 0;
    wstring Data = L"";
    uint ClilocID = 0;
    int MaxWidth = 0;
    WISP_GEOMETRY::CPoint2Di Position = WISP_GEOMETRY::CPoint2Di();
    bool Use = false;

    CToolTip();
    ~CToolTip();

    void Reset();

    class CRenderObject *m_Object{ NULL };

    CGLTextTexture Texture;

    void CreateTextTexture(CGLTextTexture &texture, const wstring &str, int &width, int minWidth);

    void Set(const wstring &str, int maxWidth = 0);

    void Set(int clilocID, const string &str, int maxWidth = 0, bool toCamelCase = false);

    void Draw(int cursorWidth = 0, int cursorHeight = 0);
};

extern CToolTip g_ToolTip;

#endif
