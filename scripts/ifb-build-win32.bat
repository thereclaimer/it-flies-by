@echo off

@set cl_preprocessor= /D UNICODE ^
                      /D _UNICODE

@set cl_flags=        /MD /utf-8 /Zi 

@set cl_output=       /Fe:bin\ItFliesBy.exe ^
                      /Fd:bin\ItFliesBy.pdb ^
                      /Fo:bin\ItFliesBy.obj

@set cl_includes=     /I .\src\                ^
                      /I .\src\platform\win32  ^
                      /I .\src\common                          

@set cl_source=       src\platform\win32\ifb-win32-src.cpp

@set cl_linker=       /link /LIBPATH:vcpkg_installed\x64-windows\lib

@set cl_libs=         user32.lib opengl32.lib gdi32.lib

call cl.exe           ^
    %cl_preprocessor% ^
    %cl_flags%        ^
    %cl_output%       ^
    %cl_includes%     ^
    %cl_source%       ^
    %cl_linker%       ^
    %cl_libs%