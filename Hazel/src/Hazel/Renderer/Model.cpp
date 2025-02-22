#include"hzpch.h"
#include "Model.h"
#include"Hazel/Renderer/Mesh.h"
#include"Hazel/Renderer/Shader.h"
#include"Hazel/Renderer/Texture.h"

namespace Hazel {
	void Model::DrawModelRecursive(Ref<MeshNode> node, Ref<Shader> shader)
	{
		for (auto mesh : node->meshes)
			mesh->DrawMesh(shader);
		for (auto childnode : node->children) {
			DrawModelRecursive(childnode, shader);
		}
	}

	Ref<Mesh> Model::GetMeshRecursive(Ref<MeshNode> node)
	{
		for (auto mesh : node->meshes)
			return mesh;
		for (auto childnode : node->children)
			return GetMeshRecursive(childnode);
	}

	Ref<Mesh> Model::GetMesh()
	{
		return GetMeshRecursive(RootNode);
	}

	void Model::loadModel(std::string path)
	{
		Assimp::Importer importer;
		auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			HZ_ERROR("ASSIMP::", importer.GetErrorString());
			return;
		}
		directory = path.substr(0, path.find_last_of('/')) + "/";
		RootNode = CreateRef<MeshNode>();
		processNode(scene->mRootNode, RootNode, scene);
	}

	void Model::processNode(aiNode* ainode, Ref<MeshNode> node, const aiScene* scene)
	{
		for (int i = 0; i < ainode->mNumMeshes; i++) {
			aiMesh* aimesh = scene->mMeshes[ainode->mMeshes[i]];
			node->meshes.push_back(processMesh(aimesh, scene));
		}
		for (int i = 0; i < ainode->mNumChildren; i++) {
			node->children.push_back(CreateRef<MeshNode>());
			processNode(ainode->mChildren[i], node->children[i], scene);
		}
	}

	Ref<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex>vertices;
		std::vector<unsigned int>indices;
		std::vector<Ref<Texture2D>>textures;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex v;
			v.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			v.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			if (mesh->mTextureCoords[0]) {
				v.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				v.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
				v.BiTangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
			}
			else
			{
				v.TexCoords = glm::vec2(0);
				v.Tangent = glm::vec3(0);
				v.BiTangent = glm::vec3(0);
			}
			v.EntityID = 100;
			vertices.push_back(v);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		if (mesh->mMaterialIndex > 0)
		{
			auto* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Ref<Texture2D>> ambients = loadMaterialTextures(material, aiTextureType_AMBIENT, "ambient");
			std::vector<Ref<Texture2D>> diffuses = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
			std::vector<Ref<Texture2D>> speculars = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
			std::vector<Ref<Texture2D>> normals = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
			textures.insert(textures.end(), ambients.begin(), ambients.end());
			textures.insert(textures.end(), diffuses.begin(), diffuses.end());
			textures.insert(textures.end(), speculars.begin(), speculars.end());
			textures.insert(textures.end(), normals.begin(), normals.end());

		}
		else
			HZ_ERROR("Loading No Material In This Mesh." );
		//return CreateRef<Mesh>(std::move(vertices), std::move(indices), std::move(textures));
		return CreateRef<Mesh>(vertices, (indices), (textures));

	}

	std::vector< Ref<Texture2D>> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector< Ref<Texture2D>>textures;
		int a = material->GetTextureCount(type);
		for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
			aiString str;
			material->GetTexture(type, i, &str);//按照默认命名规则，获取纹理文件名
			std::string path = directory + str.C_Str();
			bool skip = false;
			for (auto tex : textures_loaded) {
				if (path == tex->GetPath()) {
					textures.push_back(tex);
					skip = true;
					break;
				}
			}
			if (!skip) {
				auto texture =Texture2D::Create(path);
				texture->GetShaderUniformName() = typeName;
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}

		}
		return textures;
	}
}
