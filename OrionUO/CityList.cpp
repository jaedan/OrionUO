

#include "stdafx.h"

CCityList g_CityList;

CCityItem::CCityItem()
    : m_City("", L"")
{
}

CCityItem::~CCityItem()
{
}

void CCityItem::InitCity()
{
    WISPFUN_DEBUG("c183_f1");

    m_City = g_CityManager.GetCity(Name);
}

CCityItemNew::CCityItemNew()
    : CCityItem()
{
}

CCityItemNew::~CCityItemNew()
{
}

CCityList::CCityList()
{
}

CCityList::~CCityList()
{
    WISPFUN_DEBUG("c184_f1");
    Clear();
}

CCityItem *CCityList::GetCity(int index)
{
    WISPFUN_DEBUG("c184_f2");

    if (index < m_CityList.size())
        return m_CityList[index];

    return NULL;
}

void CCityList::Clear()
{
    WISPFUN_DEBUG("c184_f3");
    for (vector<CCityItem *>::iterator i = m_CityList.begin(); i != m_CityList.end(); ++i)
        delete *i;

    m_CityList.clear();
}
