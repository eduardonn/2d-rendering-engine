#pragma once

#include "core/Object.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/random.hpp>
#include <memory>
#include "BasicShader.hpp"
#include "core/Rigidbody.hpp"

class Ball : Object
{
public:
    Ball(const glm::vec2& pos, std::shared_ptr<BasicShader> shader);
	~Ball();

    void Update() final;

	glm::vec2& Position();

	BasicShaderData ShaderData;
	Rigidbody* Rb;

private:
	std::shared_ptr<BasicShader> m_Shader;
	glm::vec2 m_InitialPosition;
	glm::vec3 m_Color{ glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f) };
	float m_RotationOffset = glm::linearRand(0.0f, 3.1415f + 3.1415f * .5f);
    float m_RotationRadius = 0.05f;
    float m_RotationSpeed = glm::linearRand(1.0f, 2.5f);

};