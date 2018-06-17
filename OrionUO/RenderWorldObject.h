

#ifndef RENDERWORLDOBJECT_H
#define RENDERWORLDOBJECT_H

class CRenderWorldObject : public CRenderObject
{
protected:
    char m_Z = 0;

public:
    char GetZ() { return m_Z; };
    void SetZ(char val);
    short PriorityZ = 0;
    RENDER_OBJECT_TYPE RenderType = ROT_GAME_OBJECT;
#if UO_RENDER_LIST_SORT == 1
    uchar CurrentRenderIndex = 0;
    uchar UseInRender = 0;
#endif
    bool NoDrawTile = false;

public:
    CRenderWorldObject(
        RENDER_OBJECT_TYPE renderType,
        int serial,
        ushort graphic,
        ushort color,
        int x,
        int y,
        char z);
    virtual ~CRenderWorldObject();

    uchar m_DrawTextureColor[4];

    bool ProcessAlpha(int maxAlpha);

    bool RemovedFromRender();

    virtual void AddText(CTextData *td) {}

    virtual bool TextCanBeTransparent(CRenderTextObject *text) { return false; }

    virtual bool TranparentTest(int playerZ) { return false; }

    CRenderWorldObject *m_NextXY{ NULL };
    CRenderWorldObject *m_PrevXY{ NULL };

    class CLandObject *GetLand();

    virtual ushort GetLightID() { return 0; }

    virtual void Draw(int x, int y) {}

    virtual void Select(int x, int y) {}

    virtual void UpdateGraphicBySeason() {}

    void RemoveRender();

    virtual bool IsBackground() { return false; }
    virtual bool IsWeapon() { return false; }
    virtual bool IsTransparent() { return false; }
    virtual bool IsTranslucent() { return false; }
    virtual bool IsWall() { return false; }
    virtual bool IsDamaging() { return false; }
    virtual bool IsImpassable() { return false; }
    virtual bool IsWet() { return false; }
    virtual bool IsUnknown() { return false; }
    virtual bool IsSurface() { return false; }
    virtual bool IsBridge() { return false; }
    virtual bool IsStackable() { return false; }
    virtual bool IsWindow() { return false; }
    virtual bool IsNoShoot() { return false; }
    virtual bool IsPrefixA() { return false; }
    virtual bool IsPrefixAn() { return false; }
    virtual bool IsInternal() { return false; }
    virtual bool IsFoliage() { return false; }
    virtual bool IsPartialHue() { return false; }
    virtual bool IsUnknown1() { return false; }
    virtual bool IsMap() { return false; }
    virtual bool IsContainer() { return false; }
    virtual bool IsWearable() { return false; }
    virtual bool IsLightSource() { return false; }
    virtual bool IsAnimated() { return false; }
    virtual bool IsNoDiagonal() { return false; }
    virtual bool IsUnknown2() { return false; }
    virtual bool IsArmor() { return false; }
    virtual bool IsRoof() { return false; }
    virtual bool IsDoor() { return false; }
    virtual bool IsStairBack() { return false; }
    virtual bool IsStairRight() { return false; }

    virtual bool IsWorldObject() { return true; }

    virtual bool IsStaticGroupObject() { return false; }

    virtual bool IsLandObject() { return false; }
    virtual bool IsStaticObject() { return false; }
    virtual bool IsMultiObject() { return false; }
    virtual bool IsEffectObject() { return false; }

    virtual class CRenderStaticObject *StaticGroupObjectPtr() { return NULL; }
    virtual class CLandObject *LandObjectPtr() { return NULL; }
    virtual class CGameCharacter *GameCharacterPtr() { return NULL; }

    virtual void UpdateRealDrawCoordinates();
    virtual void UpdateDrawCoordinates();
};

#endif
