project "DreamSTL.Tests"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir "%{wks.location}/Build/Bin/%{cfg.buildcfg}/%{prj.name}"
	objdir    "%{wks.location}/Build/Intermediate/%{cfg.buildcfg}/%{prj.name}"

	files       { "Source/**.h", "Source/**.cpp" }
	includedirs { "../Include" }
	warnings "Extra"

	filter "action:vs*"
		buildoptions { "/utf-8", "/Zc:__cplusplus" }
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
	filter {}