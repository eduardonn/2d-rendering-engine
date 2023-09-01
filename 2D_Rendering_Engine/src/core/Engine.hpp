#pragma once

#include "rendering/Renderer.hpp"
#include "World.hpp"
#include "PhysicsSystem.hpp"

struct GLFWwindow;

class Engine
{
public:
	void Run();

protected:
	virtual void Init();
	virtual void Update(float deltaTime);

	GLFWwindow* m_Window;
	World m_World;

private:

	int InitWindow();
	void MainLoop();
};