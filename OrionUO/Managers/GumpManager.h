

#ifndef GUMPMANAGER_H
#define GUMPMANAGER_H

class CGumpManager : public CBaseQueue
{
private:
    int GetNonpartyStatusbarsCount();

    void SaveDefaultGumpProperties(WISP_FILE::CBinaryFileWriter &writer, CGump *gump, int size);

public:
    CGumpManager()
        : CBaseQueue()
    {
    }
    virtual ~CGumpManager() {}

    void AddGump(CGump *obj);

    CGump *UpdateContent(int serial, int id, const GUMP_TYPE &type);

    CGump *UpdateGump(int serial, int id, const GUMP_TYPE &type);

    CGump *GetGump(int serial, int id, const GUMP_TYPE &type);

    CGump *GetTextEntryOwner();

    CGump *GumpExists(uintptr_t gumpID);

    void CloseGump(uint serial, uint ID, GUMP_TYPE Type);

    void RemoveGump(CGump *obj);

    void RedrawAll();

    void OnDelete();

    void RemoveRangedGumps();

    void PrepareContent();

    void RemoveMarked();

    void PrepareTextures();

    void Draw(bool blocked);

    void Select(bool blocked);

    void InitToolTip();

    void OnLeftMouseButtonDown(bool blocked);
    bool OnLeftMouseButtonUp(bool blocked);
    bool OnLeftMouseButtonDoubleClick(bool blocked);

    void OnRightMouseButtonDown(bool blocked);
    void OnRightMouseButtonUp(bool blocked);
    bool OnRightMouseButtonDoubleClick(bool blocked) { return false; }

    void OnMidMouseButtonScroll(bool up, bool blocked);

    void OnDragging(bool blocked);

    bool OnCharPress(const WPARAM &wParam, const LPARAM &lParam, bool blocked);
    bool OnKeyDown(const WPARAM &wParam, const LPARAM &lParam, bool blocked);

    void Load(const string &path);
    void Save(const string &path);
};

extern CGumpManager g_GumpManager;

#endif
