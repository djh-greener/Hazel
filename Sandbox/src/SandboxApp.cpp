#include<Hazel.h>
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"Platform/OpenGL/OpenGLShader.h"
#include"imgui/imgui.h"
using namespace Hazel;

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),
		m_Camera(*(new OrthographicCamera(-1.6, 1.6, -0.9, 0.9))),
		m_CameraPosition(0), m_CameraRotation(0),
		m_SquarePosition(0)
	{
		//Triangle
		m_VATriangle = VertexArray::Create();
		m_VATriangle->Bind();
		{
			float vertices[3 * 7] = {
				   -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
					0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
					0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
			};
			Ref<VertexBuffer>vertexBuffer;
			vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
			BufferLayout layout =
			{
				 {ShaderDataType::Float3,"aPosition"},
				 {ShaderDataType::Float4,"aColor"},
			};
			vertexBuffer->SetLayout(layout);
			m_VATriangle->AddVertexBuffer(vertexBuffer);

			unsigned int indices[3] = { 0,1,2 };
			Ref<IndexBuffer>indexBuffer;
			indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
			m_VATriangle->SetIndexBuffer(indexBuffer);
		}

		//Square
		m_VASquare = VertexArray::Create();
		m_VASquare->Bind();
		{
			float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
			};
			Ref<VertexBuffer>vertexBuffer;
			vertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
			BufferLayout layout =
			{
				 {ShaderDataType::Float3,"aPosition"},
				 {ShaderDataType::Float2,"aTexCoord"},

			};
			vertexBuffer->SetLayout(layout);
			m_VASquare->AddVertexBuffer(vertexBuffer);

			unsigned int indices[6] = { 0,1,2 ,2,3,0 };
			Ref<IndexBuffer>indexBuffer;
			indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
			m_VASquare->SetIndexBuffer(indexBuffer);

			m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		}
		{
			std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_Projection;
			uniform mat4 u_View;
			uniform mat4 u_Model;

			out vec4 v_Color;
			void main()
			{
				v_Color = a_Color;
				gl_Position = u_Projection*u_View*u_Model*vec4(a_Position, 1.0);	
			}
		)";

			std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)";

			m_ShaderTriangle.reset(Shader::Create(vertexSrc, fragmentSrc));

			std::string blueVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_Projection;
			uniform mat4 u_View;
			uniform mat4 u_Model;

			void main()
			{
				gl_Position =u_Projection * u_View * u_Model * vec4(a_Position, 1.0);	
			}
		)";

			std::string blueFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			uniform vec3 u_Color;
			void main()
			{
				color = vec4(u_Color,1);
			}
		)";

			m_ShaderSquare.reset(Shader::Create(blueVertexSrc, blueFragmentSrc));

			std::string TextureVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_Projection;
			uniform mat4 u_View;
			uniform mat4 u_Model;
			
			out vec2 TexCoord;
			void main()
			{
				gl_Position =u_Projection * u_View * u_Model * vec4(a_Position, 1.0);	
				TexCoord = a_TexCoord;
			}
		)";

			std::string TextureFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec2 TexCoord;
			uniform sampler2D u_Texture;
			void main()
			{
				color = vec4(TexCoord,0,1);
				color = texture(u_Texture,TexCoord);
			}
		)";

			m_ShaderTexture.reset(Shader::Create(TextureVertexSrc, TextureFragmentSrc));
			std::dynamic_pointer_cast<OpenGLShader>(m_ShaderTexture)->Bind();
			std::dynamic_pointer_cast<OpenGLShader>(m_ShaderTexture)->UploadUniformInt("u_Texture", 0);
		}
	}

	virtual void OnImGuiRender()override
	{
		//ImGui::Begin("Test");
		//ImGui::End();
	}
	void OnUpdate(Timestep ts) override
	{
		if (Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		m_Camera.SetPosition(m_CameraPosition);

		//Camera Anticlockwise rotate along z axis
		if (Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation.z += m_CameraRotateSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation.z -= m_CameraRotateSpeed * ts;
		m_Camera.SetRotation(m_CameraRotation);

		if (Input::IsKeyPressed(HZ_KEY_J))
			m_SquarePosition.x -= m_CameraMoveSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_L))
			m_SquarePosition.x += m_CameraMoveSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_K))
			m_SquarePosition.y -= m_CameraMoveSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_I))
			m_SquarePosition.y += m_CameraMoveSpeed * ts;

		RenderCommand::ClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();
		Renderer::BeginScene(m_Camera);
		

		m_Texture->Bind();
		Renderer::Submit(m_ShaderTexture, m_VASquare, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//Renderer::Submit(m_ShaderTriangle, m_VATriangle);
		Renderer::EndScene();
	}


private:
	Ref<Shader>m_ShaderTriangle;
	Ref<Shader>m_ShaderSquare,m_ShaderTexture;
	Ref<VertexArray>m_VATriangle;
	Ref<VertexArray>m_VASquare;
	Ref<Texture2D>m_Texture;

	Camera& m_Camera;
	glm::vec3 m_CameraPosition;
	glm::vec3 m_CameraRotation;
	float m_CameraMoveSpeed = 0.1;
	float m_CameraRotateSpeed = 10;

	glm::vec3 m_SquarePosition;
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer);
	}

	~Sandbox()
	{

	}

};
namespace Hazel {
	Application* CreateApplication()
	{
		return new Sandbox;
	}
}
