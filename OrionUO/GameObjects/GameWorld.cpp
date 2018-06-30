

#include "stdafx.h"

CGameWorld *g_World = NULL;

CGameWorld::CGameWorld(int serial)
{
    WISPFUN_DEBUG("c22_f1");
    CreatePlayer(serial);
}

CGameWorld::~CGameWorld()
{
    WISPFUN_DEBUG("c22_f2");
    RemovePlayer();

    CGameObject *obj = m_Items;
    while (obj != NULL)
    {
        CGameObject *next = (CGameObject *)obj->m_Next;
        RemoveObject(obj);

        obj = next;
    }

    m_Map.clear();

    m_Items = NULL;
}

void CGameWorld::ResetObjectHandlesState()
{
    QFOR(obj, m_Items, CGameObject *)
    obj->ClosedObjectHandle = false;
}

void CGameWorld::ProcessSound(int ticks, CGameCharacter *gc)
{
    WISPFUN_DEBUG("c22_f3");
    if (g_ConfigManager.FootstepsSound && gc->IsHuman() && !gc->Hidden())
    {
        if (gc->IsMoving() && gc->LastStepSoundTime < ticks)
        {
            int incID = gc->StepSoundOffset;
            int soundID = 0x012B;
            int delaySound = 400;

            if (gc->FindLayer(OL_MOUNT) != NULL)
            {
                if (gc->m_Steps.back().run)
                {
                    soundID = 0x0129;
                    delaySound = 150;
                }
                else
                {
                    incID = 0;
                    delaySound = 350;
                }
            }

            delaySound = delaySound * 13 / 10;

            soundID += incID;
            gc->StepSoundOffset = (incID + 1) % 2;

            g_Orion.PlaySoundEffect(soundID, g_SoundManager.GetVolumeValue());

            gc->LastStepSoundTime = ticks + delaySound;
        }
    }
}

