#include "CircleShader.hpp"

#include <glm/gtc/type_ptr.hpp>

CircleShader::CircleShader(std::shared_ptr<Mesh> mesh)
	: Shader("res/shaders/circle.shader")
	, m_Mesh(mesh)
{
	m_UPositions = glGetUniformLocation(m_ID, "u_Positions");
	m_URadiuses = glGetUniformLocation(m_ID, "u_Radiuses");
	m_UColors = glGetUniformLocation(m_ID, "u_Colors");

	Init();
}

void CircleShader::Init()
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

	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(m_Mesh->Indices[0]) * m_Mesh->Indices.size(),
		&m_Mesh->Indices[0],
		GL_STATIC_DRAW
	);
}

void CircleShader::AddObject(glm::vec2 position, float radius, glm::vec4 color)
{
	++m_ObjectCount;
	m_Positions.push_back(position);
	m_Radiuses.push_back(radius);
	m_Colors.push_back(color);
}

void CircleShader::Clear()
{
	m_ObjectCount = 0;
	m_Positions.clear();
	m_Radiuses.clear();
	m_Colors.clear();
}

void CircleShader::SetPositions(GLsizei count, const float* value)
{
	glUniform2fv(m_UPositions, count, value);
}

void CircleShader::SetRadiuses(GLsizei count, const float* value)
{
	glUniform1fv(m_URadiuses, count, value);
}

void CircleShader::SetColors(GLsizei count, const float* value)
{
	glUniform4fv(m_UColors, count, value);
}

void CircleShader::Render()
{
	if (!m_ObjectCount)
		return;

	Use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

	int i = 0;
	size_t objectsLeftToDraw = m_ObjectCount;
	while (objectsLeftToDraw > m_MaxObjectsPerDrawCall)
	{
		SetPositions(m_MaxObjectsPerDrawCall, glm::value_ptr(m_Positions[i]));
		SetRadiuses(m_MaxObjectsPerDrawCall, &m_Radiuses[i]);
		SetColors(m_MaxObjectsPerDrawCall, glm::value_ptr(m_Colors[i]));
		//glDrawElements(m_Mesh->m_PrimitiveType, (GLsizei)m_Mesh->m_Indices.size(), GL_UNSIGNED_INT, (GLsizei)m_MaxObjectsPerDrawCall);
		glDrawElements(m_Mesh->PrimitiveType, (GLsizei)m_Mesh->Indices.size(), GL_UNSIGNED_INT, nullptr);

		i += m_MaxObjectsPerDrawCall;
		objectsLeftToDraw -= m_MaxObjectsPerDrawCall;
	}

	SetPositions(objectsLeftToDraw, glm::value_ptr(m_Positions[i]));
	SetRadiuses(objectsLeftToDraw, &m_Radiuses[i]);
	SetColors(objectsLeftToDraw, glm::value_ptr(m_Colors[i]));
	glDrawElementsInstanced(m_Mesh->PrimitiveType, (GLsizei)m_Mesh->Indices.size(), GL_UNSIGNED_INT, nullptr, (GLsizei)objectsLeftToDraw);

	glDisable(GL_BLEND);
}