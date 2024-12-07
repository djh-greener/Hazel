#include "Hazel/Core/Core.h"
#ifdef HZ_PLATFORM_WINDOWS
//hide main function
namespace Hazel {
	extern Application* CreateApplication();
}

int main() {
	Hazel::Log::Init();
	HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
	auto app = Hazel::CreateApplication();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
	app->Run();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Shutdown.json");
	delete app;
	HZ_PROFILE_END_SESSION();
}
#endif