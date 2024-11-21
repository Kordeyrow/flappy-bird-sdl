
OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}/"
BinariesDir = "../bin/"
SourceDir = "src/"

project "BirdEngine"
   kind "SharedLib"
   language "C++"
   cppdialect "C++14"
   staticruntime "off"
   targetdir (BinariesDir .. OutputDir .. "%{prj.name}")
   objdir (BinariesDir .. "Intermediates/" .. OutputDir .. "%{prj.name}")
   files { SourceDir .. "**.h",  SourceDir .. "**.cpp" }

   includedirs
   {
        SourceDir
   }

   filter "system:windows"
       systemversion "latest"
       defines { }

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