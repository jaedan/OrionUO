

#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

class CRenderObject : public CBaseQueueItem
{
public:
    uint Serial = 0;
    ushort Graphic = 0;
    ushort Color = 0;

protected:
    int m_X = 0;

public:
    int GetX() { return m_X; };
    void SetX(int val);

protected:
    int m_Y = 0;

public:
    int GetY() { return m_Y; };
    void SetY(int val);
    int RealDrawX = 0;
    int RealDrawY = 0;
    int DrawX = 0;
    int DrawY = 0;
    bool Changed = true;

    CRenderObject(int serial, ushort graphic, ushort color, short x, short y);
    virtual ~CRenderObject();

    virtual void PrepareTextures() {}

    virtual void OnMouseEnter() {}
    virtual void OnMouseExit() {}

    virtual bool IsGUI() { return false; }
    virtual bool IsText() { return false; }
    virtual bool IsWorldObject() { return false; }
    virtual bool IsGameObject() { return false; }

    virtual void UpdateRealDrawCoordinates() {}
    virtual void UpdateDrawCoordinates() {}
    virtual void UpdateTextCoordinates() {}
    virtual void FixTextCoordinates() {}
};

#endif
