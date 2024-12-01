#include<Hazel.h>
#include"glm/gtc/matrix_transform.hpp"
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
		m_VATriangle.reset(VertexArray::Create());
		m_VATriangle->Bind();
		{
			float vertices[3 * 7] = {
				   -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
					0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
					0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
			};
			std::shared_ptr<VertexBuffer>vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
			BufferLayout layout =
			{
				 {ShaderDataType::Float3,"aPosition"},
				 {ShaderDataType::Float4,"aColor"},
			};
			vertexBuffer->SetLayout(layout);
			m_VATriangle->AddVertexBuffer(vertexBuffer);

			unsigned int indices[3] = { 0,1,2 };
			std::shared_ptr<IndexBuffer>indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			m_VATriangle->SetIndexBuffer(indexBuffer);
		}

		//Square
		m_VASquare.reset(VertexArray::Create());
		m_VASquare->Bind();
		{
			float squareVertices[3 * 4] = {
				-0.75f, -0.75f, 0.0f,
				 0.75f, -0.75f, 0.0f,
				 0.75f,  0.75f, 0.0f,
				-0.75f,  0.75f, 0.0f
			};
			std::shared_ptr<VertexBuffer>vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
			BufferLayout layout =
			{
				 {ShaderDataType::Float3,"aPosition"},
			};
			vertexBuffer->SetLayout(layout);
			m_VASquare->AddVertexBuffer(vertexBuffer);

			unsigned int indices[6] = { 0,1,2 ,2,3,0 };
			std::shared_ptr<IndexBuffer>indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			m_VASquare->SetIndexBuffer(indexBuffer);
		}
		{
			std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_Projection;
			uniform mat4 u_View;
			uniform mat4 u_Model;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_Projection*u_View*u_Model*vec4(a_Position, 1.0);	
			}
		)";

			std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

			m_ShaderTriangle.reset(new Shader(vertexSrc, fragmentSrc));

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

			
			void main()
			{
				color = vec4(0.2f,0.3f,0.8f,1.0f);
			}
		)";

			m_ShaderSquare.reset(new Shader(blueVertexSrc, blueFragmentSrc));
		}
	}

	virtual void OnImGuiRender()override
	{
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
		glm::mat4 transform = glm::translate(glm::mat4(1), m_SquarePosition);

		RenderCommand::ClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();
		Renderer::BeginScene(m_Camera);
		Renderer::Submit(m_ShaderSquare, m_VASquare,transform);

		Renderer::Submit(m_ShaderTriangle, m_VATriangle);
		Renderer::EndScene();
	}


private:
	std::shared_ptr<Shader>m_ShaderTriangle;
	std::shared_ptr<Shader>m_ShaderSquare;
	std::shared_ptr<VertexArray>m_VATriangle;
	std::shared_ptr<VertexArray>m_VASquare;

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
