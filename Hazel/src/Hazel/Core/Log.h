#pragma once
#include"Hazel/Core/Core.h"
#include<spdlog/spdlog.h>
#include<spdlog/fmt/ostr.h>
namespace Hazel {

	class  Log
	{
	public:
		static void Init();
		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;

	};

}

//core
#define HZ_CORE_TRACE(...)		::Hazel::Log::GetCoreLogger()->trace("{0} ({1}:{2})", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define HZ_CORE_INFO(...)			::Hazel::Log::GetCoreLogger()->info("{0} ({1}:{2})", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define HZ_CORE_WARN(...)		::Hazel::Log::GetCoreLogger()->warn("{0} ({1}:{2})", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define HZ_CORE_ERROR(...)		::Hazel::Log::GetCoreLogger()->error("{0} ({1}:{2})", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define HZ_CORE_CRITICAL(...)	::Hazel::Log::GetCoreLogger()->critical("{0} ({1}:{2})", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__)
//client
#define HZ_TRACE(...)					::Hazel::Log::GetClientLogger()->trace("{0} ({1}:{2})", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define HZ_INFO(...)					::Hazel::Log::GetClientLogger()->info("{0} ({1}:{2})", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define HZ_WARN(...)					::Hazel::Log::GetClientLogger()->warn("{0} ({1}:{2})", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define HZ_ERROR(...)					::Hazel::Log::GetClientLogger()->error("{0} ({1}:{2})", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define HZ_CRITICAL(...)				::Hazel::Log::GetClientLogger()->critical("{0} ({1}:{2})", __VA_ARGS__, std::filesystem::path(__FILE__).filename().string(), __LINE__)
