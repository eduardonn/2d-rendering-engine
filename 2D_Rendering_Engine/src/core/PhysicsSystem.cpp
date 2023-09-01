#include "PhysicsSystem.hpp"
#include "Time.hpp"

#include <glm/glm.hpp>
#include <vector>

using namespace glm;

PhysicsSystem::PhysicsSystem() : m_SpatialHashGrid(100, 2.0f, .1f) {}

SpatialHashGrid<Rigidbody>& PhysicsSystem::HashGrid()
{
	return Get().m_SpatialHashGrid;
}

void PhysicsSystem::AddObject(Rigidbody* rb)
{
	Get().m_Rigidbodies.push_back(rb);
}

void PhysicsSystem::RemoveObject(Rigidbody* rb)
{
	for (size_t i = 0; i < Get().m_Rigidbodies.size(); ++i)
		if (rb == Get().m_Rigidbodies[i])
			Get().m_Rigidbodies.erase(Get().m_Rigidbodies.begin() + i);
}
	
void PhysicsSystem::Update(float deltaTime)
{
	//Get().SimpleSolver(deltaTime);
	Get().VerletSolver(deltaTime);
	//Get().CustomSolver(deltaTime);
}

/*
Implementation from youtube channel Ten Minute Physics:
https://www.youtube.com/watch?v=ThhdlMbGT5g
*/
void PhysicsSystem::SimpleSolver(float deltaTime)
{
#define HASH_GRID
#ifdef HASH_GRID
	Get().m_SpatialHashGrid.Create(m_Rigidbodies);
#endif
	m_SystemEnergy = 0.0f;
	for (Rigidbody* rb1 : m_Rigidbodies)
	{
		m_SystemEnergy += length(rb1->Velocity);

		//rb1->Velocity += m_Gravity * deltaTime;
		*rb1->Position += rb1->Velocity * deltaTime;

		// Check collision on world bounds
		if ((*rb1->Position).y + rb1->Radius > m_WorldBounds[1].y)
		{
			(*rb1->Position).y = m_WorldBounds[1].y - rb1->Radius;
			rb1->Velocity.y *= -m_RestitutionCoefficient;
		}
		else if ((*rb1->Position).y - rb1->Radius < m_WorldBounds[0].y)
		{
			(*rb1->Position).y = m_WorldBounds[0].y + rb1->Radius;
			rb1->Velocity.y *= -m_RestitutionCoefficient;
		}
		if ((*rb1->Position).x + rb1->Radius > m_WorldBounds[1].x)
		{
			(*rb1->Position).x = m_WorldBounds[1].x - rb1->Radius;
			rb1->Velocity.x *= -m_RestitutionCoefficient;
		}
		else if ((*rb1->Position).x - rb1->Radius < m_WorldBounds[0].x)
		{
			(*rb1->Position).x = m_WorldBounds[0].x + rb1->Radius;
			rb1->Velocity.x *= -m_RestitutionCoefficient;
		}
#ifdef HASH_GRID
		Get().m_SpatialHashGrid.Query(*rb1->Position, 2.0f * rb1->Radius);
		for (uint32_t i = 0; i < Get().m_SpatialHashGrid.QuerySize; ++i)
		{
			Rigidbody* rb2 = Get().m_SpatialHashGrid.QueryResult[i];
#else
		for (Rigidbody* rb2 : m_Rigidbodies)
		{
#endif
			if (rb1 == rb2)
				continue;

			vec2 collisionDir = *rb2->Position - *rb1->Position;
			float distanceSqr = dot(collisionDir, collisionDir);
			float sumRadius = rb1->Radius + rb2->Radius;

			if (distanceSqr < sumRadius * sumRadius)
			{
				float overlapLength = sumRadius - sqrt(distanceSqr);
				vec2 collisionDirNormalized = normalize(collisionDir);

				// Velocity component in collision direction
				float colVel1 = dot(rb1->Velocity, collisionDirNormalized);
				float colVel2 = dot(rb2->Velocity, collisionDirNormalized);

				// Calculate the new velocity in the collision direction
				float newColVel1 = (rb1->Mass * colVel1 + rb2->Mass * colVel2
					- rb2->Mass * (colVel1 - colVel2) * m_RestitutionCoefficient) / (rb1->Mass + rb2->Mass);
				float newColVel2 = (rb1->Mass * colVel1 + rb2->Mass * colVel2
					- rb1->Mass * (colVel2 - colVel1) * m_RestitutionCoefficient) / (rb1->Mass + rb2->Mass);

				vec2 posCorrection = collisionDirNormalized * overlapLength * .5f;
				*rb1->Position -= posCorrection;
				*rb2->Position += posCorrection;

				rb1->Velocity += collisionDirNormalized * (newColVel1 - colVel1);
				rb2->Velocity += collisionDirNormalized * (newColVel2 - colVel2);
			}
		}
	}
}

/*
Implementation of Verlet Integration.
From Youtube channel Pezzza's Work:
https://www.youtube.com/watch?v=lS_qeBy3aQI
*/
void PhysicsSystem::VerletSolver(float deltaTime)
{
	Get().m_SpatialHashGrid.Create(m_Rigidbodies);

	deltaTime = glm::min(deltaTime, 1.0f / 60); // Limit frametime so physics doesn't break too soon
	float subDt = deltaTime / m_SubSteps;
	float subDtSqr = subDt * subDt;

	for (uint32_t subStep = 0; subStep < m_SubSteps; ++subStep)
	{
		for (Rigidbody* rb1 : m_Rigidbodies)
		{
			rb1->Velocity = *rb1->Position - rb1->OldPosition;
			rb1->OldPosition = *rb1->Position;

			rb1->Acceleration += m_Gravity;
			*rb1->Position += rb1->Velocity + rb1->Acceleration * subDtSqr;
			rb1->Acceleration = { 0.0f };

			// Check collision on world bounds
			if ((*rb1->Position).y + rb1->Radius > m_WorldBounds[1].y)
				(*rb1->Position).y = m_WorldBounds[1].y - rb1->Radius;
			else if ((*rb1->Position).y - rb1->Radius < m_WorldBounds[0].y)
				(*rb1->Position).y = m_WorldBounds[0].y + rb1->Radius;
			if ((*rb1->Position).x + rb1->Radius > m_WorldBounds[1].x)
				(*rb1->Position).x = m_WorldBounds[1].x - rb1->Radius;
			else if ((*rb1->Position).x - rb1->Radius < m_WorldBounds[0].x)
				(*rb1->Position).x = m_WorldBounds[0].x + rb1->Radius;

			Get().m_SpatialHashGrid.Query(*rb1->Position, rb1->Radius * 2.0f);
			for (uint32_t i = 0; i < Get().m_SpatialHashGrid.QuerySize; ++i)
			{
				Rigidbody* rb2 = Get().m_SpatialHashGrid.QueryResult[i];

				if (rb1 == rb2) continue;

				vec2 collisionVec = *rb2->Position - *rb1->Position;
				float sumRadius = rb1->Radius + rb2->Radius;

				if (dot(collisionVec, collisionVec) < sumRadius * sumRadius)
				{
					float collisionDistance = length(collisionVec);
					float collisionOverlap = sumRadius - collisionDistance;
					vec2 collisionDir = collisionVec / collisionDistance;
					float massRatio = rb1->Mass / (rb1->Mass + rb2->Mass);

					*rb1->Position -= collisionDir * collisionOverlap * massRatio;
					*rb2->Position += collisionDir * collisionOverlap * (1.0f - massRatio);
				}
			}
		}
	}
}

void PhysicsSystem::CustomSolver(float deltaTime)
{
	Get().m_SpatialHashGrid.Create(m_Rigidbodies);

	m_SystemEnergy = 0.0f;
	for (Rigidbody* rb1 : m_Rigidbodies)
	{
		rb1->Velocity += m_Gravity * deltaTime;
		*rb1->Position += rb1->Velocity * deltaTime;

		// Check collision on screen bounds
		if ((*rb1->Position).y - rb1->Radius < -1.0f)
		{
			(*rb1->Position).y = -0.999999999f + rb1->Radius;
			rb1->Velocity.y *= -m_RestitutionCoefficient;
		}
		else if ((*rb1->Position).y + rb1->Radius > 1.0f)
		{
			(*rb1->Position).y = 0.999999999f - rb1->Radius;
			rb1->Velocity.y *= -m_RestitutionCoefficient;
		}
		if ((*rb1->Position).x + rb1->Radius > 1.0f)
		{
			(*rb1->Position).x = 0.999999999f - rb1->Radius;
			rb1->Velocity.x *= -m_RestitutionCoefficient;
		}
		else if ((*rb1->Position).x - rb1->Radius < -1.0f)
		{
			(*rb1->Position).x = -0.999999999f + rb1->Radius;
			rb1->Velocity.x *= -m_RestitutionCoefficient;
		}

		// Check collision on other circles
		Get().m_SpatialHashGrid.Query(*rb1->Position, 2.0f * rb1->Radius);
		for (uint32_t i = 0; i < Get().m_SpatialHashGrid.QuerySize; ++i)
		{
			Rigidbody* rb2 = Get().m_SpatialHashGrid.QueryResult[i];
			if (rb1 == rb2)
				continue;

			vec2 collisionDir = *rb2->Position - *rb1->Position;
			float distance = length(collisionDir);

			float overlapLength = rb1->Radius + rb2->Radius - distance;
			if (overlapLength > 0.0f)
			{
				// Adjust position so it doesn't overlap anymore
				vec2 collisionDirNormalized = normalize(collisionDir);
				*rb1->Position -= collisionDirNormalized * overlapLength * 0.5f;
				*rb2->Position += collisionDirNormalized * overlapLength * 0.5f;

				if (dot(rb1->Velocity, rb1->Velocity) == 0)
					continue;

				// How much energy will be transfered from rb2 to rb
				float energyTransferCoefficient = dot(normalize(rb1->Velocity), collisionDirNormalized);

				rb2->Velocity +=
					collisionDirNormalized
					* length(rb1->Velocity)
					* energyTransferCoefficient
					* m_RestitutionCoefficient;

				rb1->Velocity *= (1.0f - energyTransferCoefficient) * m_RestitutionCoefficient;

				rb1->Velocity = reflect(rb1->Velocity, collisionDirNormalized);
			}
		}
		m_SystemEnergy += length(rb1->Velocity);
	}
}

float PhysicsSystem::SystemEnergy()
{
	return Get().m_SystemEnergy;
}