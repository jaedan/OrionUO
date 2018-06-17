

#ifndef CUSTOMHOUSEMULTIOBJECT_H
#define CUSTOMHOUSEMULTIOBJECT_H

class CCustomHouseMultiObject : public CMultiObject
{
public:
    CCustomHouseMultiObject(ushort graphic, ushort color, short x, short y, char z, int flags);

    virtual ~CCustomHouseMultiObject();

    virtual bool IsCustomHouseMulti() { return true; }
};

#endif
