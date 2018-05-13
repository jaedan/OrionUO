

#include "stdafx.h"

CPathFinder g_PathFinder;

CPathFinder::CPathFinder()
{
}

CPathFinder::~CPathFinder()
{
}

bool CPathFinder::CreateItemsList(vector<CPathObject> &list, int x, int y, int stepState)
{
    WISPFUN_DEBUG("c177_f1");
    int blockX = x / 8;
    int blockY = y / 8;

    uint blockIndex = (blockX * g_MapBlockSize[g_CurrentMap].Height) + blockY;

    if (blockIndex >= g_MapManager.MaxBlockIndex)
        return false;

    CMapBlock *block = g_MapManager.GetBlock(blockIndex);

    if (block == NULL)
    {
        block = g_MapManager.AddBlock(blockIndex);
        block->X = blockX;
        block->Y = blockY;
        g_MapManager.LoadBlock(block);
    }

    int bx = x % 8;
    int by = y % 8;

    bool ignoreGameCharacters =
        (IgnoreStaminaCheck || (stepState == PSS_DEAD_OR_GM) || g_Player->IgnoreCharacters() ||
         g_Player->Stam >= g_Player->MaxStam);
    bool isGM = (g_Player->Graphic == 0x03DB);

    for (CRenderWorldObject *obj = block->GetRender(bx, by); obj != NULL; obj = obj->m_NextXY)
    {
        if (g_CustomHouseGump != NULL && obj->GetZ() < g_Player->GetZ())
            continue;

        ushort graphic = obj->Graphic;

        if (obj->IsLandObject())
        {
            if ((graphic < 0x01AE && graphic != 2) || (graphic > 0x01B5 && graphic != 0x01DB))
            {
                CLandObject *land = (CLandObject *)obj;

                uint flags = POF_IMPASSABLE_OR_SURFACE;
                uint64 tiledataFlags = g_Orion.GetLandFlags(graphic);

                if (stepState == PSS_ON_SEA_HORSE)
                {
                    if (IsWet(tiledataFlags))
                        flags = POF_IMPASSABLE_OR_SURFACE | POF_SURFACE | POF_BRIDGE;
                }
                else
                {
                    if (!IsImpassable(tiledataFlags))
                        flags = POF_IMPASSABLE_OR_SURFACE | POF_SURFACE | POF_BRIDGE;

                    if (stepState == PSS_FLYING && IsNoDiagonal(tiledataFlags))
                        flags |= POF_NO_DIAGONAL;
                }

                int landMinZ = land->MinZ;
                int landAverageZ = land->AverageZ;
                int landHeight = landAverageZ - landMinZ;

                list.push_back(CPathObject(flags, landMinZ, landAverageZ, landHeight, obj));
            }
        }
        else if (obj->IsStaticGroupObject())
        {
            bool canBeAdd = true;
            bool dropFlags = false;

            STATIC_TILES *tileInfo = obj->StaticGroupObjectPtr()->GetStaticData();

            if (obj->IsGameObject())
            {
                CGameObject *go = (CGameObject *)obj;

                if (go->NPC)
                {
                    CGameCharacter *gc = (CGameCharacter *)obj;

                    if (!ignoreGameCharacters && !gc->Dead() && !gc->IgnoreCharacters())
                        list.push_back(CPathObject(
                            POF_IMPASSABLE_OR_SURFACE,
                            obj->GetZ(),
                            obj->GetZ() + DEFAULT_CHARACTER_HEIGHT,
                            DEFAULT_CHARACTER_HEIGHT,
                            obj));

                    canBeAdd = false;
                }
                else if (((CGameItem *)obj)->MultiBody || obj->IsInternal())
                    canBeAdd = false;
                else if (
                    stepState == PSS_DEAD_OR_GM &&
                    (go->IsDoor() || tileInfo->Weight <= 0x5A || (isGM && !go->Locked())))
                    dropFlags = true;
                else
                    dropFlags = ((graphic >= 0x3946 && graphic <= 0x3964) || graphic == 0x0082);
            }
            else if (
                g_CustomHouseGump != NULL && obj->IsMultiObject() &&
                ((CMultiObject *)obj)->IsCustomHouseMulti() &&
                !(((CMultiObject *)obj)->State & CHMOF_GENERIC_INTERNAL))
                canBeAdd = false;

            if (canBeAdd)
            {
                uint flags = 0;

                if (stepState == PSS_ON_SEA_HORSE)
                {
                    if (obj->IsWet())
                        flags = POF_SURFACE | POF_BRIDGE;
                }
                else
                {
                    if (obj->IsImpassable() || obj->IsSurface())
                        flags = POF_IMPASSABLE_OR_SURFACE;

                    if (!obj->IsImpassable())
                    {
                        if (obj->IsSurface())
                            flags |= POF_SURFACE;

                        if (obj->IsBridge())
                            flags |= POF_BRIDGE;
                    }

                    if (stepState == PSS_DEAD_OR_GM)
                    {
                        if (graphic <= 0x0846)
                        {
                            if (!(graphic != 0x0846 && graphic != 0x0692 &&
                                  (graphic <= 0x06F4 || graphic > 0x06F6)))
                                dropFlags = true;
                        }
                        else if (graphic == 0x0873)
                            dropFlags = true;
                    }

                    if (dropFlags)
                        flags &= 0xFFFFFFFE;

                    if (stepState == PSS_FLYING && obj->IsNoDiagonal())
                        flags |= POF_NO_DIAGONAL;
                }

                if (flags)
                {
                    int objZ = obj->GetZ();
                    int staticHeight = tileInfo->Height;
                    int staticAverageZ = staticHeight;

                    if (obj->IsBridge())
                        staticAverageZ /= 2;

                    list.push_back(
                        CPathObject(flags, objZ, staticAverageZ + objZ, staticHeight, obj));
                }
            }
        }
    }

    return !list.empty();
}

