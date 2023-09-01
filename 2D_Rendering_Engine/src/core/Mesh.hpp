#pragma once

#include "rendering/Renderer.hpp"

#include <memory>
#include <vector>

class Mesh
{
public:
	Mesh();
	Mesh(double* vertices, size_t verticesSize, uint32_t* indices, size_t indicesSize, int primitiveType);
	static Mesh* Circle(const uint16_t verticesCount, const float radius);
	static Mesh* Quad(const double width = 1.0, const double height = 1.0);

	std::vector<double> Vertices;
	std::vector<unsigned int> Indices;
	unsigned int PrimitiveType;
};