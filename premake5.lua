-- premake5 file to build
-- http://premake.github.io/
--
--[[
Usage: 
	premake5.exe --os=windows vs2015
	premake5.exe --os=linux gmake
]]

workspace "csv_parser_RFC4180"
	configurations { "Release", "Debug" }
	platforms { "x64", "x32" }
	language "C++"
	-- flags { "StaticRuntime", }

	filter { "configurations:Debug*" }
		flags { "Symbols" }
	filter { "configurations:Release*" }
		defines { "NDEBUG" }
		optimize "On"
	filter {}

project "csv_parser_RFC4180"
	kind "StaticLib"
	files { "csv_parser/csv_parser.h", "csv_parser/csv_parser.cpp" }

project "test"
	kind "ConsoleApp"
	files { "csv_parser/test_run.cpp" }
	links { "csv_parser_RFC4180" }
