-- premake5.lua
workspace "RogueOne"
    architecture "x86"

    configurations { "Debug", "Release", "Dist" }
    startproject "RogueOne"

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
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        -- logging includes
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/RocketUtils/RocketUtils/include",
        "%{prj.name}/vendor/glad/include",
        "%{prj.name}/vendor/glfw/include/glfw"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
    
    links
    {
        "RocketUtils",
        "Glad",
        "GLFW"
    }

    defines
    {
         
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

project "RogueOne"
    location "RogueOne"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files   
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        -- logging includes
        --"%{prj.name}/vendor/spdlog/include"
        "RocketEngine/src",
        "RocketEngine/vendor/spdlog/include",
        "RocketEngine/vendor/RocketUtils/RocketUtils/include"
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
        defines "RG_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RG_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "RG_DIST"
        optimize  "On"

    ----------------------------------------------------------

include "RocketEngine/vendor/glad"
include "RocketEngine/vendor/glfw"
include "RocketEngine/vendor/RocketUtils/lib"
