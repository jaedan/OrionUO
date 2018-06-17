

#ifndef GUIPAGE_H
#define GUIPAGE_H

class CGUIPage : public CBaseGUI
{
public:
    int Index = 0;

    CGUIPage(int index);
    virtual ~CGUIPage();
};

#endif
