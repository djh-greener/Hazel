#pragma once
#include"Hazel/Core/Core.h"
#include"Hazel/Scene/Entity.h"


#include<filesystem>
#include "assimp/scene.h"

namespace Hazel {
	class Shader;
	class Texture2D;
	class StaticMesh;
	using fspath = std::filesystem::path;

	struct StaticMeshNode {
		std::vector<Ref<StaticMesh>>meshes;
		std::vector<Ref<StaticMeshNode>>children;
	};
	class StaticMeshComponent
	{

	public:
		StaticMeshComponent() = default;
		void DrawStaticMesh(Ref<Shader> shader);
		
		//返回Mesh树的第一个Mesh
		Ref<StaticMesh>GetStaticMesh(){ return GetStaticMeshRecursive(RootNode); }

		void loadStaticMesh(fspath path);
		void processNode(aiNode* ainode, Ref<StaticMeshNode> node, const aiScene* scene);
		Ref<StaticMesh> processStaticMesh(aiMesh* mesh, const aiScene* scene);
		std::vector< Ref<Texture2D>> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);

		void DrawStaticMeshRecursive(Ref<StaticMeshNode> node, Ref<Shader> shader);
		Ref<StaticMesh>GetStaticMeshRecursive(Ref<StaticMeshNode> node);


		Entity Owner;
		Ref<StaticMeshNode>RootNode;//the mesh tree that use same shader
		fspath directory;
		fspath name;
		std::vector<Ref<Texture2D>> textures_loaded;
	};
}
