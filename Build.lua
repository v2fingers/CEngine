workspace("CEngine")
architecture("x64")
configurations({ "Debug", "Release" })
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
	runtime("Release")
	optimize("On")
	symbols("On")

	filter({})
end

project("Sandbox")
kind("ConsoleApp")
language("C")
cdialect("C23")
SetupProject()

files({
	"Source/%{prj.name}/**.h",
	"Source/%{prj.name}/**.cpp",
})

includedirs({
	"Source/Runtime",
	"Source/ThirdParty",
	"Source/%{prj.name}",
})

links({
	"vulkan",
	"glfw",
	"Runtime",
})

project("Runtime")
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
	"m",
})