void CGameWorld::ProcessAnimation()
{
    WISPFUN_DEBUG("c22_f4");
    deque<CGameObject *> toRemove;

    QFOR(obj, m_Items, CGameObject *)
    {
        if (obj->NPC)
        {
            CGameCharacter *gc = obj->GameCharacterPtr();
            uchar dir = gc->GetAnimationDirection();
            gc->ProcessAnimation();

            ProcessSound(g_Ticks, gc);

            if (gc->LastAnimationChangeTime < g_Ticks && !gc->NoIterateAnimIndex())
            {
                char frameIndex = gc->AnimIndex;

                if (gc->AnimationFromServer && !gc->AnimationDirection)
                    frameIndex--;
                else
                    frameIndex++;

                ushort id = gc->GetMountAnimation();
                int animGroup = gc->GetAnimationGroup(id);
                gc->ProcessGargoyleAnims(animGroup);

                CGameItem *mount = gc->FindLayer(OL_MOUNT);
                if (mount != NULL)
                {
                    switch (animGroup)
                    {
                        case PAG_FIDGET_1:
                        case PAG_FIDGET_2:
                        case PAG_FIDGET_3:
                        {
                            id = mount->GetMountAnimation();
                            animGroup = gc->GetAnimationGroup(id);
                            break;
                        }
                        default:
                            break;
                    }
                }

                bool mirror = false;

                g_AnimationManager.GetAnimDirection(dir, mirror);

                int currentDelay = CHARACTER_ANIMATION_DELAY;

                if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT && dir < 5)
                {
                    CTextureAnimationDirection &direction =
                        g_AnimationManager.m_DataIndex[id].m_Groups[animGroup].m_Direction[dir];
                    g_AnimationManager.AnimID = id;
                    g_AnimationManager.AnimGroup = animGroup;
                    g_AnimationManager.Direction = dir;
                    if (direction.FrameCount == 0)
                        g_AnimationManager.LoadDirectionGroup(direction);

                    if (direction.Address != 0 || direction.IsUOP)
                    {
                        direction.LastAccessTime = g_Ticks;
                        int fc = direction.FrameCount;

                        if (gc->AnimationFromServer)
                        {
                            currentDelay += currentDelay * (int)(gc->AnimationInterval + 1);

                            if (!gc->AnimationFrameCount)
                                gc->AnimationFrameCount = fc;
                            else
                                fc = gc->AnimationFrameCount;

                            if (gc->AnimationDirection)
                            {
                                if (frameIndex >= fc)
                                {
                                    frameIndex = 0;

                                    if (gc->AnimationRepeat)
                                    {
                                        uchar repCount = gc->AnimationRepeatMode;

                                        if (repCount == 2)
                                        {
                                            repCount--;
                                            gc->AnimationRepeatMode = repCount;
                                        }
                                        else if (repCount == 1)
                                            gc->SetAnimation(0xFF);
                                    }
                                    else
                                        gc->SetAnimation(0xFF);
                                }
                            }
                            else
                            {
                                if (frameIndex < 0)
                                {
                                    if (!fc)
                                        frameIndex = 0;
                                    else
                                        frameIndex = fc - 1;

                                    if (gc->AnimationRepeat)
                                    {
                                        uchar repCount = gc->AnimationRepeatMode;

                                        if (repCount == 2)
                                        {
                                            repCount--;
                                            gc->AnimationRepeatMode = repCount;
                                        }
                                        else if (repCount == 1)
                                            gc->SetAnimation(0xFF);
                                    }
                                    else
                                        gc->SetAnimation(0xFF);
                                }
                            }
                        }
                        else
                        {
                            if (frameIndex >= fc)
                            {
                                frameIndex = 0;

                                if (obj->Serial & 0x80000000)
                                    toRemove.push_back(obj);
                            }
                        }

                        gc->AnimIndex = frameIndex;
                    }
                    else if (obj->Serial & 0x80000000)
                        toRemove.push_back(obj);
                }
                else if (obj->Serial & 0x80000000)
                    toRemove.push_back(obj);

                gc->LastAnimationChangeTime = g_Ticks + currentDelay;
            }
        }
        else if (obj->IsCorpse())
        {
            CGameItem *gi = (CGameItem *)obj;
            uchar dir = gi->Layer;

            if (obj->LastAnimationChangeTime < g_Ticks)
            {
                char frameIndex = obj->AnimIndex + 1;

                ushort id = obj->GetMountAnimation();

                bool mirror = false;

                g_AnimationManager.GetAnimDirection(dir, mirror);

                if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT && dir < 5)
                {
                    int animGroup = g_AnimationManager.GetDieGroupIndex(id, gi->UsedLayer);

                    CTextureAnimationDirection &direction =
                        g_AnimationManager.m_DataIndex[id].m_Groups[animGroup].m_Direction[dir];
                    g_AnimationManager.AnimID = id;
                    g_AnimationManager.AnimGroup = animGroup;
                    g_AnimationManager.Direction = dir;
                    if (direction.FrameCount == 0)
                        g_AnimationManager.LoadDirectionGroup(direction);

                    if (direction.Address != 0 || direction.IsUOP)
                    {
                        direction.LastAccessTime = g_Ticks;
                        int fc = direction.FrameCount;

                        if (frameIndex >= fc)
                        {
                            frameIndex = fc - 1;
                        }

                        obj->AnimIndex = frameIndex;
                    }
                }

                obj->LastAnimationChangeTime = g_Ticks + CHARACTER_ANIMATION_DELAY;
            }
        }

        obj->UpdateEffects();
    }

    if (!toRemove.empty())
    {
        for (CGameObject *obj : toRemove)
        {
            g_CorpseManager.Remove(0, obj->Serial);

            RemoveObject(obj);
        }

        g_GameScreen.RenderListInitalized = false;
    }
}

void CGameWorld::CreatePlayer(int serial)
{
    WISPFUN_DEBUG("c22_f5");
    RemovePlayer();

    g_PlayerSerial = serial;
    g_Player = new CPlayer(serial);

    m_Map[serial] = g_Player;

    if (m_Items != NULL)
        m_Items->Add(g_Player);
    else
    {
        m_Items = g_Player;
        m_Items->m_Next = NULL;
        m_Items->m_Prev = NULL;
    }
}

void CGameWorld::RemovePlayer()
{
    WISPFUN_DEBUG("c22_f6");
    if (g_Player != NULL)
    {
        RemoveFromContainer(g_Player);
        m_Map[g_Player->Serial] = NULL;
        m_Map.erase(g_Player->Serial);
        delete g_Player;
        g_Player = NULL;
        g_PlayerSerial = 0;
    }
}

