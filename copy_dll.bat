@echo off
set dll_win32_debug_src=dependencies\SDL\VisualC\Win32\Debug\SDL2.dll
set dll_win32_release_src=dependencies\SDL\VisualC\Win32\Release\SDL2.dll

set dll_win32_debug_dist=build\raytracer\Debug\SDL2.dll
set dll_win32_release_dist=build\raytracer\Release\SDL2.dll

copy /Y %dll_win32_debug_src% %dll_win32_debug_dist%
copy /Y %dll_win32_release_src% %dll_win32_release_dist%