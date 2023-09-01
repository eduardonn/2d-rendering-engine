#pragma once

#include "vendor/imgui/imgui.h"
#include "core/rendering/Rendered.hpp"
#include "core/Object.hpp"

#include <GLFW/glfw3.h>
#include <memory>
#include <string>
#include <vector>

class UI : public Object, Rendered
{
public:
	UI(GLFWwindow* window);
	~UI();

	void Update() final;
	void Render() final;

	// TODO: Move to debug_utils
	void SaveLogFile(const std::string& fileName);

	ImGuiIO m_IO;
	double m_ElapsedTime = 0;
	float m_MaxFrameTime = 0;
	float m_MinFrameTime = FLT_MAX;
	double m_FrameTime = 0;
	double m_FrameTimesSum = 0;
	uint64_t m_FrameTimeCount = 0;
	char m_FileNameBuffer[128] = {};
};