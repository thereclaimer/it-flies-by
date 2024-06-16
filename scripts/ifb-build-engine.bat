@echo off

@set cl_includes= /I dependencies         ^
                  /I src                  ^
                  /I src\common           ^
                  /I src\math             ^
                  /I src\engine           ^
                  /I src\engine\assets    ^
                  /I src\engine\core      ^
                  /I src\engine\devtools  ^
                  /I src\engine\memory    ^
                  /I src\engine\physics   ^
                  /I src\engine\rendering ^
                  /I src\engine\scene     ^
                  /I src\engine\sprite 

@set cl_flags= /DLL /w /LD /Zi

@set cl_output= /Fd: bin\ItFliesBy.Engine.pdb ^
                /Fo: bin\ItFliesBy.Engine.obj ^
                /Fe: bin\ItFliesBy.Engine.dll

@set cl_src=   src\engine\ifb-engine.cpp
@set cl_link=  /link
@set cl_libs=  opengl32.lib glu32.lib 

call cl.exe       ^
    %cl_flags%    ^
    %cl_includes% ^
    %cl_output%   ^
    %cl_src%      ^
    %cl_link%     ^
    %cl_libs%