void CGameWorld::SetPlayer(int serial)
{
    WISPFUN_DEBUG("c22_f6");
    if (serial != g_Player->Serial)
        CreatePlayer(serial);
}

CGameItem *CGameWorld::GetWorldItem(int serial)
{
    WISPFUN_DEBUG("c22_f8");
    WORLD_MAP::iterator i = m_Map.find(serial);

    if (i == m_Map.end() || (*i).second == NULL)
    {
        CGameItem *obj = new CGameItem(serial);

        m_Map[serial] = obj;

        if (m_Items != NULL)
            m_Items->AddObject(obj);
        else
        {
            m_Items = obj;
            m_Items->m_Next = NULL;
            m_Items->m_Prev = NULL;
        }

        return obj;
    }

    return (CGameItem *)(*i).second;
}

CGameCharacter *CGameWorld::GetWorldCharacter(int serial)
{
    WISPFUN_DEBUG("c22_f9");
    WORLD_MAP::iterator i = m_Map.find(serial);

    if (i == m_Map.end() || (*i).second == NULL)
    {
        CGameCharacter *obj = new CGameCharacter(serial);

        m_Map[serial] = obj;

        if (m_Items != NULL)
            m_Items->AddObject(obj);
        else
        {
            m_Items = obj;
            m_Items->m_Next = NULL;
            m_Items->m_Prev = NULL;
        }

        return obj;
    }

    return i->second->GameCharacterPtr();
}

CGameObject *CGameWorld::FindWorldObject(int serial)
{
    WISPFUN_DEBUG("c22_f10");
    CGameObject *result = NULL;

    WORLD_MAP::iterator i = m_Map.find(serial);
    if (i != m_Map.end())
        result = (*i).second;

    return result;
}

CGameItem *CGameWorld::FindWorldItem(int serial)
{
    WISPFUN_DEBUG("c22_f11");
    CGameItem *result = NULL;

    WORLD_MAP::iterator i = m_Map.find(serial);
    if (i != m_Map.end() && !((*i).second)->NPC)
        result = (CGameItem *)(*i).second;

    return result;
}

CGameCharacter *CGameWorld::FindWorldCharacter(int serial)
{
    WISPFUN_DEBUG("c22_f12");
    CGameCharacter *result = NULL;

    WORLD_MAP::iterator i = m_Map.find(serial);
    if (i != m_Map.end() && ((*i).second)->NPC)
        result = i->second->GameCharacterPtr();

    return result;
}

void CGameWorld::ReplaceObject(CGameObject *obj, int newSerial)
{
    WISPFUN_DEBUG("c22_f12_1");

    m_Map[obj->Serial] = NULL;
    m_Map.erase(obj->Serial);

    QFOR(item, obj->m_Items, CGameObject *)
    item->Container = newSerial;

    m_Map[newSerial] = obj;
    obj->Serial = newSerial;
}

void CGameWorld::RemoveObject(CGameObject *obj)
{
    WISPFUN_DEBUG("c22_f13");
    RemoveFromContainer(obj);

    uint serial = obj->Serial;
    m_Map[serial] = NULL;
    m_Map.erase(serial);
    delete obj;
}

void CGameWorld::RemoveFromContainer(CGameObject *obj)
{
    WISPFUN_DEBUG("c22_f14");
    uint containerSerial = obj->Container;

    if (containerSerial != 0xFFFFFFFF)
    {
        if (containerSerial < 0x40000000)
            g_GumpManager.UpdateContent(containerSerial, 0, GT_PAPERDOLL);
        else
            g_GumpManager.UpdateContent(containerSerial, 0, GT_CONTAINER);

        CGameObject *container = FindWorldObject(containerSerial);

        if (container != NULL)
            container->Reject(obj);
        else
            obj->Container = 0xFFFFFFFF;
    }
    else
    {
        g_GameScreen.RenderListInitalized = false;

        if (m_Items != NULL)
        {
            if (m_Items == obj)
            {
                m_Items = (CGameObject *)m_Items->m_Next;
                if (m_Items != NULL)
                    m_Items->m_Prev = NULL;
            }
            else
            {
                if (obj->m_Next != NULL)
                {
                    if (obj->m_Prev != NULL)
                    {
                        obj->m_Prev->m_Next = obj->m_Next;
                        obj->m_Next->m_Prev = obj->m_Prev;
                    }
                    else
                        obj->m_Next->m_Prev = NULL;
                }
                else if (obj->m_Prev != NULL)
                    obj->m_Prev->m_Next = NULL;
            }
        }
    }

    obj->m_Next = NULL;
    obj->m_Prev = NULL;
    obj->RemoveRender();
}

