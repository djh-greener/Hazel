#pragma once

#include"glm/glm.hpp"
#include"Hazel/Camera/Camera.h"
namespace Hazel {

	//face to XoY plane,so only rotation.z is used
	class OrthographicCamera: public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);

	private:
		void RecalculateProjectionViewMatrix()override;
		void Init(float left, float right, float bottom, float top);
	};
}