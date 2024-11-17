#include<Hazel.h>
class Sandbox : public Hazel::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};
namespace Hazel {
	Application* CreateApplication()
	{
		return new Sandbox;
	}
}