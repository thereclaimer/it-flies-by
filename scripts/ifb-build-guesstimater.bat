@echo off

@set cl_includes= /I src ^
                  /I src/common

@set cl_flags=    /Zi

@set cl_output=   /Fe: bin\ItFliesBy.Guesstimater.exe  ^
                  /Fd: bin\ItFliesBy.Guesstimater.pdb ^
                  /Fo: bin\ItFliesBy.Guesstimater.obj

@set cl_source=   src\tools\guesstimater\itfliesby-guesstimater.cpp

@set cl_linker=   /link

@set cl_libs=     user32.lib   ^
                  Shell32.lib  ^
                  Kernel32.lib ^
                  PowrProf.lib

call cl.exe       ^
    %cl_includes% ^
    %cl_flags%    ^
    %cl_output%   ^
    %cl_source%   ^
    %cl_linker%   ^
    %cl_libs%