int CPathFinder::CalculateMinMaxZ(
    int &minZ, int &maxZ, int newX, int newY, int currentZ, int newDirection, int stepState)
{
    WISPFUN_DEBUG("c177_f2");
    const int offsetX[10] = { 0, 1, 1, 1, 0, -1, -1, -1, 0, 1 };
    const int offsetY[10] = { -1, -1, 0, 1, 1, 1, 0, -1, -1, -1 };

    minZ = -128;
    maxZ = currentZ;

    newDirection &= 7;
    int direction = (newDirection ^ 4);

    newX += offsetX[direction];
    newY += offsetY[direction];

    vector<CPathObject> list;

    if (!CreateItemsList(list, newX, newY, stepState) || !list.size())
        return 0;

    for (const CPathObject &obj : list)
    {
        CRenderWorldObject *rwo = obj.m_Object;
        int averageZ = obj.AverageZ;

        if (averageZ <= currentZ && rwo->IsLandObject() && ((CLandObject *)rwo)->IsStretched)
        {
            int avgZ = ((CLandObject *)rwo)->CalculateCurrentAverageZ(newDirection);

            if (minZ < avgZ)
                minZ = avgZ;

            if (maxZ < avgZ)
                maxZ = avgZ;
        }
        else
        {
            if ((obj.Flags & POF_IMPASSABLE_OR_SURFACE) && averageZ <= currentZ && minZ < averageZ)
                minZ = averageZ;

            if ((obj.Flags & POF_BRIDGE) && currentZ == averageZ)
            {
                int z = obj.Z;
                int height = z + obj.Height;

                if (maxZ < height)
                    maxZ = height;

                if (minZ > z)
                    minZ = z;
            }
        }
    }

    maxZ += 2;

    return maxZ;
}

