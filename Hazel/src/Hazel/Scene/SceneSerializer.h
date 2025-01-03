#pragma once


#include"Scene.h"
#include"Hazel/Core/Core.h"
namespace Hazel {
	class SceneSerializer
	{
	public:
		 SceneSerializer(const Ref<Scene>& scene);

		 void Serialize(const std::string& filepath);
		 void SerializeRuntime(const std::string& filepath);

		 bool DeSerialize(const std::string& filepath);
		 bool DeSerializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}