
OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}/"
BinariesDir = "../bin/"
SourceDir = "src/"
ProjectPrefix = "proj-"
EngineProjName = "BirdEngine"

project "FlappyBird"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++14"
   staticruntime "off"
   targetdir (BinariesDir .. OutputDir .. "%{prj.name}")
   objdir (BinariesDir .. "Intermediates/" .. OutputDir .. "%{prj.name}")
   files { SourceDir .. "**.h",  SourceDir .. "**.cpp" }
   
   includedirs
   {
        SourceDir,
        "../SDL2/include",
        "../SDL2_image/include",
	    -- "../" .. ProjectPrefix .. EngineProjName .. "/" .. SourceDir,
   }

   links
   {
        "SDL2",
        "SDL2main",
        "SDL2_image",
        -- "BirdEngine",
   }

   libdirs
   {
        "../SDL2/lib/x64",
        "../SDL2_image/lib/x64",
   }

   dependson
   {
        EngineProjName
   }
   

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

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