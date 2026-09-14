workspace("CEngine")
architecture("x64")
configurations({ "Debug", "Release" })
toolset("clang")

newoption {
	trigger = "asan",
	description = "Enable AddressSanitizer"
}

function SetupProject()
	language("C")
	cdialect("C99")
	location("Build")
	targetdir("%{wks.location}/Build/Binaries/%{cfg.buildcfg}")
	objdir("%{wks.location}/Build/Intermediates/%{prj.name}/%{cfg.buildcfg}")
	debugdir("%{wks.location}/Build/Binaries/%{cfg.buildcfg}")

	staticruntime("Off")

	filter("configurations:Debug")
	defines({ "DEBUG" })
	runtime("Debug")
	symbols("On")
	buildoptions({
		"-Wno-nullability-completeness",
	})

	filter("configurations:Release")
	runtime("Release")
	optimize("On")
	symbols("On")

	filter("options:asan")
	buildoptions({
		"-Wno-nullability-completeness",
		"-fsanitize=address",
		"-fno-omit-frame-pointer"
	})

	linkoptions({
		"-fsanitize=address",
	})

	filter({})
end

project("Testbed")
kind("ConsoleApp")
SetupProject()

files({
	"Source/%{prj.name}/**.c",
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
