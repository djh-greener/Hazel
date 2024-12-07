#include<Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Sandbox2D/Sandbox2D.h"
using namespace Hazel;
class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D);
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
