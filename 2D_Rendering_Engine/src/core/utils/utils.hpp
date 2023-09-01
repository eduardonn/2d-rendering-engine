#pragma once

namespace Math
{
    float toRange(float value, float valueMin, float valueMax, float targetMin, float targetMax)
    {
        return value / valueMax * (targetMax - targetMin) - (targetMax - targetMin) / 2.f;
    }   
}