#pragma once
#include"Hazel/Core/Core.h"
#include<optional>
namespace Hazel
{
	class Texture2D;
	class Shader;
	class Material
	{
	public:
		Material(std::vector < Ref<Texture2D>>&& textures) :textures(std::move(textures)) {};
		Material(std::vector < Ref<Texture2D>>& textures) :textures(textures) {};
		std::vector < Ref<Texture2D>> textures;
		void Apply(Ref<Shader>shader);
		void UnApply();
	};
}
