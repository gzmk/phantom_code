/***********************************************************
 *
 * ShaderMan.cpp
 *
 * Project:
 *
 * GlutGLSL
 *
 * Original Author:
 *
 * Matthew Jones, 2008
 *
 * Description:
 *
 * This file declares the shader manager class, which
 * handles all of the shader loading, compiling and
 * initialisation phases.  Uniform variables to be passed
 * to the shader are also handled by a set of overloaded
 * functions, so that the interface to shaders is nicely
 * abstracted, and the programmer does not have to deal
 * with any shader specifics other than through the
 * functions provided by this class.
 *
 ***********************************************************/

#ifndef SHADERMAN__H
	#include "ShaderMan.h"
#endif


///////////////////////////////////////////////////
// Constructor + Destructor
///////////////////////////////////////////////////

ShaderMan::ShaderMan()
{
	// Default constructor taking no arguments
	Init(0);
}


ShaderMan::ShaderMan(unsigned int shadernum)
{
	// Constructor with one argument for initialisation
	Init(shadernum);
}


void ShaderMan::Init(unsigned int shadernum)
{
	// Holders for shader and program addresses
	_vertexShader = 0;
	_fragmentShaderA = 0;
	_fragmentShaderB = 0;
	_shaderProgram = 0;

	_initialised = false; // True after InitShader()
	_numVars = 0; // Number of variables declared with InitShaderVar()
	_varList = NULL; // List of uniform variable addresses for loaded shader

	_shaderNum = shadernum;
}


ShaderMan::~ShaderMan()
{
	if (_initialised)
	{
		// Detach shader and delete program
	}

	_initialised = false;
	_numVars = 0;

	if (_varList)
		delete _varList;

	_varList = NULL;
}


///////////////////////////////////////////////////
// Shader loading/compiling/initialising
///////////////////////////////////////////////////

bool ShaderMan::InitShader(const char* vertshader, const char* fragshader)
{
	// This function reads and compiles the shader code in
	// the textfiles provided (e.g. shader.frag and shader.vert).
	// Any errors in compilation are printed to standard output.
	if (!_initialised)
	{
		char *vs = NULL,*fs = NULL,*fs2 = NULL;

		_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		_fragmentShaderA = glCreateShader(GL_FRAGMENT_SHADER);
		_fragmentShaderB = glCreateShader(GL_FRAGMENT_SHADER);

		vs = TextFileRead(vertshader);
		fs = TextFileRead(fragshader);

		const char * vv = vs;
		const char * ff = fs;

		glShaderSource(_vertexShader, 1, &vv,NULL);
		glShaderSource(_fragmentShaderA, 1, &ff,NULL);

		free(vs);free(fs);

		glCompileShader(_vertexShader);
		glCompileShader(_fragmentShaderA);

		PrintShaderInfoLog(_vertexShader);
		PrintShaderInfoLog(_fragmentShaderA);
		PrintShaderInfoLog(_fragmentShaderB);

		_shaderProgram = glCreateProgram();
		glAttachShader(_shaderProgram,_vertexShader);
		glAttachShader(_shaderProgram,_fragmentShaderA);

		glLinkProgram(_shaderProgram);
		PrintProgramInfoLog(_shaderProgram);

		glUseProgram(_shaderProgram);

		_initialised = true;

		return true;
	}
	
	return false;
}


unsigned int ShaderMan::InitShaderVar(const char* varname)
{
	// Add a shader variable to the list (using its name),
	// so it can be quickly referenced and set later on.

	if (_initialised)
	{
		unsigned int counter1;
		GLint *newvarlist = new GLint[_numVars+1];

		if (_varList!=NULL)
		{
			for (counter1=0;counter1<_numVars;counter1++)
			{
				newvarlist[counter1] = _varList[counter1];
			}
		}

		newvarlist[_numVars] = glGetUniformLocation(_shaderProgram,varname);

		delete _varList;
		_varList = newvarlist;

		_numVars++;

		// Return a number > 0 if valid
		return _numVars;
	}

	return 0;
}


bool ShaderMan::Apply()
{
	if (_initialised)
	{
		// Only need to do this if another shader has been applied
		// and you want to go back to using this one (must be initialised first).
		
		glUseProgram(_shaderProgram);

		return true;
	}

	return false;
}


void ShaderMan::UnApply()
{
	glUseProgram(0);
}


///////////////////////////////////////////////////
// Overloaded functions for setting shader
// uniform variables
///////////////////////////////////////////////////

