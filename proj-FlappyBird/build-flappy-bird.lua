
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
   files { SourceDir .. "**.h",  SourceDir .. "**.cpp" }
   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
--    files {
--         "imgui/imgui.cpp",
--         "imgui/imgui_draw.cpp",
--         "imgui/imgui_widgets.cpp",
--         "imgui/imgui_tables.cpp",
--         "imgui/backends/imgui_impl_sdl2.cpp",
--         "imgui/backends/imgui_impl_opengl3.cpp"
--    }

   includedirs
   {
        SourceDir,
	  -- Include Engine
	    "../" .. ProjectPrefix .. EngineProjName .. "/" .. SourceDir,
        "../SDL2/include",
        "../SDL2_image/include",
        "../imgui",
        "../imgui/backends"
   }

   links
   {
        "BirdEngine"
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