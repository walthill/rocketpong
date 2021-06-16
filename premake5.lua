-- premake5.lua
workspace "RogueOne"
    architecture "x86"

    configurations 
    { 
        "Debug", 
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
        "%{prj.name}/vendor/RocketUtils/RocketUtils/includes"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    defines
    {
         
    }

    postbuildcommands
    {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/RogueOne")
    }

    filter "configurations:Debug"
        defines "RG_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RG_RELEASE"
        symbols "On"
    
    filter "configurations:Dist"
        defines "RG_DIST"
        symbols "On"

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
        "RocketEngine/src"
    }
        
    links
    {
        "RocketEngine.dll"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    defines
    {
         
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

