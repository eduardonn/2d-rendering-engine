#include "Ball.hpp"
#include "core/Time.hpp"
#include "core/utils/debug_utils.hpp"

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtx/transform.hpp>

#include <memory>

Ball::Ball(const glm::vec2& pos, std::shared_ptr<BasicShader> shader)
	: m_Shader(shader)
{
	ShaderData = shader->AddObject(pos, m_Color);
	m_InitialPosition = pos;
	Rb = new Rigidbody(ShaderData.Position, 0.01f);
}

Ball::~Ball()
{
	m_Shader->RemoveObject(ShaderData);
	delete Rb;
}

void Ball::Update()
{
	//SetPosition(m_InitialPosition + glm::vec2(
 //       glm::sin(Time::GetElapsedTime() * m_RotationSpeed + m_RotationOffset) * m_RotationRadius,
	//	glm::cos(Time::GetElapsedTime() * m_RotationSpeed + m_RotationOffset) * m_RotationRadius
 //   ));
 //   
	 //m_Shader->SetColor(m_RenderData.ColorIndex, m_Color);
}

glm::vec2& Ball::Position()
{
	return *ShaderData.Position;
}