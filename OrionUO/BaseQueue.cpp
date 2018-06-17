

#include "stdafx.h"

CBaseQueueItem::CBaseQueueItem()
    : CBaseQueue()
    , m_Next(NULL)
    , m_Prev(NULL)
{
}

CBaseQueueItem::~CBaseQueueItem()
{
    WISPFUN_DEBUG("c180_f1");

    Clear();

    CBaseQueueItem *item = m_Next;

    while (item != NULL && item != this)
    {
        CBaseQueueItem *next = item->m_Next;
        item->m_Next = NULL;
        delete item;
        item = next;
    }
}

CBaseQueue::CBaseQueue()
    : m_Items(NULL)
{
}

CBaseQueue::~CBaseQueue()
{
    WISPFUN_DEBUG("c181_f1");

    Clear();
}

void CBaseQueue::Clear()
{
    WISPFUN_DEBUG("c181_f2");

    if (m_Items != NULL)
    {
        CBaseQueueItem *item = m_Items;
        m_Items = NULL;

        while (item != NULL)
        {
            CBaseQueueItem *next = item->m_Next;
            item->m_Next = NULL;
            delete item;
            item = next;
        }
    }
}

CBaseQueueItem *CBaseQueue::Add(CBaseQueueItem *item)
{
    WISPFUN_DEBUG("c181_f3");

    if (item != NULL)
    {
        if (m_Items == NULL)
            m_Items = item;
        else
        {
            CBaseQueueItem *current = m_Items;

            while (current->m_Next != NULL)
                current = current->m_Next;

            current->m_Next = item;
            item->m_Prev = current;
        }
    }

    return item;
}

void CBaseQueue::Delete(CBaseQueueItem *item)
{
    WISPFUN_DEBUG("c181_f4");

    if (item != NULL)
    {
        Unlink(item);

        item->m_Next = NULL;
        item->m_Prev = NULL;
        delete item;
    }
}

void CBaseQueue::Delete(int index)
{
    WISPFUN_DEBUG("c181_f5");

    Delete(Get(index));
}

int CBaseQueue::GetItemIndex(CBaseQueueItem *item)
{
    WISPFUN_DEBUG("c181_f6");
    int index = 0;

    BQFOR (current, m_Items)
    {
        if (current == item)
            return index;

        index++;
    }

    return -1;
}

int CBaseQueue::GetItemsCount()
{
    WISPFUN_DEBUG("c181_f7");
    int count = 0;

    BQFOR (current, m_Items)
        count++;

    return count;
}

CBaseQueueItem *CBaseQueue::Get(int index)
{
    WISPFUN_DEBUG("c181_f8");
    CBaseQueueItem *item = m_Items;

    for (; item != NULL && index; item = item->m_Next, index--)
        ;

    return item;
}

void CBaseQueue::Insert(CBaseQueueItem *first, CBaseQueueItem *item)
{
    WISPFUN_DEBUG("c181_f9");
    if (first == NULL)
    {
        item->m_Next = m_Items;
        item->m_Prev = NULL;

        if (m_Items != NULL)
            m_Items->m_Prev = item;

        m_Items = item;
    }
    else
    {
        CBaseQueueItem *next = first->m_Next;
        item->m_Next = next;
        item->m_Prev = first;

        first->m_Next = item;

        if (next != NULL)
            next->m_Prev = item;
    }
}

void CBaseQueue::Unlink(CBaseQueueItem *item)
{
    WISPFUN_DEBUG("c181_f10");

    if (item != NULL)
    {
        if (item == m_Items)
        {
            m_Items = m_Items->m_Next;

            if (m_Items != NULL)
                m_Items->m_Prev = NULL;
        }
        else
        {
            item->m_Prev->m_Next = item->m_Next;

            if (item->m_Next != NULL)
                item->m_Next->m_Prev = item->m_Prev;
        }
    }
}

void CBaseQueue::MoveToFront(CBaseQueueItem *item)
{
    WISPFUN_DEBUG("c181_f11");

    if (item != NULL && item != m_Items)
    {
        Unlink(item);

        if (m_Items != NULL)
            m_Items->m_Prev = item;

        item->m_Next = m_Items;
        item->m_Prev = NULL;

        m_Items = item;
    }
}

void CBaseQueue::MoveToBack(CBaseQueueItem *item)
{
    WISPFUN_DEBUG("c181_f12");

    if (item != NULL)
    {
        Unlink(item);

        CBaseQueueItem *last = Last();

        if (last == NULL)
            m_Items = item;
        else
            last->m_Next = item;

        item->m_Prev = last;
        item->m_Next = NULL;
    }
}

bool CBaseQueue::Move(CBaseQueueItem *item, bool up)
{
    WISPFUN_DEBUG("c181_f13");

    bool result = (item != NULL);

    if (result)
    {
        if (up)
        {
            CBaseQueueItem *prev = item->m_Prev;

            result = (prev != NULL);

            if (result)
            {
                if (prev == m_Items)
                {
                    prev->m_Prev = item;
                    prev->m_Next = item->m_Next;
                    m_Items = item;
                    item->m_Prev = NULL;
                    item->m_Next = prev;
                }
                else
                {
                    CBaseQueueItem *prevprev = prev->m_Prev;
                    prev->m_Prev = item;
                    prev->m_Next = item->m_Next;
                    prevprev->m_Next = item;
                    item->m_Prev = prevprev;
                    item->m_Next = prev;
                }
            }
        }
        else
        {
            CBaseQueueItem *next = item->m_Next;

            result = (next != NULL);

            if (result)
            {
                if (item == m_Items)
                {
                    item->m_Next = next->m_Next;
                    item->m_Prev = next;
                    m_Items = item;
                    m_Items->m_Prev = NULL;
                    m_Items->m_Next = item;
                }
                else
                {
                    CBaseQueueItem *prev = item->m_Prev;
                    prev->m_Next = next;
                    item->m_Next = next->m_Next;
                    item->m_Prev = next;
                    next->m_Prev = prev;
                    next->m_Next = item;
                }
            }
        }
    }

    return result;
}

CBaseQueueItem *CBaseQueue::Last()
{
    WISPFUN_DEBUG("c181_f14");

    CBaseQueueItem *last = m_Items;

    while (last != NULL && last->m_Next != NULL)
        last = last->m_Next;

    return last;
}
