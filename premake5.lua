-- A solution contains projects, and defines the available configurations
solution "lowrezjam"
	configurations { "Debug", "Release" }
	
	includedirs { "./", "./glcommon", "/usr/local/include" }

	-- stuff up here common to all projects
	kind "ConsoleApp"
	language "C++"
	--location "build"
	--targetdir "build"
	location "."
	targetdir "."

	configuration "linux"
		links { "SDL2", "m" }
	
	configuration "windows"
		--linkdir "/mingw64/lib"
		--buildoptions "-mwindows"
		links { "mingw32", "SDL2main", "SDL2" }

	configuration { "gmake" }
		buildoptions { "-fno-rtti", "-fno-exceptions", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	configuration "Debug"
		defines { "DEBUG" }
		symbols "On"

	configuration "Release"
		defines { "NDEBUG" }
		optimize "Speed"

	project "tinyshooter"
		language "C++"
		configuration { "gmake" }
			buildoptions { "-fno-rtti", "-fno-exceptions", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }
		files {
			"./tinyshooter.cpp",
			"./glcommon/rsw_math.cpp",
			"./glcommon/rsw_glframe.cpp",
			"./glcommon/rsw_primitives.cpp",
			"./glcommon/gltools.cpp",
			"./glcommon/rsw_halfedge.cpp",
			"./glcommon/controls.cpp",
			"./glcommon/c_utils.cpp"
		}
