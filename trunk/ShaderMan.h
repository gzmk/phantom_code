/***********************************************************
 *
 * ShaderMan.h
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
 * This file defines the shader manager class, which
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
#define SHADERMAN__H

#ifndef GLOBALS__H
	#include "Globals.h"
#endif


#include <string>

#include <GL/glew.h>
#include <GL/glut.h>


class ShaderMan
{
  public:

	// Constructor + Destructor
	ShaderMan();

	ShaderMan(unsigned int shadernum);

	~ShaderMan();


  protected:

	void Init(unsigned int shadernum);


  public:

    // Shader loading/compiling/initialising
	virtual bool InitShader(const char* vertshader, const char* fragshader);

	virtual unsigned int InitShaderVar(const char* varname);

	virtual bool Apply();

	virtual void UnApply();


  public:

	// Overloaded functions for setting shader uniform variables
	void SetUniformVar1i(const unsigned int varnum, const int var);

	void SetUniformVar2i(const unsigned int varnum, const int var1, const int var2);

	void SetUniformVar3i(const unsigned int varnum, const int var1, const int var2, const int var3);

	void SetUniformVar4i(const unsigned int varnum, const int var1, const int var2, const int var3, const int var4);


	void SetUniformVar1iv(const unsigned int varnum, const unsigned int count, int* vars);

	void SetUniformVar2iv(const unsigned int varnum, const unsigned int count, int* vars);

	void SetUniformVar3iv(const unsigned int varnum, const unsigned int count, int* vars);

	void SetUniformVar4iv(const unsigned int varnum, const unsigned int count, int* vars);


	void SetUniformVar1f(const unsigned int varnum, const float var);

	void SetUniformVar2f(const unsigned int varnum, const float var1, const float var2);

	void SetUniformVar3f(const unsigned int varnum, const float var1, const float var2, const float var3);

	void SetUniformVar4f(const unsigned int varnum, const float var1, const float var2, const float var3, const float var4);


	void SetUniformVar1fv(const unsigned int varnum, const unsigned int count, float* vars);

	void SetUniformVar2fv(const unsigned int varnum, const unsigned int count, float* vars);

	void SetUniformVar3fv(const unsigned int varnum, const unsigned int count, float* vars);

	void SetUniformVar4fv(const unsigned int varnum, const unsigned int count, float* vars);


  protected:

	// Shader debugging functions
	void PrintProgramInfoLog(GLuint obj);

	void PrintShaderInfoLog(GLuint obj);

	// Text file access (for compiling shaders)
	char* TextFileRead(const char *fn);
	
	int TextFileWrite(char *fn, char *s);


  protected:

	// Holders for shader and program addresses
	GLuint _vertexShader;
	GLuint _fragmentShaderA;
	GLuint _fragmentShaderB;
	GLuint _shaderProgram;

	// Private variables
	bool _initialised; // True after InitShader()
	unsigned int _numVars; // Number of variables declared with InitShaderVar()
	unsigned int _shaderNum;
	GLint* _varList; // List of uniform variable addresses for loaded shader
};


#endif