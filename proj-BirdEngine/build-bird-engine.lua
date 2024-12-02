
OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}/"
BinariesDir = "../bin/"
SourceDir = "src/"
ProjectPrefix = "proj-"
EngineProjName = "BirdEngine"
GameProj = "FlappyBird"
ThisProj = EngineProjName
TargetDLLdir = BinariesDir.. OutputDir.. GameProj
-- Libs
SDL2DllPath = "../SDL2/lib/x64/"
SDL2DllPathFile = SDL2DllPath .. "SDL2.dll"
SDL2_imageDllPath = "../SDL2_image/lib/x64/"
SDL2_imageDllPathFile = SDL2_imageDllPath .. "SDL2_image.dll"

project "BirdEngine"
   kind "SharedLib"
   language "C++"
   cppdialect "C++14"
   staticruntime "off"
   targetdir (BinariesDir .. OutputDir .. ThisProj)
   objdir (BinariesDir.. "Intermediates/".. OutputDir.. ThisProj)
   files { SourceDir.. "**.h",  SourceDir.. "**.cpp" }

   includedirs
   {
        SourceDir,
        "../SDL2/include",
        "../SDL2_image/include",
        "../SDL2/include",
        "../SDL2_image/include",
   }

   links
   {
        "SDL2",
        "SDL2main",
        "SDL2_image"
   }

   libdirs
   {
        SDL2DllPath,
        SDL2_imageDllPath,
   }

   filter "system:windows"
       systemversion "latest"
       defines { }
       postbuildcommands {
           'if not exist "'.. TargetDLLdir.. '" mkdir "'.. TargetDLLdir.. '"',
           '{COPY} "C:/dev/FlappyBird/FlappyBird/bin/windows-x86_64/Debug/BirdEngine/'..EngineProjName..'.dll" "C:/dev/FlappyBird/FlappyBird/bin/windows-x86_64/Debug/FlappyBird"'
       }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"