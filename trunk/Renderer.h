#ifndef RENDERER__H
#define RENDERER__H


#include <string>
#include <sstream>
#include <windows.h>
#include <process.h>
#include <Shellapi.h>


class Renderer
{
public:

	Renderer();

	~Renderer();

public:

	static bool RenderFile(std::string outfilename);

	static bool IsRendering();

private:

	static void Render(void*);

public:

	static std::string OctanePath;
	static std::string SceneFilePath;
	static std::string ObjPath;

private:

	static bool Rendering;
	static std::string CurrentFilename;
};


#endif
