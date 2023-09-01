#pragma once

#include <glm/glm.hpp>
#include <vector>

template<typename T>
class SpatialHashGrid
{
public:
	SpatialHashGrid(const uint32_t maxElements, const float tableSizeMultiplier, const float cellSize);

	void Create(const std::vector<T*>& rigidbodies);
	void Add(const glm::vec2& pos, uint32_t index);
	void Query(const glm::vec2& pos, float distance);

	std::vector<T*> QueryResult;
	uint32_t QuerySize = 0;

private:
	uint32_t Hash(const glm::ivec2& cell);
	void Init(size_t maxElements);
	glm::ivec2 PosToCell(const glm::vec2& pos);
	size_t m_TableSize;
	size_t m_MaxElements;
	float m_TableSizeMultiplier;
	float m_CellSize;
	std::vector<T*> m_Elements;
	std::vector<uint32_t> m_CellsStart;
};

template<typename T>
SpatialHashGrid<T>::SpatialHashGrid(const uint32_t maxElements, const float tableSizeMultiplier, const float cellSize)
	: m_CellSize(1.0f / cellSize), m_TableSizeMultiplier(tableSizeMultiplier)
{
	Init(maxElements);
}

template<typename T>
void SpatialHashGrid<T>::Init(size_t maxElements)
{
	m_MaxElements = maxElements;
	m_TableSize = maxElements * m_TableSizeMultiplier;
	m_Elements.resize(m_MaxElements);
	QueryResult.resize(m_MaxElements);
	m_CellsStart.resize(m_TableSize + 1);
}

template<typename T>
void SpatialHashGrid<T>::Add(const glm::vec2& pos, uint32_t index)
{
	uint32_t hashIndex = Hash(PosToCell(pos));

	++m_CellsStart[hashIndex];
}

template<typename T>
void SpatialHashGrid<T>::Create(const std::vector<T*>& rigidbodies)
{
	if (rigidbodies.size() > m_MaxElements)
	{
		Init(rigidbodies.size() * 1.5f);
	}

	for (uint32_t& element : m_CellsStart)
	{
		element = 0;
	}

	for (Rigidbody* element : rigidbodies)
	{
		uint32_t hashIndex = Hash(PosToCell(*element->Position));
		++m_CellsStart[hashIndex];
	}

	uint32_t sum = 0;
	for (uint32_t i = 0; i < m_CellsStart.size(); ++i)
	{
		sum += m_CellsStart[i];
		m_CellsStart[i] = sum;
	}

	for (Rigidbody* element : rigidbodies)
	{
		m_Elements[--m_CellsStart[Hash(PosToCell(*element->Position))]] = element;
	}

}

template<typename T>
void SpatialHashGrid<T>::Query(const glm::vec2& pos, float distance)
{
	QuerySize = 0;

	glm::ivec2 startCell = PosToCell(pos - distance);
	glm::ivec2 endCell = PosToCell(pos + distance);

	for (int32_t y = startCell.y; y <= endCell.y; ++y)
	{
		for (int32_t x = startCell.x; x <= endCell.x; ++x)
		{
			uint32_t hashIndex = Hash({ x, y });
			uint32_t cellStart = m_CellsStart[hashIndex];
			uint32_t cellEnd = m_CellsStart[hashIndex + 1];

			for (uint32_t i = cellStart; i < cellEnd; ++i)
			{
				QueryResult[QuerySize++] = m_Elements[i];
			}
		}
	}
}

template<typename T>
uint32_t SpatialHashGrid<T>::Hash(const glm::ivec2& cell)
{
	uint32_t hash = static_cast<uint32_t>(cell.x * 92837111) ^ static_cast<uint32_t>(cell.y * 689287499);
	return glm::abs(hash) % m_TableSize;
}

template<typename T>
inline glm::ivec2 SpatialHashGrid<T>::PosToCell(const glm::vec2& pos)
{
	return { static_cast<uint32_t>(pos.x * m_CellSize), static_cast<uint32_t>(pos.y * m_CellSize) };
}