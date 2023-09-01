#pragma once

#include <memory>

class Rendered
{
public:
    Rendered();
    ~Rendered();
    virtual void Render() = 0;

    // TODO: Implement
    // void EnableRender();
    // void DisableRender();
};