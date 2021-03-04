outputDirSementic = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

rootPath = "%{wks.location}/../../../"
sourcePath = rootPath .. "Sources/"
projectsFilesLocation = "./projects/" .. _ACTION

workspace "6502_Cpu"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

	platforms 
	{
		"Win64", 
		"Linux64"
	}

	flags
	{
		"MultiProcessorCompile",
	}

	filter { "platforms:Win64" }
		system "windows"
		architecture "x64"

	filter { "platforms:Linux64" }
		system "linux"
		architecture "x64"
	filter {}

	location(projectsFilesLocation);

	project "Sandbox"
		kind("ConsoleApp")
		language("C++")
		cppdialect("C++17")

		rtti("Off")
		exceptionhandling("Off")
		warnings("Extra")
		flags("NoPCH")
		staticruntime("Off")

		location(projectsFilesLocation)
		targetdir(rootPath .. "/output/bin/" .. outputDirSementic)
		objdir(rootPath .. "/output/obj/" .. outputDirSementic)

		files
		{
			sourcePath .. "/**.hpp",
			sourcePath .. "/**.cpp",
		}

		includedirs
		{
			sourcePath
		}

		configuration { "linux", "gmake" }
	  		buildoptions { "`wx-config --cxxflags`", "-ansi", "-pedantic" }

		configuration { "vs2019" }
	  		buildoptions { "" }

		filter "system:windows"
			systemversion "latest"

			linkoptions 
			{
				"/NODEFAULTLIB:library"
			}

		filter "system:Linux"

		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			runtime "Release"
			optimize "on"
		filter {}

