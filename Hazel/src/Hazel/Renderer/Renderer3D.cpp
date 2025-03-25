#include "hzpch.h"
#include "Renderer3D.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/UniformBuffer.h"
#include"Hazel/Scene/Components.h"
#include"Hazel/Renderer/Mesh/StaticMeshComponent.h"
#include"Hazel/Renderer/Mesh/BaseGeometryComponent.h"
#include "Hazel/Camera/CameraComponent.h"
#include"Hazel/Renderer/Light/PointLightComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Hazel {

	struct Renderer3DData {
		const uint32_t Max_PointLights = 32;
		struct PointLightData
		{//std140 layout
			glm::vec3 Position;
			float Pad1;
			glm::vec3 Color;
			float Pad2;
			float Linear;
			float Quadratic;
			glm::vec2 Pad3;
		};
		struct CameraData
		{
			glm::mat4 View;
			glm::mat4 Projection;
			glm::vec3  ViewPos;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
		std::vector<PointLightData> PointLightsBuffer;
		Ref<UniformBuffer> PointLightsUniformBuffer;

		std::unordered_map<std::string, Ref<Shader>> Shaders;
		std::unordered_map<std::string, Ref<Framebuffer>> Framebuffers;
	};
	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		HZ_ERROR("size: {0}",sizeof(Renderer3DData::PointLightData));
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);

		s_Data.PointLightsBuffer.resize(s_Data.Max_PointLights);
		s_Data.PointLightsUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::PointLightData)*s_Data.Max_PointLights, 1);

		s_Data.Shaders["Mesh"] = Shader::Create("assets/shaders/Mesh.shader");
		s_Data.Shaders["PointLight"] = Shader::Create("assets/shaders/PointLight.shader");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		fbSpec.Samples = 1;
		s_Data.Framebuffers["Default"] = Framebuffer::Create(fbSpec);
	}

	void Renderer3D::RendererScene(entt::registry& SceneRegistry)
	{
		//Find Main Camera, Set Shader's CameraUniformBlock 
		
		auto CameraView = SceneRegistry.view< CameraComponent,TransformComponent>();
		for (auto entity : CameraView)
		{
			auto& CameraComp = CameraView.get< CameraComponent>(entity);
			auto& TransformComp = CameraView.get< TransformComponent>(entity);

			if (CameraComp.IsPrimary())
			{
				s_Data.CameraBuffer.View = CameraComp.GetProjMatrix();
				s_Data.CameraBuffer.Projection = CameraComp.GetViewMatrix();
				s_Data.CameraBuffer.ViewPos = TransformComp.Position;

				s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));
				break;
			}
		}

		//Set LightUniformBlock
		s_Data.Shaders["PointLight"]->Bind();
		auto LightView = SceneRegistry.view<TransformComponent, PointLightComponent>();
		int numLights = 0;
		for (auto &entity : LightView)
		{
			auto &[TransformComp, PointLightComp] = LightView.get< TransformComponent, PointLightComponent>(entity);
			//Draw PointLightCube
			s_Data.Shaders["PointLight"]->SetMat4("u_Model", TransformComp.GetTransform());
			s_Data.Shaders["PointLight"]->SetFloat3("u_Color", PointLightComp.Color);
			PointLightComp.DrawMesh(s_Data.Shaders["PointLight"]);
			//Set PointLight UnifromBuffer
			s_Data.PointLightsBuffer[numLights].Position = TransformComp.Position;
			s_Data.PointLightsBuffer[numLights].Color = PointLightComp.Color;
			s_Data.PointLightsBuffer[numLights].Linear = PointLightComp.Linear;
			s_Data.PointLightsBuffer[numLights].Quadratic = PointLightComp.Quadratic;
			numLights++;
		}

		s_Data.PointLightsUniformBuffer->SetData(s_Data.PointLightsBuffer.data(), sizeof(Renderer3DData::PointLightData)* numLights);
		s_Data.Shaders["Mesh"]->Bind();
		s_Data.Shaders["Mesh"]->SetInt("numPointLights", numLights);

		//Draw Static Mesh
		auto MeshView = SceneRegistry.view<TransformComponent,StaticMeshComponent>();
		for (auto &entity : MeshView)
		{
			auto &[TransformComp,StaticMeshComp ]= MeshView.get< TransformComponent, StaticMeshComponent>(entity);
			//TODO: Instance

			s_Data.Shaders["Mesh"]->SetMat4("u_Model", TransformComp.GetTransform());
			StaticMeshComp.DrawStaticMesh(s_Data.Shaders["Mesh"]);
		}
		//Draw Base Geometry Mesh
		auto BaseMeshView = SceneRegistry.view<TransformComponent, BaseGeometryComponent>();
		for (auto entity : BaseMeshView)
		{
			auto [TransformComp, BaseMeshComp] = BaseMeshView.get< TransformComponent, BaseGeometryComponent>(entity);
			s_Data.Shaders["Mesh"]->SetMat4("u_Model", TransformComp.GetTransform());
			BaseMeshComp.DrawMesh(s_Data.Shaders["Mesh"]);
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
