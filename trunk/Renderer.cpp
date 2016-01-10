#include "Renderer.h"


std::string Renderer::OctanePath = "C:/Apps/OctaneRender_beta2.58e/octane.exe";
std::string Renderer::SceneFilePath = "C:/Apps/OctaneRender_beta2.58e/Scenes";
std::string Renderer::ObjPath = "F:/Projects/SVN/GlutSinglePotato/trunk/testobj.obj";
bool Renderer::Rendering = false;
std::string Renderer::CurrentFilename = "";



Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}


bool Renderer::RenderFile(std::string outfilename)
{
	if (Rendering)
		return false;

	Rendering = true;

	CurrentFilename = outfilename;

	_beginthread(Renderer::Render, 0, 0);

	return true;
}


bool Renderer::IsRendering()
{
	return Rendering;
}


void Renderer::Render(void*)
{
	// Start the octane rendering process
	//HINSTANCE hRet;
	const char *szPath = OctanePath.c_str();
	const char *params = NULL;
	std::string paramstr;

	{
		std::stringstream ss;
		ss << "\"" << SceneFilePath << "/Beach.ocs\" -q -e -s 10 --cam-pos-x 0.0325 -t \"RenderTarget\" -m \"OctaneTest.obj\" -r \"" << ObjPath << "\"" << " -o " << CurrentFilename << ".png";
		paramstr = ss.str();
		params = paramstr.c_str();
	}

	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.hwnd = NULL;
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.nShow = SW_SHOW;
	sei.lpVerb = NULL;
    sei.lpFile = szPath;
	sei.lpParameters = params;
	sei.lpDirectory = NULL;
	sei.hInstApp = NULL;

	BOOL res = ShellExecuteEx(&sei);

	if (sei.hProcess)
	{
		WaitForSingleObject(sei.hProcess, INFINITE);

		CloseHandle(sei.hProcess);
	}

	/*hRet = ShellExecute(
		HWND_DESKTOP, //Parent window
		"open",       //Operation to perform
		szPath,       //Path to program
		params,         //Parameters
		NULL,         //Default directory
		SW_SHOW);     //How to open*/

	// The function returns a HINSTANCE (not really useful in this case)
	// So therefore, to test its result, we cast it to a LONG.
	// Any value over 32 represents success!

	//if ((LONG)hRet > 32)
	if (res)
	{
		std::string impath = "C:/Apps/ImageMagick/convert.exe";
		szPath = impath.c_str();
		params = NULL;
		
		{
			std::stringstream ss;
			ss << CurrentFilename << ".png " << CurrentFilename << ".png -compose CopyOpacity -composite " << CurrentFilename << ".tga";
			paramstr = ss.str();
			params = paramstr.c_str();
		}

		SHELLEXECUTEINFO sei2 = {0};
		sei2.cbSize = sizeof(SHELLEXECUTEINFO);
		sei2.hwnd = NULL;
		sei2.fMask = SEE_MASK_NOCLOSEPROCESS;
		sei2.nShow = SW_SHOW;
		sei2.lpVerb = NULL;
		sei2.lpFile = szPath;
		sei2.lpParameters = params;
		sei2.lpDirectory = NULL;
		sei2.hInstApp = NULL;

		res = ShellExecuteEx(&sei2);

		/*hRet = ShellExecute(
			HWND_DESKTOP, //Parent window
			"open",       //Operation to perform
			szPath,       //Path to program
			params,         //Parameters
			NULL,         //Default directory
			SW_SHOW);     //How to open*/

		if (res)
		{
			int hh = 2;
			hh++;
		}
		else
		{
			int jj = 2;
			jj++;
		}
	}

	//convert testrender.png testrender.png -compose CopyOpacity -composite testrender.tga

	Rendering = false;
}