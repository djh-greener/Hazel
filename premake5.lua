--include "./vendor/premake/premake_customization/solution_items.lua"
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

    --solution_items
	--{
	--	".editorconfig"
	--}
	flags
	{
		"MultiProcessorCompile"
	}
-- Include directories relative to root folder (solution directory)
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Dependencies"
    --include "vendor/premake"
    include "Hazel/vendor/GLAD"
    include "Hazel/vendor/imgui"
    include "Hazel/vendor/GLFW"--include GLFW's premake5.lua
    include "Hazel/vendor/yaml-cpp"
group ""
include "Hazel"
include "Sandbox"
include "Hazelnut"