bool CPathFinder::CalculateNewZ(int x, int y, char &z, int direction)
{
    WISPFUN_DEBUG("c177_f3");
    int stepState = PSS_NORMAL;

    if (g_Player->Dead() || g_Player->Graphic == 0x03DB)
    {
        stepState = PSS_DEAD_OR_GM;
    }
    else
    {
        if (g_Player->Flying())
            stepState = PSS_FLYING;
        else
        {
            CGameItem *mount = g_Player->FindLayer(OL_MOUNT);

            if (mount != NULL && mount->Graphic == 0x3EB3)
                stepState = PSS_ON_SEA_HORSE;
        }
    }

    int minZ = -128;
    int maxZ = z;

    CalculateMinMaxZ(minZ, maxZ, x, y, z, direction, stepState);

    vector<CPathObject> list;

    if (g_CustomHouseGump != NULL)
    {
        RECT rect = { g_CustomHouseGump->StartPos.X,
                      g_CustomHouseGump->StartPos.Y,
                      g_CustomHouseGump->EndPos.X,
                      g_CustomHouseGump->EndPos.Y };
        POINT pos = { x, y };

        if (!PtInRect(&rect, pos))
            return false;
    }

    if (!CreateItemsList(list, x, y, stepState) || !list.size())
        return false;

    auto compareFunction = [](const void *obj1, const void *obj2) {
        int result = 0;

        if (obj1 != NULL && obj2 != NULL)
        {
            result = ((CPathObject *)obj1)->Z - ((CPathObject *)obj2)->Z;

            if (!result)
                result = (((CPathObject *)obj1)->Height - ((CPathObject *)obj2)->Height);
        }

        return result;
    };

    std::qsort(&list[0], list.size(), sizeof(CPathObject), compareFunction);

    list.push_back(CPathObject(POF_IMPASSABLE_OR_SURFACE, 128, 128, 128, NULL));

    int resultZ = -128;

    if (z < minZ)
        z = (char)minZ;

    int currentTempObjZ = 1000000;
    int currentZ = -128;

    int listSize = (int)list.size();

    IFOR (i, 0, listSize)
    {
        const CPathObject &obj = list[i];

        if ((obj.Flags & POF_NO_DIAGONAL) && stepState == PSS_FLYING)
        {
            int objAverageZ = obj.AverageZ;

            int delta = abs(objAverageZ - (int)z);

            if (delta <= 25)
            {
                if (objAverageZ != -128)
                    resultZ = objAverageZ;
                else
                    resultZ = currentZ;

                break;
            }
        }

        if (obj.Flags & POF_IMPASSABLE_OR_SURFACE)
        {
            int objZ = obj.Z;

            if (objZ - minZ >= DEFAULT_BLOCK_HEIGHT)
            {
                DFOR (j, i - 1, 0)
                {
                    const CPathObject &tempObj = list[j];

                    if (tempObj.Flags & (POF_SURFACE | POF_BRIDGE))
                    {
                        int tempAverageZ = tempObj.AverageZ;

                        if (tempAverageZ >= currentZ &&
                            objZ - tempAverageZ >= DEFAULT_BLOCK_HEIGHT &&
                            ((tempAverageZ <= maxZ && (tempObj.Flags & POF_SURFACE)) ||
                             ((tempObj.Flags & POF_BRIDGE) && tempObj.Z <= maxZ)))
                        {
                            int delta = abs((int)z - tempAverageZ);

                            if (delta < currentTempObjZ)
                            {
                                currentTempObjZ = delta;
                                resultZ = tempAverageZ;
                            }
                        }
                    }
                }
            }

            if (g_ConfigManager.AutomaticallyOpenDoors && obj.m_Object != NULL &&
                obj.m_Object->IsDoor())
            {
                g_Orion.OpenDoor();
            }

            int averageZ = obj.AverageZ;

            if (minZ < averageZ)
                minZ = averageZ;

            if (currentZ < averageZ)
                currentZ = averageZ;
        }
    }

    z = (char)resultZ;

    return (resultZ != -128);
}

void CPathFinder::GetNewXY(Direction direction, int &x, int &y)
{
    WISPFUN_DEBUG("c177_f4");
    switch (direction)
    {
        case DIR_NORTH:
        {
            y--;
            break;
        }
        case DIR_NORTHEAST:
        {
            x++;
            y--;
            break;
        }
        case DIR_EAST:
        {
            x++;
            break;
        }
        case DIR_SOUTHEAST:
        {
            x++;
            y++;
            break;
        }
        case DIR_SOUTH:
        {
            y++;
            break;
        }
        case DIR_SOUTHWEST:
        {
            x--;
            y++;
            break;
        }
        case DIR_WEST:
        {
            x--;
            break;
        }
        case DIR_NORTHWEST:
        {
            x--;
            y--;
            break;
        }
    }
}

bool CPathFinder::CanWalk(Direction &direction, int &x, int &y, char &z)
{
    WISPFUN_DEBUG("c177_f5");

    const uchar directions[3] = { 0, 1, -1 };
    bool allowed = false;

    for (int i = 0; i < 3; i++)
    {
        int newX = x;
        int newY = y;
        char newZ = z;
        Direction newDirection = (Direction)((direction + directions[i]) % 8);

        GetNewXY(newDirection, newX, newY);
        allowed = CalculateNewZ(newX, newY, newZ, newDirection);

        if (!allowed)
        {
            continue;
        }

        if (newDirection % 2)
        {
            const uchar adjacents[2] = { -1, 1 };

            for (int j = 0; j < 2; j++)
            {
                int testX = x;
                int testY = y;
                char testZ = z;
                Direction testDirection = (Direction)((newDirection + adjacents[j]) % 8);

                GetNewXY(testDirection, testX, testY);
                allowed = CalculateNewZ(testX, testY, testZ, testDirection);

                if (!allowed)
                {
                    break;
                }
            }
        }

        if (allowed)
        {
            x = newX;
            y = newY;
            z = newZ;
            direction = newDirection;
            break;
        }
    }

    return allowed;
}

