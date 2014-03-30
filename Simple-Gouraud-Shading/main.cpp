#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "GL/glew.h"
#include <GL/glut.h>

const std::string strVertexShader(
	"#version 330\n"
	"layout(location = 0) in vec4 position;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = position;\n"
	"}\n"
	);

const std::string strFragmentShader(
	"#version 330\n"
	"out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\n"
	);

const std::string myStrFragmentShader(
	"#version 330\n"
	"out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = vec4(0.4f, 1.0f, 0.4f, 1.0f);\n"
	"}\n"
	);

GLuint vertexShader, fragmentShader;


GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}

GLuint theProgram;
GLuint myProgram;

void loadShaders()
{
	std::vector<GLuint> shaderList;
	std::vector<GLuint> myshaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));
	theProgram = CreateProgram(shaderList);
	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

	myshaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	myshaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, myStrFragmentShader));
	myProgram = CreateProgram(myshaderList);
	std::for_each(myshaderList.begin(), myshaderList.end(), glDeleteShader);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(myProgram);
	glBegin(GL_TRIANGLES);
	glVertex2d(-1.0f, -1.0f);
	glVertex2d(0.0f, 1.0f);
	glVertex2d(1.0f, 0.0f);
	glEnd();
	glUseProgram(0);
	glFlush();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(400, 400);
	glutInitWindowSize(400, 400);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glutCreateWindow("Gouraud shading");
	glewInit();
	loadShaders();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
