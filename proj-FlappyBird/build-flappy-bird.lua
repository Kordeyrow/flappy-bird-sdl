
OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}/"
BinariesDir = "../bin/"
SourceDir = "src/"
ProjectPrefix = "proj-"
EngineProjName = "BirdEngine"
GameProj = "FlappyBird"
ThisProj = GameProj

-- Libs
SDL2DllPath = "../SDL2/lib/x64/"
SDL2DllPathFile = SDL2DllPath .. "SDL2.dll"
SDL2_imageDllPath = "../SDL2_image/lib/x64/"
SDL2_imageDllPathFile = SDL2_imageDllPath .. "SDL2_image.dll"
-- Assets
AssetsDir = "assets"


project ("" .. ThisProj)
   kind "ConsoleApp"
   -- kind "WindowedApp"
   language "C++"
   cppdialect "C++14"
   staticruntime "off"
   targetdir (BinariesDir .. OutputDir .. ThisProj)
   objdir (BinariesDir .. "Intermediates/" .. OutputDir .. ThisProj)
   files { SourceDir .. "**.h",  SourceDir .. "**.cpp" }
   
   includedirs
   {
        SourceDir,
        "../SDL2/include",
        "../SDL2_image/include",
	    "../" .. ProjectPrefix .. EngineProjName .. "/" .. SourceDir,
   }

   links
   {
        "SDL2",
        "SDL2main",
        "SDL2_image",
        EngineProjName,
   }

   libdirs
   {
        SDL2DllPath,
        SDL2_imageDllPath,
   }

   dependson
   {
       EngineProjName
   }
   

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }
       postbuildcommands {
           '{COPY} "'..SDL2DllPathFile..'" "%{cfg.targetdir}"',
           '{COPY} "'..SDL2_imageDllPathFile..'" "%{cfg.targetdir}"',
           '{MKDIR} "%{cfg.targetdir}/' .. path.getname(AssetsDir) .. '"',
           '{COPYDIR} "' .. AssetsDir .. '" "%{cfg.targetdir}/' .. path.getname(AssetsDir) .. '"'
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