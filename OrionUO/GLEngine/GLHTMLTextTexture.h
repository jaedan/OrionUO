

#ifndef GLHTMLTEXTTEXTURE_H
#define GLHTMLTEXTTEXTURE_H

class CGLHTMLTextTexture : public CGLTextTexture
{
private:
    deque<WEB_LINK_RECT> m_WebLinkRect;

public:
    CGLHTMLTextTexture();
    virtual ~CGLHTMLTextTexture();

    void ClearWebLink() { m_WebLinkRect.clear(); }

    void AddWebLink(WEB_LINK_RECT &wl) { m_WebLinkRect.push_back(wl); }

    ushort WebLinkUnderMouse(int x, int y);
};

#endif
