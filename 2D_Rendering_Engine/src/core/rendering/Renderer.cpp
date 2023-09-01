#include "Renderer.hpp"
#include "Rendered.hpp"
#include "../CircleShader.hpp"
#include "../Mesh.hpp"
#include "../utils/shaders_utils.hpp"
#include "../utils/debug_utils.hpp"

#include <GL/glew.h>

#include <iostream>
#include <cstring>
#include <memory>

using namespace std;

namespace Renderer {
    // For rendering single objects (objects which must be drawed from its own draw call)
    static vector<double> m_Vertices;
    static vector<unsigned int> m_Indices;
	static vector<glm::vec2> m_UPositions;
	static vector<float> m_URadiuses;
	static vector<glm::vec4> m_UColors;
    static vector<Rendered*> m_RenderedObjects;
	static std::unique_ptr<CircleShader> m_CircleShader;

    void Init()
    {
        LOG_LN("Single Rendered Objects count: " << m_RenderedObjects.size());

		m_CircleShader = std::make_unique<CircleShader>(std::shared_ptr<Mesh>(Mesh::Quad(1.0f, 1.0f)));
    }

    void Render()
    {
        glClear(GL_COLOR_BUFFER_BIT);

		RenderDynamicObjects();
        RenderObjects();
    }

    // Draw each object from its own draw call
    void RenderObjects()
    {
        for(Rendered *object : m_RenderedObjects)
        {
            object->Render();
        }
    }

	void RenderDynamicObjects()
	{
		m_CircleShader->Render();
		m_CircleShader->Clear();
	}

	void DrawCircle(glm::vec2 position, float radius, glm::vec4 color)
	{
		m_CircleShader->AddObject(position, radius, color);
	}

    void AddObject(Rendered* object)
    {
        m_RenderedObjects.push_back(object);
    }

    size_t GetObjectCount()
    {
        return m_RenderedObjects.size();
    }
}
