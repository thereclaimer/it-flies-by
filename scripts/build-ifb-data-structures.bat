::----------------------------------------------------------------
:: START
::----------------------------------------------------------------

@echo off

pushd ..

::----------------------------------------------------------------
:: VARIABLES
::----------------------------------------------------------------

::build path
@set path_build=          build\debug

::vcpkg install directories
@set path_vcpkg=              vcpkg_installed
@set path_vcpkg_lib=          %path_vcpkg%\x64-windows\lib

::include paths 
@set path_include_data_structures= data-structures\include
@set path_include_platform=        platform\include
@set path_include_memory=          memory\include
@set path_include_common=          common
@set path_include_external=        external
@set path_include_vcpkg=           %path_vcpkg%\x64-windows\include

::----------------------------------------------------------------
:: DEPENDENCIES
::----------------------------------------------------------------

if not exist %path_build%\bin mkdir %path_build%\bin
if not exist %path_build%\obj mkdir %path_build%\obj
if not exist %path_build%\lib mkdir %path_build%\lib

::----------------------------------------------------------------
:: COMPILER ARGUMENTS
::----------------------------------------------------------------

@set cl_flags=      /c    ^
                    /MD   ^
                    /Z7   ^
                    /EHsc ^
                    /std:c++17


@set cl_output=     /Fo:%path_build%\obj\ItFliesBy.DataStructures.obj

@set cl_includes=   /I %path_include_data_structures%  ^
                    /I %path_include_platform%         ^
                    /I %path_include_memory%           ^
                    /I %path_include_common%           ^
                    /I %path_include_external%         ^
                    /I %path_include_vcpkg%

@set cl_source=     data-structures\src\ifb-data-structures.cpp

::----------------------------------------------------------------
:: BUILD
::----------------------------------------------------------------

call cl.exe       ^
    %cl_flags%    ^
    %cl_output%   ^
    %cl_includes% ^
    %cl_source%

::----------------------------------------------------------------
:: LIB
::----------------------------------------------------------------

call lib.exe                                          ^
    /OUT:build\debug\lib\ItFliesBy.DataStructures.lib ^
    %path_build%\obj\ItFliesBy.DataStructures.obj

::----------------------------------------------------------------
:: END
::----------------------------------------------------------------

popd