

#include "stdafx.h"

COptionsMacroManager g_OptionsMacroManager;

COptionsMacroManager::COptionsMacroManager()
    : CBaseQueue()
{
}

COptionsMacroManager::~COptionsMacroManager()
{
}

void COptionsMacroManager::LoadFromMacro()
{
    WISPFUN_DEBUG("c149_f1");
    Clear();

    QFOR(obj, g_MacroManager.m_Items, CMacro *)
    Add(obj->GetCopy());

    if (m_Items == NULL)
        Add(CMacro::CreateBlankMacro());
}
