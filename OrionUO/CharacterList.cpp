

#include "stdafx.h"

CCharacterList g_CharacterList;

CCharacterList::CCharacterList()
{
    WISPFUN_DEBUG("c182_f1");

    Clear();
}

void CCharacterList::Clear()
{
    WISPFUN_DEBUG("c182_f2");
    IFOR (i, 0, 7)
        m_Name[i] = "";

    Selected = 0;
    OnePerson = false;
    Have6Slot = false;
    Have7Slot = false;
}

void CCharacterList::SetName(intptr_t pos, const string &name)
{
    WISPFUN_DEBUG("c182_f3");

    if (pos >= 0 && pos < Count)
        m_Name[pos] = name;
}

string CCharacterList::GetName(intptr_t pos) const
{
    WISPFUN_DEBUG("c182_f4");
    string result = "";

    if (pos >= 0 && pos < Count)
        result = m_Name[pos];

    return result;
}

string CCharacterList::GetSelectedName() const
{
    WISPFUN_DEBUG("c182_f5");
    string result = "";

    if (Selected >= 0 && Selected < Count)
        result = m_Name[Selected];

    return result;
}
