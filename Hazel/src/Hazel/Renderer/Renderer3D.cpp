#include "hzpch.h"
#include "Renderer3D.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/UniformBuffer.h"
#include"Hazel/Scene/Components.h"
#include"Hazel/Renderer/StaticMeshComponent.h"
#include "Hazel/Camera/CameraComponent.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Hazel {
	struct Renderer3DData {
		struct CameraData
		{
			glm::mat4 View;
			glm::mat4 Projection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;

		std::unordered_map<std::string, Ref<Shader>> Shaders;
		std::unordered_map<std::string, Ref<Framebuffer>> Framebuffers;
	};
	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
		s_Data.Shaders["Default"] = Shader::Create("assets/shaders/Model.glsl");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		fbSpec.Samples = 1;
		s_Data.Framebuffers["Default"] = Framebuffer::Create(fbSpec);
	}

	void Renderer3D::RendererScene(entt::registry& SceneRegistry)
	{
		//Find Main Camera, Set Shader's Uniform View&Projection
		auto CameraView = SceneRegistry.view< CameraComponent>();
		for (auto entity : CameraView)
		{
			auto& CameraComp = CameraView.get< CameraComponent>(entity);
			if (CameraComp.IsPrimary())
			{
				s_Data.Shaders["Default"]->Bind();
				s_Data.CameraBuffer.View = CameraComp.GetProjMatrix();
				s_Data.CameraBuffer.Projection = CameraComp.GetViewMatrix();
				s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));
				break;
			}
		}

		//Draw Static Mesh
		auto MeshGroup = SceneRegistry.view<TransformComponent,StaticMeshComponent>();
		for (auto entity : MeshGroup)
		{
			auto [TransformComp,StaticMeshComp ]= MeshGroup.get< TransformComponent, StaticMeshComponent>(entity);
			//TODO: Instance
			s_Data.Shaders["Default"]->Bind();

			s_Data.Shaders["Default"]->SetMat4("u_Model", TransformComp.GetTransform());
			StaticMeshComp.DrawStaticMesh(s_Data.Shaders["Default"]);
		}

	}
	void Renderer3D::OnViewportResize(uint32_t ViewportWidth, uint32_t ViewportHeight)
	{
		for (auto& fbo : s_Data.Framebuffers)
			fbo.second->Resize(ViewportWidth, ViewportHeight);

	}
	Framebuffer& Renderer3D::GetLastFramebuffer()
	{
		return *s_Data.Framebuffers["Default"];
	}
}