void CGameWorld::ClearContainer(CGameObject *obj)
{
    WISPFUN_DEBUG("c22_f15");
    if (!obj->Empty())
        obj->Clear();
}

void CGameWorld::PutContainer(CGameObject *obj, CGameObject *container)
{
    WISPFUN_DEBUG("c22_f16");
    RemoveFromContainer(obj);
    container->AddItem(obj);
}

void CGameWorld::MoveToTop(CGameObject *obj)
{
    WISPFUN_DEBUG("c22_f17");
    if (obj == NULL)
        return;

    if (obj->Container == 0xFFFFFFFF)
        g_MapManager.AddRender(obj);

    if (obj->m_Next == NULL)
        return;

    if (obj->Container == 0xFFFFFFFF)
    {
        if (obj->m_Prev == NULL)
        {
            m_Items = (CGameObject *)obj->m_Next;
            m_Items->m_Prev = NULL;

            CGameObject *item = m_Items;

            while (item != NULL)
            {
                if (item->m_Next == NULL)
                {
                    item->m_Next = obj;
                    obj->m_Prev = item;
                    obj->m_Next = NULL;

                    break;
                }

                item = (CGameObject *)item->m_Next;
            }
        }
        else
        {
            CGameObject *item = (CGameObject *)obj->m_Next;

            obj->m_Prev->m_Next = obj->m_Next;
            obj->m_Next->m_Prev = obj->m_Prev;

            while (item != NULL)
            {
                if (item->m_Next == NULL)
                {
                    item->m_Next = obj;
                    obj->m_Prev = item;
                    obj->m_Next = NULL;

                    break;
                }

                item = (CGameObject *)item->m_Next;
            }
        }
    }
    else
    {
        CGameObject *container = FindWorldObject(obj->Container);

        if (container == NULL)
            return;

        if (obj->m_Prev == NULL)
        {
            container->m_Items = obj->m_Next;
            container->m_Items->m_Prev = NULL;

            CGameObject *item = (CGameObject *)container->m_Items;

            while (item != NULL)
            {
                if (item->m_Next == NULL)
                {
                    item->m_Next = obj;
                    obj->m_Prev = item;
                    obj->m_Next = NULL;

                    break;
                }

                item = (CGameObject *)item->m_Next;
            }
        }
        else
        {
            CGameObject *item = (CGameObject *)obj->m_Next;

            obj->m_Prev->m_Next = obj->m_Next;
            obj->m_Next->m_Prev = obj->m_Prev;

            while (item != NULL)
            {
                if (item->m_Next == NULL)
                {
                    item->m_Next = obj;
                    obj->m_Prev = item;
                    obj->m_Next = NULL;

                    break;
                }

                item = (CGameObject *)item->m_Next;
            }
        }
    }
}

