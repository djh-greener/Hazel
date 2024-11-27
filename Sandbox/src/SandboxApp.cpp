#include<Hazel.h>
//#include"imgui/imgui.h"
class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	virtual void OnImGuiRender()override
	{
		//ImGui::Begin("Test");
		//ImGui::Text("ExampleLayer");
		//ImGui::End();
	}
	void OnUpdate() override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("Tab Key is Pressed");
	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_TRACE("{0}", event);
	}

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
