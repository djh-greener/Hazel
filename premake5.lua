
include "Dependencies.lua"
workspace "Hazel"
    architecture "x86_64"
    startproject "Hazelnut"
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
-- Include directories relative to root folder (solution directory)
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Dependencies"
    include "Hazel/vendor/GLAD"
    include "Hazel/vendor/imgui"
    include "Hazel/vendor/GLFW"
    include "Hazel/vendor/yaml-cpp"
group ""
include "Hazel"
--include "Sandbox" --this project is no use
include "Hazelnut"
