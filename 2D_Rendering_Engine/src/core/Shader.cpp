#include "Shader.hpp"
#include "utils/shaders_utils.hpp"
#include <GL/glew.h>

Shader::Shader(const char* path)
{
	ShaderProgramSource source = parseShader(path);
	m_ID = createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::Use()
{
	glUseProgram(m_ID);
}