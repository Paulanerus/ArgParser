workspace "ArgParser"
    configurations {"Debug", "Release"}

project "ArgParser"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    architecture "x86_64"
    targetdir "bin/%{cfg.buildcfg}"
    location "build/"

    warnings "Extra"

    includedirs {"include/"}

    files {"example/**.cpp"}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"