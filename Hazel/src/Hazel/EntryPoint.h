
#ifdef HZ_PLATFORM_WINDOWS
//hide main function
extern Hazel::Application* Hazel::CreateApplication();

int main() {
	Hazel::Log::Init();
	
	HZ_CORE_WARN("Hazel Warning");
	HZ_INFO("App Info:{0}",199);

	Hazel::Application* application = Hazel::CreateApplication();
	application->Run();
	delete application;
	return 0;
}
#endif