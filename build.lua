
ProjectPrefix = "proj-"
EngineProjName = "BirdEngine"
GameProj = "FlappyBird"

workspace "FlappyBird"
        architecture "x64"
        configurations { "Debug", "Release", "Dist" }
        startproject "" .. GameProj

include "" .. ProjectPrefix .. EngineProjName .. "/" .. "build-bird-engine.lua"
include "" .. ProjectPrefix .. GameProj .. "/" .. "build-flappy-bird.lua"