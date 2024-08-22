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

    files {"example/**.cpp"}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

        filter "system:linux"
            linkoptions{"-fuse-ld=mold"}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"

        filter "system:linux"
            linkoptions{"-fuse-ld=mold"}