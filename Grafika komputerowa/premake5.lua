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
		"src/vendor/glad/glad.h",
		"src/vendor/glad/glad.c",
		"src/vendor/glm/glm/**.hpp",
		"src/vendor/glm/glm/**.cpp",
		"src/vendor/KHR/**.h",
		"src/vendor/GLFW/**.h",
		"src/vendor/stb/**.h",
		"src/vendor/stb/**.cpp"
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