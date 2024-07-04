@echo off

@set cl_includes= /I src        ^
                  /I src/common ^
                  /I src/math   ^
                  /I vcpkg_installed/x64-windows/include           


@set cl_flags=    /Zi

@set cl_output=   /Fe: bin\ItFliesBy.AssetFileBuilder.exe  ^
                  /Fd: bin\ItFliesBy.AssetFileBuilder.pdb ^
                  /Fo: bin\ItFliesBy.AssetFileBuilder.obj

@set cl_source=   src\tools\asset-file-builder\itfliesby-asset-file-builder.cpp

@set cl_linker=   /link

@set cl_libs=     user32.lib ^
                  Shell32.lib

call cl.exe           ^
    %cl_preprocessor% ^
    %cl_includes%     ^
    %cl_flags%        ^
    %cl_output%       ^
    %cl_source%       ^
    %cl_linker%       ^
    %cl_libs%