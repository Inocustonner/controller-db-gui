@echo OFF
setlocal EnableDelayedExpansion 
set srcPath=%~dp0src\
set objPath=%~dp0obj\
set binPath=%~dp0bin\
set CompilerFlags=/EHsc /W3 /wd4101 /wd4201 /wd4477 /nologo /Fo%objPath% /Zi /I E:\Projects\nana\include /Zc:wchar_t /std:c++17
set CompilerFlags=%CompilerFlags% /D _CRT_SECURE_NO_WARNINGS /D_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING /D DEBUG_TABLE
set LinkerFlags=/SUBSYSTEM:CONSOLE /incremental:no /NOLOGO /OUT:%binPath%bin.exe /LTCG 

rem SymbolTable.cpp
set TUnits=main.cpp
set StaticLibs="E:\Projects\nana\build\bin\nana_v142_Release_x86.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib"

if not exist %objPath% mkdir %objPath%
if not exist %binPath% mkdir %binPath%

cd %srcPath%

cl %TUnits% %CompilerFlags% /link %LinkerFlags%  /DYNAMICBASE %StaticLibs% 

del vc140.pdb

cd ..