#pragma once

#include "core/Shader.hpp"
#include "core/rendering/Rendered.hpp"
#include "core/Mesh.hpp"
#include <GL/glew.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <array>
#include <memory>

struct BasicShaderData
{
	glm::vec2* Position;
	glm::vec3* Color;
};

class BasicShader : Shader, Rendered
{
public:
    BasicShader(std::shared_ptr<Mesh> mesh);

	// Uniforms
    void SetColors(GLsizei count, const float* value);
	void SetPositions(GLsizei count, const float* value);

	BasicShaderData AddObject(const glm::vec2& position = { 0.0f, 0.0f }, const glm::vec3& color = { 1.0f, 0.0f, 1.0f });
	void Clear();
	
	void RemoveObject(BasicShaderData renderData);
	void Render() override;

private:
	void Init();

	static const uint32_t s_NumberObjectsLimit = 200000;

	const int m_MaxObjectsPerDrawCall = 500;
	size_t m_ObjectCount = 0;
	std::shared_ptr<Mesh> m_Mesh;
	std::array<glm::vec2, s_NumberObjectsLimit> m_Positions;
	std::array<glm::vec3, s_NumberObjectsLimit> m_Colors;
	uint32_t m_VertexBuffer;

	// Uniform IDs
    GLint m_UPositions;
    GLint m_UColors;
};