#include "Mesh.hpp"
#include "utils/debug_utils.hpp"
#include <glm/trigonometric.hpp>
#include <GL/glew.h>
#include <iostream>
#include <memory>
#include <vector>
#include <array>

Mesh::Mesh() {}

Mesh* Mesh::Quad(const double width, const double height)
{
	Mesh* mesh = new Mesh();
	mesh->PrimitiveType = GL_TRIANGLE_FAN;

	mesh->Vertices = {
		-width * 0.5, -height * 0.5,
		width * 0.5, -height * 0.5,
		width * 0.5, height * 0.5,
		-width * 0.5f, height * 0.5
	};

	mesh->Indices = { 0, 1, 2, 3 };

	return mesh;
}

Mesh* Mesh::Circle(const uint16_t verticesCount, const float radius)
{
	Mesh* mesh = new Mesh();
	mesh->PrimitiveType = GL_TRIANGLE_FAN;

	float angleStep = 6.28f / verticesCount;
	float currentAngle = 0.0f;

	int coordsCount = verticesCount + 1;
	mesh->Vertices.reserve(coordsCount);

	mesh->Indices.reserve(verticesCount + 2); // 1 more for center vertex and 1 more for repeating last vertex

	mesh->Vertices.emplace_back(0.0f);
	mesh->Vertices.emplace_back(0.0f);
	for (uint16_t i = 0; i < verticesCount; ++i)
	{
		mesh->Vertices.emplace_back(glm::cos(currentAngle) * radius);
		mesh->Vertices.emplace_back(glm::sin(currentAngle) * radius);
		currentAngle += angleStep;
	}

	for (uint16_t i = 0; i < verticesCount + 1; ++i)
		mesh->Indices.emplace_back(i);
	mesh->Indices.emplace_back(1); // Repeat first vertex that is not the center to close circle

	// Repeat vertex for drawing without indices buffer
	mesh->Vertices.emplace_back(mesh->Vertices[2]);
	mesh->Vertices.emplace_back(mesh->Vertices[3]);

	return mesh;
}