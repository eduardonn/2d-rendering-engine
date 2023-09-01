#pragma once

class Shader
{
public:
	Shader(const char* path);
	~Shader();
	void Use();
protected:
	unsigned int m_ID;
};