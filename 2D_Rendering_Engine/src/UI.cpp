#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include <stdio.h>

#include "UI.hpp"
#include "core/World.hpp"
#include "core/Time.hpp"
#include "core/rendering/Renderer.hpp"
#include "core/PhysicsSystem.hpp"
#include "core/utils/debug_utils.hpp"
#include <fstream>
#include <glm/glm.hpp>

UI::UI(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_IO = ImGui::GetIO(); (void)m_IO;
	m_IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	m_IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void UI::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	m_FrameTime = Time::ElapsedTime() - m_ElapsedTime;
	m_ElapsedTime = Time::ElapsedTime();
	m_MaxFrameTime = glm::max(m_MaxFrameTime, (float)m_FrameTime);
	m_MinFrameTime = glm::min(m_MinFrameTime, (float)m_FrameTime);
	m_FrameTimesSum += m_FrameTime;
	++m_FrameTimeCount;

	ImGui::Begin("Profiler");
	ImGui::Text("Frametime: %.3f ms/frame (%.1f FPS)", m_FrameTime * 1000.0f, 1.0f / m_FrameTime);
	ImGui::Text("Max Frametime: %.3f ms/frame (%.1f FPS)", m_MaxFrameTime * 1000.0f, 1.0f / m_MaxFrameTime);
	ImGui::Text("Min Frametime: %.3f ms/frame (%.1f FPS)", m_MinFrameTime * 1000.0f, 1.0f / m_MinFrameTime);
	ImGui::Text("Average Frametime: %.3f ms/frame (%.1f FPS)",
		m_FrameTimesSum / m_FrameTimeCount * 1000.0f, 1.0f / (m_FrameTimesSum / m_FrameTimeCount));
	ImGui::InputText("File name", m_FileNameBuffer, sizeof(m_FileNameBuffer));
	if (ImGui::Button("Save"))
		this->SaveLogFile(m_FileNameBuffer);
	ImGui::End();

	ImGui::Begin("Physics System");
	ImGui::Text("System Energy: %.2f", PhysicsSystem::SystemEnergy());
	ImGui::Text("Number of objects: %d", World::g_Objects.size());
	ImGui::End();
}

void UI::SaveLogFile(const std::string& fileName)
{
	double avgFrameTime = m_FrameTimesSum / m_FrameTimeCount;
	std::ofstream outStream;
	outStream.open(fileName + ".log", std::ios::out);
	outStream << "Number of objects: " << Renderer::GetObjectCount() << std::endl << std::endl;
	outStream << "\t\t\tFrametime\tFPS:" << std::endl;
	outStream << "Min:\t\t" << m_MinFrameTime * 1000.0f << "ms\t" << 1.0f / m_MinFrameTime << " FPS" << std::endl;
	outStream << "Average:\t" << avgFrameTime * 1000.0f << "ms\t" << 1.0f / avgFrameTime << " FPS" << std::endl;
	outStream << "Max:\t\t" << m_MaxFrameTime * 1000.0f << "ms\t" << 1.0f / m_MaxFrameTime << " FPS" << std::endl;
	outStream.close();
}

void UI::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UI::~UI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}