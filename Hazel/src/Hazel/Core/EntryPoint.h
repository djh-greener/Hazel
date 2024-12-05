
#ifdef HZ_PLATFORM_WINDOWS
//hide main function
namespace Hazel {
	extern Application* CreateApplication();
}

int main() {
	Hazel::Log::Init();
	auto application = Hazel::CreateApplication();
	application->Run();
	delete application;
	return 0;
}
#endif