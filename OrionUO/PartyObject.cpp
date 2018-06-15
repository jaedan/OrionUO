/***********************************************************************************
**
** PartyObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CPartyObject::CPartyObject()
{
}
//----------------------------------------------------------------------------------
string CPartyObject::GetName(int index)
{
    WISPFUN_DEBUG("c197_f1");
    if (Serial)
    {
        if (Character == NULL)
            Character = g_World->FindWorldCharacter(Serial);
        if (Character != NULL)
            return Character->GetName();
    }

    char buf[10] = { 0 };
    sprintf_s(buf, "[%i]", index);

    return string(buf);
}
//----------------------------------------------------------------------------------
