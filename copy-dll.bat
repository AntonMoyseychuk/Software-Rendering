@echo off
set dll_win32_debug_src=dependencies\SDL\VisualC\Win32\Debug\SDL2.dll
set dll_win32_release_src=dependencies\SDL\VisualC\Win32\Release\SDL2.dll

set dll_win32_debug_raytracer=build\raytracer\Debug\SDL2.dll
set dll_win32_release_raytracer=build\raytracer\Release\SDL2.dll

set dll_win32_debug_rasterizer=build\rasterizer\Debug\SDL2.dll
set dll_win32_release_rasterizer=build\rasterizer\Release\SDL2.dll



copy /Y %dll_win32_debug_src%       %dll_win32_debug_raytracer%
copy /Y %dll_win32_release_src%     %dll_win32_release_raytracer%
copy /Y %dll_win32_debug_src%       %dll_win32_debug_rasterizer%
copy /Y %dll_win32_release_src%     %dll_win32_release_rasterizer%