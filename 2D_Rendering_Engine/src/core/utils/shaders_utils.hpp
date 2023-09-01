#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

ShaderProgramSource parseShader(const std::string& filePath);
unsigned int compileShader(unsigned int type, const std::string& source);
unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);