CGameObject *CGameWorld::SearchWorldObject(
    int serialStart, int scanDistance, SCAN_TYPE_OBJECT scanType, SCAN_MODE_OBJECT scanMode)
{
    WISPFUN_DEBUG("c22_f18");
    CGameObject *result = NULL;

    CGameObject *start = FindWorldObject(serialStart);

    int count = 2;
    int startI = 0;

    if (scanMode == SMO_PREV)
    {
        if (start == NULL || start->m_Prev == NULL)
        {
            start = m_Items;
            startI = 1;
        }
        else
            start = (CGameObject *)start->m_Prev;
    }
    else
    {
        if (start == NULL || start->m_Next == NULL)
        {
            start = m_Items;
            startI = 1;
        }
        else
            start = (CGameObject *)start->m_Next;
    }

    if (start != NULL)
    {
        CGameObject *obj = start;
        int distance = 100500;
        CGameObject *distanceResult = NULL;

        IFOR (i, startI, count && result == NULL)
        {
            if (i)
            {
                obj = m_Items;

                if (scanMode == SMO_PREV)
                {
                    while (obj != NULL && obj->m_Next != NULL)
                        obj = (CGameObject *)obj->m_Next;
                }
            }

            while (obj != NULL && result == NULL)
            {
                int dist = GetDistance(obj, g_Player);

                if (obj->Serial != serialStart && dist <= scanDistance)
                {
                    bool condition = false;

                    if (scanType == STO_OBJECTS)
                        condition = (!obj->NPC && !((CGameItem *)obj)->MultiBody);
                    else if (obj->NPC && !obj->IsPlayer())
                    {
                        if (scanType == STO_HOSTILE)
                        {
                            CGameCharacter *gc = obj->GameCharacterPtr();

                            condition =
                                (gc->Notoriety >= NT_SOMEONE_GRAY && gc->Notoriety <= NT_MURDERER);
                        }
                        else if (scanType == STO_PARTY)
                            condition = g_Party.Contains(obj->Serial);

                        else
                            condition = true;
                    }

                    if (condition)
                    {
                        if (scanMode == SMO_NEAREST)
                        {
                            if (dist < distance)
                            {
                                distance = dist;
                                distanceResult = obj;
                            }
                        }
                        else
                        {
                            result = obj;

                            break;
                        }
                    }
                }

                if (scanMode == SMO_PREV)
                    obj = (CGameObject *)obj->m_Prev;
                else
                    obj = (CGameObject *)obj->m_Next;
            }
        }

        if (distanceResult != NULL)
            result = distanceResult;
    }

    return result;
}

void CGameWorld::MoveObject(int serial, int x, int y, char z)
{
    CGameObject *obj = FindWorldObject(serial);

    if (obj == nullptr)
    {
        return;
    }

    obj->SetX(x);
    obj->SetY(y);
    obj->SetZ(z);

    if (!obj->NPC)
    {
        obj->OnGraphicChange(0);
    }
    else if (obj == g_Player)
    {
        g_RemoveRangeXY.X = x;
        g_RemoveRangeXY.Y = y;
    }

    MoveToTop(obj);
}

void CGameWorld::UpdateItem(
    int serial,
    ushort graphic,
    uchar graphicIncrement,
    int count,
    int x,
    int y,
    char z,
    uchar direction,
    ushort color,
    uchar flags)
{
    LOG("UpdateItem 0x%08lX:0x%04X 0x%04X (%i) %d:%d:%d %i\n",
        serial,
        graphic,
        color,
        count,
        x,
        y,
        z,
        direction);

    CGameItem *item = NULL;
    CGameObject *obj = FindWorldObject(serial);

    if (g_ObjectInHand.Enabled && g_ObjectInHand.Serial == serial)
    {
        if (g_ObjectInHand.Container && g_ObjectInHand.Container != 0xFFFFFFFF)
        {
            if (!g_ObjectInHand.Layer)
                g_GumpManager.UpdateContent(g_ObjectInHand.Container, 0, GT_CONTAINER);
            else
                g_GumpManager.UpdateContent(g_ObjectInHand.Container, 0, GT_PAPERDOLL);
        }

        g_ObjectInHand.UpdatedInWorld = true;
    }

    bool created = false;

    if (obj == NULL)
    {
        LOG("created ");
        created = true;

        item = GetWorldItem(serial);

        if (item == NULL)
        {
            LOG("No memory?\n");
            return;
        }

        obj = item;
    }
    else
    {
        LOG("updated ");

        if (obj->Container != 0xFFFFFFFF)
        {
            RemoveFromContainer(obj);
            obj->Container = 0xFFFFFFFF;
            m_Items->AddObject(obj);
        }

        item = (CGameItem *)obj;
    }

    if (obj == NULL)
        return;

    obj->MapIndex = g_CurrentMap;

    if (graphic != 0x2006)
        graphic += graphicIncrement;

    item->MultiBody = false;
    item->Graphic = graphic;

    if (item->Dragged)
    {
        g_GumpManager.CloseGump(serial, 0, GT_DRAG);
        item->Dragged = false;
    }

    item->SetX(x);
    item->SetY(y);
    item->SetZ(z);
    item->LightID = direction;

    if (graphic == 0x2006)
        item->Layer = direction;

    item->Color = g_ColorManager.FixColor(color, (color & 0x8000));

    if (!count)
        count = 1;

    item->Count = count;
    item->SetFlags(flags);

    item->OnGraphicChange(direction);

    LOG("serial:0x%08X graphic:0x%04X color:0x%04X count:%i xyz:%d,%d,%d light:%i flags:0x%02X\n",
        obj->Serial,
        obj->Graphic,
        obj->Color,
        item->Count,
        obj->GetX(),
        obj->GetY(),
        obj->GetZ(),
        direction,
        obj->GetFlags());

    if (created && g_ConfigManager.ShowIncomingNames && !obj->Clicked && !obj->GetName().length())
    {
        if (obj->IsCorpse())
            g_Orion.Click(obj->Serial);
    }

    MoveToTop(obj);
}

