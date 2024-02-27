workspace "ArgParser"
    configurations {"Debug", "Release"}

project "ArgParser"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    architecture "x86_64"
    targetdir "bin/%{cfg.buildcfg}"
    location "build/"

    files {"src/**.hpp", "src/**.cpp"}

    filter "configurations:Debug"
        linkoptions{"-fuse-ld=mold"}
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        linkoptions{"-fuse-ld=mold"}
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"