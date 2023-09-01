#pragma once

#include "core/Engine.hpp"
#include "core/Mesh.hpp"
#include "core/Time.hpp"

#include "BasicShader.hpp"
#include "Ball.hpp"
#include "UI.hpp"

#include <memory>
#include <vector>

class Simulation : public Engine
{
public:
	Simulation();
	Simulation(const Simulation& other);
    ~Simulation();
private:
    void Init() final;
    void Update(float deltaTime) final;
	void RestartSimulation();
    void SetupObjectsGrid(int numRows, int numCols, float margin);
    void SetupObjectsColliding();
    void SetupUI();
	void SpawnBall(double x, double y);

    std::vector<Ball*> m_Balls;
    std::shared_ptr<Mesh> m_CircleMesh;
    std::shared_ptr<BasicShader> m_Shader;
    std::unique_ptr<UI> m_UI;

	float m_BallControlSpeed = 10.0f;
	double m_BallSpawnLastTime = Time::ElapsedTime();
	float m_BallSpawnInterval = 0.01f;
	float m_BallSpawnRadius = 0.05f;
	bool m_Jumped = false;
	float m_ShowingCellElementsInterval = 0.1f;
	double m_LastGridTestTime = Time::ElapsedTime();
};