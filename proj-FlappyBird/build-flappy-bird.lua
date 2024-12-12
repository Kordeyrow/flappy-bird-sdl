
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
glewLib_FolderPath = "../vendor/libs/glew-2.1.0/lib/Release/x64/"
glewDLL_FilePath = "../vendor/libs/glew-2.1.0/bin/Release/x64/glew32.dll"
-- Assets
AssetsDir = "assets"


project ("" .. ThisProj)
   kind "WindowedApp"
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
        "../vendor/libs/glew-2.1.0/include",
	    "../" .. ProjectPrefix .. EngineProjName .. "/" .. SourceDir,
   }
   
   links
   {
        "SDL2",
        "SDL2main",
        "SDL2_image",
        "glew32",
        "opengl32",
        EngineProjName,
   }

   libdirs
   {
        SDL2DllPath,
        SDL2_imageDllPath,
        glewLib_FolderPath,
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
           '{COPY} "'..glewDLL_FilePath..'" "%{cfg.targetdir}"',
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