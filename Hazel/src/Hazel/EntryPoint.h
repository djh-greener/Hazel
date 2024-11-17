
#ifdef HZ_PLATFORM_WINDOWS
//隐藏main函数，创建Application的行为由用户定义，入口点由引擎调用
extern Hazel::Application* Hazel::CreateApplication();

int main() {
	Hazel::Application* application = Hazel::CreateApplication();
	application->Run();
	delete application;
	return 0;
}
#endif