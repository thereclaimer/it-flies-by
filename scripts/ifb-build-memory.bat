::memory

@echo off

@set cl_includes= /I dependencies /I src\ /I src\common /I src\math
@set cl_flags=    /DLL /w  /LD /Zi /Fd: bin\\ItFliesBy.Memory.pdb /Fo: bin\\ItFliesBy.Memory.obj /Fe: bin\\ItFliesBy.Memory.dll
@set cl_source=   src\\memory\\ifb-memory.cpp 

call cl.exe ^
%cl_includes% ^
%cl_flags%    ^
%cl_source%