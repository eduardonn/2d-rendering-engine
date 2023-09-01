workspace "2D_Rendering_Engine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "2D_Rendering_Engine"
	location "2D_Rendering_Engine"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"dependencies/glew-2.1.0/include",
		"dependencies/glfw-3.3.8/include",
		"dependencies/glm"
	}

	libdirs
	{
		"dependencies/glew-2.1.0/lib/Release/x64",
		"dependencies/glfw-3.3.8/lib-vc2017"
	}

	links
	{
		"opengl32.lib",
		"glew32s.lib",
		"glfw3.lib"
	}

	defines
	{
		"GLEW_STATIC"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG_MODE"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE_MODE"
		optimize "on"

	filter "configurations:Dist"
		optimize "on"
