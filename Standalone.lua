workspace "DreamSTL"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "DreamSTL.Tests"

include "premake5.lua"
include "Tests"