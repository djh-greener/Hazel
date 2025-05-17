#include"hzpch.h"
#include"SceneSerializer.h"
#include"Entity.h"
#include"Components.h"
#include"Hazel/Camera/CameraComponent.h"
#include"Hazel/Renderer/Material/Material.h"
#include"Hazel/Renderer/Mesh/StaticMeshComponent.h"
#include"Hazel/Renderer/Light/PointLightComponent.h"
#include"Hazel/Renderer/Light/DirLightComponent.h"

#include<yaml-cpp/yaml.h>

namespace YAML {
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}
namespace Hazel {
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y<< YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene)
	{
		 
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; 
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			std::cout << tag << std::endl;

			out << YAML::EndMap; 
		}
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::BeginMap; 
				out << YAML::Key << "Translation" << YAML::Value << tc.Position;
				out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
				out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap; 
		}
		if (entity.HasComponent<CameraComponent>())
		{
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; 
				out << YAML::Key << "Camera" << YAML::Value;
				out << YAML::BeginMap; 
					out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
					out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
					out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
				out << YAML::EndMap; 

				out << YAML::Key << "MoveSpeed" << YAML::Value << cameraComponent.MoveSpeed;
				out << YAML::Key << "RotateSpeed" << YAML::Value << cameraComponent.RotateSpeed;
				out << YAML::Key << "LastMousePos" << YAML::Value << cameraComponent.LastMousePos;
				out << YAML::Key << "PrimaryID" << YAML::Value << cameraComponent.PrimaryID;
			out << YAML::EndMap; 
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; 
				auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
				out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
				if (spriteRendererComponent.Texture)
					out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.Texture->GetPath();

				out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;

			out << YAML::EndMap; 
		}
		if (entity.HasComponent<StaticMeshComponent>())
		{
			out << YAML::Key << "StaticMeshComponent" << YAML::BeginMap;  
			{
				auto& staticMeshComp = entity.GetComponent<StaticMeshComponent>();

				if (!staticMeshComp.directory.empty()) 
					out << YAML::Key << "Path" << YAML::Value<< (staticMeshComp.directory / staticMeshComp.name).string();

				out << YAML::Key << "Materials" << YAML::BeginSeq;  
				for (auto& mesh : staticMeshComp.meshes) {
					out << YAML::BeginMap;  
					{
						out << YAML::Key << "MeshName" << YAML::Value << mesh->name;

						out << YAML::Key << "Textures" << YAML::BeginSeq;
						for (auto& texture : mesh->m_Material->textures) {
							out << texture->GetShaderUniformName() + ":" + texture->GetPath();  
						}
						out << YAML::EndSeq;
					}
					out << YAML::EndMap;
				}
				out << YAML::EndSeq;
			}
			out << YAML::EndMap;  
		}
		if (entity.HasComponent<PointLightComponent>())
		{
			out << YAML::Key << "PointLightComponent";
			out << YAML::BeginMap;
			auto& PointLightComp = entity.GetComponent<PointLightComponent>();
			out << YAML::Key << "Color" << YAML::Value << PointLightComp.Color;
			out << YAML::Key << "Linear" << YAML::Value << PointLightComp.Linear;
			out << YAML::Key << "Quadratic" << YAML::Value << PointLightComp.Quadratic;
			out << YAML::Key << "Intensity" << YAML::Value << PointLightComp.Intensity;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<DirLightComponent>())
		{
			out << YAML::Key << "DirLightComponent";
			out << YAML::BeginMap;
			auto& DirLightComp = entity.GetComponent<DirLightComponent>();
			out << YAML::Key << "Color" << YAML::Value << DirLightComp.Color;
			out << YAML::Key << "Intensity" << YAML::Value << DirLightComp.Intensity;

			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key<<"Scene"<< YAML::Value << "Untitled";
		out << YAML::Key << "Entities"<< YAML::Value << YAML::BeginSeq;
		//entt's view is reverse, so reverse to save
		auto view = m_Scene->m_Registry.view<entt::entity>();
		for (auto it = view.rbegin(), last = view.rend(); it != last; ++it) {
			Entity entity = { *it, m_Scene.get() };
			if (!entity) {
				continue;
			}
			SerializeEntity(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

	}
	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		HZ_CORE_ASSERT(false,"Not Implemented");

	}
	bool SceneSerializer::DeSerialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			HZ_CORE_ERROR("Failed to load .hazel file '{0}'\n     {1}", filepath, e.what());
			return false;
		}
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		HZ_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entitiesRootNode = data["Entities"];
		if (!entitiesRootNode)
			return true;
		//reverse DeSerialize entities
		std::vector<YAML::Node>entities(entitiesRootNode.begin(), entitiesRootNode.end());
		for (auto it = entities.begin(); it !=entities.end(); it++)
		{
			YAML::Node entity = *it;
			uint64_t uuid = entity["Entity"].as<uint64_t>(); 

			std::string name;
			auto tagComponent = entity["TagComponent"];
			if (tagComponent)
				name = tagComponent["Tag"].as<std::string>();

			HZ_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

			Entity deserializedEntity = m_Scene->CreateEntity(name);

			auto transformComponent = entity["TransformComponent"];
			if (transformComponent)
			{
				// Entities always have transforms
				auto& tc = deserializedEntity.GetComponent<TransformComponent>();
				tc.Position = transformComponent["Translation"].as<glm::vec3>();
				tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc.Scale = transformComponent["Scale"].as<glm::vec3>();
			}

			auto cameraComponent = entity["CameraComponent"];
			if (cameraComponent)
			{
				auto& cc = deserializedEntity.AddComponent<CameraComponent>();

				auto& cameraProps = cameraComponent["Camera"];

				cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
				cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
				cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

				cc.MoveSpeed = cameraComponent["MoveSpeed"].as<float>();
				cc.RotateSpeed = cameraComponent["RotateSpeed"].as<float>();
				cc.LastMousePos = cameraComponent["LastMousePos"].as<glm::vec2>();
				cc.PrimaryID = cameraComponent["PrimaryID"].as<int>();
			}

			auto spriteRendererComponent = entity["SpriteRendererComponent"];
			if (spriteRendererComponent)
			{
				auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
				src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				if (spriteRendererComponent["TexturePath"])
					src.Texture = Texture2D::Create(spriteRendererComponent["TexturePath"].as<std::string>());
				if (spriteRendererComponent["TilingFactor"])
					src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();

			}

			auto StaticMeshComp = entity["StaticMeshComponent"];
			if (StaticMeshComp)
			{
				auto& src = deserializedEntity.AddComponent<StaticMeshComponent>();
				fspath path;
				std::unordered_map<std::string, Ref<Material>> overrideMaterialPerMesh;
				if (StaticMeshComp["Path"])
				{
					path = StaticMeshComp["Path"].as<std::string>();
				}
				if (StaticMeshComp["Materials"]) {
					const YAML::Node& materialsNode = StaticMeshComp["Materials"];
					for (const auto& matNode : materialsNode) {

						std::string meshName;
						if (matNode["MeshName"]) {
							meshName = matNode["MeshName"].as<std::string>();
						}
						std::vector < Ref<Texture2D>> textures;
						auto meshMaterial = CreateRef<Material>(textures);
						if (matNode["Textures"]) {
							for (const auto& texStr : matNode["Textures"]) {
								std::string texInfo = texStr.as<std::string>();
								size_t colonPos = texInfo.find(':');
								if (colonPos != std::string::npos) {
									std::string uniformName = texInfo.substr(0, colonPos);
									std::string path = texInfo.substr(colonPos + 1);
									auto Texture = Texture2D::Create(path, uniformName == "diffuse");
									Texture->SetShaderUniformName(uniformName);
									meshMaterial->textures.push_back(Texture);
								}
							}
						}
						overrideMaterialPerMesh[meshName] = meshMaterial;
					}
					src.loadStaticMesh(path, overrideMaterialPerMesh);
				}
			}

			auto PointLightComp = entity["PointLightComponent"];
			if (PointLightComp)
			{
				auto& src = deserializedEntity.AddComponent<PointLightComponent>();
				if (PointLightComp["Color"])
					src.Color = PointLightComp["Color"].as<glm::vec3>();
				if (PointLightComp["Linear"])
					src.Linear = PointLightComp["Linear"].as<float>();
				if (PointLightComp["Quadratic"])
					src.Quadratic = PointLightComp["Quadratic"].as<float>();
				if (PointLightComp["Intensity"])
					src.Intensity = PointLightComp["Intensity"].as<float>();
			}
			auto DirLightComp = entity["DirLightComponent"];
			if (DirLightComp)
			{
				auto& src = deserializedEntity.AddComponent<DirLightComponent>();
				if (DirLightComp["Color"])
					src.Color = DirLightComp["Color"].as<glm::vec3>();
				if (DirLightComp["Intensity"])
					src.Intensity = DirLightComp["Intensity"].as<float>();
			}
		}


		return true;
	}
	bool SceneSerializer::DeSerializeRuntime(const std::string& filepath)
	{
		HZ_CORE_ASSERT(false,"Not Implemented");
		return false;
	}
}
