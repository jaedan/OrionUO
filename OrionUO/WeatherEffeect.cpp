/***********************************************************************************
**
** WeatherEffect.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/

#include "stdafx.h"

CWeatherEffect::CWeatherEffect()
{
    ID = RandomInt(2000);

    ScaleRatio = (float)(ID % 20) / 13.0f;
}
