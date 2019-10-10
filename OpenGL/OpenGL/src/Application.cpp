#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

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
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}