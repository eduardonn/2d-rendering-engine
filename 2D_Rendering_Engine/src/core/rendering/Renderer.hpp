#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <vector>

class Rendered;

namespace Renderer
{
    void Init();
    void Render();
    void AddObject(Rendered* object);
    void RenderObjects();
	void DrawCircle(glm::vec2 position, float radius = 0.2f, glm::vec4 color = { 0.2f, 1.0f, 0.1f, 1.0f });
	void RenderDynamicObjects();
    size_t GetObjectCount();
};