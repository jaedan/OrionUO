

#ifndef PATHFINDER_H
#define PATHFINDER_H

const int PATHFINDER_MAX_NODES = 10000;

enum PATH_STEP_STATE
{
    PSS_NORMAL = 0,
    PSS_DEAD_OR_GM,
    PSS_ON_SEA_HORSE,
    PSS_FLYING
};

enum PATH_OBJECT_FLAGS
{
    POF_IMPASSABLE_OR_SURFACE = 0x00000001,
    POF_SURFACE = 0x00000002,
    POF_BRIDGE = 0x00000004,
    POF_NO_DIAGONAL = 0x00000008
};

class CPathFinder
{
public:
    bool AutoWalking = false;
    bool PathFindidngCanBeCancelled = false;
    bool IgnoreStaminaCheck = false;

protected:
    bool CalculateNewZ(int x, int y, char &z, int direction);

    int CalculateMinMaxZ(
        int &minZ, int &maxZ, int newX, int newY, int currentZ, int newDirection, int stepState);

    bool CreateItemsList(vector<CPathObject> &list, int x, int y, int stepState);

    WISP_GEOMETRY::CPoint2Di m_StartPoint{ WISP_GEOMETRY::CPoint2Di() };
    WISP_GEOMETRY::CPoint2Di m_EndPoint{ WISP_GEOMETRY::CPoint2Di() };

    int m_GoalNode{ 0 };
    bool m_GoalFound{ 0 };

    int m_ActiveOpenNodes{ 0 };
    int m_ActiveClosedNodes{ 0 };

    int m_PathFindDistance{ 0 };

    CPathNode m_OpenList[PATHFINDER_MAX_NODES];
    CPathNode m_ClosedList[PATHFINDER_MAX_NODES];

    CPathNode *m_Path[PATHFINDER_MAX_NODES];

    int m_PointIndex{ 0 };

    int m_PathSize{ 0 };

    int GetGoalDistCost(const WISP_GEOMETRY::CPoint2Di &p, int cost);

    bool DoesNotExistOnOpenList(int x, int y, int z);

    bool DoesNotExistOnClosedList(int x, int y, int z);

    int
    AddNodeToList(int list, int direction, int x, int y, int z, CPathNode *parentNode, int cost);

    bool OpenNodes(CPathNode *node);

    int FindCheapestNode();

    bool FindPath(int maxNodes);

public:
    CPathFinder();
    virtual ~CPathFinder();

    void GetNewXY(uchar direction, int &x, int &y);

    bool CanWalk(uchar &direction, int &x, int &y, char &z);

    bool WalkTo(int x, int y, int z, int distance);

    void ProcessAutowalk();

    void StopAutoWalk();
};

extern CPathFinder g_PathFinder;

#endif
