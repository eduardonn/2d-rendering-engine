#include "Engine.hpp"
#include "utils/shaders_utils.hpp"
#include "utils/debug_utils.hpp"
#include "Object.hpp"
#include "Time.hpp"
#include "Config.hpp"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void Engine::Run()
{
	InitWindow();
	Init();
	Renderer::Init();
	MainLoop();
}

void Engine::MainLoop()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		float deltaTime = static_cast<float>(Time::DeltaTime());

		Time::Update();
		this->Update(deltaTime);
		m_World.Update();
		PhysicsSystem::Update(deltaTime);
		Renderer::Render();

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void Engine::Init()
{
}

void Engine::Update(float deltaTime)
{
}

int Engine::InitWindow()
{
	if (!glfwInit())
		return -1;

	//glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(
		Config::c_WindowWidth,
		Config::c_WindowHeight,
		Config::c_WindowTitle,
		NULL,
		NULL
	);

	if (!m_Window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(m_Window);

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW init error" << std::endl;

	//glfwSwapInterval(1);

	std::cout << (const char*)glGetString(GL_VERSION) << std::endl;

	return 0;
}