#pragma once
#include"Hazel/Core/Core.h"
#include"Hazel/Scene/Entity.h"


#include<filesystem>
#include "assimp/scene.h"

namespace Hazel {
	class Shader;
	class Texture2D;
	class StaticMesh;
	class Material;
	using fspath = std::filesystem::path;

	class StaticMeshComponent
	{
	public:
		StaticMeshComponent() = default;
		void DrawStaticMesh(Ref<Shader> shader);
		void loadStaticMesh(fspath path,std::unordered_map<std::string,Ref<Material>> overrideMaterialPerMesh);
	private:
		Ref<StaticMesh> processStaticMesh(aiMesh* mesh, const aiScene* scene, std::unordered_map<std::string, Ref<Material>>& overrideMaterialPerMesh);
		std::vector< Ref<Texture2D>> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	public:
		Entity Owner;
		std::vector<Ref<StaticMesh>>meshes;
		fspath directory;
		fspath name;
		std::vector<Ref<Texture2D>> textures_loaded;
	};
}
