#include"hzpch.h"
#include "Material.h"

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/Shader.h"
void Hazel::Material::Apply(Ref<Shader> shader)
{
	if (textures.empty())
	{
		auto texture = Texture2D::Create("Resources/Icons/SceneHierarchyPanel/TextureIcon.png",true);
		texture->SetShaderUniformName("diffuse");
		textures.push_back(texture);
	}
	unsigned int diffuseNr = 0, specularNr = 0, ambientNr = 0, normalNr = 0;
	for (int i = 0; i < textures.size(); i++) {
		textures[i]->Bind(i);
		shader->Bind();
		std::string varName = "material." + textures[i]->GetShaderUniformName();
		if (textures[i]->GetShaderUniformName() == "diffuse")
			varName += std::to_string(++diffuseNr);
		else if (textures[i]->GetShaderUniformName() == "specular")
			varName += std::to_string(++specularNr);
		else if (textures[i]->GetShaderUniformName() == "ambient")
			varName += std::to_string(++ambientNr);
		else if (textures[i]->GetShaderUniformName() == "normal")
			varName += std::to_string(++normalNr);
		else
			HZ_ERROR("Error Texture Type!");
		shader->SetInt(varName, i);

	}
}

void Hazel::Material::UnApply()
{
	for (int i = 0; i < textures.size(); i++) {
		textures[i]->UnBind(i);
	}
}
