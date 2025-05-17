#include"hzpch.h"
#include "StaticMeshComponent.h"

#include"Hazel/Scene/Components.h"
#include"Hazel/Renderer/Material/Material.h"
#include"Hazel/Renderer/Mesh/StaticMesh.h"
#include"Hazel/Renderer/Shader.h"
#include"Hazel/Renderer/Texture.h"
#include"Hazel/Core/Timer.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <filesystem>
#include<queue>

namespace Hazel {

	void StaticMeshComponent::DrawStaticMesh(Ref<Shader> shader)
	{
		for (size_t i = 0; i < meshes.size(); i++)
		{
			meshes[i]->DrawStaticMesh(shader);
		}
	}

	namespace fs = std::filesystem;
	void StaticMeshComponent::loadStaticMesh(fspath path, std::unordered_map<std::string, Ref<Material>> overrideMaterialPerMesh)
	{
		Assimp::Importer importer;
		auto scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			HZ_ERROR("ASSIMP::", importer.GetErrorString());
			return;
		}
		fs::path fs_path(path);
		directory = fs_path.parent_path();
		name = fs_path.filename();
		
		//BFS
		//TODO:MultiThread
		std::queue<aiNode*>queue;
		queue.push(scene->mRootNode);
		while (!queue.empty())
		{
			aiNode* curNode = queue.front();
			queue.pop();

			for (size_t i = 0; i < curNode->mNumMeshes; i++)
			{
				meshes.push_back(processStaticMesh(scene->mMeshes[curNode->mMeshes[i]], scene, overrideMaterialPerMesh));
			}
			for (size_t i = 0; i < curNode->mNumChildren; i++)
			{
				queue.push(curNode->mChildren[i]);
			}
		}

	}


	Ref<StaticMesh> StaticMeshComponent::processStaticMesh(aiMesh* mesh, const aiScene* scene, std::unordered_map<std::string, Ref<Material>>& overrideMaterialPerMesh)
	{
		std::vector<StaticMeshVertex>vertices(mesh->mNumVertices);
		std::vector<uint32_t>indices;
		std::vector<Ref<Texture2D>>textures;
		std::string meshName = mesh->mName.C_Str();
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			vertices[i].Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertices[i].Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			if (mesh->mTextureCoords[0]) {
				vertices[i].TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
				vertices[i].Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				vertices[i].BiTangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}
			else
			{
				vertices[i].TexCoords = { 0,0 };
				vertices[i].Tangent = { 0,0,0 };
				vertices[i].BiTangent = { 0,0,0 };
			}
			vertices[i].EntityID = (uint32_t)Owner;
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		if (overrideMaterialPerMesh.empty())
		{
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
				HZ_ERROR("Loading No Material In This Mesh.");
		}
		else
		{
			if (overrideMaterialPerMesh.find(meshName)!= overrideMaterialPerMesh.end())
			{
				textures = overrideMaterialPerMesh[meshName]->textures;
			}
		}
		auto staticMesh = CreateRef<StaticMesh>(vertices, indices, std::move(textures));
		staticMesh->name = meshName;
		return staticMesh;
	}

	std::vector< Ref<Texture2D>> StaticMeshComponent::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector< Ref<Texture2D>>textures;
		int a = material->GetTextureCount(type);
		for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
			aiString str;
			material->GetTexture(type, i, &str);//按照默认命名规则，获取纹理文件名
			std::string path = (fs::path(directory) / fs::path(str.C_Str())).string();
			bool skip = false;
			for (auto tex : textures_loaded) 
			{
				if (path == tex->GetPath()) 
				{
					textures.push_back(tex);
					skip = true;
					break;
				}
			}
			if (!skip) {
				//TODO:only diffuse gamma is ture
				auto texture =Texture2D::Create(path,true,false);

				texture->SetShaderUniformName(typeName);
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}

		}
		return textures;
	}
}
