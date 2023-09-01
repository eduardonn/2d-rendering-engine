#include "BasicShader.hpp"
#include "core/utils/debug_utils.hpp"
#include "GL/glew.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

BasicShader::BasicShader(std::shared_ptr<Mesh> mesh)
    : Shader("res/shaders/basic.shader")
	, m_Mesh(mesh)
{
    m_UPositions = glGetUniformLocation(m_ID, "u_Positions");
    m_UColors = glGetUniformLocation(m_ID, "u_Colors");

	Init();
}

void BasicShader::Init()
{
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(m_Mesh->Vertices[0]) * m_Mesh->Vertices.size(),
		&m_Mesh->Vertices[0],
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
}

BasicShaderData BasicShader::AddObject(const glm::vec2& position, const glm::vec3& color)
{
	m_Positions[m_ObjectCount] = position;
	m_Colors[m_ObjectCount] = color;

	BasicShaderData shaderData = { &m_Positions[m_ObjectCount], &m_Colors[m_ObjectCount] };

	++m_ObjectCount;

	return shaderData;
}

void BasicShader::Clear()
{
	m_ObjectCount = 0;
}

void BasicShader::RemoveObject(BasicShaderData shaderData)
{
	--m_ObjectCount;
	//for (uint32_t i = 0; i < m_ObjectCount; ++i)
	//{
	//	if (m_Positions[i] == shaderData.Position)
	//}
	//uint32_t objectIndex;

	//RAW_LOG_LN("Index: ");
	//m_Positions.erase(m_Positions.begin() + shaderData.Position);
	//m_Colors.erase(m_Colors.begin() + shaderData.Color);
}

// Render in batches of size m_MaxObjectsPerDrawCall
// without using glVertexAttribDivisor for compatibility with OpenGL 3.1
void BasicShader::Render()
{
	if (!m_ObjectCount)
		return;

	Use();
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);

	int i = 0;
	size_t objectsLeftToDraw = m_ObjectCount;
	while (objectsLeftToDraw > m_MaxObjectsPerDrawCall)
	{

		SetPositions(m_MaxObjectsPerDrawCall, glm::value_ptr(m_Positions[i]));
		SetColors(m_MaxObjectsPerDrawCall, glm::value_ptr(m_Colors[i]));
		glDrawArraysInstanced(m_Mesh->PrimitiveType, 0, (GLsizei)m_Mesh->Vertices.size(), (GLsizei)m_MaxObjectsPerDrawCall);

		i += m_MaxObjectsPerDrawCall;
		objectsLeftToDraw -= m_MaxObjectsPerDrawCall;
	}

	SetPositions(objectsLeftToDraw, glm::value_ptr(m_Positions[i]));
	SetColors(objectsLeftToDraw, glm::value_ptr(m_Colors[i]));
	glDrawArraysInstanced(m_Mesh->PrimitiveType, 0, (GLsizei)m_Mesh->Vertices.size(), (GLsizei)objectsLeftToDraw);
}

void BasicShader::SetPositions(GLsizei count, const float* value)
{
    glUniform2fv(m_UPositions, count, value);
}

void BasicShader::SetColors(GLsizei count, const float* value)
{
	glUniform3fv(m_UColors, count, value);
}