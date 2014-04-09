#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <glload/gl_3_3.h>
#include <glload/gll.h>
#include <GL/freeglut.h>


const std::string strVertexShader(
	"#version 330\n"
	"layout(location = 0) in vec4 position;\n"
	"layout(location = 1) in vec4 color;\n"
	"smooth out vec4 theColor;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = position;\n"
	"	theColor = color;\n"
	"}\n"
	);

const std::string myStrFragmentShader(
	"#version 330\n"
	"out vec4 outputColor;\n"
	"smooth in vec4 theColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = theColor;\n"
	"}\n"
	);

GLuint vertexShader, fragmentShader;

const float cubeVertexData[] = {
/*	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
*/
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,

	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,

	0.5f, 1.0f, 0.5f, 1.0f,
	0.5f, 0.7f, 0.7f, 1.0f,
	0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.7f, 0.7f, 1.0f,

	0.5f, 0.5f, 1.0f, 1.0f,
	0.7f, 0.5f, 0.7f, 1.0f,
	1.0f, 0.5f, 1.0f, 1.0f,
	0.7f, 0.5f, 0.7f, 1.0f
};

const GLshort cubeIndexData[] = 
{
	0, 2, 1,
	0, 3, 2,

	1, 2, 6,
	1, 6, 5,

	5, 6, 4,
	6, 7, 4,

	7, 3, 4,
	3, 0, 4,

	3, 6, 2,
	3, 7, 6,

	0, 1, 5,
	0, 5, 4,
};

GLuint vertexBufferObject;
GLuint indexBufferObject;
GLuint vao;

void initializeVertexObject()
{
	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndexData), cubeIndexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void init()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	size_t colorDataOffset = sizeof(float) * 3 * 8;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

	glBindVertexArray(0);

/*	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f); */
}

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

	myshaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	myshaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, myStrFragmentShader));
	myProgram = CreateProgram(myshaderList);
	std::for_each(myshaderList.begin(), myshaderList.end(), glDeleteShader);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(myProgram);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, sizeof(cubeIndexData) / sizeof(cubeIndexData[0]), GL_UNSIGNED_SHORT, 0);
	/*
	glBegin(GL_TRIANGLES);
	glVertex2d(-1.0f, -1.0f);
	glVertex2d(0.0f, 1.0f);
	glVertex2d(1.0f, 0.0f);
	glEnd();
	*/
	glBindVertexArray(0);
	glUseProgram(0);
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	if(key == 27)
		exit(0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(400, 400);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Gouraud shading");
	LoadFunctions();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	loadShaders();
	initializeVertexObject();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}