#pragma once
#include"Hazel/Core/Core.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace Hazel {
	class Shader;
	class Texture2D;
	class Mesh;

	struct MeshNode {
		std::vector<Ref<Mesh>>meshes;
		std::vector<Ref<MeshNode>>children;
	};

	class Model
	{
	public:
		Model(std::string path) {
			loadModel(path);
		}
		void DrawModel(Ref<Shader> shader) { DrawModelRecursive(RootNode, shader); }
		//����Mesh���ĵ�һ��Mesh
		Ref<Mesh>GetMesh();
	private:
		Ref<MeshNode>RootNode;//ʹ��ͬһ����ɫ����������
		std::string directory;//ģ������Ŀ¼
		std::vector<Ref<Texture2D>> textures_loaded;//ȫ�ֵ��Ѽ�������

		void loadModel(std::string path);
		void processNode(aiNode* ainode, Ref<MeshNode> node, const aiScene* scene);
		Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector< Ref<Texture2D>> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);

		void DrawModelRecursive(Ref<MeshNode> node, Ref<Shader> shader);
		Ref<Mesh>GetMeshRecursive(Ref<MeshNode> node);
	};
}