void ShaderMan::SetUniformVar1i(const unsigned int varnum, const int var)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform1i(_varList[varnum2], (GLint) var);
	}
}


void ShaderMan::SetUniformVar2i(const unsigned int varnum, const int var1, const int var2)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform2i(_varList[varnum2], (GLint) var1, (GLint) var2);
	}
}


void ShaderMan::SetUniformVar3i(const unsigned int varnum, const int var1, const int var2, const int var3)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform3i(_varList[varnum2], (GLint) var1, (GLint) var2, (GLint) var3);
	}
}


void ShaderMan::SetUniformVar4i(const unsigned int varnum, const int var1, const int var2, const int var3, const int var4)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform4i(_varList[varnum2], (GLint) var1, (GLint) var2, (GLint) var3, (GLint) var4);
	}
}


void ShaderMan::SetUniformVar1iv(const unsigned int varnum, const unsigned int count, int* vars)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform1iv(_varList[varnum2], (GLsizei) count, (GLint*) vars);
	}
}


void ShaderMan::SetUniformVar2iv(const unsigned int varnum, const unsigned int count, int* vars)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform2iv(_varList[varnum2], (GLsizei) count, (GLint*) vars);
	}
}


void ShaderMan::SetUniformVar3iv(const unsigned int varnum, const unsigned int count, int* vars)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform3iv(_varList[varnum2], (GLsizei) count, (GLint*) vars);
	}
}


void ShaderMan::SetUniformVar4iv(const unsigned int varnum, const unsigned int count, int* vars)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform4iv(_varList[varnum2], (GLsizei) count, (GLint*) vars);
	}
}


void ShaderMan::SetUniformVar1f(const unsigned int varnum, const float var)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform1f(_varList[varnum2], (GLfloat) var);
	}
}


void ShaderMan::SetUniformVar2f(const unsigned int varnum, const float var1, const float var2)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform2f(_varList[varnum2], (GLfloat) var1, (GLfloat) var2);
	}
}


void ShaderMan::SetUniformVar3f(const unsigned int varnum, const float var1, const float var2, const float var3)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform3f(_varList[varnum2], (GLfloat) var1, (GLfloat) var2, (GLfloat) var3);
	}
}


void ShaderMan::SetUniformVar4f(const unsigned int varnum, const float var1, const float var2, const float var3, const float var4)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform4f(_varList[varnum2], (GLfloat) var1, (GLfloat) var2, (GLfloat) var3, (GLfloat) var4);
	}
}


void ShaderMan::SetUniformVar1fv(const unsigned int varnum, const unsigned int count, float* vars)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform1fv(_varList[varnum2], (GLsizei) count, (GLfloat*) vars);
	}
}


void ShaderMan::SetUniformVar2fv(const unsigned int varnum, const unsigned int count, float* vars)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform2fv(_varList[varnum2], (GLsizei) count, (GLfloat*) vars);
	}
}


void ShaderMan::SetUniformVar3fv(const unsigned int varnum, const unsigned int count, float* vars)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform3fv(_varList[varnum2], (GLsizei) count, (GLfloat*) vars);
	}
}


void ShaderMan::SetUniformVar4fv(const unsigned int varnum, const unsigned int count, float* vars)
{
	if ((varnum > 0) && (_varList!=NULL))
	{
		// A zero varnum indicates a false variable number.
		// Calculate correct index into list by subtracting 1.
		unsigned int varnum2 = varnum - 1;

		if (varnum2 < _numVars)
			glUniform4fv(_varList[varnum2], (GLsizei) count, (GLfloat*) vars);
	}
}


///////////////////////////////////////////////////
// Shader debugging functions
///////////////////////////////////////////////////

void ShaderMan::PrintProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, (GLint*) &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, (GLint) infologLength, (GLint*) &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}


void ShaderMan::PrintShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, (GLint*) &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, (GLint) infologLength, (GLint*) &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}


///////////////////////////////////////////////////
// Text file access (for compiling shaders)
///////////////////////////////////////////////////

char* ShaderMan::TextFileRead(const char *fn)
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL)
	{
		fp = fopen(fn, "rt");

		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0)
			{
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}

			fclose(fp);
		}
	}

	return content;
}


int ShaderMan::TextFileWrite(char *fn, char *s)
{
	FILE *fp;
	int status = 0;

	if (fn != NULL)
	{
		fp = fopen(fn, "w");

		if (fp != NULL)
		{			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;

			fclose(fp);
		}
	}

	return(status);
}
