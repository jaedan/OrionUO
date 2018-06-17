

#ifndef GUIMENUOBJECT_H
#define GUIMENUOBJECT_H

class CGUIMenuObject : public CGUITilepic
{
    string Text = "";

public:
    CGUIMenuObject(int serial, ushort graphic, ushort color, int x, int y, const string &text);
    virtual ~CGUIMenuObject();

    virtual bool Select();

    virtual void OnMouseEnter();
    virtual void OnMouseExit();
};

#endif