void CGameWorld::UpdateMulti(
    int serial,
    ushort graphic,
    uchar graphicIncrement,
    int count,
    int x,
    int y,
    char z,
    uchar direction,
    ushort color,
    uchar flags)
{
    LOG("UpdateMulti 0x%08lX:0x%04X 0x%04X (%i) %d:%d:%d %i\n",
        serial,
        graphic,
        color,
        count,
        x,
        y,
        z,
        direction);

    CGameItem *item = NULL;
    CGameObject *obj = FindWorldObject(serial);

    if (g_ObjectInHand.Enabled && g_ObjectInHand.Serial == serial)
    {
        if (g_ObjectInHand.Container && g_ObjectInHand.Container != 0xFFFFFFFF)
        {
            if (!g_ObjectInHand.Layer)
                g_GumpManager.UpdateContent(g_ObjectInHand.Container, 0, GT_CONTAINER);
            else
                g_GumpManager.UpdateContent(g_ObjectInHand.Container, 0, GT_PAPERDOLL);
        }

        g_ObjectInHand.UpdatedInWorld = true;
    }

    if (obj == NULL)
    {
        LOG("created ");

        item = GetWorldItem(serial);

        if (item == NULL)
        {
            LOG("No memory?\n");
            return;
        }

        obj = item;
    }
    else
    {
        LOG("updated ");

        if (obj->Container != 0xFFFFFFFF)
        {
            RemoveFromContainer(obj);
            obj->Container = 0xFFFFFFFF;
            m_Items->AddObject(obj);
        }

        item = (CGameItem *)obj;
    }

    if (obj == NULL)
        return;

    obj->MapIndex = g_CurrentMap;

    if (graphic != 0x2006)
        graphic += graphicIncrement;

    item->MultiBody = true;
    item->WantUpdateMulti = ((graphic & 0x3FFF) != obj->Graphic) || (obj->GetX() != x) ||
                            (obj->GetY() != y) || (obj->GetZ() != z);
    item->Graphic = graphic & 0x3FFF;

    if (item->Dragged)
    {
        g_GumpManager.CloseGump(serial, 0, GT_DRAG);
        item->Dragged = false;
    }

    item->SetX(x);
    item->SetY(y);
    item->SetZ(z);
    item->LightID = direction;

    if (graphic == 0x2006)
        item->Layer = direction;

    item->Color = g_ColorManager.FixColor(color, (color & 0x8000));

    if (!count)
        count = 1;

    item->Count = count;
    item->SetFlags(flags);

    item->OnGraphicChange(direction);

    LOG("serial:0x%08X graphic:0x%04X color:0x%04X count:%i xyz:%d,%d,%d light:%i flags:0x%02X\n",
        obj->Serial,
        obj->Graphic,
        obj->Color,
        item->Count,
        obj->GetX(),
        obj->GetY(),
        obj->GetZ(),
        direction,
        obj->GetFlags());

    MoveToTop(obj);
}

