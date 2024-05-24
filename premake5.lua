workspace "Grafika komputerowa"
    architecture "x64"
    startproject "Grafika komputerowa"

    configurations {
        "Debug",
        "Release"
    }

    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

    OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
    include "Grafika komputerowa/premake5.lua"