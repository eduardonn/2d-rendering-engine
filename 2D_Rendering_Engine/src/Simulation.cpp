#include "Ball.hpp"
#include "Simulation.hpp"
#include "core/Mesh.hpp"
#include "core/PhysicsSystem.hpp"
#include "core/utils/debug_utils.hpp"
#include "core/utils/utils.hpp"
#include "core/Config.hpp"

#include <memory>
#include <glm/gtc/random.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

Simulation::Simulation(const Simulation& other)
{}

Simulation::Simulation()
{}

void Simulation::Init()
{
	m_CircleMesh = std::shared_ptr<Mesh>(Mesh::Circle(8, 0.01f));
	m_Shader = std::make_shared<BasicShader>(m_CircleMesh);
	
    //SetupObjectsColliding();
	SetupObjectsGrid(32, 32, 0.2f);
	SetupUI();
}

void Simulation::SetupObjectsColliding()
{
	m_Balls.push_back(new Ball({ -0.5f, 0.0f }, m_Shader));
	m_Balls[0]->Rb->Velocity = { 0.3f, 0.0f };
	m_Balls.push_back(new Ball({ 0.5f, 0.0f }, m_Shader));
	m_Balls[1]->Rb->Velocity = { -0.6f, 0.0f };
}

void Simulation::SetupObjectsGrid(int numRows, int numCols, float margin)
{
    float minCoord = -1 + margin;
    float maxCoord = 1 - margin;
	m_Balls.reserve(numRows * numCols);

    for (float row = 0; row < numRows; ++row)
    {
        for (float col = 0; col < numCols; ++col)
        {
            m_Balls.push_back(new Ball(
                {
                    Math::toRange(col, 0.f, numCols - 1.f, minCoord, maxCoord) + glm::linearRand(-0.01f, 0.01f),
                    Math::toRange(row, 0.f, numRows - 1.f, minCoord, maxCoord)
                },
				m_Shader
            ));
        }
    }
}

void Simulation::Update(float deltaTime)
{
	glm::vec2 movementDir(0.0f);

	// WASD Movement
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		movementDir.y += 1.0f;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		movementDir.y -= 1.0f;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		movementDir.x -= 1.0f;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		movementDir.x += 1.0f;
	}
	if (movementDir.x != 0 || movementDir.y != 0)
	{
		m_Balls[0]->Rb->AddVelocity(normalize(movementDir) * m_BallControlSpeed * deltaTime);
	}

	// Jump
	if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!m_Jumped)
		{
			m_Jumped = true;
			m_Balls[0]->Rb->AddVelocity({ 0.0f, 3.0f });
		}
	}
	else
		m_Jumped = false;

	// Spawn Balls
	if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		if (Time::ElapsedTime() - m_BallSpawnLastTime > m_BallSpawnInterval)
		{
			m_BallSpawnLastTime = Time::ElapsedTime();
			double screenX, screenY, x, y;
			glfwGetCursorPos(m_Window, &screenX, &screenY);

			for (uint32_t i = 0; i < 10; ++i)
			{
				x = screenX * 2.0f / Config::c_WindowWidth - 1.0f;
				y = -(screenY * 2.0f / Config::c_WindowHeight - 1.0f);

				x += glm::linearRand(-m_BallSpawnRadius, m_BallSpawnRadius);
				x = glm::clamp(x, -1.0 + m_BallSpawnRadius, 1.0 - m_BallSpawnRadius);
				y += glm::linearRand(-m_BallSpawnRadius, m_BallSpawnRadius);
				y = glm::clamp(y, -1.0 + m_BallSpawnRadius, 1.0 - m_BallSpawnRadius);

				SpawnBall(x, y);
			}
		}
	}

	// Draw circle on mouse position
	//double x, y;
	//glfwGetCursorPos(m_Window, &x, &y);
	//x = x * 2.0f / Config::c_WindowWidth - 1.0f;
	//y = -(y * 2.0f / Config::c_WindowHeight - 1.0f);
	//Renderer::DrawCircle({ (float)x, (float)y }, 0.1f);

	// Background decoration
	Renderer::DrawCircle({ 0.6f, -0.2f }, 0.4f, { 0.2f, 0.0f, 0.5f, 0.2f });
	Renderer::DrawCircle({ -0.6f, 0.3f }, 0.7f, { 0.2f, 0.0f, 0.5f, 0.2f });
	
	// Hash grid test: by pressing H, balls' colors from the queried grids are
	// changed and number of queried balls are printed
	if (glfwGetKey(m_Window, GLFW_KEY_H) == GLFW_PRESS)
	{
		if (Time::ElapsedTime() - m_LastGridTestTime < m_ShowingCellElementsInterval) return;
		m_LastGridTestTime = Time::ElapsedTime();

		double x, y;
		glfwGetCursorPos(m_Window, &x, &y);
		x = x * 2.0f / Config::c_WindowWidth - 1.0f;
		y = -(y * 2.0f / Config::c_WindowHeight - 1.0f);

		auto hashGrid = PhysicsSystem::HashGrid();
		hashGrid.Query({ x, y }, m_Balls[0]->Rb->Radius);
		RAW_LOG_LN("QuerySize: " << hashGrid.QuerySize);
		for (uint32_t i = 0; i < hashGrid.QuerySize; ++i)
		{
			auto foundRb = hashGrid.QueryResult[i];
			for (Ball* ball : m_Balls)
			{
				if (ball->Rb == foundRb)
				{
					*ball->ShaderData.Color = {
						glm::linearRand(0.0f, 1.0f),
						glm::linearRand(0.0f, 1.0f),
						glm::linearRand(0.0f, 1.0f)
					};
					break;
				}
			}
		}
	}

	// Restart Simulation
	if (glfwGetKey(m_Window, GLFW_KEY_R) == GLFW_PRESS)
	{
		RestartSimulation();
	}

	if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		exit(0);
	}
}

void Simulation::RestartSimulation()
{
	for (int i = 0; i < m_Balls.size(); ++i)
	{
		delete m_Balls[i];
	}

	m_Balls.clear();
	m_Shader->Clear();

	SetupObjectsGrid(32, 32, 0.2f);
}

void Simulation::SpawnBall(double x, double y)
{
	m_Balls.push_back(new Ball({ x, y }, m_Shader));
}

void Simulation::SetupUI()
{
	m_UI = std::make_unique<UI>(m_Window);
}

Simulation::~Simulation()
{
	for (int i = 0; i < m_Balls.size(); ++i)
	{
		delete m_Balls[i];
	}

	m_Balls.clear();
	m_Shader->Clear();
}