void CGameWorld::UpdateMobile(
    int serial,
    ushort graphic,
    uchar graphicIncrement,
    int count,
    int x,
    int y,
    char z,
    uchar direction,
    ushort color,
    uchar flags)
{
    LOG("UpdateMobile 0x%08lX:0x%04X 0x%04X (%i) %d:%d:%d %i\n",
        serial,
        graphic,
        color,
        count,
        x,
        y,
        z,
        direction);

    CGameCharacter *character = NULL;
    CGameObject *obj = FindWorldObject(serial);

    Direction dir = (Direction)(direction & 0x7);
    bool run = direction & 0x80;

    bool created = false;

    if (obj == NULL)
    {
        created = true;
        LOG("created ");

        character = GetWorldCharacter(serial);

        if (character == NULL)
        {
            LOG("No memory?\n");
            return;
        }

        obj = character;
        character->Graphic = graphic + graphicIncrement;
        character->OnGraphicChange(1000);
        character->Dir = dir;
        character->Run = run;
        character->Color = g_ColorManager.FixColor(color, (color & 0x8000));
        character->SetX(x);
        character->SetY(y);
        character->SetZ(z);
        character->SetFlags(flags);
    }
    else
    {
        LOG("updated ");
        character = (CGameCharacter *)obj;
    }

    if (obj == NULL)
        return;

    obj->MapIndex = g_CurrentMap;

    graphic += graphicIncrement;

    bool found = character->QueueStep(x, y, z, dir, run);

    if (!found)
    {
        character->SetX(x);
        character->SetY(y);
        character->SetZ(z);
        character->Dir = dir;
        character->Run = run;

        character->m_Steps.clear();

        character->OffsetX = 0;
        character->OffsetY = 0;
        character->OffsetZ = 0;
    }

    character->Graphic = graphic & 0x3FFF;
    character->Color = g_ColorManager.FixColor(color, (color & 0x8000));
    character->SetFlags(flags);

    LOG("NPC serial:0x%08X graphic:0x%04X color:0x%04X xyz:%d,%d,%d flags:0x%02X direction:%d notoriety:%d\n",
        obj->Serial,
        obj->Graphic,
        obj->Color,
        obj->GetX(),
        obj->GetY(),
        obj->GetZ(),
        obj->GetFlags(),
        character->Dir,
        character->Notoriety);

    if (created && g_ConfigManager.ShowIncomingNames && !obj->Clicked && !obj->GetName().length())
    {
        g_Orion.Click(obj->Serial);
    }

    MoveToTop(obj);
}

void CGameWorld::UpdatePlayer(
    int serial,
    ushort graphic,
    uchar graphicIncrement,
    int count,
    int x,
    int y,
    char z,
    uchar direction,
    ushort color,
    uchar flags,
    bool resync)
{
    if (serial != g_PlayerSerial || g_Player == nullptr)
    {
        return;
    }

    if (resync)
    {
        /* Only update position on packets that represent a resynchronization point. */

        Direction dir = (Direction)(direction & 0x7);
        bool run = direction & 0x80;

        int endX, endY;
        char endZ;
        Direction endDir;

        g_Player->GetEndPosition(endX, endY, endZ, endDir);

        g_Player->SequenceNumber = 0;

        if (endX == x && endY == y)
        {
            /* The player was moving toward this location anyway. */
            if (endDir != dir)
            {
                g_Player->m_RequestedSteps.clear();
                g_Player->QueueStep(x, y, z, dir, run);
            }
        }
        else
        {
            LOG("Player was moving to (%d, %d, %d, %x) but UpdatePlayer is snapping to (%d, %d, %d, %x)\n",
                endX,
                endY,
                endZ,
                endDir,
                x,
                y,
                z,
                dir);
            g_Player->ResetSteps();
            g_Player->SetX(x);
            g_Player->SetY(y);
            g_Player->SetZ(z);
            g_Player->Dir = dir;
            g_Player->Run = run;
        }

        g_Player->CloseBank();

        g_RemoveRangeXY.X = x;
        g_RemoveRangeXY.Y = y;
    }

    bool updateStatusbar = (g_Player->GetFlags() != flags);

    g_GameScreen.UpdateDrawPos = true;

    bool oldDead = g_Player->Dead();
    ushort oldGraphic = g_Player->Graphic;

    g_Player->Graphic = graphic;
    g_Player->OnGraphicChange();
    g_Player->Color = g_ColorManager.FixColor(color);
    g_Player->SetFlags(flags);

    g_Weather.Reset();

    if (oldGraphic && oldGraphic != g_Player->Graphic)
    {
        if (g_Player->Dead())
            g_Target.Reset();
    }

    if (oldDead != g_Player->Dead())
    {
        if (g_Player->Dead())
            g_Orion.ChangeSeason(ST_DESOLATION, DEATH_MUSIC_INDEX);
        else
            g_Orion.ChangeSeason(g_OldSeason, g_OldSeasonMusic);
    }

    if (updateStatusbar)
        g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);

    g_GumpManager.RemoveRangedGumps();

    MoveToTop(g_Player);
}