int CPathFinder::GetGoalDistCost(const WISP_GEOMETRY::CPoint2Di &p, int cost)
{
    WISPFUN_DEBUG("c177_f8");
    return (abs(m_EndPoint.X - p.X) + abs(m_EndPoint.Y - p.Y)) * cost;
}

bool CPathFinder::DoesNotExistOnOpenList(int x, int y, int z)
{
    WISPFUN_DEBUG("c177_f9");
    bool result = false;

    IFOR (i, 0, PATHFINDER_MAX_NODES)
    {
        CPathNode &node = m_OpenList[i];

        if (node.Used && node.X == x && node.Y == y && node.Z == z)
        {
            result = true;
            break;
        }
    }

    return result;
}

bool CPathFinder::DoesNotExistOnClosedList(int x, int y, int z)
{
    WISPFUN_DEBUG("c177_f10");
    bool result = false;

    IFOR (i, 0, PATHFINDER_MAX_NODES)
    {
        CPathNode &node = m_ClosedList[i];

        if (node.Used && node.X == x && node.Y == y && node.Z == z)
        {
            result = true;
            break;
        }
    }

    return result;
}

int CPathFinder::AddNodeToList(
    int list, int direction, int x, int y, int z, CPathNode *parentNode, int cost)
{
    WISPFUN_DEBUG("c177_f11");
    if (!list)
    {
        if (!DoesNotExistOnClosedList(x, y, z))
        {
            if (!DoesNotExistOnOpenList(x, y, z))
            {
                IFOR (i, 0, PATHFINDER_MAX_NODES)
                {
                    CPathNode &node = m_OpenList[i];

                    if (!node.Used)
                    {
                        node.Used = true;

                        node.Dir = (Direction)direction;
                        node.X = x;
                        node.Y = y;
                        node.Z = z;
                        WISP_GEOMETRY::CPoint2Di p(x, y);

                        node.DistFromGoalCost = GetGoalDistCost(p, cost);
                        node.DistFromStartCost = parentNode->DistFromStartCost + cost;
                        node.Cost = node.DistFromGoalCost + node.DistFromStartCost;

                        node.m_Parent = parentNode;

                        if (GetDistance(m_EndPoint, p) <= m_PathFindDistance)
                        {
                            m_GoalFound = true;
                            m_GoalNode = (int)i;
                        }

                        m_ActiveOpenNodes++;

                        return (int)i;
                    }
                }
            }
            else
            {
                IFOR (i, 0, PATHFINDER_MAX_NODES)
                {
                    CPathNode &node = m_OpenList[i];

                    if (node.Used)
                    {
                        if (node.X == x && node.Y == y && node.Z == z)
                        {
                            int startCost = parentNode->DistFromStartCost + cost;

                            if (node.DistFromStartCost > startCost)
                            {
                                node.m_Parent = parentNode;
                                node.DistFromStartCost = startCost + cost;
                                node.Cost = node.DistFromGoalCost + node.DistFromStartCost;
                            }

                            return (int)i;
                        }
                    }
                }
            }
        }
        else
            return 0;
    }
    else
    {
        parentNode->Used = false;

        IFOR (i, 0, PATHFINDER_MAX_NODES)
        {
            CPathNode &node = m_ClosedList[i];

            if (!node.Used)
            {
                node.Used = true;

                node.DistFromGoalCost = parentNode->DistFromGoalCost;
                node.DistFromStartCost = parentNode->DistFromStartCost;
                node.Cost = node.DistFromGoalCost + node.DistFromStartCost;
                node.Dir = parentNode->Dir;
                node.X = parentNode->X;
                node.Y = parentNode->Y;
                node.Z = parentNode->Z;
                node.m_Parent = parentNode->m_Parent;

                m_ActiveOpenNodes--;
                m_ActiveClosedNodes++;

                return (int)i;
            }
        }
    }

    return -1;
}

bool CPathFinder::OpenNodes(CPathNode *node)
{
    WISPFUN_DEBUG("c177_f12");
    bool found = false;

    for (uint8_t i = 0; i < DIR_INVALID; i++)
    {
        Direction direction = (Direction)i;
        int x = node->X;
        int y = node->Y;
        char z = (char)node->Z;

        uchar oldDirection = direction;

        if (CanWalk(direction, x, y, z))
        {
            if (direction != oldDirection)
                continue;

            int diagonal = i % 2;

            if (diagonal)
            {
                Direction wantDirection = (Direction)i;
                int wantX = node->X;
                int wantY = node->Y;

                GetNewXY(wantDirection, wantX, wantY);

                if (x != wantX || y != wantY)
                    diagonal = -1;
            }

            if (diagonal >= 0 && AddNodeToList(0, direction, x, y, z, node, 1 + diagonal) != -1)
                found = true;
            else
                continue;
        }
    }

    return found;
}

