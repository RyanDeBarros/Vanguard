#include <iostream>

#include <GLFW/glfw3.h>

int main()
{
	std::cout << "Welcome to Vanguard!" << std::endl;

	if (!glfwInit())
		return 1;
	GLFWwindow* window = glfwCreateWindow(1440, 1080, "Hello World", nullptr, nullptr);
	if (!window)
		return 1;

	for (;;)
	{
		glfwPollEvents();
		if (glfwWindowShouldClose(window))
			break;
	}
	return 0;
}
