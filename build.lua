workspace "FlappyBird"
        architecture "x64"
        configurations { "Debug", "Release" }

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "FlappyBird"
        location "FlappyBird"
        kind "ConsoleApp"
        language "C++"
        targetdir ("bin/" .. output_dir .. "%{prj.name}")
        objdir ("bin-int/" .. output_dir .. "%{prj.name}")

        filter "configurations:Debug"
           defines { "DEBUG" }
           symbols "On"
     
        filter "configurations:Release"
           defines { "NDEBUG" }
           optimize "On"

        files { "%{prj.name}/**.h", "%{prj.name}/**.cpp" }


        includedirs {
                "SDL2/include",
                "SDL2_image/include",
                "imgui",
                "imgui/backends"
        }
        files {
                "imgui/imgui.cpp",
                "imgui/imgui_draw.cpp",
                "imgui/imgui_widgets.cpp",
                "imgui/imgui_tables.cpp",
                "imgui/backends/imgui_impl_sdl2.cpp",
                "imgui/backends/imgui_impl_opengl3.cpp"
        }