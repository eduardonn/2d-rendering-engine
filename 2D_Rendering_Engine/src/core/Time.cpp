#include <GLFW/glfw3.h>

namespace Time
{
	static double m_ElapsedTime = 0.0f;
	static double m_DeltaTime = 0.0f;

	double ElapsedTime()
	{
		return m_ElapsedTime;
	}

	double DeltaTime()
	{
		return m_DeltaTime;
	}

	void Update()
	{
		m_DeltaTime = glfwGetTime() - m_ElapsedTime;
		m_ElapsedTime = glfwGetTime();
	}
}