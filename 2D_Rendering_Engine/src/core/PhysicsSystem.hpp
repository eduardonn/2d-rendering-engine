#pragma once

#include <vector>
#include "Rigidbody.hpp"
#include "SpatialHashGrid.hpp"

class PhysicsSystem
{
public:
	PhysicsSystem(const PhysicsSystem& other) = delete;
	void operator=(PhysicsSystem) = delete;

	static PhysicsSystem& Get() {
		static PhysicsSystem instance;
		return instance;
	};
	static void AddObject(Rigidbody* rb);
	static void RemoveObject(Rigidbody* rb);
	static void Update(float deltaTime);
	static float SystemEnergy();
	static SpatialHashGrid<Rigidbody>& HashGrid();

private:
	PhysicsSystem();
	void SimpleSolver(float deltaTime);
	void VerletSolver(float deltaTime);
	void CustomSolver(float deltaTime);
	
	SpatialHashGrid<Rigidbody> m_SpatialHashGrid;
	float m_SystemEnergy = 0.0f;
	std::vector<Rigidbody*> m_Rigidbodies;
	vec2 m_Gravity{ 0.0f, -.981f };
	float m_RestitutionCoefficient = .6f;
	uint8_t m_SubSteps = 5;
	vec2 m_WorldBounds[2] = { vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f) };
};