#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

struct ShaderProgramSource
{
	string VertexSource;
	string FragmentSource;
};
static ShaderProgramSource ParceShader(const string& filepath)
{
	ifstream stream(filepath);
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	string line;
	stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != string::npos) {
			if (line.find("vertex") != string::npos) {
				//cambia a modo vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != string::npos)
			{
				//cambia a modo fragment
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };
}
static unsigned int CompileShader(unsigned int type, const string& source)//compila shaders
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	//error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));//BRUJERIA
		glGetShaderInfoLog(id, length, &length, message);
		cout << "Error al compilar " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << endl;
		cout << message << endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);//pone los chaders junto al programa
	glAttachShader(program, fs);
	glLinkProgram(program);//linkea el programa con los shaders
	glValidateProgram(program);
	glDeleteShader(vs);//los shaders ya estan en el programa asi que se pueden borrar los intermedios 
	glDeleteShader(fs);
	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Ventana PogU", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	//para ver que version de GL es 
	cout << glGetString(GL_VERSION) << endl;
	//Glew especifica que tiene que inicialisarce en un current rendering context por eso va despues de glfwMakeContextCurrent y no antes
	if (glewInit() != GLEW_OK)
		cout << "Error al inicialisar glew" << endl;

	//array de vertices para el buffer
	float positions[] = {
		-0.5f, -0.5f,//0
		 0.5f, -0.5f,//1
		 0.5f,  0.5f,//2

		 //0.5f,  0.5f,
		-0.5f,  0.5f,//3
		//-0.5f, -0.5f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	//Vertex buffer datos que le pasamos a la VRAM para despues dibujar
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	//Vertex attribute es el layout del buffer, es para que el programa sepa que es lo que le estoy mandando 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//Index buffer
	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	//crear shader
	//string vertexShader =
	//	"#version 330 core\n"
	//	"\n"
	//	"layout(location = 0) in vec4 position;\n"
	//	"\n"
	//	"void main()\n"
	//	"{\n"
	//	" gl_Position = position;\n"
	//	"}\n";
	//string fragmentShader =
	//	"#version 330 core\n"
	//	"\n"
	//	"layout(location = 0) out vec4 color;\n"
	//	"\n"
	//	"void main()\n"
	//	"{\n"
	//	" color = vec4(1.0, 0.0, 0.0, 1.0);\n"
	//	"}\n";
	ShaderProgramSource sourse = ParceShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(sourse.VertexSource, sourse.FragmentSource);
	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_Color");
	glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f);

	float r = 0.0f;
	float increment = 0.05f;
	cout << "VERTEX:" << endl;
	cout << sourse.VertexSource << endl;
	cout << "FRAGMENT" << endl;
	cout << sourse.FragmentSource << endl;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Not worky for same reason likely outdated
		glBegin(Gl_TRIANGLES);
		glVertex2d(-0.5f, -0.5f);
		glVertex2d( 0.0f,  0.5f);
		glVertex2d( 0.5f, -0.5f);
		glEnd();
		*/
		glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
		/*Draw call*/
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if (r > 1.0f) {
			increment = -0.05f;
		}
		else if (r < 0.0f) {
			increment = 0.05f;
		}

		r += increment;
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}