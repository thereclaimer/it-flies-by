@echo off

@set cl_includes= /I src                  ^
                  /I src\common           ^
                  /I src\math             ^
                  /I src\engine           ^
                  /I src\engine\common    ^
                  /I src\engine\assets    ^
                  /I src\engine\core      ^
                  /I src\engine\devtools  ^
                  /I src\engine\memory    ^
                  /I src\engine\physics   ^
                  /I src\engine\rendering ^
                  /I src\engine\scene     ^
                  /I src\engine\sprite    ^
                  /I .\vcpkg_installed\x64-windows\include           


@set cl_flags=    /DLL /w /LD /Zi

@set cl_output=   /Fd: bin\ItFliesBy.Engine.pdb ^
                  /Fo: bin\ItFliesBy.Engine.obj ^
                  /Fe: bin\ItFliesBy.Engine.dll

@set cl_src=      src\engine\ifb-engine-src.cpp

@set cl_linker=   /link                                     ^
                  /LIBPATH:vcpkg_installed\x64-windows\lib ^
                  /LIBPATH:bin

@set cl_libs=     opengl32.lib ^
                  glew32.lib   ^
                  imgui.lib 

call cl.exe           ^
    %cl_flags%        ^
    %cl_includes%     ^
    %cl_output%       ^
    %cl_src%          ^
    %cl_linker%       ^
    %cl_libs%
