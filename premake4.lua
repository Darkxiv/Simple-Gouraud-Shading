dofile("../glsdk/links.lua")

solution("Simple-Gouraud-Shading")
	configurations {"Debug", "Release"}
		defines {"_CRT_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_DEPRECATE", "_SCL_SECURE_NO_WARNINGS", "TIXML_USE_STL"}
		defines {"FREEGLUT_STATIC"}
		
    	configuration "windows"
        	defines {"WIN32"}
        	
       	configuration "linux"
    	    defines {"LOAD_X11"}
			
	project "Gouraud Shading"
		kind "ConsoleApp"
		language "c++"
		files  "*.cpp"
		files  "*.h"
		
		UseLibs("glload", "glimage", "glm", "glutil", "glmesh", "freeglut")
		
		configuration "Debug"
			defines {"DEBUG", "_DEBUG"}
			flags "Symbols"
			targetname("Gouraud Shading")
		
		configuration "Release"
			defines {"RELEASE", "NDEBUG"};
			flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue"};
			targetname("Gouraud Shading")

		configuration {"windows"}
			links {"glu32", "opengl32", "gdi32", "winmm", "user32"}

	    configuration "linux"
	        links {"GL", "GLU"}
	