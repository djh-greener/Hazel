#pragma once
#include"Hazel/Core/Core.h"
#include<glm/glm.hpp>

namespace Hazel::UI
{


	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100);
	
}
