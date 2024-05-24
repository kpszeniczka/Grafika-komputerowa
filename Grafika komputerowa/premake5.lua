project "Grafika komputerowa"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("../bin-int/" .. OutputDir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glad/glad.h",
		"vendor/glad/glad.c",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.cpp",
		"vendor/KHR/**.h",
		"vendor/GLFW/**.h",
		"vendor/stb/**.h",
		"vendor/stb/**.cpp"
	}

	includedirs
	{
		"src",
		"src/vendor",
		"src/vendor/stb"
	}

	libdirs 
	{ 
		"../Biblioteki/lib"
	}

	links 
	{ 
		"glfw3.lib",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"