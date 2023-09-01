#pragma once

#include "Mesh.hpp"
#include "rendering/Renderer.hpp"
#include <memory>

class Object
{
public:
	Object();
	~Object();

	virtual void Init() {}
	virtual void Update() {}
	void Disable();
};