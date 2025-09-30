#include "hzpch.h"
#include "Renderer3D.h"

#include"Hazel/Core/GlobalData.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/UniformBuffer.h"
#include"Hazel/Scene/Components.h"
#include"Hazel/Renderer/Mesh/StaticMeshComponent.h"
#include "Hazel/Camera/CameraComponent.h"
#include"Hazel/Renderer/Light/PointLightComponent.h"
#include"Hazel/Renderer/Light/DirLightComponent.h"
#include"Hazel/Scene/Scene.h"


#include<glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Hazel {
	
	struct Renderer3DData {
		const uint32_t MaxPointLights = 1024;
		const uint32_t MaxDirLights = 4;

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
		struct DirLightData
		{//std140 layout
			glm::vec3 Direction;
			float Pad1;
			glm::vec3 Color;
			float Pad2;
			glm::mat4 LightSpaceMatrix;
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
		std::vector<DirLightData> DirLightsBuffer;
		Ref<UniformBuffer> DirLightsUniformBuffer;

		std::unordered_map<std::string, Ref<Shader>> Shaders;
		std::unordered_map<std::string, Ref<Framebuffer>> Framebuffers;
	};
	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);

		s_Data.PointLightsBuffer.resize(s_Data.MaxPointLights);
		s_Data.PointLightsUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::PointLightData)*s_Data.MaxPointLights, 1);

		s_Data.DirLightsBuffer.resize(s_Data.MaxDirLights);
		s_Data.DirLightsUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::DirLightData) * s_Data.MaxDirLights, 2);

		s_Data.Shaders["Mesh"] = Shader::Create("assets/shaders/Mesh.shader");
		s_Data.Shaders["Light"] = Shader::Create("assets/shaders/Light.shader");
		s_Data.Shaders["SingleColor"] = Shader::Create("assets/shaders/SingleColor.shader");
		s_Data.Shaders["ShadowMap"] = Shader::Create("assets/shaders/ShadowMap.shader");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		fbSpec.Samples = 1;
		s_Data.Framebuffers["Default"] = Framebuffer::Create(fbSpec);

		// Shadow map framebuffer
		FramebufferSpecification shadowFbSpec;
		shadowFbSpec.Width = 1024;
		shadowFbSpec.Height = 1024;
		shadowFbSpec.Attachments = { FramebufferTextureFormat::DEPTH24STENCIL8 };
		shadowFbSpec.FixedSize = true;
		s_Data.Framebuffers["ShadowMap"] = Framebuffer::Create(shadowFbSpec);
	}


	void Renderer3D::RendererScene(entt::registry& SceneRegistry)
	{
		//Find Main Camera, Set Shader's CameraUniformBlock 
		auto CameraView = SceneRegistry.view< CameraComponent, TransformComponent>();
		CameraComponent* MainCameraComp = nullptr;
		for (auto entity : CameraView)
		{
			auto& [CameraComp, TransformComp] = CameraView.get< CameraComponent, TransformComponent>(entity);

			if (CameraComp.IsPrimary())
			{
				MainCameraComp = &CameraComp;
				s_Data.CameraBuffer.View = CameraComp.GetViewMatrix();
				s_Data.CameraBuffer.Projection = CameraComp.GetProjMatrix();
				s_Data.CameraBuffer.ViewPos = TransformComp.Position;
				s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));
				break;
			}
		}
		// Pass1 :Render depth map from light's View
		{
			s_Data.Framebuffers["ShadowMap"]->Bind();
			glClear(GL_DEPTH_BUFFER_BIT);
			uint32_t ShadowMapWidth = s_Data.Framebuffers["ShadowMap"]->GetSpecification().Width;
			uint32_t ShadowMapHeight = s_Data.Framebuffers["ShadowMap"]->GetSpecification().Height;
			glViewport(0, 0, ShadowMapWidth, ShadowMapHeight);

			// Set up light space matrices for each directional light
			auto DirLightView = SceneRegistry.view<TransformComponent, DirLightComponent>();
			int numDirLights = 0;
			for (auto& entity : DirLightView)
			{
				auto& [TransformComp, DirLightComp] = DirLightView.get<TransformComponent, DirLightComponent>(entity);

				// Calculate light space matrix
				glm::mat4 lightView = glm::lookAt(TransformComp.Position, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));

				glm::mat4 lightProjection = glm::ortho<float>(-40,40,-40,40,0.1,100);

				s_Data.DirLightsBuffer[numDirLights].LightSpaceMatrix = lightProjection * lightView;

				// Render scene depth to shadow map
				//TODO: For Multi DirLights,Render Multi Depth Map,Now Only Support One DirLight
				s_Data.Shaders["ShadowMap"]->Bind();
				s_Data.Shaders["ShadowMap"]->SetMat4("u_LightSpaceMatrix", s_Data.DirLightsBuffer[numDirLights].LightSpaceMatrix);

				RenderSceneMeshes(SceneRegistry, s_Data.Shaders["ShadowMap"]);

				numDirLights++;
			}
		}
		
		s_Data.Framebuffers["Default"]->Bind();
		uint32_t Width = s_Data.Framebuffers["Default"]->GetSpecification().Width;
		uint32_t Height = s_Data.Framebuffers["Default"]->GetSpecification().Height;
		glViewport(0, 0, Width, Height);
		glStencilMask(0x00);


		//Draw And Set Light
		s_Data.Shaders["Light"]->Bind();
		auto PointLightView = SceneRegistry.view<TransformComponent, PointLightComponent>();
		int numPointLights = 0;
		for (auto &entity : PointLightView)
		{
			auto &[TransformComp, PointLightComp] = PointLightView.get< TransformComponent, PointLightComponent>(entity);
			//Draw PointLightCube
			s_Data.Shaders["Light"]->SetMat4("u_Model", TransformComp.GetTransform());
			s_Data.Shaders["Light"]->SetFloat3("u_Color", PointLightComp.Color * PointLightComp.Intensity);
			PointLightComp.DrawMesh(s_Data.Shaders["Light"]);
			//Set PointLight UnifromBuffer
			s_Data.PointLightsBuffer[numPointLights].Position = TransformComp.Position;
			s_Data.PointLightsBuffer[numPointLights].Color = PointLightComp.Color * PointLightComp.Intensity;
			s_Data.PointLightsBuffer[numPointLights].Linear = PointLightComp.Linear;
			s_Data.PointLightsBuffer[numPointLights].Quadratic = PointLightComp.Quadratic;
			numPointLights++;
		}
		s_Data.PointLightsUniformBuffer->SetData(s_Data.PointLightsBuffer.data(), sizeof(Renderer3DData::PointLightData)* numPointLights);

		auto DirLightView = SceneRegistry.view<TransformComponent, DirLightComponent>();
		int numDirLights = 0;
		for (auto& entity : DirLightView)
		{
			auto& [TransformComp, DirLightComp] = DirLightView.get< TransformComponent, DirLightComponent>(entity);
			s_Data.Shaders["Light"]->SetMat4("u_Model", TransformComp.GetTransform());
			s_Data.Shaders["Light"]->SetFloat3("u_Color", DirLightComp.Color * DirLightComp.Intensity);
			DirLightComp.DrawMesh(s_Data.Shaders["Light"]);

			s_Data.DirLightsBuffer[numDirLights].Direction = -TransformComp.Position ;
			s_Data.DirLightsBuffer[numDirLights].Color = DirLightComp.Color * DirLightComp.Intensity;
			numDirLights++;
		}
		s_Data.DirLightsUniformBuffer->SetData(s_Data.DirLightsBuffer.data(), sizeof(Renderer3DData::DirLightData) * numDirLights);


		//Draw Static Mesh
		s_Data.Shaders["Mesh"]->Bind();
		s_Data.Shaders["Mesh"]->SetInt("numPointLights", numPointLights);
		s_Data.Shaders["Mesh"]->SetInt("numDirLights", numDirLights);
		auto DepthTexture = s_Data.Framebuffers["ShadowMap"]->GetDepthAttachmentRendererID();
		glBindTextureUnit(10, DepthTexture);
		s_Data.Shaders["Mesh"]->SetInt("DepthTexture", 10);
		RenderSceneMeshes(SceneRegistry, s_Data.Shaders["Mesh"]);

		

		//Stencil Test
		auto& SelectedEntity = GlobalData::GetGlobalData().SelectedEntity;
		if (SelectedEntity)
		{
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			s_Data.Shaders["SingleColor"]->Bind();
			if (SelectedEntity.HasComponent<StaticMeshComponent>())
			{
				auto& TransformComp = SelectedEntity.GetComponent<TransformComponent>();
				auto& MeshComp = SelectedEntity.GetComponent<StaticMeshComponent>();
				s_Data.Shaders["SingleColor"]->SetMat4("u_Model", TransformComp.GetTransform());
				MeshComp.DrawStaticMesh(s_Data.Shaders["SingleColor"]);
			}
			glStencilMask(0xFF);
		}

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
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
	void RenderSceneMeshes(entt::registry& SceneRegistry,Ref<Shader>& CurrentShader)
	{
		auto& SelectedEntity = GlobalData::GetGlobalData().SelectedEntity;
		auto MeshView = SceneRegistry.view<TransformComponent, StaticMeshComponent>();
		bool WriteStencil = CurrentShader->GetName() == "Mesh";
		for (auto& entity : MeshView)
		{
			if (WriteStencil && entity == SelectedEntity)
			{
				glStencilMask(0xFF);
				auto& [TransformComp, StaticMeshComp] = MeshView.get< TransformComponent, StaticMeshComponent>(entity);
				CurrentShader->SetMat4("u_Model", TransformComp.GetTransform());
				StaticMeshComp.DrawStaticMesh(CurrentShader);
				glStencilMask(0x00);
			}
			else {
				auto& [TransformComp, StaticMeshComp] = MeshView.get< TransformComponent, StaticMeshComponent>(entity);
				CurrentShader->SetMat4("u_Model", TransformComp.GetTransform());
				StaticMeshComp.DrawStaticMesh(CurrentShader);
			}
		}
	}
	//Bug
	std::vector<float> CalcProjectionRange(CameraComponent& CameraComp, const glm::mat4& lightViewMatrix)
	{
		auto cameraViewMatrix = CameraComp.GetViewMatrix();
		float AspectRatio = CameraComp.Camera.GetPerspectiveAspectRatio();
		float FOV = CameraComp.Camera.GetPerspectiveVerticalFOV();
		float Near = CameraComp.Camera.GetPerspectiveNearClip();
		float Far = CameraComp.Camera.GetPerspectiveFarClip();

		float tanHalfFOV = tan(FOV);
		float nearHeight = 2.0f * tanHalfFOV * Near;
		float nearWidth = nearHeight * AspectRatio;
		float farHeight = 2.0f * tanHalfFOV * Far;
		float farWidth = farHeight * AspectRatio;

		//Calc 8 Frustum Corners in Camera View Space
		glm::vec3 Corners[8] = {
			glm::vec3(-nearWidth * 0.5f, -nearHeight * 0.5f, -Near),
			glm::vec3(nearWidth * 0.5f, -nearHeight * 0.5f, -Near),
			glm::vec3(nearWidth * 0.5f, nearHeight * 0.5f, -Near),
			glm::vec3(-nearWidth * 0.5f, nearHeight * 0.5f, -Near),
			glm::vec3(-farWidth * 0.5f, -farHeight * 0.5f, -Far),
			glm::vec3(farWidth * 0.5f, -farHeight * 0.5f, -Far),
			glm::vec3(farWidth * 0.5f, farHeight * 0.5f, -Far),
			glm::vec3(-farWidth * 0.5f, farHeight * 0.5f, -Far),
		};

		auto invcameraViewMatrix = glm::inverse(cameraViewMatrix);
		for (int i = 0; i < 8; i++) {
			Corners[i] = glm::vec4(Corners[i], 1.0f);// to World Space
			Corners[i] = lightViewMatrix * glm::vec4(Corners[i], 1.0f);	//to Light View Space
		}
		glm::vec3 minBounds = Corners[0];
		glm::vec3 maxBounds = Corners[0];
		for (int i = 0; i < 8; i++) {
			minBounds = glm::min(minBounds, Corners[i]);
			maxBounds = glm::max(maxBounds, Corners[i]);
		}
		std::vector<float>res(6);
		res[0]= minBounds.x;//left
		res[1]= maxBounds.x;//right
		res[2] = minBounds.y;//bottom
		res[3] = maxBounds.y;//top
		res[4] = minBounds.z;//near
		res[5] = maxBounds.z;//far
		return res;
	}
}
