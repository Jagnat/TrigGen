call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"
cl main.cpp SDL2.lib SDL2main.lib OpenGL32.lib /nologo /MT /Iinclude /O1 ^
/D_CRT_SECURE_NO_WARNINGS /link /OUT:TrigGen.exe /LIBPATH:lib /SUBSYSTEM:WINDOWS /NODEFAULTLIB:library
pause