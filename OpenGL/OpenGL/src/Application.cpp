#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

static unsigned int CompileShader(unsigned int type,const string& source)//compila shaders
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

	//para ver que version de GL es 
	cout << glGetString(GL_VERSION) << endl;
	//Glew especifica que tiene que inicialisarce en un current rendering context por eso va despues de glfwMakeContextCurrent y no antes
	if (glewInit() != GLEW_OK)
		cout << "Error al inicialisar glew" << endl;
	
	//array de vertices para el buffer
	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	//Vertex buffer datos que le pasamos a la VRAM para despues dibujar
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	//Vertex attribute es el layout del buffer, es para que el programa sepa que es lo que le estoy mandando 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//crear shader
	string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		" gl_Position = position;\n"
		"}\n";
	string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		" color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";
	unsigned int shader = CreateShader(vertexShader,fragmentShader);
	glUseProgram(shader);
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

		/*Draw call*/
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}