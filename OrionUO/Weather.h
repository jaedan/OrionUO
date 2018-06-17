

#ifndef WEATHER_H
#define WEATHER_H

class CWeather
{
public:
    uchar Type = 0;

    uchar Count = 0;

    uchar CurrentCount = 0;

    uchar Temperature = 0;

    uint Timer = 0;

    char Wind = 0;

    uint WindTimer = 0;

    uint LastTick = 0;

    float SimulationRatio = 37.0f;

    CWeather();
    ~CWeather() { m_Effects.clear(); }

    deque<CWeatherEffect> m_Effects;

    void Reset();

    void Generate();

    void Draw(int x, int y);
};

extern CWeather g_Weather;

#endif
