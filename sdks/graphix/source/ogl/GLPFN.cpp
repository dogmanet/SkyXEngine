#include "GXContext.h"

#include "GLPFN.h"

template <class T>
void glfnptr(T* p, const char * name)
{
	*p = (T)wglGetProcAddress(name);
}

#define GLPROC(name) \
	glfnptr(&name, #name)

IDSRGLPFN::IDSRGLPFN()
{
	GLPROC(wglSwapIntervalEXT);

	GLPROC(glGenBuffers);
	GLPROC(glBindBuffer);
	GLPROC(glBufferData);
	GLPROC(glDeleteBuffers);

	GLPROC(glGenVertexArrays);
	GLPROC(glBindVertexArray);
	GLPROC(glDeleteVertexArrays);

	GLPROC(glVertexAttribPointer);
	GLPROC(glEnableVertexAttribArray);


	GLPROC(glDrawElementsBaseVertex);
	GLPROC(glDrawElementsInstanced);
	
	
	GLPROC(glGetProgramiv);
	GLPROC(glGetProgramBinary);
	GLPROC(glProgramBinary);
	GLPROC(glGetProgramInfoLog);
	GLPROC(glCreateProgram);
	GLPROC(glLinkProgram);
	GLPROC(glUseProgram);
	GLPROC(glDeleteProgram);


	GLPROC(glCreateShader);
	GLPROC(glShaderSource);
	GLPROC(glCompileShader);
	GLPROC(glDeleteShader);
	GLPROC(glAttachShader);
	GLPROC(glDetachShader);
	GLPROC(glGetShaderiv);
	GLPROC(glGetShaderInfoLog);
}

#undef GLPROC
