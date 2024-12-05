workspace "Hazel"
    architecture "x64"
    startproject "Sandbox"
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
	flags
	{
		"MultiProcessorCompile"
	}
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
includeDir = {}
includeDir["GLFW"] = "Hazel/vendor/GLFW/include"
includeDir["GLAD"] = "Hazel/vendor/GLAD/include"
includeDir["imgui"] = "Hazel/vendor/imgui"
includeDir["glm"] = "Hazel/vendor/glm"
includeDir["stb_image"] = "Hazel/vendor/stb_image"

group "Dependencies"
    include "Hazel/vendor/GLAD"
    include "Hazel/vendor/imgui"
    include "Hazel/vendor/GLFW"--include GLFW's premake5.lua
	
group ""
project "Hazel"
    location "Hazel"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    cppdialect "C++17"


    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "hzpch.h"
    pchsource "Hazel/src/hzpch.cpp"
    files	{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",

        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
    }
    includedirs	{
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{includeDir.GLFW}",
        "%{includeDir.GLAD}",
        "%{includeDir.imgui}",
        "%{includeDir.glm}",
        "%{includeDir.stb_image}",
    
    }
    links{
        "GLFW",
        "GLAD",
        "imgui",
        "opengl32.lib"
    }
    filter "system:windows"
        systemversion "latest"
    
        defines
        {
            "GLFW_INCLUDE_NONE",
        }
    
    
    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "on"
    
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
    	"%{prj.name}/src/**.h",
    	"%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Hazel/vendor/spdlog/include",
        "Hazel/src",
        "%{includeDir.glm}",
        "Hazel/vendor",
    }

    links
    {
        "Hazel",
    }
    
    filter "system:windows"
        systemversion "latest"


    filter "configurations:Debug"
        defines "HZ_DEBUG"		
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "on"