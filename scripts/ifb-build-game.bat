@echo off

@set cl_includes= /I src                ^
                  /I src\common         ^
                  /I src\engine\include ^
                  /I src\game           ^
                  /I src\game\core      ^
                  /I src\game\scenes    ^
                  /I src\scenes         ^
                  /I .\vcpkg_installed\x64-windows\include           

@set cl_flags=    /DLL /w /LD /Zi

@set cl_output=   /Fd: bin\ItFliesBy.Game.pdb ^
                  /Fo: bin\ItFliesBy.Game.obj ^
                  /Fe: bin\ItFliesBy.Game.dll

@set cl_src=      src\game\ifb-game-src.cpp

@set cl_linker=   /link /LIBPATH:vcpkg_installed\x64-windows\lib /LIBPATH:bin

@set cl_libs=     Itfliesby.Engine.lib 

call cl.exe       ^
    %cl_flags%    ^
    %cl_includes% ^
    %cl_output%   ^
    %cl_src%      ^
    %cl_linker%   ^
    %cl_libs%