void CGameWorld::UpdateItemInContainer(CGameObject *obj, CGameObject *container, int x, int y)
{
    obj->SetX(x);
    obj->SetY(y);
    PutContainer(obj, container);

    uint containerSerial = container->Serial;

    CGump *gump = g_GumpManager.UpdateContent(containerSerial, 0, GT_BULLETIN_BOARD);

    if (gump != NULL)
        CPacketBulletinBoardRequestMessageSummary(containerSerial, obj->Serial).Send();
    else
    {
        gump = g_GumpManager.UpdateContent(containerSerial, 0, GT_SPELLBOOK);

        if (gump == NULL)
        {
            gump = g_GumpManager.UpdateContent(containerSerial, 0, GT_CONTAINER);

            if (gump != NULL && gump->GumpType == GT_CONTAINER)
                ((CGumpContainer *)gump)->UpdateItemCoordinates(obj);
        }

        if (gump != NULL && !container->NPC)
        {
            ((CGameItem *)container)->Opened = true;
        }
    }

    CGameObject *top = container->GetTopObject();

    if (top != NULL)
    {
        top = top->FindSecureTradeBox();

        if (top != NULL)
            g_GumpManager.UpdateContent(0, top->Serial, GT_TRADE);
    }
}

void CGameWorld::UpdateContainedItem(
    int serial,
    ushort graphic,
    uchar graphicIncrement,
    ushort count,
    int x,
    int y,
    int containerSerial,
    ushort color)
{
    if (g_ObjectInHand.Serial == serial && g_ObjectInHand.Dropped)
        g_ObjectInHand.Clear();

    CGameObject *container = FindWorldObject(containerSerial);

    if (container == NULL)
        return;

    CGameObject *obj = FindWorldObject(serial);

    if (obj != NULL && (!container->IsCorpse() || ((CGameItem *)obj)->Layer == OL_NONE))
    {
        RemoveObject(obj);
        obj = NULL;
    }

    if (obj == NULL)
    {
        if (serial & 0x40000000)
            obj = GetWorldItem(serial);
        else
            obj = GetWorldCharacter(serial);
    }

    if (obj == NULL)
    {
        LOG("No memory?\n");
        return;
    }

    obj->MapIndex = g_CurrentMap;

    obj->Graphic = graphic + graphicIncrement;
    obj->OnGraphicChange();
    obj->Color = g_ColorManager.FixColor(color, (color & 0x8000));

    if (!count)
        count = 1;

    obj->Count = count;

    UpdateItemInContainer(obj, container, x, y);

    MoveToTop(obj);

    LOG("\t|0x%08X<0x%08X:%04X*%d (%d,%d) %04X\n",
        containerSerial,
        serial,
        graphic + graphicIncrement,
        count,
        x,
        y,
        color);
}

void CGameWorld::Dump(uchar tCount, uint serial)
{
    WISPFUN_DEBUG("c22_f19");
    LOG("World Dump:\n\n");

    CGameObject *obj = m_Items;

    if (serial != 0xFFFFFFFF)
    {
        obj = FindWorldObject(serial);
        if (obj != NULL)
            obj = (CGameObject *)obj->m_Items;
    }

    while (obj != NULL)
    {
        if (obj->Container == serial)
        {
            if (obj->Serial == g_Player->Serial)
                LOG("---Player---\n");

            IFOR (i, 0, tCount)
                LOG("\t");

            LOG("%s%08X:%04X[%04X](%%02X)*%i\tin 0x%08X XYZ=%i,%i,%i on Map %i\n",
                (obj->NPC ? "NPC: " : "Item: "),
                obj->Serial,
                obj->Graphic,
                obj->Color,
                obj->Count,
                obj->Container,
                obj->GetX(),
                obj->GetY(),
                obj->GetZ(),
                obj->MapIndex);

            if (obj->m_Items != NULL)
                Dump(tCount + 1, obj->Container);
        }

        obj = (CGameObject *)obj->m_Next;
    }
}
