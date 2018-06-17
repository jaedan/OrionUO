

#include "stdafx.h"

CBaseProfession::CBaseProfession()
    : CBaseQueueItem()
{
}

CBaseProfession::~CBaseProfession()
{
}

bool CBaseProfession::AddDescription(int desc, string name, const char *val)
{
    WISPFUN_DEBUG("c199_f1");
    bool result = (DescriptionIndex == desc);

    if (result)
    {
        if (!NameClilocID)
            Name = name;

        g_FontManager.SetUseHTML(true);

        if (desc == -2)
            Description = ToWString(val);
        else
            Description = ToWString(name + "\n" + val);

        g_FontManager.SetUseHTML(false);
    }
    else
    {
        for (CBaseProfession *obj = (CBaseProfession *)m_Items; obj != NULL && !result;
             obj = (CBaseProfession *)obj->m_Next)
            result = obj->AddDescription(desc, name, val);
    }

    return result;
}

CProfessionCategory::CProfessionCategory()
    : CBaseProfession()
{
}

CProfessionCategory::~CProfessionCategory()
{
}

void CProfessionCategory::AddChildren(string child)
{
    Childrens += child + "|";
}

CProfession::CProfession()
    : CBaseProfession()
{
    memset(&m_SkillIndex[0], 0, sizeof(m_SkillIndex));
    memset(&m_SkillValue[0], 0, sizeof(m_SkillValue));
}

CProfession::~CProfession()
{
}
