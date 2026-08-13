workspace("Test-Engine")
architecture("x64")
configurations({ "Debug", "Release", "Dist" })
toolset("clang")

function SetupProject()
	location("Build")
	targetdir("%{wks.location}/Build/Binaries/%{cfg.buildcfg}")
	objdir("%{wks.location}/Build/Intermediates/%{prj.name}/%{cfg.buildcfg}")
	debugdir("%{wks.location}/Build/Binaries/%{cfg.buildcfg}")

	staticruntime("Off")

	buildoptions({
		"-Wno-nullability-completeness",
	})

	filter("configurations:Debug")
	defines({ "DEBUG" })
	runtime("Debug")
	symbols("On")

	filter("configurations:Release")
	defines({ "RELEASE" })
	runtime("Release")
	optimize("On")
	symbols("On")

	filter("configurations:Dist")
	defines({ "DIST" })
	runtime("Release")
	optimize("On")
	symbols("Off")

	filter({})
end


project("Sandbox")
kind("ConsoleApp")
language("C++")
cppdialect("C++20")
SetupProject()

files({
	"Source/%{prj.name}/**.h",
	"Source/%{prj.name}/**.cpp",
})

includedirs({
	"Source/ThirdParty",
	"Source/%{prj.name}",
})

links({
	"vulkan",
	"glfw",
	"CTest",
})

project("CTest")
kind("StaticLib")
language("C")
cdialect("C23")
SetupProject()

files({
	"Source/%{prj.name}/**.h",
	"Source/%{prj.name}/**.c",
})

includedirs({
	"Source/ThirdParty",
	"Source/%{prj.name}",
})

links({
	"vulkan",
	"glfw",
})