int CPathFinder::FindCheapestNode()
{
    WISPFUN_DEBUG("c177_f13");
    int cheapestCost = 9999999;
    int cheapestNode = -1;

    IFOR (i, 0, PATHFINDER_MAX_NODES)
    {
        if (m_OpenList[i].Used)
        {
            if (m_OpenList[i].Cost < cheapestCost)
            {
                cheapestNode = (int)i;
                cheapestCost = m_OpenList[i].Cost;
            }
        }
    }

    int result = -1;

    if (cheapestNode != -1)
        result = AddNodeToList(1, 0, 0, 0, 0, &m_OpenList[cheapestNode], 2);

    return result;
}

bool CPathFinder::FindPath(int maxNodes)
{
    WISPFUN_DEBUG("c177_f14");
    int curNode = 0;

    m_ClosedList[0].Used = true;

    m_ClosedList[0].X = m_StartPoint.X;
    m_ClosedList[0].Y = m_StartPoint.Y;
    m_ClosedList[0].Z = g_Player->GetZ();
    m_ClosedList[0].m_Parent = NULL;

    m_ClosedList[0].DistFromGoalCost = GetGoalDistCost(m_StartPoint, 0);
    m_ClosedList[0].Cost = m_ClosedList[0].DistFromGoalCost;

    while (AutoWalking)
    {
        OpenNodes(&m_ClosedList[curNode]);

        if (m_GoalFound)
        {
            int totalNodes = 0;

            CPathNode *GoalNode = &m_OpenList[m_GoalNode];

            while (GoalNode->m_Parent != NULL && GoalNode != GoalNode->m_Parent)
            {
                GoalNode = GoalNode->m_Parent;
                totalNodes++;
            };

            totalNodes++;

            m_PathSize = totalNodes;

            GoalNode = &m_OpenList[m_GoalNode];

            while (totalNodes > 0)
            {
                totalNodes--;
                m_Path[totalNodes] = GoalNode;
                GoalNode = GoalNode->m_Parent;
            };

            break;
        }

        curNode = FindCheapestNode();

        if (curNode == -1)
            return false;

        if (m_ActiveClosedNodes >= maxNodes)
            return false;
    }

    return true;
}

bool CPathFinder::WalkTo(int x, int y, int z, int distance)
{
    WISPFUN_DEBUG("c177_f15");
    IFOR (i, 0, PATHFINDER_MAX_NODES)
        m_OpenList[i].Reset();

    IFOR (i, 0, PATHFINDER_MAX_NODES)
        m_ClosedList[i].Reset();

    m_StartPoint.X = g_Player->GetX();
    m_StartPoint.Y = g_Player->GetY();
    m_EndPoint.X = x;
    m_EndPoint.Y = y;
    m_GoalNode = 0;
    m_GoalFound = false;
    m_ActiveOpenNodes = 0;
    m_ActiveClosedNodes = 0;
    m_PathFindDistance = distance;
    m_PathSize = 0;
    PathFindidngCanBeCancelled = true;

    StopAutoWalk();
    AutoWalking = true;

    if (FindPath(PATHFINDER_MAX_NODES))
    {
        m_PointIndex = 1;
        ProcessAutowalk();
    }
    else
        AutoWalking = false;

    return (m_PathSize != 0);
}

void CPathFinder::ProcessAutowalk()
{
    WISPFUN_DEBUG("c177_f16");
    if (AutoWalking && g_Player != NULL && !g_DeathScreenTimer &&
        g_Player->m_RequestedSteps.size() < MAX_STEPS_COUNT &&
        g_Player->LastStepRequestTime <= g_Ticks)
    {
        if (m_PointIndex >= 0 && m_PointIndex < m_PathSize)
        {
            CPathNode *p = m_Path[m_PointIndex];

            int x, y;
            char z;
            Direction olddir;

            g_Player->GetEndPosition(x, y, z, olddir);

            if (olddir == p->Dir)
                m_PointIndex++;

            if (!g_Player->Walk(p->Dir, g_ConfigManager.AlwaysRun))
                StopAutoWalk();
        }
        else
            StopAutoWalk();
    }
}

void CPathFinder::StopAutoWalk()
{
    WISPFUN_DEBUG("c177_f17");
    AutoWalking = false;
    m_PathSize = 0;
}
