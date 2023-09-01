#include "Rendered.hpp"
#include "Renderer.hpp"
#include "../utils/debug_utils.hpp"

Rendered::Rendered()
{
    Renderer::AddObject(this);
}

Rendered::~Rendered()
{
    // Renderer::RemoveObject(this);
}

void Rendered::Render()
{
}