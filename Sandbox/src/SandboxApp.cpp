#include<Hazel.h>

using namespace Hazel;

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),
		m_Camera(*(new OrthographicCamera(-1.6, 1.6, -0.9, 0.9))),
		m_CameraPosition(0)
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

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_Projection*u_View*vec4(a_Position, 1.0);	
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

			void main()
			{
				gl_Position =u_Projection*u_View * vec4(a_Position, 1.0);	
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
	void OnUpdate() override
	{
		if (Input::IsKeyPressed(HZ_KEY_LEFT) || Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed;
		if (Input::IsKeyPressed(HZ_KEY_RIGHT) || Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed;
		if (Input::IsKeyPressed(HZ_KEY_DOWN) || Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed;
		if (Input::IsKeyPressed(HZ_KEY_UP) || Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed;
		m_Camera.SetPosition(m_CameraPosition);

		RenderCommand::ClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();
		Renderer::BeginScene(m_Camera);
		Renderer::Submit(m_ShaderSquare, m_VASquare);

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

	float m_CameraMoveSpeed = 0.001;
	float m_CameraRotateSpeed = 0.1;

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
