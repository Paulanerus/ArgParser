workspace "ArgParser"
    configurations {"Debug", "Release"}

project "ArgParser"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    architecture "x86_64"
    targetdir "bin/%{cfg.buildcfg}"
    location "build/"

    warnings "Extra"

    includedirs {"include/"}

    files {"example/**.hpp", "example/**.cpp"}

    filter "configurations:Debug"
        filter "system:linux"
            linkoptions{"-fuse-ld=mold"}

        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        filter "system:linux"
            linkoptions{"-fuse-ld=mold"}
        
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"