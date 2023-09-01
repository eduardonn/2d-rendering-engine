#pragma once

#include "Shader.hpp"
#include "Mesh.hpp"

#include <GL/glew.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <vector>
#include <memory>

class CircleShader : public Shader
{
public:
	CircleShader(std::shared_ptr<Mesh> mesh);
	void Init();
	void Render();
	void AddObject(glm::vec2 position, float radius, glm::vec4 color);
	void SetPositions(GLsizei count, const float* value);
	void SetRadiuses(GLsizei count, const float* value);
	void SetColors(GLsizei count, const float* value);
	void Clear();

	// Uniform IDs
	GLint m_UPositions;
	GLint m_URadiuses;
	GLint m_UColors;

	const int m_MaxObjectsPerDrawCall = 500;
	size_t m_ObjectCount = 0;
	uint32_t m_VertexBuffer;
	uint32_t m_IndexBuffer;

	std::shared_ptr<Mesh> m_Mesh;
	std::vector<glm::vec2> m_Positions;
	std::vector<float> m_Radiuses;
	std::vector<glm::vec4> m_Colors;
};

