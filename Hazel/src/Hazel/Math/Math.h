#pragma once
#include<glm/glm.hpp>

namespace Hazel::Math{
	bool DeComposeTransform(const glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

}
