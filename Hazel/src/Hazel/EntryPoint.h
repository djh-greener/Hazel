
#ifdef HZ_PLATFORM_WINDOWS
//hide main function
extern Hazel::Application* Hazel::CreateApplication();

int main() {
	Hazel::Log::Init();
	Hazel::Application* application = Hazel::CreateApplication();
	application->Run();
	delete application;
	return 0;
}
#endif