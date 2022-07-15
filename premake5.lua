-- premake5.lua
workspace "RocketPong"
    architecture "x86"

    configurations { "Debug", "Release", "Dist" }
    startproject "RocketPong"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--------------------------------------------------------------------

project "RocketEngine"
    location "RocketEngine"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files   
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/kc_truetypeassembler/**.h",
        "%{prj.name}/vendor/kc_truetypeassembler/**.cpp"
    }

    includedirs 
    {
        -- logging includes
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/RocketUtils/RocketUtils/include",
        "%{prj.name}/vendor/glad/include",
        "%{prj.name}/vendor/glfw/include/glfw",
        "%{prj.name}/vendor/stb_image",
        "%{prj.name}/vendor/kc_truetypeassembler",
        "%{prj.name}/vendor/yaml-cpp/include",
        "%{prj.name}/vendor/soloud/include",
        "%{prj.name}/vendor/glm"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
    
    links
    {
        "RocketUtils",
        "Glad",
        "GLFW",
		"yaml-cpp",
        "soloud"
    }

	defines
    {
        "YAML_CPP_STATIC_DEFINE"
    }

    postbuildcommands
    {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/%{wks.name}")
    }

    filter "configurations:Debug"
        defines "RKT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RKT_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "RKT_DIST"
        optimize "On"
    
--------------------------------------------------------------------------

project "RocketPong"
    location "RocketPong"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files   
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
		"%{prj.name}/assets/shaders/**.frag",
		"%{prj.name}/assets/shaders/**.vert"
    }

    includedirs
    {
        "%{prj.name}/src/",
        "RocketEngine/src",
        "RocketEngine/vendor/spdlog/include",
        "RocketEngine/vendor/RocketUtils/RocketUtils/include",
        "RocketEngine/vendor/stb_image",
        "RocketEngine/vendor/kc_truetypeassembler",
        "RocketEngine/vendor/glm"
    }
        
    links 
    {
        "RocketEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    defines
    {
         
    }

    flags
	{
		"MultiProcessorCompile"
	}

    postbuildcommands
    {
        
    }

    filter "configurations:Debug"
        defines "RKP_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RKP_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "RKP_DIST"
        optimize  "On"

    ----------------------------------------------------------

group "Dependencies"
	include "RocketEngine/vendor/GLFW"
	include "RocketEngine/vendor/Glad"
	include "RocketEngine/vendor/RocketUtils/lib"
	include "RocketEngine/vendor/yaml-cpp"
	include "RocketEngine/vendor/soloud"
group ""